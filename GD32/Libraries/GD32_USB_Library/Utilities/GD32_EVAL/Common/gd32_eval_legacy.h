/**
  ******************************************************************************
  * @file    stm32_eval_legacy.h
  * @author  MCD Application Team
  * @version V5.0.2
  * @date    05-March-2012
  * @brief   This file contains defines legacy for STM32 EVAL drivers.
  ******************************************************************************
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32_EVAL_LEGACY_H
#define __STM32_EVAL_LEGACY_H

#ifdef __cplusplus
 extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* STM322xG_EVAL Defines Legacy */
#define STM3220F_LCD_Init       STM322xG_LCD_Init

/* Button Defines Legacy */ 
#define Button_WAKEUP        BUTTON_WAKEUP
#define Button_TAMPER        BUTTON_TAMPER
#define Button_KEY           BUTTON_KEY
#define Button_RIGHT         BUTTON_RIGHT
#define Button_LEFT          BUTTON_LEFT
#define Button_UP            BUTTON_UP
#define Button_DOWN          BUTTON_DOWN
#define Button_SEL           BUTTON_SEL
#define Mode_GPIO            KEY_MODE_GPIO
#define Mode_EXTI            KEY_MODE_EXTI
#define Button_Mode_TypeDef  ButtonMode_TypeDef
#define JOY_CENTER           JOY_SEL
#define JOY_State_TypeDef    JOYState_TypeDef 

#ifdef __cplusplus
}
#endif

#endif /* __STM32_EVAL_LEGACY_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
