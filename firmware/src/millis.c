#include "millis.h"
#include "driver_init.h"

volatile uint64_t ticks = 0;
static struct timer_task TIMER_0_millis_task;

void increment_millis(const struct timer_task *const timer_task) {
    CRITICAL_SECTION_ENTER()
    ticks++;
    CRITICAL_SECTION_LEAVE()
}

int32_t millis_init() {
    TIMER_0_millis_task.interval = 1;
    TIMER_0_millis_task.cb       = increment_millis;
    TIMER_0_millis_task.mode     = TIMER_TASK_REPEAT;

    return timer_add_task(&TIMER_0, &TIMER_0_millis_task);
}

uint64_t millis() {
    uint64_t cp;
    CRITICAL_SECTION_ENTER()
    cp = ticks;
    CRITICAL_SECTION_LEAVE()
    return cp;
}
