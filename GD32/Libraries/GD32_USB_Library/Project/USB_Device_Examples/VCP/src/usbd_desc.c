/**
  ******************************************************************************
  * @file    usbd_desc.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB descriptors and strings formating method
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_desc.h"
#include "usbd_enum.h"
#include "usbd_conf.h"
#include "usb_regs.h"

/** @defgroup USBD_DESC
  * @{
  */

/** @defgroup USBD_DESC_Private_Defines
  * @{
  */
#define USBD_VID                        0x28e9
#define USBD_PID                        0x0182

/** @defgroup USB_String_Descriptors
  * @{
  */ 
#define USBD_LANGID_STRING              0x0409
#define USBD_MFC_STRING                 "GD32 Microelectronics"
#define USBD_PRODUCT_STRING             "GD32 Virtual ComPort in FS Mode"
#define USBD_SERIALNUMBER_STRING        "00000000022B"
#define USBD_CONFIGURATION_STRING       "Virtual ComPort Config"
#define USBD_INTERFACE_STRING           "Virtual ComPort Interface"
/**
  * @}
  */ 

/** @defgroup USBD_DESC_Private_Variables
  * @{
  */ 

USBD_Desc_cb_TypeDef USER_desc =
{
    USBD_USER_DevDesc,
    USBD_USER_LANGIDStrDesc,
    USBD_USER_MFCStrDesc,
    USBD_USER_ProdStrDesc,
    USBD_USER_SerialStrDesc,
    USBD_USER_ConfigStrDesc,
    USBD_USER_ItfStrDesc,
};

/* USB standard device descriptor */
uint8_t USBD_DeviceDesc[USB_DEVICE_DESC_SIZE] =
{
    0x12,                       /* bLength: device descriptor length */
    USB_DESCTYPE_DEVICE,        /* bDescriptorType: device descriptor type */
    0x00,                       /* bcdUSB: USB2.0 */
    0x02,
    0x02,                       /* bDeviceClass: CDC class */
    0x00,                       /* bDeviceSubClass: no set */
    0x00,                       /* bDeviceProtocol: no set */
    USB_OTG_FS_MAX_EP0_SIZE,    /* bMaxPacketSize0: endpoint 0 max packet size */
    LOWBYTE(USBD_VID),          /* idVendor low byte */
    HIGHBYTE(USBD_VID),         /* idVendor high byte */
    LOWBYTE(USBD_PID),          /* idProduct low byte */
    HIGHBYTE(USBD_PID),         /* idProduct high byte */
    0x00,                       /* bcdDevice: GD32 USB device(rel. 1.0) */
    0x01,
    USBD_MFC_STR_IDX,           /* iManufacturer: index of manufacturer string */
    USBD_PRODUCT_STR_IDX,       /* iProduct: index of product string */
    USBD_SERIAL_STR_IDX,        /* iSerialNumber: index of serial number string */
    USBD_CFG_MAX_NUM            /* bNumConfigurations: only one configuration */
};

/* USB Standard Device Qualifier Descriptor */
uint8_t USBD_DeviceQualifierDesc[USB_DEV_QUALIFIER_DESC_LEN] =
{
    USB_DEV_QUALIFIER_DESC_LEN,
    USB_DESCTYPE_DEVICE_QUALIFIER,
    0x00,
    0x02,
    0x00,
    0x00,
    0x00,
    0x40,
    0x01,
    0x00,
};

/* USB language ID Descriptor */
uint8_t USBD_LANGIDDesc[USB_LANGID_STRING_SIZE] =
{
     USB_LANGID_STRING_SIZE,
     USB_DESCTYPE_STRING,
     LOWBYTE(USBD_LANGID_STRING),
     HIGHBYTE(USBD_LANGID_STRING),
};

/**
  * @}
  */

/** @defgroup USBD_DESC_Private_Functions
  * @{
  */

/**
  * @brief  Get the device descriptor
  * @param  Speed: usb device speed
  * @param  pLen: data length pointer
  * @retval Descriptor buffer pointer
  */
uint8_t* USBD_USER_DevDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen = sizeof(USBD_DeviceDesc);

    return USBD_DeviceDesc;
}

/**
  * @brief  Get the LangID string descriptor
  * @param  Speed: usb device speed
  * @param  pLen: data length pointer
  * @retval Descriptor buffer pointer
  */
uint8_t* USBD_USER_LANGIDStrDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen =  sizeof(USBD_LANGIDDesc);

    return USBD_LANGIDDesc;
}


/**
  * @brief  Get the product string descriptor
  * @param  Speed: usb device speed
  * @param  pLen: data length pointer
  * @retval Descriptor buffer pointer
  */
uint8_t* USBD_USER_ProdStrDesc (uint8_t Speed, uint16_t *pLen)
{
    if(Speed == USB_OTG_SPEED_HIGH)
    {

    }
    else if(Speed == USB_OTG_SPEED_FULL)
    {
        USBD_GetUnicodeString ((uint8_t *)USBD_PRODUCT_STRING, USBD_StrDesc, pLen);
    }

    return USBD_StrDesc;
}

/**
  * @brief  Get the manufacturer string descriptor
  * @param  Speed: usb device speed
  * @param  pLen: data length pointer
  * @retval Descriptor buffer pointer
  */
uint8_t* USBD_USER_MFCStrDesc (uint8_t Speed, uint16_t *pLen)
{
    USBD_GetUnicodeString ((uint8_t *)USBD_MFC_STRING, USBD_StrDesc, pLen);

    return USBD_StrDesc;
}

/**
  * @brief  Get the serial number string descriptor
  * @param  Speed: usb device speed
  * @param  pLen: data length pointer
  * @retval Descriptor buffer pointer
  */
uint8_t *  USBD_USER_SerialStrDesc (uint8_t Speed, uint16_t *pLen)
{
    if(Speed  == USB_OTG_SPEED_HIGH)
    {

    }
    else if(Speed == USB_OTG_SPEED_FULL)
    {
        USBD_GetUnicodeString ((uint8_t *)USBD_SERIALNUMBER_STRING, USBD_StrDesc, pLen);
    }

    return USBD_StrDesc;
}

/**
  * @brief  Get the configuration string descriptor
  * @param  Speed: usb device speed
  * @param  pLen: data length pointer
  * @retval Descriptor buffer pointer
  */
uint8_t* USBD_USER_ConfigStrDesc (uint8_t Speed, uint16_t *pLen)
{
    if(Speed == USB_OTG_SPEED_HIGH)
    {

    }
    else if(Speed == USB_OTG_SPEED_FULL)
    {
        USBD_GetUnicodeString ((uint8_t *)USBD_CONFIGURATION_STRING, USBD_StrDesc, pLen); 
    }

    return USBD_StrDesc;  
}

/**
  * @brief  Get the interface string descriptor
  * @param  Speed: usb device speed
  * @param  pLen: data length pointer
  * @retval Descriptor buffer pointer
  */
uint8_t* USBD_USER_ItfStrDesc (uint8_t Speed, uint16_t *pLen)
{
    if(Speed == USB_OTG_SPEED_HIGH)
    {

    }
    else if(Speed == USB_OTG_SPEED_FULL)
    {
        USBD_GetUnicodeString ((uint8_t *)USBD_INTERFACE_STRING, USBD_StrDesc, pLen);
    }

    return USBD_StrDesc;  
}

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/

