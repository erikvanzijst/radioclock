#include <atmel_start.h>
#include "atmel_start_pins.h"

int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();

    gpio_set_pin_level(LED, false);
	/* Replace with your application code */
	while (1) {
	}
}
