#include <stdarg.h>
#include "hal_calendar.h"
#include "log.h"

void _ulog(enum log_level_t lvl, const char *filename, uint16_t line, const char *function, const char * restrict fmt, ...) {
    struct calendar_date_time dt;
    calendar_get_date_time(&CALENDAR_0, &dt);
    printf("%04d-%02d-%02d %02d:%02d:%02d %s [%s:%u] %s(): ",
           dt.date.year, dt.date.month, dt.date.day, dt.time.hour, dt.time.min, dt.time.sec,
           lvl == VERBOSE ? "VERBOSE" : lvl == INFO ? "INFO" : (lvl == WARN ? "WARN" : (lvl == ERROR ? "ERR " : "UNKN")),
           get_filename(filename), line, function);

    va_list argptr;
    va_start(argptr, fmt);
    vprintf(fmt, argptr);
    va_end(argptr);

    putchar('\r');
    putchar('\n');
}
