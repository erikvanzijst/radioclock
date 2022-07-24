#include <stdio.h>
#include "../include/dcf.h"
#include "atmel_start_pins.h"

void dcf_data_isr(void) {
    bool val = gpio_get_pin_level(DCF_DATA);
    printf("%d\r\n", val);
    gpio_set_pin_level(LED, val);
}
