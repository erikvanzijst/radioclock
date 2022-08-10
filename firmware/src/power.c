#include "power.h"
#include "hal_ext_irq.h"
#include "log.h"

volatile bool pwr_sense_line = true;

void pwr_isr(void) {
    pwr_sense_line = gpio_get_pin_level(PWR_SENSE);
    ulog(INFO, "USB Power %s", pwr_sense_line ? "ON" : "OFF")
}

bool usb_power() {
    return pwr_sense_line;
}

int32_t pwr_init() {
    int32_t err = ERR_NONE;
    if ((err = ext_irq_register(PIN_PA05, pwr_isr))) {
        ulog(ERROR, "ext_irq_register() failed (%ld)", (unsigned long)err)
    }
    pwr_sense_line = gpio_get_pin_level(PWR_SENSE);
    return err;
}
