/**
  ******************************************************************************
  * @file    gd3210c_eval.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file provides
  *            - Set of firmware functions to manage Leds, push-button and COM ports
  *            - Low level initialization functions
  *              available on GD3210C-EVAL evaluation board from GDMicroelectronics.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "gd3210c_eval.h"

/** @addtogroup Utilities
  * @{
  */ 

/** @addtogroup GD32_EVAL
  * @{
  */ 

/** @addtogroup GD3210CL_EVAL
  * @{
  */

/** @defgroup GD3210CL_EVAL_LOW_LEVEL
  * @{
  */

/** @defgroup GD3210C_EVAL_LOW_LEVEL_Private_Variables
  * @{
  */ 
//GPIO_TypeDef* GPIO_PORT[LEDn] = {LED1_GPIO_PORT, LED2_GPIO_PORT,
//                                 LED3_GPIO_PORT, LED4_GPIO_PORT};

//const uint16_t GPIO_PIN[LEDn] = {LED1_PIN, LED2_PIN, 
//                                 LED3_PIN, LED4_PIN};

//const uint32_t GPIO_CLK[LEDn] = {LED1_GPIO_CLK, LED2_GPIO_CLK, 
//                                 LED3_GPIO_CLK, LED4_GPIO_CLK};

GPIO_TypeDef* KEY_PORT[KEYn] = {WAKEUP_KEY_GPIO_PORT, TAMPER_KEY_GPIO_PORT,
                                USER_KEY_GPIO_PORT, RIGHT_KEY_GPIO_PORT,
                                LEFT_KEY_GPIO_PORT, UP_KEY_GPIO_PORT}; 

const uint16_t KEY_PIN[KEYn] = {WAKEUP_KEY_PIN, TAMPER_KEY_PIN,
                                USER_KEY_PIN, RIGHT_KEY_PIN,
                                LEFT_KEY_PIN, UP_KEY_PIN}; 

const uint32_t KEY_CLK[KEYn] = {WAKEUP_KEY_GPIO_CLK, TAMPER_KEY_GPIO_CLK,
                                USER_KEY_GPIO_CLK, RIGHT_KEY_GPIO_CLK,
                                LEFT_KEY_GPIO_CLK, UP_KEY_GPIO_CLK};

const uint16_t KEY_EXTI_LINE[KEYn] = {WAKEUP_KEY_EXTI_LINE,
                                      TAMPER_KEY_EXTI_LINE, 
                                      USER_KEY_EXTI_LINE,
                                      RIGHT_KEY_EXTI_LINE,
                                      LEFT_KEY_EXTI_LINE,
                                      UP_KEY_EXTI_LINE};

const uint16_t KEY_PORT_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_PORT_SOURCE,
                                        TAMPER_KEY_EXTI_PORT_SOURCE, 
                                        USER_KEY_EXTI_PORT_SOURCE,
                                        RIGHT_KEY_EXTI_PORT_SOURCE,
                                        LEFT_KEY_EXTI_PORT_SOURCE,
                                        UP_KEY_EXTI_PORT_SOURCE};

const uint16_t KEY_PIN_SOURCE[KEYn] = {WAKEUP_KEY_EXTI_PIN_SOURCE,
                                       TAMPER_KEY_EXTI_PIN_SOURCE, 
                                       USER_KEY_EXTI_PIN_SOURCE,
                                       RIGHT_KEY_EXTI_PIN_SOURCE,
                                       LEFT_KEY_EXTI_PIN_SOURCE,
                                       UP_KEY_EXTI_PIN_SOURCE}; 

const uint16_t KEY_IRQn[KEYn] = {WAKEUP_KEY_EXTI_IRQn, TAMPER_KEY_EXTI_IRQn,
                                 USER_KEY_EXTI_IRQn, RIGHT_KEY_EXTI_IRQn,
                                 LEFT_KEY_EXTI_IRQn, UP_KEY_EXTI_IRQn};

USART_TypeDef* COM_USART[COMn] = {EVAL_COM2};

GPIO_TypeDef* COM_TX_PORT[COMn] = {EVAL_COM2_TX_GPIO_PORT};

GPIO_TypeDef* COM_RX_PORT[COMn] = {EVAL_COM2_RX_GPIO_PORT};

const uint32_t COM_USART_CLK[COMn] = {EVAL_COM2_CLK};

const uint32_t COM_TX_PORT_CLK[COMn] = {EVAL_COM2_TX_GPIO_CLK};
 
const uint32_t COM_RX_PORT_CLK[COMn] = {EVAL_COM2_RX_GPIO_CLK};

const uint16_t COM_TX_PIN[COMn] = {EVAL_COM2_TX_PIN};

const uint16_t COM_RX_PIN[COMn] = {EVAL_COM2_RX_PIN};
 
DMA_InitPara   sEEDMA_InitStructure; 

/**
  * @}
  */

/** @defgroup GD3210CL_EVAL_LOW_LEVEL_Private_Functions
  * @{
  */

/**
  * @brief  Configures LED GPIO
  * @param  LedNum: Specifies the Led to be configured
  * @retval None
  */
//void GD_EVAL_LEDInit(Led_TypeDef LedNum)
//{
//    GPIO_InitPara  GPIO_InitStructure;

