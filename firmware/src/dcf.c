#include <stdio.h>

#include "atmel_start_pins.h"
#include "hal_calendar.h"

#include "dcf.h"
#include "dcf_parser.h"
#include "millis.h"
#include "atmel_start.h"

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
                        printf("Time sync: %04d-%02d-%02d %02d:%02d:00\r\n", dt.year, dt.month, dt.day, dt.hour, dt.min);
                        set_rtc(&dt);
                        break;
                    case DCF_ERR_START:
                        printf("ERR: DCF invalid start-of-minute mark\r\n");
                        break;
                    case DCF_ERR_START_OF_TIME:
                        printf("ERR: DCF invalid start-of-time mark\r\n");
                        break;
                    case DCF_ERR_CORRUPT:
                        printf("ERR: DCF reception corrupt\r\n");
                        break;
                    case DCF_ERR_PARITY_MINUTES:
                    case DCF_ERR_PARITY_HOURS:
                    case DCF_ERR_PARITY_DATE:
                        printf("ERR: DCF parity error\r\n");
                        break;
                    default:
                        printf("ERR: unknown error in parse_dcf()\r\n");
                }
            } else {
                printf("DCF capture corrupt.\r\n");
            }
            dcf_corrupt = false;
            dcf_bits = 0;

        } else {
            printf("ERR: DCF pulse length mismatch (%lums)\r\n", (unsigned long)duration);
            dcf_corrupt = true;
        }
        dcf_prev_posedge = now;

    } else {    // negative edge
        uint64_t bit = 0;
        if (duration > 60 && duration < 140) {          // logic 0
            printf("DCF: 0\r\n");
            bit = 0;
        } else if (duration > 160 && duration < 240) {  // logic 1
            printf("DCF: 1\r\n");
            bit = 0x400000000000000;                    // bit 58 set to 1
        } else {
            printf("ERR: DCF interval length mismatch (%lums)\r\n", (unsigned long)duration);
            dcf_corrupt = true;
        }
        dcf_bits >>= 1;
        dcf_bits |= bit;
    }

    gpio_set_pin_level(LED, edge);
}
