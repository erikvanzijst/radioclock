#ifndef FIRMWARE_LOG_H
#define FIRMWARE_LOG_H

#include "atmel_start.h"
#include "hal_calendar.h"

#include "utils.h"

enum log_level_t {
    VERBOSE,
    INFO,
    WARN,
    ERROR
};

#ifndef LOG_LEVEL
#define LOG_LEVEL INFO
#endif

void _ulog(enum log_level_t lvl, const char *filename, uint16_t line, const char *function, const char * restrict fmt, ...);

/**
 * Log a line to UART.
 *
 * @param level     the severity
 * @param format    a format string without newline
 * @param ...       arguments to `format`
 */
#define ulog(___lvl, ___fmt, ...)                                                                                   \
    if (___lvl >= LOG_LEVEL) {                                                                                      \
        _ulog(___lvl, get_filename(__FILE__), __LINE__, __FUNCTION__, ___fmt, ##__VA_ARGS__);                       \
    }

#endif //FIRMWARE_LOG_H
