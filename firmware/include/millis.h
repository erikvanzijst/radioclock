#ifndef FIRMWARE_MILLIS_H
#define FIRMWARE_MILLIS_H

#include <stdint.h>

/**
 * Call this once at system initialization to start the wallclock (this is not the RTC).
 */
void millis_init();

/**
 * @return  the number of milliseconds since boot.
 */
uint64_t millis();

#endif //FIRMWARE_MILLIS_H
