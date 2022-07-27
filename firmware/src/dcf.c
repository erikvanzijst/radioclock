#include "atmel_start.h"
#include "atmel_start_pins.h"
#include "hal_calendar.h"

#include "dcf.h"
#include "dcf_parser.h"
#include "log.h"
#include "millis.h"

uint64_t dcf_bits = 0;
uint64_t dcf_prev_posedge = 0;
bool dcf_corrupt = true;

void set_rtc(struct date_time_t *dt) {
    struct calendar_time time;
    struct calendar_date date;

    time.sec = 0;
    time.min = dt->min;
    time.hour = dt->hour;
    date.day = dt->day;
    date.month = dt->month;
    date.year = dt->year;

    calendar_set_time(&CALENDAR_0, &time);
    calendar_set_date(&CALENDAR_0, &date);
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
                struct date_time_t dt;
                switch (parse_dcf(dcf_bits, &dt)) {
                    case 0:
                        ulog(INFO, "Time sync: %04d-%02d-%02d %02d:%02d:00", dt.year, dt.month, dt.day, dt.hour, dt.min);
                        set_rtc(&dt);
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
                ulog(WARN, "DCF capture corrupt");
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
