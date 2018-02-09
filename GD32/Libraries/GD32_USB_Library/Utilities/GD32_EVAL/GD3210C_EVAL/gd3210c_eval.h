/**
  ******************************************************************************
  * @file    gd3210cl_eval.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   GD3210CL_EVAL's Leds, Keys and COM ports hardware resources defines.
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD3210CL_EVAL_H
#define __GD3210CL_EVAL_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup GD32_EVAL
  * @{
  */
    
/** @addtogroup GD3210CL_EVAL
  * @{
  */ 

/** @addtogroup GD3210CL_EVAL_LOW_LEVEL
  * @{
  */ 
  
/** @defgroup GD3210CL_EVAL_LOW_LEVEL_Exported_Types
  * @{
  */
//typedef enum 
//{
//    LED1 = 0,
//    LED2 = 1,
//    LED3 = 2,
//    LED4 = 3
//}
//Led_TypeDef;

typedef enum 
{
    KEY_WAKEUP = 0,
    KEY_TAMPER = 1,
    KEY_USER = 2,
    KEY_RIGHT = 3,
    KEY_LEFT = 4,
    KEY_UP = 5,
    KEY_DOWN = 6,
    KEY_SEL = 7
}
Button_TypeDef;

typedef enum 
{
    KEY_MODE_GPIO = 0,
    KEY_MODE_EXTI = 1
}
KEYMode_TypeDef;

typedef enum 
{
    PB_NONE = 0,
    PB_SEL = 1,
    PB_DOWN = 2,
    PB_LEFT = 3,
    PB_RIGHT = 4,
    PB_UP = 5
}
PBState_TypeDef;

typedef enum 
{
    COM2 = 0,
//    COM1 = 1
}
COM_TypeDef;

/**
  * @}
  */ 

/** @defgroup GD3210CL_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */

/** @addtogroup GD3210CL_EVAL_LOW_LEVEL_LED
  * @{
  */
//#define LEDn                             4

//#define LED1_PIN                         GPIO_PIN_0
//#define LED1_GPIO_PORT                   GPIOC
//#define LED1_GPIO_CLK                    RCC_APB2PERIPH_GPIOC
//  
//#define LED2_PIN                         GPIO_PIN_2
//#define LED2_GPIO_PORT                   GPIOC
//#define LED2_GPIO_CLK                    RCC_APB2PERIPH_GPIOC

//#define LED3_PIN                         GPIO_PIN_0
//#define LED3_GPIO_PORT                   GPIOE
//#define LED3_GPIO_CLK                    RCC_APB2PERIPH_GPIOE

//#define LED4_PIN                         GPIO_PIN_1
//#define LED4_GPIO_PORT                   GPIOE
//#define LED4_GPIO_CLK                    RCC_APB2PERIPH_GPIOE


/**
  * @}
  */ 
  
/** @addtogroup GD3210CL_EVAL_LOW_LEVEL_KEY
  * @{
  */  
#define KEYn                          6 

/**
  * @brief Wakeup Key
  */
#define WAKEUP_KEY_PIN                GPIO_PIN_0
#define WAKEUP_KEY_GPIO_PORT          GPIOA
#define WAKEUP_KEY_GPIO_CLK           RCC_APB2PERIPH_GPIOA
#define WAKEUP_KEY_EXTI_LINE          EXTI_LINE0
#define WAKEUP_KEY_EXTI_PORT_SOURCE   GPIO_PORT_SOURCE_GPIOA
#define WAKEUP_KEY_EXTI_PIN_SOURCE    GPIO_PINSOURCE0
#define WAKEUP_KEY_EXTI_IRQn          EXTI0_IRQn

/**
  * @brief Tamper Key
  */
#define TAMPER_KEY_PIN                GPIO_PIN_13
#define TAMPER_KEY_GPIO_PORT          GPIOC
#define TAMPER_KEY_GPIO_CLK           RCC_APB2PERIPH_GPIOC
#define TAMPER_KEY_EXTI_LINE          EXTI_LINE13
#define TAMPER_KEY_EXTI_PORT_SOURCE   GPIO_PORT_SOURCE_GPIOC
#define TAMPER_KEY_EXTI_PIN_SOURCE    GPIO_PINSOURCE13
#define TAMPER_KEY_EXTI_IRQn          EXTI15_10_IRQn 

