#include "atmel_start_pins.h"
#include "hal_gpio.h"

#include "peripherals.h"
#include "display.h"
#include "log.h"
#include "dht.h"

int32_t power_up_peripherals() {
    gpio_set_pin_level(PERIPHERAL_CTL, false);  // turn on power to displays and DHT20 module

    if (display_init()) {
        ulog(ERROR, "display_init() failed")
        return -1;
    }
    if (dht_init()) {
        ulog(ERROR, "dht_init() failed")
        return -1;
    }
    return ERR_NONE;
}

int32_t power_down_peripherals() {
    if (dht_deinit()) {
        ulog(ERROR, "dht_deinit() failed")
        return -1;
    }
    if (display_deinit()) {
        ulog(ERROR, "display_deinit() failed")
        return -1;
    }
    gpio_set_pin_level(PERIPHERAL_CTL, true);
    return ERR_NONE;
}
