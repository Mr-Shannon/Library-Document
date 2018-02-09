/**
  ******************************************************************************
  * @file    usbd_hid_core.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB HID device class core functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_hid_core.h"
#include "usbd_desc.h"
#include "usbd_enum.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_Device_Class_Library
  * @{
  */

/** @defgroup USBD_HID
  * @{
  */

/** @defgroup USBD_HID_Private_FunctionPrototypes
  * @{
  */
static uint8_t  USBD_HID_Init               (void *pudev, uint8_t CfgIdx);
static uint8_t  USBD_HID_DeInit             (void *pudev, uint8_t CfgIdx);
static uint8_t  USBD_HID_GetClassDescriptor (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_HID_ClassReqHandle     (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_HID_GetInterface       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_HID_SetInterface       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t* USBD_HID_GetCfgDesc         (uint8_t Speed, uint16_t *pLen);
static uint8_t  USBD_HID_DataIn             (void *pudev, uint8_t EpID);

/**
  * @}
  */ 

/** @defgroup USBD_HID_Private_Variables
  * @{
  */
USBD_Class_cb_TypeDef  USBD_HID_cb = 
{
    USBD_HID_Init,
    USBD_HID_DeInit,
    USBD_HID_GetClassDescriptor,
    USBD_HID_ClassReqHandle,
    USBD_HID_GetInterface,
    USBD_HID_SetInterface,
    NULL,  /* GetFeature */
    NULL,  /* ClearFeature */
    NULL,  /* EP0_TxSent */
    NULL,  /* EP0_RxReady */
    USBD_HID_DataIn, 
    NULL,  /* DataOut */
    NULL,  /* SOF */
    NULL,
    NULL,
    USBD_HID_GetCfgDesc,

#ifdef USB_OTG_HS_CORE
    USBD_HID_GetCfgDesc, /* Use same config as per FS */
#endif
};

extern __IO uint8_t PrevXferComplete;

static uint32_t USBD_HID_AltSet = 0;
static uint32_t USBD_HID_Protocol = 0;
static uint32_t USBD_HID_IdleState = 0;

/* USB HID device Configuration Descriptor */
static uint8_t USBD_HID_CfgDesc[USB_HID_CONFIG_DESC_SIZE] =
{
    0x09,         /* bLength: configuration descriptor size */
    USB_DESCTYPE_CONFIGURATION, /* bDescriptorType: configuration descriptor type */
    USB_HID_CONFIG_DESC_SIZE,   /* wTotalLength: configuration descriptor set total length */
    0x00,
    0x01,         /* bNumInterfaces: 1 interface */
    0x01,         /* bConfigurationValue: configuration value */
    0x00,         /* iConfiguration: index of string descriptor describing the configuration */
    0xA0,         /* bmAttributes: device attributes (bus powered and support remote wakeup) */
    0x32,         /* bMaxPower 100 mA: this current is used for detecting Vbus */

    /************** interface descriptor  ****************/
    0x09,         /* bLength: interface descriptor size */
    USB_DESCTYPE_INTERFACE,/* bDescriptorType: interface descriptor type */
    0x00,         /* bInterfaceNumber: number of interface */
    0x00,         /* bAlternateSetting: alternate setting */
    0x01,         /* bNumEndpoints: just use 1 endpoint for Tx */
    0x03,         /* bInterfaceClass: HID class */
    0x01,         /* bInterfaceSubClass: 0 = no boot, 1 = BIOS boot */
    0x02,         /* nInterfaceProtocol: 0 = none, 1 = keyboard, 2 = mouse */
    0x00,         /* iInterface: index of interface string descriptor */

    /******************** HID descriptor  ********************/
    0x09,         /* bLength: HID descriptor size */
    HID_DESC_TYPE,/* bDescriptorType: HID */
    0x11,         /* bcdHID: HID class protocol(HID1.11) */
    0x01,
    0x00,         /* bCountryCode: device country code */
    0x01,         /* bNumDescriptors: number of HID class descriptors to follow */
    0x22,         /* bDescriptorType: followed class descriptor type(report descriptor) */
    HID_MOUSE_REPORT_DESC_SIZE,/* wItemLength: total length of report descriptor */
    0x00,

    /******************** Mouse endpoint descriptor ********************/
    0x07,          /* bLength: Endpoint Descriptor size */
    USB_DESCTYPE_ENDPOINT, /* bDescriptorType: endpoint descriptor type */
    HID_IN_EP,     /* bEndpointAddress: endpoint address (EP1_IN) */
    0x03,          /* bmAttributes: endpoint attribute(interrupt endpoint) */
    HID_IN_PACKET, /* wMaxPacketSize: 4 bytes max */
    0x00,
    0x0A,          /* bInterval: polling interval (10 ms) */
};

#ifdef USB_OTG_HS_CORE

/* USB HID device Configuration Descriptor */
static uint8_t USBD_HID_Desc[USB_HID_DESC_SIZE] =
{
    0x09,         /* bLength: HID Descriptor size */
    HID_DESC_TYPE,/* bDescriptorType: HID */
    0x11,         /* bcdHID: HID Class Spec release number */
    0x01,
    0x00,         /* bCountryCode: Hardware target country */
    0x01,         /* bNumDescriptors: Number of HID class descriptors to follow */
    0x22,         /* bDescriptorType */
    HID_MOUSE_REPORT_DESC_SIZE,/* wItemLength: Total length of Report descriptor */
    0x00,
};

#endif

static uint8_t HID_MOUSE_ReportDesc[HID_MOUSE_REPORT_DESC_SIZE] =
{
    0x05,   0x01,   /* USAGE_PAGE (Generic Desktop) */ 
    0x09,   0x02,   /* USAGE (Mouse)                */
    0xA1,   0x01,   /* COLLECTION (Application)     */

    0x09,   0x30,   /* USAGE (X)                    */
    0x09,   0x31,   /* USAGE (Y)                    */

    0x15,   0x81,   /* LOGICAL_MINIMUM (-127)       */
    0x25,   0x7F,   /* LOGICAL_MAXIMUM (127)        */
    0x75,   0x08,   /* REPORT_SIZE (8)              */
    0x95,   0x02,   /* REPORT_COUNT (2)             */

    0x81,   0x06,   /* INPUT (Data,Var,Rel)         */
    0xc0,           /* END_COLLECTION               */
};

/**
  * @}
  */

/** @defgroup USBD_HID_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the HID device
  * @param  pudev: pointer to usb device instance
  * @param  ConfigIndex: configuration index
  * @retval usb device operation status
  */
static uint8_t  USBD_HID_Init (void *pudev, uint8_t CfgIdx)
{
    /* Initialize Tx endpoint */
    DCD_EP_Init(pudev,
                HID_IN_EP,
                USB_OTG_EPTYPE_INT,
                HID_IN_PACKET);

    /* Initialize Rx endpoint */
    DCD_EP_Init(pudev,
                HID_OUT_EP,
                USB_OTG_EPTYPE_INT,
                HID_OUT_PACKET);

    return USBD_OK;
}

/**
  * @brief  De-Initialize the HID device
  * @param  pudev: pointer to usb device instance
  * @param  ConfigIndex: Configuration index
  * @retval usb device operation status
  */
static uint8_t  USBD_HID_DeInit (void *pudev, uint8_t CfgIdx)
{
    /* Deinitialize HID endpoints */
    DCD_EP_DeInit (pudev, HID_IN_EP);
    DCD_EP_DeInit (pudev, HID_OUT_EP);

    return USBD_OK;
}

/**
  * @brief  Handle the HID class-specific requests
  * @param  pudev: pointer to usb device instance
  * @param  req: device class-specific request
  * @retval usb device operation status
  */
static uint8_t  USBD_HID_ClassReqHandle (void *pudev, USB_DEVICE_REQ *req)
{
    switch (req->bRequest)
    {
        case GET_REPORT:
            /* No use for this driver */
            break;

        case GET_IDLE:
            USBD_CtlTx (pudev, (uint8_t *)&USBD_HID_IdleState, 1);
            break;

        case GET_PROTOCOL:
            USBD_CtlTx (pudev, (uint8_t *)&USBD_HID_Protocol, 1);
            break;

        case SET_REPORT:
            /* No use for this driver */
            break;

        case SET_IDLE:
            USBD_HID_IdleState = (uint8_t)(req->wValue >> 8);
            break;

        case SET_PROTOCOL:
            USBD_HID_Protocol = (uint8_t)(req->wValue);
            break;

        default:
            USBD_CtlError (pudev, req);
            return USBD_FAIL; 
    }

    return USBD_OK;
}

/**
  * @brief  Get HID class descriptor(Report descriptor)
  * @param  pudev: pointer to usb device instance
  * @param  req: device request
  * @retval usb device operation status
  */
static uint8_t  USBD_HID_GetClassDescriptor (void *pudev, USB_DEVICE_REQ *req)
{
    uint16_t len = 0;
    uint8_t  *pbuf = NULL;

    switch(req->wValue >> 8)
    {
        case HID_REPORT_DESCTYPE:
            len = MIN(HID_MOUSE_REPORT_DESC_SIZE, req->wLength);
            pbuf = (uint8_t *)HID_MOUSE_ReportDesc;
            break;

        case HID_DESC_TYPE:
            len = MIN(USB_HID_DESC_SIZE, req->wLength);
            pbuf = (uint8_t *)USBD_HID_CfgDesc + 0x12;
            break;

        default:
            break;
    }

    USBD_CtlTx (pudev, pbuf, len);

    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Get Interface
  * @param  pudev: usb device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_HID_GetInterface (void *pudev, USB_DEVICE_REQ *req)
{
    USBD_CtlTx (pudev, (uint8_t *)&USBD_HID_AltSet, 1);

    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Set Interface
  * @param  pudev: pointer to device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_HID_SetInterface (void *pudev, USB_DEVICE_REQ *req)
{
    USBD_HID_AltSet = (uint8_t)(req->wValue);

    return USBD_OK;
}

/**
  * @brief  Send mouse report
  * @param  pudev: pointer to usb device instance
  * @param  report: pointer to HID report
  * @param  Len: data length
  * @retval usb device operation status
  */
uint8_t USBD_HID_SendReport (USB_OTG_CORE_HANDLE  *pudev,
                                         uint8_t *report,
                                         uint16_t Len)
{
    /* Check if USB is configured */
    if (pudev->dev.device_cur_status == USB_OTG_STATUS_CONFIGURED)
    {
        PrevXferComplete = 0;

        DCD_EP_Tx (pudev, HID_IN_EP, report, Len);
    }

    return USBD_OK;
}

/**
  * @brief  Get mouse device configuration descriptor
  * @param  USBSpeed: current device speed
  * @param  pLen: pointer to data length
  * @retval Pointer to descriptor buffer
  */
static uint8_t  *USBD_HID_GetCfgDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen = sizeof (USBD_HID_CfgDesc);

    return USBD_HID_CfgDesc;
}

/**
  * @brief  Handle data IN Stage
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval usb device operation status
  */
static uint8_t  USBD_HID_DataIn (void *pudev, uint8_t EpID)
{
    /* Ensure that the FIFO is empty before a new transfer,
     * this condition could be caused by a new transfer 
     * before the end of the previous transfer
     */
    DCD_EP_FlushFifo(pudev, HID_IN_EP);

    PrevXferComplete = 1;

    return USBD_OK;
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
