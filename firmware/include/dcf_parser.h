#ifndef FIRMWARE_DCF_PARSER_H
#define FIRMWARE_DCF_PARSER_H

#include <stdint.h>

enum DCF_ERR {
    DCF_ERR_NONE = 0,
    DCF_ERR_START = -1,
    DCF_ERR_START_OF_TIME = -2,
    DCF_ERR_PARITY_MINUTES = -3,
    DCF_ERR_PARITY_HOURS = -4,
    DCF_ERR_PARITY_DATE = -5,
    DCF_ERR_CORRUPT = -6
};

struct date_time_t {
    /*range from 0 to 59*/
    uint8_t sec;
    /*range from 0 to 59*/
    uint8_t min;
    /*range from 0 to 23*/
    uint8_t hour;
    /*range from 1 to 28/29/30/31*/
    uint8_t day;
    /*range from 1 to 12*/
    uint8_t month;
    /*absolute year>= 1970(such as 2000)*/
    uint16_t year;
};

uint32_t parity(uint32_t v);

int32_t parse_dcf(uint64_t bits, struct date_time_t *const date_time);

#endif //FIRMWARE_DCF_PARSER_H
