#include "atmel_start.h"
#include "hal_calendar.h"

#include "dcf.h"
#include "dcf_parser.h"
#include "log.h"
#include "millis.h"

volatile uint64_t dcf_bits = 0;
volatile uint64_t dcf_prev_posedge = 0;
bool dcf_corrupt = true;
void (*sync_callback)(struct calendar_date_time *cal_dt) = NULL;
void (*fail_cb)(void) = NULL;

struct calendar_date_time * dt_to_calendar(struct date_time_t *dt, struct calendar_date_time *cal_dt) {
    cal_dt->time.sec = dt->sec;
    cal_dt->time.min = dt->min;
    cal_dt->time.hour = dt->hour;
    cal_dt->date.day = dt->day;
    cal_dt->date.month = dt->month;
    cal_dt->date.year = dt->year;

    return cal_dt;
}

void dcf_init(void (* sync_cb)(struct calendar_date_time *cal_dt)) {
    sync_callback = sync_cb;

    // trigger fast DCF fast sync:
    gpio_set_pin_level(DCF_PDN, true);
    delay_ms(100);
    gpio_set_pin_level(DCF_PDN, false);

    // Register interrupt handler on DCF_DATA pin
    ext_irq_register(PIN_PA06, dcf_data_isr);
}

void dcf_data_isr(void) {
    bool edge = gpio_get_pin_level(DCF_DATA);
    uint64_t now = millis();
    uint64_t duration = now - dcf_prev_posedge;

    if (edge) { // positive edge
        if (duration > 900 && duration < 1100) {
            // end of second and start of new second
        } else if (duration > 1900 && duration < 2100) {
            // minute mark, capture complete
            if (!dcf_corrupt) {
                struct calendar_date_time cal_dt;
                struct date_time_t dt;

                switch (parse_dcf(dcf_bits, &dt)) {
                    case 0:
                        dt_to_calendar(&dt, &cal_dt);   // convert to hal format

                        // set RTC:
                        calendar_set_time(&CALENDAR_0, &cal_dt.time);
                        calendar_set_date(&CALENDAR_0, &cal_dt.date);

                        // notify listeners:
                        if (sync_callback) sync_callback(&cal_dt);
                        break;
                    case DCF_ERR_START:
                        ulog(WARN, "DCF invalid start-of-minute mark");
                        break;
                    case DCF_ERR_START_OF_TIME:
                        ulog(WARN, "DCF invalid start-of-time mark");
                        break;
                    case DCF_ERR_CORRUPT:
                        ulog(WARN, "DCF reception corrupt");
                        break;
                    case DCF_ERR_PARITY_MINUTES:
                    case DCF_ERR_PARITY_HOURS:
                    case DCF_ERR_PARITY_DATE:
                        ulog(WARN, "DCF parity error");
                        break;
                    default:
                        ulog(ERROR, "unknown error in parse_dcf()");
                }
            } else {
                ulog(WARN, "DCF: capture corrupt");
            }
            dcf_corrupt = false;
            dcf_bits = 0;

        } else {
            ulog(WARN, "DCF pulse length mismatch (%lums)", (unsigned long)duration);
            dcf_corrupt = true;
        }
        dcf_prev_posedge = now;

    } else {    // negative edge
        uint64_t bit = 0;
        if (duration > 60 && duration < 140) {          // logic 0
            ulog(INFO, "DCF: 0");
            bit = 0;
        } else if (duration > 160 && duration < 240) {  // logic 1
            ulog(INFO, "DCF: 1");
            bit = 0x400000000000000;                    // bit 58 set to 1
        } else {
            ulog(WARN, "DCF interval length mismatch (%lums)", (unsigned long)duration);
            dcf_corrupt = true;
        }
        dcf_bits >>= 1;
        dcf_bits |= bit;
    }

    gpio_set_pin_level(LED, edge);
}
