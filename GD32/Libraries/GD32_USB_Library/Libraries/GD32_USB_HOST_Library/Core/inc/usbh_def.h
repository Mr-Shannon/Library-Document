/**
  ******************************************************************************
  * @file    usbh_def.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Definitions used in the USB host library
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef  __GD32F10X_USBH_DEF_H
#define  __GD32F10X_USBH_DEF_H

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_DEF
  * @{
  */

#ifndef USBH_NULL
    #define USBH_NULL ((void *)0)
#endif

#ifndef FALSE
    #define FALSE   0
#endif

#ifndef TRUE
    #define TRUE    1
#endif

#define  LE16(addr)                                    (((u16)(*((u8 *)(addr))))\
                                                        + (((u16)(*(((u8 *)(addr)) + 1))) << 8))

#define  USB_LEN_DESC_HDR                               0x02
#define  USB_LEN_DEV_DESC                               0x12
#define  USB_LEN_CFG_DESC                               0x09
#define  USB_LEN_IF_DESC                                0x09
#define  USB_LEN_EP_DESC                                0x07
#define  USB_LEN_OTG_DESC                               0x03
#define  USB_LEN_SETUP_PKT                              0x08

/* bmRequestType :D7 Data Phase Transfer Direction  */
#define  USB_REQ_DIR_MASK                               0x80
#define  USB_REQ_DIR_H2D                                0x00
#define  USB_REQ_DIR_D2H                                0x80

/* bmRequestType D6..5 Type */
#define  USB_STANDARD_REQ                               0x00
#define  USB_CLASS_REQ                                  0x20
#define  USB_VENDOR_REQ                                 0x40
#define  USB_REQ_MASK                                   0x60

/* bmRequestType D4..0 Recipient */
#define  USB_REQTYPE_DEVICE                             0x00
#define  USB_REQTYPE_INTERFACE                          0x01
#define  USB_REQTYPE_ENDPOINT                           0x02
#define  USB_REQ_RECIPIENT_MASK                         0x1f

/* Table 9-4. Standard Request Codes  */
/* bRequest , Value */
#define  USBREQ_GET_STATUS                              0x00
#define  USBREQ_CLEAR_FEATURE                           0x01
#define  USBREQ_SET_FEATURE                             0x03
#define  USBREQ_SET_ADDRESS                             0x05
#define  USBREQ_GET_DESCRIPTOR                          0x06
#define  USBREQ_SET_DESCRIPTOR                          0x07
#define  USBREQ_GET_CONFIGURATION                       0x08
#define  USBREQ_SET_CONFIGURATION                       0x09
#define  USBREQ_GET_INTERFACE                           0x0A
#define  USBREQ_SET_INTERFACE                           0x0B
#define  USBREQ_SYNCH_FRAME                             0x0C

/* Table 9-5. Descriptor Types of USB Specifications */
#define  USB_DESCTYPE_DEVICE                            0x01
#define  USB_DESCTYPE_CONFIGURATION                     0x02
#define  USB_DESCTYPE_STRING                            0x03
#define  USB_DESCTYPE_INTERFACE                         0x04
#define  USB_DESCTYPE_ENDPOINT                          0x05
#define  USB_DESCTYPE_DEVICE_QUALIFIER                  0x06
#define  USB_DESCTYPE_OTHER_SPEED_CONFIGURATION         0x07
#define  USB_DESCTYPE_INTERFACE_POWER                   0x08

#define  USB_DESCTYPE_HID                               0x21
#define  USB_DESCTYPE_HID_REPORT                        0x22


#define USB_DEVDESC_SIZE                                18
#define USB_CFGDESC_SIZE                                9
#define USB_INTDESC_SIZE                                9
#define USB_EPDESC_SIZE                                 7
#define USB_HID_DESC_SIZE                               9

/* Descriptor Type and Descriptor Index  */
/* Use the following values when calling the function USBH_GetDescriptor  */
#define  USB_DEVDESC                    ((USB_DESCTYPE_DEVICE << 8) & 0xFF00)
#define  USB_CFGDESC                    ((USB_DESCTYPE_CONFIGURATION << 8) & 0xFF00)
#define  USB_STRDESC                    ((USB_DESCTYPE_STRING << 8) & 0xFF00)
#define  USB_INTDESC                    ((USB_DESCTYPE_INTERFACE << 8) & 0xFF00)
#define  USB_EPDESC                     ((USB_DESCTYPE_INTERFACE << 8) & 0xFF00)
#define  USB_DEVQUADESC                 ((USB_DESCTYPE_DEVICE_QUALIFIER << 8) & 0xFF00)
#define  USB_OSPCFGDESC                 ((USB_DESCTYPE_OTHER_SPEED_CONFIGURATION << 8) & 0xFF00)
#define  USB_INTPWRDESC                 ((USB_DESCTYPE_INTERFACE_POWER << 8) & 0xFF00)
#define  USB_HIDREPDESC                 ((USB_DESCTYPE_HID_REPORT << 8) & 0xFF00)
#define  USB_HIDDESC                    ((USB_DESCTYPE_HID << 8) & 0xFF00)

#define  USB_EP_TYPE_CTRL                               0x00
#define  USB_EP_TYPE_ISOC                               0x01
#define  USB_EP_TYPE_BULK                               0x02
#define  USB_EP_TYPE_INTR                               0x03

