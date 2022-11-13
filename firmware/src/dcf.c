#include "atmel_start.h"
#include "hal_calendar.h"

#include "dcf.h"
#include "dcf_parser.h"
#include "log.h"
#include "power.h"

struct sync_state_t {
    volatile uint64_t dcf_bits;
    volatile uint32_t dcf_prev_posedge;
    volatile bool prev_edge;
    volatile uint64_t errors;
};

enum dcf_isr_result_t {
    BUSY,
    SYNCED,
    POWER_FAIL
};

void dcf_data_isr(void) {
//    bool edge = gpio_get_pin_level(DCF_DATA);
//    gpio_set_pin_level(LED, edge);
}

struct calendar_date_time * dt_to_calendar(struct date_time_t *dt, struct calendar_date_time *cal_dt) {
    cal_dt->time.sec = dt->sec;
    cal_dt->time.min = dt->min;
    cal_dt->time.hour = dt->hour;
    cal_dt->date.day = dt->day;
    cal_dt->date.month = dt->month;
    cal_dt->date.year = dt->year;

    return cal_dt;
}

int32_t dcf_init() {
    gpio_set_pin_level(DCF_CTL, false);         // turn on power to DCF module

    // trigger fast DCF fast sync:
    gpio_set_pin_level(DCF_PDN, true);
    delay_ms(100);
    gpio_set_pin_level(DCF_PDN, false);

    // Register interrupt handler on DCF_DATA pin
    int32_t err;
    if ((err = ext_irq_register(PIN_PA06, dcf_data_isr))) {
        ulog(ERROR, "ext_irq_register() failed (%ld)", (unsigned long)err)
        return -1;
    }
    return ERR_NONE;
}

int32_t dcf_deinit(void) {
    gpio_set_pin_level(DCF_CTL, true);         // turn off power to DCF module
    gpio_set_pin_level(LED, false);

    int32_t err;
    if ((err = ext_irq_disable(PIN_PA06))) {
        ulog(ERROR, "ext_irq_disable() failed (%ld)", (unsigned long)err)
        return -1;
    }
    if ((err = ext_irq_register(PIN_PA06, NULL))) {
        ulog(ERROR, "ext_irq_register() failed (%ld)", (unsigned long)err)
        return -1;
    }
    return ERR_NONE;
}

uint32_t dcf_millis() {
    return hri_tccount32_get_COUNT_COUNT_bf(TC4, TC_COUNT32_COUNT_MASK);
}

void dcf_millis_reset() {
    hri_tccount32_clear_COUNT_COUNT_bf(TC4, TC_COUNT32_COUNT_MASK);
}

enum dcf_isr_result_t process_data(struct sync_state_t *state) {
    if (!usb_power()) {
        return POWER_FAIL;
    }
    bool edge = gpio_get_pin_level(DCF_DATA);
    if (edge == state->prev_edge) {
//        ulog(WARN, "Spurious wakeup")
        return BUSY;
    }

    uint32_t now = dcf_millis();
    uint32_t duration = now - state->dcf_prev_posedge;
    if (edge) { // positive edge
        state->errors >>= 1;
        if (duration > 900 && duration < 1100) {
            // end of second and start of new second
        } else if (duration > 1900 && duration < 2100) {
            // minute mark, capture complete
            if (!state->errors) {
                struct calendar_date_time cal_dt;
                struct date_time_t dt;

                switch (parse_dcf(state->dcf_bits, &dt)) {
                    case 0:
                        dt_to_calendar(&dt, &cal_dt);   // convert to hal format

                        // set RTC:
                        calendar_set_time(&CALENDAR_0, &cal_dt.time);
                        calendar_set_date(&CALENDAR_0, &cal_dt.date);

                        return SYNCED;
                    case DCF_ERR_START:
                        ulog(WARN, "DCF invalid start-of-minute mark")
                        break;
                    case DCF_ERR_START_OF_TIME:
                        ulog(WARN, "DCF invalid start-of-time mark")
                        break;
                    case DCF_ERR_CORRUPT:
                        ulog(WARN, "DCF reception corrupt")
                        break;
                    case DCF_ERR_PARITY_MINUTES:
                    case DCF_ERR_PARITY_HOURS:
                    case DCF_ERR_PARITY_DATE:
                        ulog(WARN, "DCF parity error")
                        break;
                    default:
                        ulog(ERROR, "unknown error in parse_dcf()")
                }
            } else {
                ulog(WARN, "DCF: capture corrupt")
            }
            state->dcf_bits = 0;

        } else {
            ulog(WARN, "DCF pulse length mismatch (%lums)", (unsigned long)duration)
            state->errors |= 0x40000000000;  // bit 42
        }
        state->dcf_prev_posedge = now;

    } else {    // negative edge
        uint64_t bit = 0;
        if (duration > 60 && duration < 140) {          // logic 0
            ulog(INFO, "DCF: 0");
            bit = 0;
        } else if (duration > 160 && duration < 240) {  // logic 1
            ulog(INFO, "DCF: 1");
            bit = 0x400000000000000;                    // bit 58 set to 1
        } else {
            ulog(WARN, "DCF interval length mismatch (%lums)", (unsigned long)duration)
            state->errors |= 0x40000000000;
        }
        state->dcf_bits >>= 1;
        state->dcf_bits |= bit;
    }

    gpio_set_pin_level(LED, edge);
    state->prev_edge = edge;

    return BUSY;
}

enum dcf_error_t dcf_sync(int32_t max_millis) {
    struct sync_state_t sync_state = {.dcf_bits = 0, .dcf_prev_posedge = 0, .errors = 0, .prev_edge = false};

    dcf_init();
    dcf_millis_reset();

    bool timeout;
    enum dcf_isr_result_t result;
    while ((result = process_data(&sync_state)) == BUSY && !(timeout = dcf_millis() > max_millis)) {
        sleep(3);
    }

    dcf_deinit();
    return timeout ? DCF_TIMEOUT : result == SYNCED ? DCF_SUCCESSFUL : DCF_POWER_DOWN;
}
