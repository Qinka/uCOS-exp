/**
 * Hardware
 */

#ifndef _HARDWARE_H_
#define _HARDWARE_H_ 

#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <rtc.h>
#include <logger.h>

void hardware_init(void);
void leds_init(void);
void keys_init(void);
void beep_init(void);

#define LED1_ON  GPIO_ResetBits(GPIOF,GPIO_Pin_7)
#define LED1_OFF GPIO_SetBits(GPIOF,GPIO_Pin_7)

#define LED2_ON  GPIO_ResetBits(GPIOF,GPIO_Pin_8)
#define LED2_OFF GPIO_SetBits(GPIOF,GPIO_Pin_8)

#define LEDR_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_5)
#define LEDR_OFF GPIO_SetBits(GPIOB,GPIO_Pin_5)

#define LEDG_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_0)
#define LEDG_OFF GPIO_SetBits(GPIOB,GPIO_Pin_0)

#define LEDB_ON  GPIO_ResetBits(GPIOB,GPIO_Pin_1)
#define LEDB_OFF GPIO_SetBits(GPIOB,GPIO_Pin_1)

#define BEEP_ON  GPIO_ResetBits(GPIOC,GPIO_Pin_0)
#define BEEP_OFF GPIO_SetBits(GPIOC,GPIO_Pin_0)

#define KEY1_STAT GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0)
#define KEY2_STAT GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_13)


#endif // !_HARDWARE_H_