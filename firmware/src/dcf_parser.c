#include <stdint.h>
#include "dcf_parser.h"

#define BIT_VAL(bits, n) ((bits >> n) & 0x1)

int32_t parse_dcf(uint64_t bits, struct date_time_t *const date_time) {
    if (bits & 0x01) {
        return DCF_ERR_START;
    } else if (!parity((uint32_t)((bits >> 17) & 0x3))) {
        return DCF_ERR_PARITY_DST;
    } else if (((bits >> 20) & 0x01) != 1) {
        return DCF_ERR_START_OF_TIME;
    } else if (parity((uint32_t)((bits >> 21) & 0xff))) {
        return DCF_ERR_PARITY_MINUTES;
    } else if (parity((uint32_t)((bits >> 29) & 0x7f))) {
        return DCF_ERR_PARITY_HOURS;
    } else if (parity((uint32_t)((bits >> 36) & 0x7FFFFF))) {
        return DCF_ERR_PARITY_DATE;
    }

    date_time->sec   = 0;
    date_time->min   = 1 * BIT_VAL(bits, 21) + 2 * BIT_VAL(bits, 22) + 4 * BIT_VAL(bits, 23) + 8 * BIT_VAL(bits, 24) + \
                      10 * BIT_VAL(bits, 25) + 20 * BIT_VAL(bits, 26) + 40 * BIT_VAL(bits, 27);
    date_time->hour  = 1 * BIT_VAL(bits, 29) + 2 * BIT_VAL(bits, 30) + 4 * BIT_VAL(bits, 31) + 8 * BIT_VAL(bits, 32) + \
                      10 * BIT_VAL(bits, 33) + 20 * BIT_VAL(bits, 34);
    date_time->day   = 1 * BIT_VAL(bits, 36) + 2 * BIT_VAL(bits, 37) + 4 * BIT_VAL(bits, 38) + 8 * BIT_VAL(bits, 39) + \
                      10 * BIT_VAL(bits, 40) + 20 * BIT_VAL(bits, 41);
//    uint8_t day_of_week = 1 * BIT_VAL(bits, 42) + 2 * BIT_VAL(bits, 43) + 4 * BIT_VAL(bits, 44);    // unused
    date_time->month = 1 * BIT_VAL(bits, 45) + 2 * BIT_VAL(bits, 46) + 4 * BIT_VAL(bits, 47) + 8 * BIT_VAL(bits, 48) + \
                      10 * BIT_VAL(bits, 49);
    date_time->year  = 2000 + \
                       1 * BIT_VAL(bits, 50) + 2 * BIT_VAL(bits, 51) + 4 * BIT_VAL(bits, 52) + 8 * BIT_VAL(bits, 53) + \
                      10 * BIT_VAL(bits, 54) + 20 * BIT_VAL(bits, 55) + 40 * BIT_VAL(bits, 56) + 80 * BIT_VAL(bits, 57);
    date_time->dst   = BIT_VAL(bits, 17);

    return 0;
}
