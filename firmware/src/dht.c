#include "atmel_start.h"
#include "hal_i2c_m_async.h"

#include "dht.h"
#include "log.h"
#include "millis.h"

#define DHT_RESPONSE_TIME 85

typedef struct {
    uint8_t status;
    uint8_t data[5];
    uint8_t crc;
} dht_measurement_t;

int32_t temperature;    // in celsius * 10 (to avoid float formatting later on)
int32_t humidity;

struct io_descriptor *I2C_0_io;
static struct timer_task TIMER_0_trigger_task, TIMER_0_read_task;
volatile dht_measurement_t measurement;
static uint8_t CMD_START[] = {0xac, 0x33, 0x0};     // command to start a measurement
volatile uint64_t tx_timestamp = 0;

int32_t get_temperature() {
    return temperature;
}

int32_t get_humidity() {
    return humidity;
}

void read_measurement(const struct timer_task *const timer_task) {
    if (tx_timestamp && (millis() - tx_timestamp >= DHT_RESPONSE_TIME)) {
        if (io_read(I2C_0_io, (uint8_t *)&measurement, sizeof (measurement)) < 0) {
            ulog(ERROR, "io_read() failed")
        }
        tx_timestamp = 0;
    }
}

void tx_complete(struct i2c_m_async_desc *const i2c) {
    tx_timestamp = millis();
}

void start_measurement(const struct timer_task *const timer_task) {
    io_write(I2C_0_io, (uint8_t*)CMD_START, sizeof(CMD_START));
}

void rx_complete(struct i2c_m_async_desc *const i2c) {
    if ((measurement.status & 0x1) == 0x0) {
        int32_t tmp = measurement.data[4] + (measurement.data[3] << 8) + ((measurement.data[2] & 0xf) << 16);
        temperature = (((tmp * 2000) >> 20) - 500);

        tmp = ((measurement.data[2] & 0xf0) >> 4) + (measurement.data[1] << 4) + (measurement.data[0] << 12);
        humidity = (tmp * 100) >> 20;

    } else if (measurement.crc != crc8((uint8_t *)&measurement, 6)) {
        ulog(ERROR, "DHT20 I2C CRC mismatch (%d != %d)", measurement.crc, crc8((uint8_t *)&measurement, 6))

    } else {
        ulog(ERROR, "DHT20 sensor returned an error (status: %x)", measurement.status)
    }
}

int32_t dht_init(void) {

    i2c_m_async_get_io_descriptor(&I2C_0, &I2C_0_io);
    i2c_m_async_enable(&I2C_0);
    i2c_m_async_set_slaveaddr(&I2C_0, 0x38, I2C_M_SEVEN);

    // DHT20 initialization
    io_write(I2C_0_io, (uint8_t*)((uint8_t []){0xba}), 1);  // soft reset command
    delay_ms(20);  // wait for the sensor reset

    i2c_m_async_register_callback(&I2C_0, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)tx_complete);
    i2c_m_async_register_callback(&I2C_0, I2C_M_ASYNC_RX_COMPLETE, (FUNC_PTR)rx_complete);

    TIMER_0_trigger_task.interval   = 500;
    TIMER_0_trigger_task.cb         = start_measurement;
    TIMER_0_trigger_task.mode       = TIMER_TASK_REPEAT;

    TIMER_0_read_task.interval      = 100;       // collect measurement
    TIMER_0_read_task.cb            = read_measurement;
    TIMER_0_read_task.mode          = TIMER_TASK_REPEAT;

    if (timer_add_task(&TIMER_0, &TIMER_0_trigger_task) ||
        timer_add_task(&TIMER_0, &TIMER_0_read_task)) {
        return -1;
    }
    return ERR_NONE;
}
