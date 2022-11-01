#include <atmel_start.h>
#include "atmel_start_pins.h"
#include "hal_delay.h"

#include "version.h"
#include "dcf.h"
#include "millis.h"
#include "log.h"
#include "ldr.h"
#include "peripherals.h"
#include "power.h"
#include "switch.h"

// max time of a sync interval (5 minutes plus 10 seconds margin for early start)
#define MAX_SYNC_MILLIS (5 * 60 * 1000 + 10000)

volatile uint64_t last_dcf_sync = 0;
volatile bool do_sync = true;

static struct calendar_alarm alarm = {
        .cal_alarm.mode = ONESHOT,
        .cal_alarm.option = CALENDAR_ALARM_MATCH_HOUR,
        .cal_alarm.datetime.time.hour = 3,
        .cal_alarm.datetime.time.min = 1,
        .cal_alarm.datetime.time.sec = 0,
};

static void init_sync(struct calendar_descriptor *const descr) {
    do_sync = true;
}

void short_press(void) {
    ulog(INFO, "DCF time sync requested")
    last_dcf_sync = 0;
    do_sync = true;
}

void long_press(void) {
    ulog(INFO, "Resetting MCU...")
    _reset_mcu();
}

void schedule_sync() {
    calendar_set_alarm(&CALENDAR_0, &alarm, init_sync);
    ulog(INFO, "Next sync at: %04d-%02d-%02d %02d:%02d:%02d",
         alarm.cal_alarm.datetime.date.year, alarm.cal_alarm.datetime.date.month, alarm.cal_alarm.datetime.date.day,
         alarm.cal_alarm.datetime.time.hour, alarm.cal_alarm.datetime.time.min, alarm.cal_alarm.datetime.time.sec)
}

int main(void) {
    struct io_descriptor *uart_io;

    atmel_start_init();
    gpio_set_pin_level(LED, false);
    gpio_set_pin_level(PERIPHERAL_CTL, true);   // turn off power to displays and DHT20 module
    gpio_set_pin_level(DCF_CTL, true);          // turn off power to DCF77 module
    gpio_set_pin_level(LDR_SINK, false);        // enable LDR

    if (timer_start(&TIMER_0)) ulog(ERROR, "Failed to start TIMER_0!")
    if (timer_start(&TIMER_1)) ulog(ERROR, "Failed to start TIMER_1!")

    usart_sync_get_io_descriptor(&USART_0, &uart_io);
    usart_sync_enable(&USART_0);

    if (pwr_init()) {
        ulog(ERROR, "pwr_init() failed")
    }
    if (millis_init()) {
        ulog(ERROR, "millis_init() failed")
    }
    calendar_enable(&CALENDAR_0);
    ldr_init();
    switch_init(short_press, long_press);

    printf("\r\n\r\n");
    ulog(INFO, "Radio clock firmware build: %s", VERSION_STR)
    ulog(INFO, "https://github.com/erikvanzijst/radioclock")
    ulog(INFO, "Erik van Zijst <erik.van.zijst@gmail.com>\r\n")

    power_up_peripherals();

    for (;;) {
        if (do_sync) {
            ulog(INFO, "Starting time sync...")
            power_down_peripherals();

            struct calendar_date_time dt;
            switch (dcf_sync(last_dcf_sync ? MAX_SYNC_MILLIS : 0x7FFFFFFF)) {
                case DCF_SUCCESSFUL:
                    last_dcf_sync = millis();
                    calendar_get_date_time(&CALENDAR_0, &dt);
                    ulog(INFO, "Time sync: %04d-%02d-%02d %02d:%02d:00", dt.date.year, dt.date.month, dt.date.day, dt.time.hour, dt.time.min)
                    break;
                case DCF_TIMEOUT:
                    ulog(WARN, "Time sync failed (tried for %u sec)", MAX_SYNC_MILLIS / 1000)
                    break;
                case DCF_POWER_DOWN:
                    ulog(ERROR, "Time sync interrupted by USB power fail")
                    break;
            }

            power_up_peripherals();
            do_sync = false;
            schedule_sync();
        }

        if (!usb_power()) {
            power_down_peripherals();
            while (!usb_power()) {
                ulog(INFO, "Entering STANDBY...")
                sleep(3);
            }
            power_up_peripherals();
            schedule_sync();
        }
    }
}
