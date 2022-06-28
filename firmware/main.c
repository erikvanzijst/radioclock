#include <atmel_start.h>
#include <string.h>
#include "atmel_start_pins.h"
#include "hal_delay.h"

int main(void)
{
    struct io_descriptor *io;
    char * hw = "Hello world!\r\n";

	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
    gpio_set_pin_level(LED, true);

    usart_sync_get_io_descriptor(&USART_0, &io);
    usart_sync_enable(&USART_0);

    io_write(io, (uint8_t *)hw, strlen(hw));

	/* Replace with your application code */
	while (1) {
        gpio_set_pin_level(LED, !gpio_get_pin_level(LED));
        delay_ms(1000);
	}
}
