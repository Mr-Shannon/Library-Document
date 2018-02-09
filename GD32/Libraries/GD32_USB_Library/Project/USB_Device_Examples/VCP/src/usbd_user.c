/**
  ******************************************************************************
  * @file    usbd_usr.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file includes the user application layer
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_user.h"
#include "usbd_ctlx.h"
#include "lcd_log.h"

/** @addtogroup USB_OTG_DEVICE_LIBRARY
  * @{
  */

/** @defgroup USBD_USER
  * @{
  */

/** @defgroup USBD_USER_Private_Variables
  * @{
  */
USBD_User_cb_TypeDef USER_cb =
{
    USBD_USER_DeviceInit,
    USBD_USER_DeviceReset,
    USBD_USER_DeviceConfigured,
    USBD_USER_DeviceSuspended,
    USBD_USER_DeviceResumed,
    USBD_USER_DeviceConnected,
    USBD_USER_DeviceDisconnected,
};

/**
  * @}
  */

/** @defgroup USBD_USER_Private_Constants
  * @{
  */

/* LCD Messages */
const uint8_t MSG_DEV_HEADER[]         = "USB OTG FS CDC Device";
const uint8_t MSG_DEV_FOOTER[]         = "USB Device Library v1.0.0";

const uint8_t MSG_DEV_HIGHSPEED[]      = "USB Device Library v1.0.0 [HS]";
const uint8_t MSG_DEV_FULLSPEED[]      = "USB Device Library v1.0.0 [FS]";
const uint8_t MSG_DEV_UNKNOWNSPEED[]   = "USB Device Library v1.0.0 [??]";

const uint8_t MSG_DEV_CONFIG[]         = "> CDC Interface started.";
const uint8_t MSG_DEV_CONNECTED[]      = "> USB Device Connected.";
const uint8_t MSG_DEV_DISCONNECTED[]   = "> USB Device Disconnected.";

const uint8_t MSG_DEV_SUSPEND[]        = "> USB Device in Suspend Mode.";
const uint8_t MSG_DEV_RESUMED[]        = "> USB Device in Idle Mode.";
const uint8_t MSG_DEV_ERROR[]          = "> Device fault.";

const uint8_t MSG_LIB_START[]          = "> USB Device library started.";
const uint8_t MSG_DEV_NOSUP[]          = "> Device not supported.";

uint16_t LINE = 190;

/**
  * @}
  */

/** @defgroup USBD_USER_Private_Functions
  * @{
  */

/**
  * @brief  User operation for device initialization
  * @param  None
  * @retval None
  */
void USBD_USER_DeviceInit(void)
{
    GD3210CL_LCD_Init();

    LCD_LOG_Init();

    LCD_LOG_SetHeader((uint8_t *)MSG_DEV_HEADER);

    LCD_LOG_Print((uint8_t *)MSG_LIB_START, 
                  sizeof(MSG_LIB_START) - 1, 
                  White, Black);

    LCD_LOG_SetFooter((uint8_t *)MSG_DEV_FOOTER);
}

/**
  * @brief  User operation for device reset handle
  * @param  Speed: usb device speed
  * @retval None
  */
void USBD_USER_DeviceReset(uint8_t Speed)
{
    switch (Speed)
    {
        case USB_OTG_SPEED_HIGH: 
            LCD_LOG_SetFooter((uint8_t *)MSG_DEV_HIGHSPEED);
        break;

        case USB_OTG_SPEED_FULL: 
            LCD_LOG_SetFooter((uint8_t *)MSG_DEV_FULLSPEED);
            break;

        default:
            LCD_LOG_SetFooter((uint8_t *)MSG_DEV_UNKNOWNSPEED);
    }
}

/**
  * @brief  User operation for device configuration
  * @param  None
  * @retval Staus
  */
void USBD_USER_DeviceConfigured (void)
{
    LCD_LOG_Print((uint8_t *)MSG_DEV_CONFIG, 
                  sizeof(MSG_DEV_CONFIG) - 1, 
                  White, Black);
}

/**
  * @brief  User operation for device connecttion
  * @param  None
  * @retval Staus
  */
void USBD_USER_DeviceConnected (void)
{
    LCD_LOG_Print((uint8_t *)MSG_DEV_CONNECTED, 
                  sizeof(MSG_DEV_CONNECTED) - 1, 
                  White, Black);
}

/**
  * @brief  User operation for device disconnection
  * @param  None
  * @retval Staus
  */
void USBD_USER_DeviceDisconnected (void)
{
    LCD_LOG_Print((uint8_t *)MSG_DEV_DISCONNECTED, 
                  sizeof(MSG_DEV_DISCONNECTED) - 1, 
                  White, Black);
}

/**
  * @brief  User operation for device suspend
  * @param  None
  * @retval None
  */
void USBD_USER_DeviceSuspended (void)
{
    LCD_LOG_Print((uint8_t *)MSG_DEV_SUSPEND, 
                  sizeof(MSG_DEV_SUSPEND) - 1, 
                  White, Black);
}

/**
  * @brief  User operation for device resume
  * @param  None
  * @retval None
  */
void USBD_USER_DeviceResumed (void)
{
    LCD_LOG_Print((uint8_t *)MSG_DEV_RESUMED, 
                  sizeof(MSG_DEV_RESUMED) - 1, 
                  White, Black);
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
