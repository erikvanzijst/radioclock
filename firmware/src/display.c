#include <string.h>
#include "atmel_start.h"
#include "display.h"
#include "ldr.h"
#include "log.h"

struct io_descriptor *spi_io;
static struct timer_task TIMER_0_display_task;
struct calendar_date_time prev_dt;
int8_t dp_countdown = 0;

enum DSP_ERR {
    DSP_ERR_NONE = 0,
    DSP_ERR_TX_FULL = -1
};

struct tx_queue_t {
    uint8_t buf[16][4];
    uint8_t head;
    uint8_t tail;
};
struct tx_queue_t tx_queue = {.head = 0, .tail = 0};

int32_t start_next_command(void) {
    bool empty = tx_queue.head == tx_queue.tail;
    if (!empty) {
        gpio_set_pin_level(DSPL_SS, false);
        io_write(spi_io, (uint8_t *)(tx_queue.buf[tx_queue.tail]), 4);
    }
    return DSP_ERR_NONE;
}

int32_t enqueue_command(uint8_t *bytes) {
    CRITICAL_SECTION_ENTER()
    if (((tx_queue.head + 1) & 0xf) == tx_queue.tail) {
        return DSP_ERR_TX_FULL;
    } else {
        bool empty = tx_queue.head == tx_queue.tail;
        memcpy(tx_queue.buf[tx_queue.head++], bytes, 4);
        tx_queue.head &= 0xf;

        if (empty) {
            start_next_command();
        }
    }
    CRITICAL_SECTION_LEAVE()
    return DSP_ERR_NONE;
}

static void complete_cb_SPI_0(const struct spi_m_async_descriptor *const io_descr) {
    CRITICAL_SECTION_ENTER()
    gpio_set_pin_level(DSPL_SS, true);
    tx_queue.tail++;
    tx_queue.tail &= 0xf;
    start_next_command();
    CRITICAL_SECTION_LEAVE()
}

void update_display(const struct timer_task *const timer_task) {
    struct calendar_date_time dt;
    uint8_t text[16];
    int32_t err;

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

    for (uint8_t i = 1; i <= 8; i++) {
        // TODO: do differential updates
        if ((err = enqueue_command((uint8_t[]){i, text[i+8-1], i, text[i-1]}))) {
            ulog(ERROR, "enqueue_command() failed (%d)", err)
        }
    }

    // set brightness
    if ((err = enqueue_command((uint8_t[]){0x0a, 0xff - (ldr[0] >> 4), 0x0a, 0xff - (ldr[0] >> 4)}))) {
        ulog(ERROR, "enqueue_command() failed (%d)", err)
    }

    dp_countdown--;
    memcpy(&prev_dt, &dt, sizeof(dt));
}

int32_t display_init(void) {
    int32_t err;
    spi_m_async_get_io_descriptor(&SPI_0, &spi_io);
    spi_m_async_register_callback(&SPI_0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)complete_cb_SPI_0);
    spi_m_async_enable(&SPI_0);

    if ((err = (
            // leave shutdown mode:
            enqueue_command((uint8_t[]){0x0c, 0x01, 0x0c, 0x01}) |
            // set initial intensity:
            enqueue_command((uint8_t[]){0x0a, 0x08, 0x0a, 0x08}) |
            // set scan-limit register to all segments:
            enqueue_command((uint8_t[]){0x0b, 0x07, 0x0b, 0x07}) |
            // enable BCD decode mode:
            enqueue_command((uint8_t[]){0x09, 0xff, 0x09, 0xff})))) {

        ulog(ERROR, "enqueue_command() failed (%d)", err)
    }

    TIMER_0_display_task.interval = 50;     // 20Hz
    TIMER_0_display_task.cb = update_display;
    TIMER_0_display_task.mode = TIMER_TASK_REPEAT;

    return timer_add_task(&TIMER_0, &TIMER_0_display_task);
}

int32_t display_deinit(void) {
    return timer_remove_task(&TIMER_0, &TIMER_0_display_task);
}
