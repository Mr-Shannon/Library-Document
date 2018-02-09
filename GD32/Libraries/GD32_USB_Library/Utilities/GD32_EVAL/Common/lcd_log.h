/**
  ******************************************************************************
  * @file    lcd_log.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   header for the lcd_log.c file
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __GD32F10X_LCD_LOG_H
#define  __GD32F10X_LCD_LOG_H

/* Includes ------------------------------------------------------------------*/
#include "gd3210c_eval_lcd.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup GD32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @addtogroup LCD_LOG
  * @{
  */

/** @defgroup LCD_LOG_Exported_FunctionsPrototype
  * @{
  */
void LCD_LOG_Init           (void);
void LCD_LOG_DeInit         (void);
void LCD_LOG_SetHeader      (uint8_t *pTitle);
void LCD_LOG_SetFooter      (uint8_t *pStatus);
void LCD_LOG_Print          (uint8_t *pStr, 
                             uint16_t Len, 
                             uint16_t CharColor, 
                             uint16_t BackColor);
void LCD_LOG_ClearTextZone  (uint16_t StartX,
                             uint16_t StartY,
                             uint16_t EndX,
                             uint16_t EndY);

/**
  * @}
  */ 


#endif /* __GD32F10X_LCD_LOG_H */

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
