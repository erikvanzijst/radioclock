#include "atmel_start_pins.h"

#include "log.h"
#include "timezone.h"

void tz_isr(void) {
    timezone = gpio_get_pin_level(TZ1) ? LON : gpio_get_pin_level(TZ2) ? AMS : HEL;
    ulog(INFO, "Timezone set to: %s", get_tz_name(timezone))
}

volatile timezone_t timezone;
static const char * tz_names[] = {"London", "Amsterdam", "Helsinki"};

const char * get_tz_name(timezone_t tz) {
    return tz_names[tz];
}

int32_t timezone_init() {

    // Register interrupt handler on TZ1 and TZ2 pins
    int32_t err;
    if ((err = ext_irq_register(PIN_PA18, tz_isr)) || (err = ext_irq_register(PIN_PA19, tz_isr))) {
        ulog(ERROR, "ext_irq_register() failed (%ld)", (unsigned long)err)
        return -1;
    }
    tz_isr();

    return ERR_NONE;
}
