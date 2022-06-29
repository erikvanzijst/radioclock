/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_init.h"
#include <peripheral_clk_config.h>
#include <utils.h>
#include <hal_init.h>
#include <hpl_gclk_base.h>
#include <hpl_pm_base.h>

struct i2c_m_sync_desc I2C_0;

struct usart_sync_descriptor USART_0;

struct calendar_descriptor CALENDAR_0;

void I2C_0_PORT_init(void)
{

	gpio_set_pin_pull_mode(DHT_SDA,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(DHT_SDA, PINMUX_PA16C_SERCOM1_PAD0);

	gpio_set_pin_pull_mode(DHT_SCL,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(DHT_SCL, PINMUX_PA17C_SERCOM1_PAD1);
}

void I2C_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM1);
	_gclk_enable_channel(SERCOM1_GCLK_ID_CORE, CONF_GCLK_SERCOM1_CORE_SRC);
	_gclk_enable_channel(SERCOM1_GCLK_ID_SLOW, CONF_GCLK_SERCOM1_SLOW_SRC);
}

void I2C_0_init(void)
{
	I2C_0_CLOCK_init();
	i2c_m_sync_init(&I2C_0, SERCOM1);
	I2C_0_PORT_init();
}

void USART_0_PORT_init(void)
{

	gpio_set_pin_function(PA22, PINMUX_PA22C_SERCOM3_PAD0);

	gpio_set_pin_function(PA23, PINMUX_PA23C_SERCOM3_PAD1);
}

void USART_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBC, SERCOM3);
	_gclk_enable_channel(SERCOM3_GCLK_ID_CORE, CONF_GCLK_SERCOM3_CORE_SRC);
}

void USART_0_init(void)
{
	USART_0_CLOCK_init();
	usart_sync_init(&USART_0, SERCOM3, (void *)NULL);
	USART_0_PORT_init();
}

void delay_driver_init(void)
{
	delay_init(SysTick);
}

void CALENDAR_0_CLOCK_init(void)
{
	_pm_enable_bus_clock(PM_BUS_APBA, RTC);
	_gclk_enable_channel(RTC_GCLK_ID, CONF_GCLK_RTC_SRC);
}

void CALENDAR_0_init(void)
{
	CALENDAR_0_CLOCK_init();
	calendar_init(&CALENDAR_0, RTC);
}

void system_init(void)
{
	init_mcu();

	// GPIO on PA00

	// Set pin direction to input
	gpio_set_pin_direction(XIN32, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(XIN32,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(XIN32, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA02

	// Disable digital pin circuitry
	gpio_set_pin_direction(LDR, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(LDR, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA03

	gpio_set_pin_level(DCF_CTL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(DCF_CTL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(DCF_CTL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA04

	gpio_set_pin_level(PERIPHERAL_CTL,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(PERIPHERAL_CTL, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(PERIPHERAL_CTL, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA05

	// Disable digital pin circuitry
	gpio_set_pin_direction(PWR_SENSE, GPIO_DIRECTION_OFF);

	gpio_set_pin_function(PWR_SENSE, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA06

	// Set pin direction to input
	gpio_set_pin_direction(DCF_DATA, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(DCF_DATA,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_OFF);

	gpio_set_pin_function(DCF_DATA, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA07

	gpio_set_pin_level(DCF_PDN,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(DCF_PDN, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(DCF_PDN, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA08

	gpio_set_pin_level(DSPL_DO,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(DSPL_DO, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(DSPL_DO, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA09

	gpio_set_pin_level(DSPL_SCK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(DSPL_SCK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(DSPL_SCK, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA10

	gpio_set_pin_level(DSPL_SS,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   false);

	// Set pin direction to output
	gpio_set_pin_direction(DSPL_SS, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(DSPL_SS, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA14

	gpio_set_pin_level(LED,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LED, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LED, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA15

	// Set pin direction to input
	gpio_set_pin_direction(SWITCH, GPIO_DIRECTION_IN);

	gpio_set_pin_pull_mode(SWITCH,
	                       // <y> Pull configuration
	                       // <id> pad_pull_config
	                       // <GPIO_PULL_OFF"> Off
	                       // <GPIO_PULL_UP"> Pull-up
	                       // <GPIO_PULL_DOWN"> Pull-down
	                       GPIO_PULL_UP);

	gpio_set_pin_function(SWITCH, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA24

	gpio_set_pin_function(USB_N, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA25

	gpio_set_pin_function(USB_P, GPIO_PIN_FUNCTION_OFF);

	// GPIO on PA27

	gpio_set_pin_level(LDR_SINK,
	                   // <y> Initial level
	                   // <id> pad_initial_level
	                   // <false"> Low
	                   // <true"> High
	                   true);

	// Set pin direction to output
	gpio_set_pin_direction(LDR_SINK, GPIO_DIRECTION_OUT);

	gpio_set_pin_function(LDR_SINK, GPIO_PIN_FUNCTION_OFF);

	I2C_0_init();

	USART_0_init();

	delay_driver_init();

	CALENDAR_0_init();
}
