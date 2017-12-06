/**
 * Hardware
 */

#define _HARDWARE_C_ 
#include <hardware.h>

void hardware_init(void) {
  leds_init();
  keys_init();
  beep_init();
  rtc_init();
  logger_init();
}

void leds_init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOF, ENABLE);

  GPIO_InitTypeDef led1 = {
    .GPIO_Pin = GPIO_Pin_7,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
  };
  GPIO_Init(GPIOF,&led1);
  LED1_OFF;

  GPIO_InitTypeDef led2 = {
    .GPIO_Pin = GPIO_Pin_8,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
  };
  GPIO_Init(GPIOF,&led2);
  LED2_OFF;

  GPIO_InitTypeDef ledR = {
    .GPIO_Pin = GPIO_Pin_5,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
  };
  GPIO_Init(GPIOB,&ledR);
  LEDR_OFF;

  GPIO_InitTypeDef ledG = {
    .GPIO_Pin = GPIO_Pin_0,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
  };
  GPIO_Init(GPIOB,&ledG);
  LEDG_OFF;

  GPIO_InitTypeDef ledB = {
    .GPIO_Pin = GPIO_Pin_1,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
  };
  GPIO_Init(GPIOB,&ledB);
  LEDB_OFF;
}

void beep_init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  GPIO_InitTypeDef beep = {
    .GPIO_Pin = GPIO_Pin_0,
    .GPIO_Mode = GPIO_Mode_Out_PP,
    .GPIO_Speed = GPIO_Speed_50MHz
  };
  
  GPIO_Init(GPIOC,&beep);
  BEEP_OFF;
}

void keys_init(void) {
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

  GPIO_InitTypeDef key2 = {
    .GPIO_Pin = GPIO_Pin_13,
    .GPIO_Mode = GPIO_Mode_IN_FLOATING
  };
  GPIO_Init(GPIOC,&key2);

  GPIO_InitTypeDef key1 = {
    .GPIO_Pin = GPIO_Pin_0,
    .GPIO_Mode = GPIO_Mode_IN_FLOATING
  };
  GPIO_Init(GPIOA,&key1);
}