/**
  * @brief User Key
  */
#define USER_KEY_PIN                   GPIO_PIN_14
#define USER_KEY_GPIO_PORT             GPIOB
#define USER_KEY_GPIO_CLK              RCC_APB2PERIPH_GPIOB
#define USER_KEY_EXTI_LINE             EXTI_LINE14
#define USER_KEY_EXTI_PORT_SOURCE      GPIO_PORT_SOURCE_GPIOB
#define USER_KEY_EXTI_PIN_SOURCE       GPIO_PINSOURCE14
#define USER_KEY_EXTI_IRQn             EXTI15_10_IRQn

/**
 * @brief Right Key
 */
#define RIGHT_KEY_PIN                 GPIO_PIN_0
#define RIGHT_KEY_GPIO_PORT           GPIOA
#define RIGHT_KEY_GPIO_CLK            RCC_APB2PERIPH_GPIOA
#define RIGHT_KEY_EXTI_LINE           EXTI_LINE0
#define RIGHT_KEY_EXTI_PORT_SOURCE    GPIO_PORT_SOURCE_GPIOA
#define RIGHT_KEY_EXTI_PIN_SOURCE     GPIO_PINSOURCE0
#define RIGHT_KEY_EXTI_IRQn           EXTI0_IRQn

/**
 * @brief Left Key
 */
#define LEFT_KEY_PIN                  GPIO_PIN_13
#define LEFT_KEY_GPIO_PORT            GPIOC
#define LEFT_KEY_GPIO_CLK             RCC_APB2PERIPH_GPIOC
#define LEFT_KEY_EXTI_LINE            EXTI_LINE13
#define LEFT_KEY_EXTI_PORT_SOURCE     GPIO_PORT_SOURCE_GPIOC
#define LEFT_KEY_EXTI_PIN_SOURCE      GPIO_PINSOURCE13
#define LEFT_KEY_EXTI_IRQn            EXTI15_10_IRQn

/**
 * @brief Up Key
 */
#define UP_KEY_PIN                    GPIO_PIN_14
#define UP_KEY_GPIO_PORT              GPIOB
#define UP_KEY_GPIO_CLK               RCC_APB2PERIPH_GPIOB
#define UP_KEY_EXTI_LINE              EXTI_LINE14
#define UP_KEY_EXTI_PORT_SOURCE       GPIO_PORT_SOURCE_GPIOB
#define UP_KEY_EXTI_PIN_SOURCE        GPIO_PINSOURCE14
#define UP_KEY_EXTI_IRQn              EXTI15_10_IRQn 

/**
  * @}
  */ 

/** @addtogroup GD3210CL_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             1

/**
 * @brief Definition for COM port1, connected to USART2 (USART2 pins remapped on GPIOA)
 */ 
#define EVAL_COM2                        USART2
#define EVAL_COM2_CLK                    RCC_APB1PERIPH_USART2
#define EVAL_COM2_TX_PIN                 GPIO_PIN_2
#define EVAL_COM2_TX_GPIO_PORT           GPIOA
#define EVAL_COM2_TX_GPIO_CLK            RCC_APB2PERIPH_GPIOA
#define EVAL_COM2_RX_PIN                 GPIO_PIN_3
#define EVAL_COM2_RX_GPIO_PORT           GPIOA
#define EVAL_COM2_RX_GPIO_CLK            RCC_APB2PERIPH_GPIOA
#define EVAL_COM2_IRQn                   USART2_IRQn

/**
  * @}
  */

extern USART_TypeDef* COM_USART[COMn];

/**
  * @}
  */

/** @defgroup GD3210C_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */ 
//void GD_EVAL_LEDInit          (Led_TypeDef Led);
//void GD_EVAL_LEDOn            (Led_TypeDef Led);
//void GD_EVAL_LEDOff           (Led_TypeDef Led);
//void GD_EVAL_LEDToggle        (Led_TypeDef Led);
void GD_EVAL_KeyInit          (Button_TypeDef Button, KEYMode_TypeDef Button_Mode);
uint32_t GD_EVAL_KeyGetState  (Button_TypeDef Button);
void GD_EVAL_COMInit          (COM_TypeDef COM, USART_InitPara* USART_InitStruct);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __GD3210CL_EVAL_H */

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
