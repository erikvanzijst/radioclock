#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "dcf_parser.h"
#include "font.h"

/**
 * 64-bit version of strtol (even when compiled for 32bit systems).
 *
 * @param binstr    a (mutable) binary string (e.g. "00101101")
 * @return          the 64-bit unsigned integer value
 */
uint64_t bin2int(char *binstr) {
    uint64_t total = 0;
    while (*binstr) {
        total *= 2;
        if (*binstr++ == '1') {
            total += 1;
        }
    }
    return total;
}

/**
 * In-place string reverse.
 */
void str_reverse(char *str) {
    // Possibly lazily ripped from https://stackoverflow.com/a/784538

    if (str) {
        char * end = str + strlen(str) - 1;

        // walk inwards from both ends of the string,
        // swapping until we get to the middle
        while (str < end) {
            // swap the values in the two given variables
            *str ^= *end;
            *end ^= *str;
            *str ^= *end;

            str++;
            end--;
        }
    }
}

int main(int argc, char **argv) {
    struct date_time_t dt;

    printf("Running tests...\n");

    assert(0 == parity(0));
    assert(0 == parity(0x6996));
    assert(0 == parity(0x69966996));
    assert(0 == parity(0xffffffff));

    assert(1 == parity(1));
    assert(1 == parity(0x80000000));
    assert(1 == parity(0x69b66996));
    assert(1 == parity(0x7fffffff));

    char ts1[] = "00111101000100100100110100101110001101000110111100010001001";  // 2022-07-22 23:25
    str_reverse(ts1);
    assert(!parse_dcf(strtol(ts1, NULL, 2), &dt));
    assert(2022 == dt.year);
    assert(7 == dt.month);
    assert(22 == dt.day);
    assert(23 == dt.hour);
    assert(25 == dt.min);

    char ts2[] = "01001100100010000100101001000000110011000101111100010001000";   // 2022-07-23 18:12
    str_reverse(ts2);
    assert(!parse_dcf(strtol(ts2, NULL, 2), &dt));
    assert(2022 == dt.year);
    assert(7 == dt.month);
    assert(23 == dt.day);
    assert(18 == dt.hour);
    assert(12 == dt.min);

    // Corrupt first bit marker
    char ts3[] = "11001100100010000100101001000000110011000101111100010001000";
    str_reverse(ts3);
    assert(DCF_ERR_START == parse_dcf(strtol(ts3, NULL, 2), &dt));

    // Corrupt start of time marker
    char ts4[] = "01001100100010000100001001000000110011000101111100010001000";
    str_reverse(ts4);
    assert(DCF_ERR_START_OF_TIME == parse_dcf(strtol(ts4, NULL, 2), &dt));

    // Parity error on minutes
    char ts5[] = "01001100100010000100101001001000110011000101111100010001000";
    str_reverse(ts5);
    assert(DCF_ERR_PARITY_MINUTES == parse_dcf(strtol(ts5, NULL, 2), &dt));

    // Parity error on hours
    char ts6[] = "01001100100010000100101001000000110111000101111100010001000";
    str_reverse(ts6);
    assert(DCF_ERR_PARITY_HOURS == parse_dcf(strtol(ts6, NULL, 2), &dt));

    // Parity error on date
    char ts7[] = "01001100100010000100101001000000110011000101111100010001001";
    str_reverse(ts7);
    assert(DCF_ERR_PARITY_DATE == parse_dcf(strtol(ts7, NULL, 2), &dt));


    // 7-segment font
    char *str = "0123456789";
    char buf[11];
    encode(str, buf);
    assert(0 == memcmp((char[]){126, 48, 109, 121, 51, 91, 95, 112, 127, 123, 0}, buf, strlen(str)));

    printf("All tests passed!\n");
    return 0;
}