//    /* Enable the GPIO_LED Clock */
//    RCC_APB2PeriphClock_Enable(GPIO_CLK[LedNum], ENABLE);

//    /* Configure the GPIO_LED pin */
//    GPIO_InitStructure.GPIO_Pin = GPIO_PIN[LedNum];
//    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_OUT_PP;
//    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;

//    GPIO_Init(GPIO_PORT[LedNum], &GPIO_InitStructure);
//}

///**
//  * @brief  Turn on selected LED 
//  * @param  LedNum: Specifies the Led to be turn on
//  * @retval None
//  */
//void GD_EVAL_LEDOn(Led_TypeDef LedNum)
//{
//    GPIO_PORT[LedNum]->BOR = GPIO_PIN[LedNum];
//}

///**
//  * @brief  Turn off selected LED
//  * @param  LedNum: Specifies the Led to be turn off 
//  * @retval None
//  */
//void GD_EVAL_LEDOff(Led_TypeDef LedNum)
//{
//    GPIO_PORT[LedNum]->BCR = GPIO_PIN[LedNum];
//}

///**
//  * @brief  Toggle the selected LED
//  * @param  LedNum: Specifies the Led to be toggled
//  * @retval None
//  */
//void GD_EVAL_LEDToggle(Led_TypeDef LedNum)
//{
//    GPIO_PORT[LedNum]->DOR ^= GPIO_PIN[LedNum];
//}

/**
  * @brief  Configures Button GPIO and EXTI Line
  * @param  Button: Specifies the Button to be configured
  * @param  Button_Mode: Specifies Button mode
  * @retval None
  */
void GD_EVAL_KeyInit(Button_TypeDef Button, KEYMode_TypeDef Button_Mode)
{
    GPIO_InitPara GPIO_InitStructure;
    EXTI_InitPara EXTI_InitStructure;
    NVIC_InitPara NVIC_InitStructure;

    /* Enable the KEY Clock */
    RCC_APB2PeriphClock_Enable(KEY_CLK[Button] | RCC_APB2PERIPH_AF, ENABLE);

    /* Configure Button pin as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IPD;
    GPIO_InitStructure.GPIO_Pin = KEY_PIN[Button];
    GPIO_Init(KEY_PORT[Button], &GPIO_InitStructure);

    if (Button_Mode == KEY_MODE_EXTI)
    {
        /* Connect Button EXTI Line to Button GPIO Pin */
        GPIO_EXTILineConfig(KEY_PORT_SOURCE[Button], KEY_PIN_SOURCE[Button]);

        /* Configure Button EXTI line */
        EXTI_InitStructure.EXTI_LINE = KEY_EXTI_LINE[Button];
        EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
        EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
        EXTI_InitStructure.EXTI_LINEEnable = ENABLE;
        EXTI_Init(&EXTI_InitStructure);

        /* Enable and set Button EXTI Interrupt to the lowest priority */
        NVIC_InitStructure.NVIC_IRQ = KEY_IRQn[Button];
        NVIC_InitStructure.NVIC_IRQPreemptPriority = 0x0f;
        NVIC_InitStructure.NVIC_IRQSubPriority = 0x0f;
        NVIC_InitStructure.NVIC_IRQEnable = ENABLE;

        NVIC_Init(&NVIC_InitStructure);
    }
}

/**
  * @brief  Returns the selected Button state
  * @param  Button: Specifies the Button to be checked
  * @retval The Button GPIO pin value
  */
uint32_t GD_EVAL_KeyGetState(Button_TypeDef Button)
{
    return GPIO_ReadInputBit(KEY_PORT[Button], KEY_PIN[Button]);
}

/**
  * @brief  Configures COM port
  * @param  COM: Specifies the COM port to be configured
  * @param  USART_InitStruct: pointer to a USART_InitTypeDef structure that
  *         contains the configuration information for the specified USART peripheral
  * @retval None
  */
void GD_EVAL_COMInit(COM_TypeDef COM, USART_InitPara *USART_InitStruct)
{
    GPIO_InitPara GPIO_InitStructure;

    /* Enable GPIO clock */
    RCC_APB2PeriphClock_Enable(COM_TX_PORT_CLK[COM] | COM_RX_PORT_CLK[COM] | RCC_APB2PERIPH_AF, ENABLE);

    if (COM == COM2)
    {
        /* Enable the USART2 Pins Software Remapping */
        //GPIO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
        RCC_APB1PeriphClock_Enable(COM_USART_CLK[COM], ENABLE);
    }

    /* Configure USART Tx as alternate function push-pull */
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_AF_PP;
    GPIO_InitStructure.GPIO_Pin = COM_TX_PIN[COM];
    GPIO_InitStructure.GPIO_Speed = GPIO_SPEED_50MHZ;
    GPIO_Init(COM_TX_PORT[COM], &GPIO_InitStructure);

    /* Configure USART Rx as input floating */
    GPIO_InitStructure.GPIO_Mode = GPIO_MODE_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = COM_RX_PIN[COM];
    GPIO_Init(COM_RX_PORT[COM], &GPIO_InitStructure);

    /* USART configuration */
    USART_Init(COM_USART[COM], USART_InitStruct);

    /* Enable USART */
    USART_Enable(COM_USART[COM], ENABLE);
}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
