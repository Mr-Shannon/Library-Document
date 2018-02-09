#ifndef __PERIPH_H
#define __PERIPH_H
/**
  ******************************************************************************
  * @file    periph.h
  * @author  Illarionov Maksim
  * @version V1.0.0
  * @date    03-Apr-2017
  * @brief   ”правление вводом выводом
  ******************************************************************************
  */


#include "gd32f10x.h"


typedef enum 
{
    LED1= 0,
    LED2 = 1,
}
Leds_TypeDef;


#define LEDn                             	2

#define LED1_PIN                      GPIO_PIN_5
#define LED1_GPIO_PORT              	GPIOA
#define LED1_GPIO_CLK                	RCC_APB2PERIPH_GPIOA
  
#define LED2_PIN                      GPIO_PIN_4
#define LED2_GPIO_PORT              	GPIOA
#define LED2_GPIO_CLK               	RCC_APB2PERIPH_GPIOA


void LEDInit          (Leds_TypeDef Led);
void LEDOn            (Leds_TypeDef Led);
void LEDOff           (Leds_TypeDef Led);
void LEDToggle        (Leds_TypeDef Led);

#endif //__PERIPH_H
