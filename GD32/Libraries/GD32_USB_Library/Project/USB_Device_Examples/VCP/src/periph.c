/**
  ******************************************************************************
  * @file    periph.c
  * @author  Illarionov Maksim
  * @version V1.0.0
  * @date    03-Apr-2017
  * @brief   Управление вводом выводом
  ******************************************************************************
  */
	
	
#include "periph.h"


GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT};

const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN};

const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK};


void LEDInit(Leds_TypeDef LedNum)
{
    GPIO_InitPara  GPIO_InitStructure;

    RCC_APB2PeriphClock_Enable(GPIO_CLK[LedNum], ENABLE);
		
    /* Configure the GPIO_LED pin */
    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[LedNum];
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;

    GPIO_Init(GPIO_PORT[LedNum], &GPIO_InitStructure);
}

/**
  * @brief  Включение выбранного светодиода
  * @param  LedNum: Specifies the Led to be turn on
  * @retval None
  */
void LEDOn(Leds_TypeDef LedNum)
{
    GPIO_PORT[LedNum]->BOR = GPIO_PIN[LedNum];
}

/**
  * @brief  Выключение выбранного светодиода
  * @param  LedNum: Specifies the Led to be turn off 
  * @retval None
  */
void LEDOff(Leds_TypeDef LedNum)
{
    GPIO_PORT[LedNum]->BCR = GPIO_PIN[LedNum];
}

/**
  * @brief  Дернуть ногой
  * @param  LedNum: Specifies the Led to be toggled
  * @retval None
  */
void LEDToggle(Leds_TypeDef LedNum)
{
    GPIO_PORT[LedNum]->DOR ^= GPIO_PIN[LedNum];
}
