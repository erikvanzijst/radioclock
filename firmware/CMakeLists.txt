cmake_minimum_required(VERSION 3.22)
project(firmware C)

set(CMAKE_C_STANDARD 11)

add_compile_definitions(__SAMDA1E16B__)

include_directories(.)
include_directories(config)
include_directories(hal/include)
include_directories(hal/utils/include)
include_directories(hpl/core)
include_directories(hpl/gclk)
include_directories(hpl/pm)
include_directories(hpl/port)
include_directories(hri)
include_directories(include)
include_directories(samda1b/include)
include_directories(samda1b/include/component)
include_directories(samda1b/include/instance)
include_directories(samda1b/include/pio)

add_executable(firmware
        atmel_start.c
        stdio_start.c
        hal/utils/src/utils_list.c
        hal/utils/src/utils_assert.c
        hal/utils/src/utils_syscalls.c
        hal/utils/src/utils_event.c
        hal/src/hal_init.c
        hal/src/hal_i2c_m_sync.c
        hal/src/hal_delay.c
        hal/src/hal_ext_irq.c
        hal/src/hal_adc_sync.c
        hal/src/hal_gpio.c
        hal/src/hal_io.c
        hal/src/hal_usart_sync.c
        hal/src/hal_spi_m_sync.c
        hal/src/hal_sleep.c
        hal/src/hal_calendar.c
        hal/src/hal_atomic.c
        hal/src/hal_timer.c
        hpl/sysctrl/hpl_sysctrl.c
        hpl/pm/hpl_pm.c
        hpl/gclk/hpl_gclk.c
        hpl/core/hpl_core_m0plus_base.c
        hpl/core/hpl_init.c
        hpl/dmac/hpl_dmac.c
        hpl/adc/hpl_adc.c
        hpl/sercom/hpl_sercom.c
        hpl/rtc/hpl_rtc.c
        hpl/systick/hpl_systick.c
        hpl/eic/hpl_eic.c
        hpl/tc/hpl_tc.c
        stdio_redirect/iar/read.c
        stdio_redirect/iar/write.c
        stdio_redirect/stdio_io.c
        stdio_redirect/gcc/read.c
        stdio_redirect/gcc/write.c
        stdio_redirect/keil/Retarget.c
        samda1b/gcc/gcc/startup_samda1.c
        samda1b/gcc/system_samda1.c
        main.c
        examples/driver_examples.c
        driver_init.c
        src/dcf.c
        src/switch.c
        src/millis.c
        src/dcf_parser.c
        src/log.c
)
