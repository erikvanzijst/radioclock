#include "hal_adc_async.h"
#include "ldr.h"
#include "atmel_start.h"
#include "log.h"

volatile uint8_t ldr[1];
static struct timer_task TIMER_0_adc_task;

static void adc_conversion_complete(const struct adc_async_descriptor *const descr, const uint8_t channel) {
    if (adc_async_read_channel(&ADC_0, 0, ldr, 1) != 1) {
        ulog(ERROR, "adc_async_read_channel() failed")
    }
}

static void start_conversion(const struct timer_task *const timer_task) {
    adc_async_start_conversion(&ADC_0);
}

int32_t ldr_init() {
    if (adc_async_register_callback(&ADC_0, 0, ADC_ASYNC_CONVERT_CB, adc_conversion_complete)) {
        ulog(ERROR, "adc_async_register_callback() failed")
        return -1;
    }
    if (adc_async_enable_channel(&ADC_0, 0)) {
        ulog(ERROR, "adc_async_enable_channel() failed")
        return -1;
    }

    TIMER_0_adc_task.interval = 100;
    TIMER_0_adc_task.cb       = start_conversion;
    TIMER_0_adc_task.mode     = TIMER_TASK_REPEAT;

    if (timer_add_task(&TIMER_0, &TIMER_0_adc_task)) {
        ulog(ERROR, "timer_add_task() failed")
        return -1;
    }
    return ERR_NONE;
}
