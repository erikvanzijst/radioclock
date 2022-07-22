#include <stdio.h>
#include "atmel_start_pins.h"
#include "../include/switch.h"

void switch_isr(void) {
    bool val = gpio_get_pin_level(SWITCH);
    printf("Switch %s\r\n", val ? "released" : "pressed");
//    gpio_set_pin_level(PERIPHERAL_CTL, val);  // turn on/off power to displays and DHT20 module
}
