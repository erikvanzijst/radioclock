#include <atmel_start.h>
#include "atmel_start_pins.h"
#include "hal_delay.h"
#include "version.h"

typedef struct {
    uint8_t status;
    uint8_t data[5];
    uint8_t crc;
} dht_measurement_t;

static struct calendar_alarm alarm;

uint8_t crc8(uint8_t *ptr, uint8_t len) {
    uint8_t crc = 0xFF;
    while (len--) {
        crc ^= *ptr++;
        for (uint8_t i = 0; i < 8; i++) {
            if (crc & 0x80) {
                crc <<= 1;
                crc ^= 0x31;
            } else {
                crc <<= 1;
            }
        }
    }
    return crc;
}

static void alarm_cb(struct calendar_descriptor *const descr) {
}

int init_cal() {

    struct calendar_date date;
    struct calendar_time time;

    calendar_enable(&CALENDAR_0);

    date.year  = 2022;
    date.month = 6;
    date.day   = 29;

    time.hour = 22;
    time.min  = 00;
    time.sec  = 00;

    calendar_set_date(&CALENDAR_0, &date);
    calendar_set_time(&CALENDAR_0, &time);

    alarm.cal_alarm.datetime.time.sec = 4;
    alarm.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
    alarm.cal_alarm.mode              = REPEAT;

    calendar_set_alarm(&CALENDAR_0, &alarm, alarm_cb);
}

int main(void)
{
    struct io_descriptor *io;
    struct io_descriptor *I2C_0_io;
    struct calendar_date_time datetime;

//    uint8_t buf[7];
    dht_measurement_t measurement;


    atmel_start_init();
    gpio_set_pin_level(LED, true);
    gpio_set_pin_level(DCF_CTL, true);  // turn off power to DCF module
    gpio_set_pin_level(PERIPHERAL_CTL, false);  // turn on power to DHT20 module

    usart_sync_get_io_descriptor(&USART_0, &io);
    usart_sync_enable(&USART_0);

    printf("\r\n\r\nRadio clock firmware build: %s\r\n", VERSION_STR);
    printf("https://github.com/erikvanzijst/radioclock\r\n");
    printf("Erik van Zijst <erik.van.zijst@gmail.com>\r\n\r\n");

    delay_ms(100);  // Wait for DHT20 module to initialize
    i2c_m_sync_get_io_descriptor(&I2C_0, &I2C_0_io);
    i2c_m_sync_enable(&I2C_0);
    i2c_m_sync_set_slaveaddr(&I2C_0, 0x38, I2C_M_SEVEN);

    init_cal();

    io_write(I2C_0_io, (uint8_t*)((uint8_t []){0xba}), 1);  // soft reset command
    delay_ms(20);  // wait for the sensor reset

	while (1) {
        int32_t retval;
        gpio_set_pin_level(LED, !gpio_get_pin_level(LED));
        io_write(I2C_0_io, (uint8_t*)((uint8_t []){0xac, 0x33, 0x0}), 3);
        delay_ms(85);  // wait for the sensor to acquire a measurement
        retval = io_read(I2C_0_io, (uint8_t *)&measurement, sizeof (measurement));

        calendar_get_date_time(&CALENDAR_0, &datetime);

        if (retval < 0) {
            printf("ERR: I2C read failed: %d (see: hal/include/hpl_i2c_m_sync.h)\r\n", retval);

        } else if ((measurement.status & 0x1) == 0x0) {
            int32_t tmp = measurement.data[4] + (measurement.data[3] << 8) + ((measurement.data[2] & 0xf) << 16);
            int temperature = (((tmp * 2000) >> 20) - 500);

            tmp = ((measurement.data[2] & 0xf0) >> 4) + (measurement.data[1] << 4) + (measurement.data[0] << 12);
            int humidity = (tmp * 100) >> 20;


            printf("%02d:%02d:%02d - Temperature: %d.%dC Humidity: %d%%\r\n", datetime.time.hour, datetime.time.min, datetime.time.sec, temperature / 10, temperature % 10, humidity);

        } else if (measurement.crc != crc8((uint8_t *)&measurement, 6)) {
            printf("ERR: DHT20 I2C CRC mismatch (%d != %d)\r\n", measurement.crc, crc8((uint8_t *)&measurement, 6));

        } else {
            printf("ERR: DHT20 sensor returned an error (status: %x)\r\n", measurement.status);
        }
        delay_ms(1000);
    }
}
