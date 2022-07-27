#include <string.h>
#include "atmel_start.h"
#include "display.h"

struct io_descriptor *spi_io;
static struct timer_task TIMER_0_display_task;
struct calendar_date_time prev_dt;
int8_t dp_countdown = 0;

void update_display(const struct timer_task *const timer_task) {
    struct calendar_date_time dt;
    uint8_t text[16];

    calendar_get_date_time(&CALENDAR_0, &dt);
    if (dt.time.sec != prev_dt.time.sec) {
        dp_countdown = (int8_t)((1000 / timer_task->interval) / 2);
    }

    text[0] = dt.date.year / 1000 % 10;
    text[1] = dt.date.year / 100 % 10;
    text[2] = dt.date.year / 10 % 10;
    text[3] = dt.date.year % 10;
    text[4] = 10;
    text[5] = dt.date.month / 10 % 10;
    text[6] = dt.date.month % 10;
    text[7] = 10;
    text[8] = dt.date.day / 10 % 10;
    text[9] = dt.date.day % 10;

    text[10] = (dt.time.hour / 10 % 10) | (dp_countdown > 0 ? 0x80 : 0x0);
    text[11] = (dt.time.hour % 10)      | (dp_countdown > 0 ? 0x80 : 0x0);
    text[12] = (dt.time.min / 10 % 10)  | (dp_countdown > 0 ? 0x80 : 0x0);
    text[13] = (dt.time.min % 10)       | (dp_countdown > 0 ? 0x80 : 0x0);
    text[14] = dt.time.sec / 10 % 10;
    text[15] = dt.time.sec % 10;

    // TODO: do differential updates
    for (uint8_t i = 1; i <= 8; i++) {
        gpio_set_pin_level(DSPL_SS, false);
        io_write(spi_io, (uint8_t *)((uint8_t[]){i, text[i+8-1], i, text[i-1]}), 4);
        gpio_set_pin_level(DSPL_SS, true);
    }
    // TODO: LDR and intensity

    dp_countdown--;
    memcpy(&prev_dt, &dt, sizeof(dt));
}

int32_t display_init(void) {

    spi_m_sync_get_io_descriptor(&SPI_0, &spi_io);
    spi_m_sync_enable(&SPI_0);

    // Display initialization
    gpio_set_pin_level(DSPL_SS, false); // leave shutdown mode:
    io_write(spi_io, (uint8_t *)((uint8_t[]){0x0c, 0x01, 0x0c, 0x01}), 4);
    gpio_set_pin_level(DSPL_SS, true);

    gpio_set_pin_level(DSPL_SS, false); // set initial intensity:
    io_write(spi_io, (uint8_t *)((uint8_t[]){0x0a, 0x08, 0x0a, 0x08}), 4);
    gpio_set_pin_level(DSPL_SS, true);

    gpio_set_pin_level(DSPL_SS, false); // set scan-limit register to all segments:
    io_write(spi_io, (uint8_t *)((uint8_t[]){0x0b, 0x07, 0x0b, 0x07}), 4);
    gpio_set_pin_level(DSPL_SS, true);

    gpio_set_pin_level(DSPL_SS, false); // enable BCD decode mode:
    io_write(spi_io, (uint8_t *)((uint8_t[]){0x09, 0xff, 0x09, 0xff}), 4);
    gpio_set_pin_level(DSPL_SS, true);

    TIMER_0_display_task.interval = 100;     // 10Hz
    TIMER_0_display_task.cb = update_display;
    TIMER_0_display_task.mode = TIMER_TASK_REPEAT;

    return timer_add_task(&TIMER_0, &TIMER_0_display_task);
}

int32_t display_deinit(void) {
    return timer_remove_task(&TIMER_0, &TIMER_0_display_task);
}
