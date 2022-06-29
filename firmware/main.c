#include <atmel_start.h>
#include "atmel_start_pins.h"
#include "hal_delay.h"
#include "version.h"

int main(void)
{
    struct io_descriptor *io;

    atmel_start_init();
    gpio_set_pin_level(LED, true);
    gpio_set_pin_level(DCF_CTL, true);  // turn off power to DCF module
    gpio_set_pin_level(PERIPHERAL_CTL, false);  // turn on power to DHT20 module

    usart_sync_get_io_descriptor(&USART_0, &io);
    usart_sync_enable(&USART_0);

    printf("Radio clock firmware build: %s\r\n", VERSION_STR);
    printf("https://github.com/erikvanzijst/radioclock\r\n");
    printf("Erik van Zijst <erik.van.zijst@gmail.com>\r\n\r\n");

	/* Replace with your application code */
	while (1) {
        gpio_set_pin_level(LED, !gpio_get_pin_level(LED));
        delay_ms(1000);
	}
}
