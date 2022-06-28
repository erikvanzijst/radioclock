#include <atmel_start.h>
#include "atmel_start_pins.h"
#include "hal_delay.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
    gpio_set_pin_level(LED, true);

	/* Replace with your application code */
	while (1) {
        gpio_set_pin_level(LED, !gpio_get_pin_level(LED));
        delay_ms(1000);
	}
}
