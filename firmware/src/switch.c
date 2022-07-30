#include <stdio.h>
#include "atmel_start_pins.h"

#include "log.h"
#include "switch.h"

void switch_isr(void) {
    ulog(INFO, "Switch %s", gpio_get_pin_level(SWITCH) ? "released" : "pressed")
}