#define  USB_EP_DIR_OUT                                 0x00
#define  USB_EP_DIR_IN                                  0x80
#define  USB_EP_DIR_MSK                                 0x80

/* supported classes */
#define USB_MSC_CLASS                                   0x08
#define USB_HID_CLASS                                   0x03

/* Interface Descriptor field values for HID Boot Protocol */
#define HID_BOOT_CODE                                   0x01
#define HID_KEYBRD_BOOT_CODE                            0x01
#define HID_MOUSE_BOOT_CODE                             0x02

/* As per USB specs 9.2.6.4 :Standard request with data request timeout: 5sec
   Standard request with no data stage timeout : 50ms */
#define DATA_STAGE_TIMEOUT                              5000 
#define NODATA_STAGE_TIMEOUT                            50

/**
  * @}
  */

#define USBH_CFG_DESC_SET_SIZE (USB_CFGDESC_SIZE + USB_INTDESC_SIZE \
                                           + (USBH_MAX_EP_NUM * USB_EPDESC_SIZE))

/*  This Union is copied from usb_core.h  */
typedef union _USB_Setup
{
    uint8_t Data[8];

    struct _SetupPacket_Struct
    {
        uint8_t           bmRequestType;
        uint8_t           bRequest;
        uint16_t          wValue;
        uint16_t          wIndex;
        uint16_t          wLength;
    } b;
}
USB_Setup_TypeDef;

typedef struct _DescHeader 
{
    uint8_t bLength;                /* Size of this descriptor in bytes */
    uint8_t bDescriptorType;        /* Descriptor type (constant) */
}
USBH_DescHeader_t;

typedef struct _DeviceDescriptor
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  bcdUSB;               /* USB Specification release number which device complies too */
    uint8_t   bDeviceClass;         /* Class code */
    uint8_t   bDeviceSubClass;      /* Subclass code */
    uint8_t   bDeviceProtocol;      /* Protocol code */
    uint8_t   bMaxPacketSize0;      /* Maximum packet size for endpoint zero (only 8, 16, 32, or 64 are valid) */
    uint16_t  idVendor;             /* Vendor ID (Assigned by USB-IF) */
    uint16_t  idProduct;            /* Product ID (Assigned by Manufacturer) */
    uint16_t  bcdDevice;            /* Device Release Number in binary-coded decima */
    uint8_t   iManufacturer;        /* Index of Manufacturer String Descriptor */
    uint8_t   iProduct;             /* Index of Product String Descriptor */
    uint8_t   iSerialNumber;        /* Index of Serial Number String Descriptor */
    uint8_t   bNumConfigurations;   /* Number of possible configurations */
}
USBH_DevDesc_TypeDef;

typedef struct _ConfigurationDescriptor
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  wTotalLength;         /* Total Length of Data Returned */
    uint8_t   bNumInterfaces;       /* Number of Interfaces */
    uint8_t   bConfigurationValue;  /* Value to use as an argument to select this configuration*/
    uint8_t   iConfiguration;       /* Index of String Descriptor Describing this configuration */
    uint8_t   bmAttributes;         /* D7 Bus Powered, D6 Self Powered, D5 Remote Wakeup, D4..0 Reserved (0)*/
    uint8_t   bMaxPower;            /* Maximum Power Consumption */
}
USBH_CfgDesc_TypeDef;

typedef struct _HIDDescriptor
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint16_t  bcdHID;                   /* indicates what endpoint this descriptor is describing */
    uint8_t   bCountryCode;             /* specifies the transfer type. */
    uint8_t   bNumDescriptors;          /* specifies the transfer type. */
    uint8_t   bReportDescriptorType;    /* Maximum Packet Size this endpoint is capable of sending or receiving */  
    uint16_t  wItemLength;              /* is used to specify the polling interval of certain transfers. */
}
USBH_HIDDesc_TypeDef;


typedef struct _InterfaceDescriptor
{
    uint8_t bLength;
    uint8_t bDescriptorType;
    uint8_t bInterfaceNumber;
    uint8_t bAlternateSetting;    /* Value used to select alternative setting */
    uint8_t bNumEndpoints;        /* Number of Endpoints used for this interface */
    uint8_t bInterfaceClass;      /* Class Code (Assigned by USB Org) */
    uint8_t bInterfaceSubClass;   /* Subclass Code (Assigned by USB Org) */
    uint8_t bInterfaceProtocol;   /* Protocol Code */
    uint8_t iInterface;           /* Index of String Descriptor Describing this interface */
}
USBH_ItfDesc_TypeDef;

typedef struct _EndpointDescriptor
{
    uint8_t   bLength;
    uint8_t   bDescriptorType;
    uint8_t   bEndpointAddress; /* indicates what endpoint this descriptor is describing */
    uint8_t   bmAttributes;     /* specifies the transfer type. */
    uint16_t  wMaxPacketSize;   /* Maximum Packet Size this endpoint is capable of sending or receiving */
    uint8_t   bInterval;        /* is used to specify the polling interval of certain transfers. */
}
USBH_EpDesc_TypeDef;

/**
  * @}
  */

#endif /* __GD32F10X_USBH_DEF_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
