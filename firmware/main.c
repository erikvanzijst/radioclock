#include <atmel_start.h>
#include "atmel_start_pins.h"
#include "hal_delay.h"
#include "version.h"
#include "include/dcf.h"
#include "include/switch.h"
#include "millis.h"
#include "log.h"
#include "display.h"
#include "ldr.h"

typedef struct {
    uint8_t status;
    uint8_t data[5];
    uint8_t crc;
} dht_measurement_t;

void cal_init() {
    calendar_enable(&CALENDAR_0);
}

void dcf_sync(struct calendar_date_time *cal_dt) {
    ulog(INFO, "Time sync: %04d-%02d-%02d %02d:%02d:00", cal_dt->date.year, cal_dt->date.month, cal_dt->date.day, cal_dt->time.hour, cal_dt->time.min);

    // TODO: do from main loop?
    if (gpio_get_pin_level(PERIPHERAL_CTL)) {
        gpio_set_pin_level(PERIPHERAL_CTL, false);  // turn on power to displays and DHT20 module

        int32_t err;
        if ((err = display_init())) {
            ulog(ERROR, "display_init() failed (%d)", err)
        }
    }
}

int main(void)
{
    struct io_descriptor *uart_io;
    struct io_descriptor *i2c_io;
    struct calendar_date_time datetime;
    dht_measurement_t measurement;

    atmel_start_init();
    gpio_set_pin_level(LED, false);
    gpio_set_pin_level(DCF_CTL, false);         // turn on power to DCF module
    gpio_set_pin_level(PERIPHERAL_CTL, true);   // turn off power to displays and DHT20 module
    gpio_set_pin_level(LDR_SINK, false);        // enable LDR

    if (timer_start(&TIMER_0)) {
        ulog(ERROR, "Failed to start TIMER_0!");
    }
    millis_init();

    usart_sync_get_io_descriptor(&USART_0, &uart_io);
    usart_sync_enable(&USART_0);

    delay_ms(100);  // Wait for DHT20 module to initialize
    i2c_m_sync_get_io_descriptor(&I2C_0, &i2c_io);
    i2c_m_sync_enable(&I2C_0);
    i2c_m_sync_set_slaveaddr(&I2C_0, 0x38, I2C_M_SEVEN);

    printf("\r\n\r\n");
    ulog(INFO, "Radio clock firmware build: %s", VERSION_STR);
    ulog(INFO, "https://github.com/erikvanzijst/radioclock");
    ulog(INFO, "Erik van Zijst <erik.van.zijst@gmail.com>\r\n");

    cal_init();
    dcf_init(dcf_sync);
    ldr_init();

    // DHT20 initialization
    io_write(i2c_io, (uint8_t*)((uint8_t []){0xba}), 1);  // soft reset command
    delay_ms(20);  // wait for the sensor reset

    ext_irq_register(PIN_PA15, switch_isr);

    for (int step = 0;;) {
        int32_t retval;

        if (!gpio_get_pin_level(PERIPHERAL_CTL) && false) {
//            gpio_set_pin_level(LED, !gpio_get_pin_level(LED));

            io_write(i2c_io, (uint8_t*)((uint8_t []){0xac, 0x33, 0x0}), 3);
            delay_ms(85);  // wait for the sensor to acquire a measurement
            retval = io_read(i2c_io, (uint8_t *)&measurement, sizeof (measurement));

            calendar_get_date_time(&CALENDAR_0, &datetime);

            if (retval < 0) {
                ulog(ERROR, "I2C read failed: %ld (see: hal/include/hpl_i2c_m_sync.h)", retval);

            } else if ((measurement.status & 0x1) == 0x0) {
                int32_t tmp = measurement.data[4] + (measurement.data[3] << 8) + ((measurement.data[2] & 0xf) << 16);
                int temperature = (((tmp * 2000) >> 20) - 500);

                tmp = ((measurement.data[2] & 0xf0) >> 4) + (measurement.data[1] << 4) + (measurement.data[0] << 12);
                int humidity = (tmp * 100) >> 20;

//                if (adc_sync_read_channel(&ADC_0, 0, ldr, 1) != 1) {
//                    ulog(ERROR, "adc_sync_read_channel() failed");
//                }

                ulog(INFO, "%02d:%02d:%02d - Temperature: %d.%dC Humidity: %d%% Brightness: %d", datetime.time.hour, datetime.time.min, datetime.time.sec, temperature / 10, temperature % 10, humidity, ldr[0]);

            } else if (measurement.crc != crc8((uint8_t *)&measurement, 6)) {
                ulog(ERROR, "DHT20 I2C CRC mismatch (%d != %d)", measurement.crc, crc8((uint8_t *)&measurement, 6));

            } else {
                ulog(ERROR, "DHT20 sensor returned an error (status: %x)", measurement.status);
            }
        }

        step = (step + 1 % 8);
        delay_ms(1000);

//        ulog(INFO, "LDR: %d\r\n", ldr[0]);

        //        gpio_toggle_pin_level(LED);
    }
}
