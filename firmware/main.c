#include <atmel_start.h>
#include "atmel_start_pins.h"
#include "hal_delay.h"
#include "version.h"
#include "include/dcf.h"
#include "include/switch.h"
#include "millis.h"
#include "log.h"

typedef struct {
    uint8_t status;
    uint8_t data[5];
    uint8_t crc;
} dht_measurement_t;

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

void init_cal() {

    struct calendar_date date;
    struct calendar_time time;

    calendar_enable(&CALENDAR_0);
}


int main(void)
{
    struct io_descriptor *uart_io;
    struct io_descriptor *i2c_io;
    struct io_descriptor *spi_io;
    struct calendar_date_time datetime;
    dht_measurement_t measurement;

    atmel_start_init();
    gpio_set_pin_level(LED, false);
    gpio_set_pin_level(DCF_CTL, false);          // turn on power to DCF module
    gpio_set_pin_level(PERIPHERAL_CTL, true);  // turn off power to displays and DHT20 module
    gpio_set_pin_level(LDR_SINK, false);        // enable LDR

    adc_sync_enable_channel(&ADC_0, 0);

    spi_m_sync_get_io_descriptor(&SPI_0, &spi_io);
    spi_m_sync_enable(&SPI_0);

    usart_sync_get_io_descriptor(&USART_0, &uart_io);
    usart_sync_enable(&USART_0);

    delay_ms(100);  // Wait for DHT20 module to initialize
    i2c_m_sync_get_io_descriptor(&I2C_0, &i2c_io);
    i2c_m_sync_enable(&I2C_0);
    i2c_m_sync_set_slaveaddr(&I2C_0, 0x38, I2C_M_SEVEN);

    // trigger fast DCF fast sync:
    gpio_set_pin_level(DCF_PDN, true);
    delay_ms(100);
    gpio_set_pin_level(DCF_PDN, false);

    printf("\r\n\r\n");
    ulog(INFO, "Radio clock firmware build: %s", VERSION_STR);
    ulog(INFO, "https://github.com/erikvanzijst/radioclock");
    ulog(INFO, "Erik van Zijst <erik.van.zijst@gmail.com>\r\n");

    init_cal();

    if (millis_init()) {
        ulog(ERROR, "Failed to start TIMER_0!");
    }

    // DHT20 initialization
    io_write(i2c_io, (uint8_t*)((uint8_t []){0xba}), 1);  // soft reset command
    delay_ms(20);  // wait for the sensor reset

    // Register interrupt handler on DCF_DATA pin
    ext_irq_register(PIN_PA06, dcf_data_isr);
    ext_irq_register(PIN_PA15, switch_isr);

    // Display initialization
    gpio_set_pin_level(DSPL_SS, false); // Leave shutdown mode:
    io_write(spi_io, (uint8_t *)((uint8_t[]){0x0c, 0x01, 0x0c, 0x01}), 4);
    gpio_set_pin_level(DSPL_SS, true);

    gpio_set_pin_level(DSPL_SS, false); // set intensity:
    io_write(spi_io, (uint8_t *)((uint8_t[]){0x0a, 0x08, 0x0a, 0x08}), 4);
    gpio_set_pin_level(DSPL_SS, true);

    gpio_set_pin_level(DSPL_SS, false); // set scan-limit register to all segments:
    io_write(spi_io, (uint8_t *)((uint8_t[]){0x0b, 0x07, 0x0b, 0x07}), 4);
    gpio_set_pin_level(DSPL_SS, true);

    gpio_set_pin_level(DSPL_SS, false); // enable BCD decode mode:
    io_write(spi_io, (uint8_t *)((uint8_t[]){0x09, 0xff, 0x09, 0xff}), 4);
    gpio_set_pin_level(DSPL_SS, true);

    for (uint8_t i = 1; i <= 8; i++) {   // set a value for each segment
        gpio_set_pin_level(DSPL_SS, false); // set intensity:
        io_write(spi_io, (uint8_t *)((uint8_t[]){i, i+8-1, i, i-1}), 4);
        gpio_set_pin_level(DSPL_SS, true);
    }

    uint8_t ldr[1];
    uint64_t prev_millis = 0;

    for (int step = 0;;) {
        int32_t retval;

        if (!gpio_get_pin_level(PERIPHERAL_CTL)) {
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

                if (adc_sync_read_channel(&ADC_0, 0, ldr, 1) != 1) {
                    ulog(ERROR, "adc_sync_read_channel() failed");
                }

                ulog(INFO, "%02d:%02d:%02d - Temperature: %d.%dC Humidity: %d%% Brightness: %d", datetime.time.hour, datetime.time.min, datetime.time.sec, temperature / 10, temperature % 10, humidity, ldr[0]);

            } else if (measurement.crc != crc8((uint8_t *)&measurement, 6)) {
                ulog(ERROR, "DHT20 I2C CRC mismatch (%d != %d)", measurement.crc, crc8((uint8_t *)&measurement, 6));

            } else {
                ulog(ERROR, "DHT20 sensor returned an error (status: %x)", measurement.status);
            }

            // SPI

            for (uint8_t i = 0; i < 8; i++) {   // set a value for each segment
                uint8_t buf[] = {i+1, i+8+step, i+1, i + step};
    //            printf("Display %d -> %d %d %d %d\r\n", step, buf[0], buf[1], buf[2], buf[3]);

                gpio_set_pin_level(DSPL_SS, false); // set intensity:
                io_write(spi_io, (uint8_t *)&buf, 4);
                gpio_set_pin_level(DSPL_SS, true);
            }

            gpio_set_pin_level(DSPL_SS, false); // set intensity:
            io_write(spi_io, (uint8_t *)((uint8_t[]){0x0a, 0xff - (ldr[0] >> 4), 0x0a, 0xff - (ldr[0] >> 4)}), 4);
            gpio_set_pin_level(DSPL_SS, true);
        }

        step = (step + 1 % 8);
        delay_ms(1000);

        calendar_get_date_time(&CALENDAR_0, &datetime);
//        printf("RTC: %02d:%02d:%02d\r\n", datetime.time.hour, datetime.time.min, datetime.time.sec);

        uint32_t now = millis();
//        printf("%lu\r\n", (unsigned long)(now - prev_millis));
        prev_millis = now;

        //        gpio_toggle_pin_level(LED);
    }
}
