/**
  ******************************************************************************
  * @file    lcd_log.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    24-Dec-2014
  * @brief   This file provides all the LCD Log firmware functions.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "lcd_log.h"
#include <string.h>

#include "usb_conf.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup GD32_EVAL
  * @{
  */

/** @addtogroup Common
  * @{
  */

/** @defgroup LCD_LOG
  * @{
  */

/** @defgroup LCD_LOG_Private_Variables
  * @{
  */
extern uint16_t LINE;

/**
  * @}
  */

/** @defgroup LCD_LOG_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the LCD Log module
  * @param  None
  * @retval None
  */
void LCD_LOG_Init (void)
{
    LCD_Clear(Black);
}

/**
  * @brief  De-Initialize the LCD Log module
  * @param  None
  * @retval None
  */
void LCD_LOG_DeInit (void)
{
}

/**
* @brief  Display the application header (title) on the LCD screen 
* @param  pTitle: pointer to the string to be displayed
* @retval None
*/
void LCD_LOG_SetHeader (uint8_t *pTitle)
{
    int i = 0;
    int Len = strlen((const char *)pTitle);

    LCD_SetRectangleColor(210, 0, 240, 320, Blue);

    for(i = 0; i < Len; i++)
    {
        LCD_PutChar(220, (80 + 8 * i), pTitle[i], Red, Green);
    }
}

/**
  * @brief  Display the application footer (status) on the LCD screen 
  * @param  pStatus: pointer to the string to be displayed
  * @retval None
  */
void LCD_LOG_SetFooter (uint8_t *pStatus)
{
    int i = 0;
    int Len = strlen((const char *)pStatus);

    LCD_SetRectangleColor(0, 0, 30, 320, Blue);

    for(i = 0; i < Len; i++)
    {
        LCD_PutChar(10, (70 + 8 * i), pStatus[i], Red, Green);
    }
}

/**
  * @brief  Clear the Text Zone 
  * @param  None 
  * @retval None
  */
void LCD_LOG_ClearTextZone(uint16_t StartX,
                            uint16_t StartY,
                            uint16_t EndX,
                            uint16_t EndY)
{
    LCD_SetRectangleColor(StartX, StartY, EndX, EndY, Black);
}

/**
  * @brief  Redirect the printf to the lcd 
  * @param  pStr: pointer to string to be displayed
  * @param  Len: string length
  * @retval None
  */
void LCD_LOG_Print (uint8_t *pStr, 
                    uint16_t Len, 
                    uint16_t CharColor, 
                    uint16_t BackColor)
{
    uint16_t i;

#if defined(USE_HOST_MODE) && defined(USE_DEVICE_MODE)
    if(LINE <= 60)
    {
        LINE = 190;
        LCD_SetRectangleColor(60, 0, 210, 320, Black);
    }
#else
    if(LINE <= 30)
    {
        LINE = 190;
        LCD_SetRectangleColor(30, 0, 210, 320, Black);
    }
#endif

    for(i = 0; i < Len; i ++)
    {
        LCD_PutChar(LINE, (10 + 8 * i), *pStr++, CharColor, BackColor);
    }

    LINE -= 20;
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
