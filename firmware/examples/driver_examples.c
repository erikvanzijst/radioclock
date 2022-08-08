/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */

#include "driver_examples.h"
#include "driver_init.h"
#include "utils.h"

static void convert_cb_ADC_0(const struct adc_async_descriptor *const descr, const uint8_t channel)
{
}

/**
 * Example of using ADC_0 to generate waveform.
 */
void ADC_0_example(void)
{
	adc_async_register_callback(&ADC_0, 0, ADC_ASYNC_CONVERT_CB, convert_cb_ADC_0);
	adc_async_enable_channel(&ADC_0, 0);
	adc_async_start_conversion(&ADC_0);
}

static void button_on_PA05_pressed(void)
{
}

static void button_on_PA06_pressed(void)
{
}

static void button_on_PA15_pressed(void)
{
}

/**
 * Example of using EXTERNAL_IRQ_0
 */
void EXTERNAL_IRQ_0_example(void)
{

	ext_irq_register(PIN_PA05, button_on_PA05_pressed);
	ext_irq_register(PIN_PA06, button_on_PA06_pressed);
	ext_irq_register(PIN_PA15, button_on_PA15_pressed);
}

/**
 * Example of using SPI_0 to write "Hello World" using the IO abstraction.
 *
 * Since the driver is asynchronous we need to use statically allocated memory for string
 * because driver initiates transfer and then returns before the transmission is completed.
 *
 * Once transfer has been completed the tx_cb function will be called.
 */

static uint8_t example_SPI_0[12] = "Hello World!";

static void complete_cb_SPI_0(const struct spi_m_async_descriptor *const io_descr)
{
	/* Transfer completed */
}

void SPI_0_example(void)
{
	struct io_descriptor *io;
	spi_m_async_get_io_descriptor(&SPI_0, &io);

	spi_m_async_register_callback(&SPI_0, SPI_M_ASYNC_CB_XFER, (FUNC_PTR)complete_cb_SPI_0);
	spi_m_async_enable(&SPI_0);
	io_write(io, example_SPI_0, 12);
}

static uint8_t I2C_0_example_str[12] = "Hello World!";

void I2C_0_tx_complete(struct i2c_m_async_desc *const i2c)
{
}

void I2C_0_example(void)
{
	struct io_descriptor *I2C_0_io;

	i2c_m_async_get_io_descriptor(&I2C_0, &I2C_0_io);
	i2c_m_async_enable(&I2C_0);
	i2c_m_async_register_callback(&I2C_0, I2C_M_ASYNC_TX_COMPLETE, (FUNC_PTR)I2C_0_tx_complete);
	i2c_m_async_set_slaveaddr(&I2C_0, 0x12, I2C_M_SEVEN);

	io_write(I2C_0_io, I2C_0_example_str, 12);
}

/**
 * Example of using USART_0 to write "Hello World" using the IO abstraction.
 */
void USART_0_example(void)
{
	struct io_descriptor *io;
	usart_sync_get_io_descriptor(&USART_0, &io);
	usart_sync_enable(&USART_0);

	io_write(io, (uint8_t *)"Hello World!", 12);
}

void delay_example(void)
{
	delay_ms(5000);
}

/**
 * Example of using CALENDAR_0.
 */
static struct calendar_alarm alarm;

static void alarm_cb(struct calendar_descriptor *const descr)
{
	/* alarm expired */
}

void CALENDAR_0_example(void)
{
	struct calendar_date date;
	struct calendar_time time;

	calendar_enable(&CALENDAR_0);

	date.year  = 2000;
	date.month = 12;
	date.day   = 31;

	time.hour = 12;
	time.min  = 59;
	time.sec  = 59;

	calendar_set_date(&CALENDAR_0, &date);
	calendar_set_time(&CALENDAR_0, &time);

	alarm.cal_alarm.datetime.time.sec = 4;
	alarm.cal_alarm.option            = CALENDAR_ALARM_MATCH_SEC;
	alarm.cal_alarm.mode              = REPEAT;

	calendar_set_alarm(&CALENDAR_0, &alarm, alarm_cb);
}

static struct timer_task TIMER_0_task1, TIMER_0_task2;

/**
 * Example of using TIMER_0.
 */
static void TIMER_0_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_0_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_0_example(void)
{
	TIMER_0_task1.interval = 100;
	TIMER_0_task1.cb       = TIMER_0_task1_cb;
	TIMER_0_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_0_task2.interval = 200;
	TIMER_0_task2.cb       = TIMER_0_task2_cb;
	TIMER_0_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_0, &TIMER_0_task1);
	timer_add_task(&TIMER_0, &TIMER_0_task2);
	timer_start(&TIMER_0);
}

static struct timer_task TIMER_1_task1, TIMER_1_task2;

/**
 * Example of using TIMER_1.
 */
static void TIMER_1_task1_cb(const struct timer_task *const timer_task)
{
}

static void TIMER_1_task2_cb(const struct timer_task *const timer_task)
{
}

void TIMER_1_example(void)
{
	TIMER_1_task1.interval = 100;
	TIMER_1_task1.cb       = TIMER_1_task1_cb;
	TIMER_1_task1.mode     = TIMER_TASK_REPEAT;
	TIMER_1_task2.interval = 200;
	TIMER_1_task2.cb       = TIMER_1_task2_cb;
	TIMER_1_task2.mode     = TIMER_TASK_REPEAT;

	timer_add_task(&TIMER_1, &TIMER_1_task1);
	timer_add_task(&TIMER_1, &TIMER_1_task2);
	timer_start(&TIMER_1);
}
