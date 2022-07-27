#ifndef FIRMWARE_LOG_H
#define FIRMWARE_LOG_H

#include "atmel_start.h"
#include "hal_calendar.h"

#include "utils.h"

enum log_level_t {
    INFO,
    WARN,
    ERROR
};

const char* get_filename(const char* path);

#ifndef LOG_LEVEL
#define LOG_LEVEL INFO
#endif

/**
 * Log a line to UART.
 *
 * @param level     the severity
 * @param format    a format string without newline
 * @param ...       arguments to `format`
 */
#define ulog(___lvl, ___fmt, ...)                                                                                   \
    if (___lvl >= LOG_LEVEL) {                                                                                      \
        struct calendar_date_time ___dt;                                                                            \
        calendar_get_date_time(&CALENDAR_0, &___dt);                                                                \
                                                                                                                    \
        printf("%04d-%02d-%02d %02d:%02d:%02d %s [%s:%u] %s(): " ___fmt "\r\n",                                     \
            ___dt.date.year, ___dt.date.month, ___dt.date.day, ___dt.time.hour, ___dt.time.min, ___dt.time.sec,     \
            ___lvl == INFO ? "INFO" : (___lvl == WARN ? "WARN" : (___lvl == ERROR ? "ERR " : "UNKN")),              \
            get_filename(__FILE__), __LINE__, __FUNCTION__,                                                         \
            ##__VA_ARGS__);                                                                                         \
    }

#endif //FIRMWARE_LOG_H
