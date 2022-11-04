#ifndef FIRMWARE_TIMEZONE_H
#define FIRMWARE_TIMEZONE_H

#ifdef __TZ__
typedef enum {
    LON = 0,
    AMS = 1,
    HEL = 2
} timezone_t;

/*
 * Current timezone (reflects the state of the timezone jumper).
 */
extern volatile timezone_t timezone;

int32_t timezone_init();

const char * get_tz_name(timezone_t tz);

#endif // __TZ__
#endif //FIRMWARE_TIMEZONE_H
