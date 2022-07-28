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
#include "dht.h"

volatile uint64_t last_dcf_sync = 0;

void cal_init() {
    calendar_enable(&CALENDAR_0);
}

void dcf_sync(struct calendar_date_time *cal_dt) {
    ulog(INFO, "Time sync: %04d-%02d-%02d %02d:%02d:00", cal_dt->date.year, cal_dt->date.month, cal_dt->date.day, cal_dt->time.hour, cal_dt->time.min);
    last_dcf_sync = millis();
}

int main(void)
{
    struct io_descriptor *uart_io;

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

    cal_init();
    dcf_init(dcf_sync);
    ldr_init();

    printf("\r\n\r\n");
    ulog(INFO, "Radio clock firmware build: %s", VERSION_STR);
    ulog(INFO, "https://github.com/erikvanzijst/radioclock");
    ulog(INFO, "Erik van Zijst <erik.van.zijst@gmail.com>\r\n");

    ext_irq_register(PIN_PA15, switch_isr);
    delay_ms(100);  // Wait for DHT20 module to initialize

    while (!last_dcf_sync);

    if (gpio_get_pin_level(PERIPHERAL_CTL)) {
        gpio_set_pin_level(PERIPHERAL_CTL, false);  // turn on power to displays and DHT20 module

        int32_t err;
        if ((err = display_init())) {
            ulog(ERROR, "display_init() failed (%ld)", err)
        }
        if ((err = dht_init())) {
            ulog(ERROR, "dht_init() failed (%ld)", err)
        }
    }

    for (;;) {
        delay_ms(1000);

        int32_t temp = get_temperature();
        ulog(INFO, "Temperature: %ld.%ldC Humidity: %ld%%", temp / 10, temp % 10, get_humidity())
//        ulog(INFO, "LDR: %d\r\n", ldr[0]);

        //        gpio_toggle_pin_level(LED);
    }
}
