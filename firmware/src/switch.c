#include "atmel_start_pins.h"

#include "log.h"
#include "switch.h"
#include "millis.h"

#define DEBOUNCE_MILLIS 40
#define LONGPRESS_MILLIS 2000

void (* short_press_cb)(void) = NULL;
void (* long_press_cb)(void) = NULL;
uint64_t last_edge = 0, last_down = 0;
bool state;

static struct timer_task TIMER_0_debounce_task;

void switch_isr(void) {
}

void read_button(const struct timer_task *const timer_task) {
    bool current_state = !gpio_get_pin_level(SWITCH);
    uint64_t now = millis();

    if (current_state != state) {
        if (now - last_edge >= DEBOUNCE_MILLIS) {
            state = current_state;

            if (!state) {   // released
                (now - last_down) >= LONGPRESS_MILLIS ? long_press_cb() : short_press_cb();
            } else {        // pressed
                last_down = now;
            }
        }
    } else {
        last_edge = now;
    }
}

int32_t switch_init(void (* short_press)(void), void (* long_press)(void)) {
    short_press_cb = short_press;
    long_press_cb = long_press;
    state = !gpio_get_pin_level(SWITCH);
    last_edge = millis();
    ext_irq_register(PIN_PA15, switch_isr);     // merely to wake from STANDBY

    TIMER_0_debounce_task.interval = 10;     // 100Hz
    TIMER_0_debounce_task.cb = read_button;
    TIMER_0_debounce_task.mode = TIMER_TASK_REPEAT;

    return timer_add_task(&TIMER_0, &TIMER_0_debounce_task);
}
