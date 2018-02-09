/**
  ******************************************************************************
  * @file    usbd_conf.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB device configuration header file
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USBD_CONF_H
#define __GD32F10X_USBD_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/** @defgroup USB_CONF_Exported_Defines
  * @{
  */ 
#define USBD_CFG_MAX_NUM                    1
#define USBD_ITF_MAX_NUM                    1

#define USBD_SELF_POWERED

#define USB_STR_DESC_MAX_SIZE               255

/** @defgroup USB_VCP_Class_Layer_Parameter
  * @{
  */ 
#define CDC_DATA_IN_EP                      EP1_IN  /* EP1 for data IN */
#define CDC_DATA_OUT_EP                     EP3_OUT /* EP3 for data OUT */
#define CDC_CMD_EP                          EP2_IN  /* EP2 for CDC commands */

/* CDC Endpoints parameters: you can fine tune these values depending on the needed baudrates and performance. */
#ifdef USE_USB_OTG_HS
    #define USB_CDC_DATA_PACKET_SIZE        512  /* Endpoint IN & OUT Packet size */
    #define USB_CDC_CMD_PACKET_SIZE         8    /* Control Endpoint Packet size */

    #define CDC_IN_FRAME_INTERVAL           40   /* Number of micro-frames between IN transfers */
    #define APP_RX_DATA_SIZE                512 /* Total size of IN buffer: 
                                                 APP_RX_DATA_SIZE*8/MAX_BAUDARATE*1000 should be > CDC_IN_FRAME_INTERVAL*8 */
#else
    #define USB_CDC_DATA_PACKET_SIZE        64   /* Endpoint IN & OUT Packet size */
    #define USB_CDC_CMD_PACKET_SIZE         8   /* Control Endpoint Packet size */

    #define CDC_IN_FRAME_INTERVAL           5    /* Number of frames between IN transfers */
    #define APP_RX_DATA_SIZE                512 /* Total size of IN buffer: 
                                                APP_RX_DATA_SIZE*8/MAX_BAUDARATE*1000 should be > CDC_IN_FRAME_INTERVAL */
#endif /* USE_USB_OTG_HS */

#define APP_FOPS                        VCP_fops

/**
  * @}
  */

#endif /* __GD32F10X_USBD_CONF_H */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/

