/*
 * Code generated from Atmel Start.
 *
 * This file will be overwritten when reconfiguring your Atmel Start project.
 * Please copy examples or other code you want to keep to a separate file
 * to avoid losing it when reconfiguring.
 */
#ifndef ATMEL_START_PINS_H_INCLUDED
#define ATMEL_START_PINS_H_INCLUDED

#include <hal_gpio.h>

// SAMDA1 has 8 pin functions

#define GPIO_PIN_FUNCTION_A 0
#define GPIO_PIN_FUNCTION_B 1
#define GPIO_PIN_FUNCTION_C 2
#define GPIO_PIN_FUNCTION_D 3
#define GPIO_PIN_FUNCTION_E 4
#define GPIO_PIN_FUNCTION_F 5
#define GPIO_PIN_FUNCTION_G 6
#define GPIO_PIN_FUNCTION_H 7

#define XIN32 GPIO(GPIO_PORTA, 0)
#define LDR GPIO(GPIO_PORTA, 2)
#define DCF_CTL GPIO(GPIO_PORTA, 3)
#define PERIPHERAL_CTL GPIO(GPIO_PORTA, 4)
#define PWR_SENSE GPIO(GPIO_PORTA, 5)
#define DCF_DATA GPIO(GPIO_PORTA, 6)
#define DCF_PDN GPIO(GPIO_PORTA, 7)
#define DSPL_DO GPIO(GPIO_PORTA, 8)
#define DSPL_SCK GPIO(GPIO_PORTA, 9)
#define DSPL_SS GPIO(GPIO_PORTA, 10)
#define DSPL_MISO GPIO(GPIO_PORTA, 11)
#define LED GPIO(GPIO_PORTA, 14)
#define SWITCH GPIO(GPIO_PORTA, 15)
#define DHT_SDA GPIO(GPIO_PORTA, 16)
#define DHT_SCL GPIO(GPIO_PORTA, 17)
#define PA22 GPIO(GPIO_PORTA, 22)
#define PA23 GPIO(GPIO_PORTA, 23)
#define USB_N GPIO(GPIO_PORTA, 24)
#define USB_P GPIO(GPIO_PORTA, 25)
#define LDR_SINK GPIO(GPIO_PORTA, 27)

#endif // ATMEL_START_PINS_H_INCLUDED
