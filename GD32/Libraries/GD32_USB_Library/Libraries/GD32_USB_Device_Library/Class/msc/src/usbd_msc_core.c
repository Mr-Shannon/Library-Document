/**
  ******************************************************************************
  * @file    usbd_msc_core.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file provides all the MSC core functions.
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_msc_mem.h"
#include "usbd_msc_core.h"
#include "usbd_msc_bot.h"
#include "usbd_enum.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */


/** @addtogroup USB_Device_Class_Library
  * @{
  */

/** @addtogroup USBD_MSC
  * @{
  */

/** @defgroup USBD_MSC_Core_Function_Prototypes
  * @{
  */
static uint8_t  USBD_MSC_Init               (void *pudev, uint8_t CfgIdx);
static uint8_t  USBD_MSC_DeInit             (void *pudev, uint8_t CfgIdx);
static uint8_t  USBD_MSC_GetClassDescriptor (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_MSC_ClassReqHandle     (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_MSC_GetInterface       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_MSC_SetInterface       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_MSC_ClearFeature       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_MSC_DataIn             (void *pudev, uint8_t EpID);
static uint8_t  USBD_MSC_DataOut            (void *pudev, uint8_t EpID);
static uint8_t *USBD_MSC_GetCfgDesc         (uint8_t speed, uint16_t *pLen);


#ifdef USB_OTG_HS_CORE
uint8_t  *USBD_MSC_GetOtherCfgDesc (uint8_t Speed, uint16_t *pLen);
#endif

/**
  * @}
  */

/** @defgroup USBD_MSC_Core_Private_Variables
  * @{
  */
USBD_Class_cb_TypeDef  USBD_MSC_cb = 
{
    USBD_MSC_Init,
    USBD_MSC_DeInit,
    USBD_MSC_GetClassDescriptor,
    USBD_MSC_ClassReqHandle,
    USBD_MSC_GetInterface,
    USBD_MSC_SetInterface,
    NULL,
    USBD_MSC_ClearFeature,
    NULL, /*EP0_TxSent*/
    NULL, /*EP0_RxReady*/
    USBD_MSC_DataIn,
    USBD_MSC_DataOut,
    NULL, /*SOF */ 
    NULL,
    NULL,
    USBD_MSC_GetCfgDesc,
#ifdef USB_OTG_HS_CORE  
    USBD_MSC_GetOtherCfgDesc,
#endif
};

/* USB Mass storage device configuration descriptor set */
uint8_t USBD_MSC_CfgDesc[USB_MSC_CONFIG_DESC_SIZE] =
{
    0x09,   /* bLength: configuration descriptor size */
    USB_DESCTYPE_CONFIGURATION, /* bDescriptorType: configuration descriptor type */
    USB_MSC_CONFIG_DESC_SIZE,   /* wTotalLength: configuration descriptor set total length */
    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: configuration value */
    0x04,   /* iConfiguration: index of string descriptor describing the configuration */
    0xC0,   /* bmAttributes: device attributes (bus powered and not support remote wakeup) */
    0x32,   /* MaxPower 100 mA: this current is used for detecting Vbus */

    /******************** interface descriptor ********************/
    0x09,   /* bLength: interface descriptor size */
    USB_DESCTYPE_INTERFACE, /* bDescriptorType: interface descriptor type*/
    0x00,   /* bInterfaceNumber: number of interface */
    0x00,   /* bAlternateSetting: alternate setting */
    0x02,   /* bNumEndpoints: use 2 endpoints for Tx and Rx */
    0x08,   /* bInterfaceClass: MSC Class */
    0x06,   /* bInterfaceSubClass: SCSI transparent*/
    0x50,   /* nInterfaceProtocol: Bulk-only transport */
    0x05,   /* iInterface: index of interface string descriptor */

    /********************  Mass storage endpoint descriptor ********************/
    0x07,       /* bLength: Endpoint Descriptor size */
    USB_DESCTYPE_ENDPOINT,         /* bDescriptorType: endpoint descriptor type */
    MSC_IN_EP,  /* bEndpointAddress: endpoint address (EP1_IN) */
    0x02,       /* bmAttributes: endpoint attribute(bulk endpoint) */
    LOWBYTE(MSC_DATA_PACKET_SIZE), /* wMaxPacketSize: 64 bytes max */
    HIGHBYTE(MSC_DATA_PACKET_SIZE),
    0x00,       /* bInterval: polling interval is ignored */

    0x07,       /* bLength: Endpoint Descriptor size */
    USB_DESCTYPE_ENDPOINT,         /* bDescriptorType: endpoint descriptor type */
    MSC_OUT_EP, /*bEndpointAddress: endpoint address (EP2_OUT) */
    0x02,       /* bmAttributes: endpoint attribute(bulk endpoint) */
    LOWBYTE(MSC_DATA_PACKET_SIZE), /* wMaxPacketSize: 64 bytes max */
    HIGHBYTE(MSC_DATA_PACKET_SIZE),
    0x00        /* bInterval: polling interval is ignored */
};

#ifdef USB_OTG_HS_CORE
__ALIGN_BEGIN uint8_t USBD_MSC_OtherCfgDesc[USB_MSC_CONFIG_DESC_SIZ] __ALIGN_END =
{
    0x09,   /* bLength: Configuation Descriptor size */
    USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,   
    USB_MSC_CONFIG_DESC_SIZ,
    0x00,
    0x01,   /* bNumInterfaces: 1 interface */
    0x01,   /* bConfigurationValue: */
    0x04,   /* iConfiguration: */
    0xC0,   /* bmAttributes: */
    0x32,   /* MaxPower 100 mA */

    /********************  Mass Storage interface ********************/
    0x09,   /* bLength: Interface Descriptor size */
    0x04,   /* bDescriptorType: */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints*/
    0x08,   /* bInterfaceClass: MSC Class */
    0x06,   /* bInterfaceSubClass : SCSI transparent command set*/
    0x50,   /* nInterfaceProtocol */
    0x05,          /* iInterface: */

    /********************  Mass Storage Endpoints ********************/
    0x07,   /*Endpoint descriptor length = 7*/
    0x05,   /*Endpoint descriptor type */
    MSC_IN_EP,   /*Endpoint address (IN, address 1) */
    0x02,   /*Bulk endpoint type */
    0x40,
    0x00,
    0x00,   /*Polling interval in milliseconds */

    0x07,   /*Endpoint descriptor length = 7 */
    0x05,   /*Endpoint descriptor type */
    MSC_OUT_EP,   /*Endpoint address (OUT, address 1) */
    0x02,   /*Bulk endpoint type */
    0x40,
    0x00,
    0x00    /*Polling interval in milliseconds*/
};
#endif 

static uint8_t  USBD_MSC_MaxLun = 0;
static uint8_t  USBD_MSC_AltSet = 0;

/**
  * @}
  */

/** @defgroup USBD_MSC_Core_Private_Function
  * @{
  */

/**
  * @brief  Initialize the mass storage configuration
  * @param  pudev: pointer to usb device instance
  * @param  cfgidx: configuration index
  * @retval status
  */
static uint8_t  USBD_MSC_Init (void *pudev, uint8_t CfgIdx)
{
    USBD_MSC_DeInit(pudev, CfgIdx);

    /* Open EP IN */
    DCD_EP_Init(pudev,
                MSC_IN_EP,
                USB_OTG_EPTYPE_BULK,
                MSC_EPIN_SIZE);

    /* Open EP OUT */
    DCD_EP_Init(pudev,
                MSC_OUT_EP,
                USB_OTG_EPTYPE_BULK,
                MSC_EPOUT_SIZE);

    /* Init the BBB  layer */
    MSC_BBB_Init(pudev); 

    return USBD_OK;
}

/**
  * @brief  DeInitilaize  the mass storage configuration
  * @param  pudev: device instance
  * @param  cfgidx: configuration index
  * @retval status
  */
static uint8_t  USBD_MSC_DeInit (void *pudev, uint8_t CfgIdx)
{
    /* Close MSC EPs */
    DCD_EP_DeInit (pudev, MSC_IN_EP);
    DCD_EP_DeInit (pudev, MSC_OUT_EP);

    /* Un Init the BBB layer */
    MSC_BBB_DeInit(pudev);

    return USBD_OK;
}

/**
  * @brief  Handle the MSC class-specific requests
  * @param  pudev: pointer to usb device instance
  * @param  req: device class-specific request
  * @retval usb device operation status
  */
static uint8_t  USBD_MSC_ClassReqHandle (void *pudev, USB_DEVICE_REQ *req)
{
    switch (req->bRequest)
    {
        case BBB_GET_MAX_LUN :
            if((req->wValue  == 0) && 
                (req->wLength == 1) &&
                ((req->bmRequestType & 0x80) == 0x80))
            {
                USBD_MSC_MaxLun = USBD_STORAGE_fops->GetMaxLun();

                if(USBD_MSC_MaxLun > 0)
                {
                    USBD_CtlTx(pudev, &USBD_MSC_MaxLun, 1);
                }
                else
                {
                    USBD_CtlError(pudev, req);

                    return USBD_FAIL; 
                }
            }
            else
            {
                USBD_CtlError(pudev, req);

                return USBD_FAIL; 
            }
            break;

        case BBB_RESET :
            if((req->wValue  == 0) && 
                (req->wLength == 0) &&
                 ((req->bmRequestType & 0x80) != 0x80))
            {
                MSC_BBB_Reset(pudev);
            }
            else
            {
                USBD_CtlError(pudev, req);

                return USBD_FAIL; 
            }
            break;

        default:
            USBD_CtlError(pudev, req);

            return USBD_FAIL; 
    }

    return USBD_OK;
}

/**
  * @brief  Get MSC class descriptor(Report descriptor)
  * @param  pudev: pointer to usb device instance
  * @param  req: device request
  * @retval usb device operation status
  */
static uint8_t  USBD_MSC_GetClassDescriptor (void *pudev, USB_DEVICE_REQ *req)
{
    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Get Interface
  * @param  pudev: pointer to usb device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_MSC_GetInterface (void *pudev, USB_DEVICE_REQ *req)
{
    USBD_CtlTx (pudev, (uint8_t *)&USBD_MSC_AltSet, 1);

    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Set Interface
  * @param  pudev: pointer to usb device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_MSC_SetInterface (void *pudev, USB_DEVICE_REQ *req)
{
    USBD_MSC_AltSet = (uint8_t)(req->wValue);

    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Clear Feature
  * @param  pudev: pointer to usb device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_MSC_ClearFeature (void *pudev, USB_DEVICE_REQ *req)
{
    /* Flush the FIFO and Clear the stall status */    
    DCD_EP_FlushFifo(pudev, (uint8_t)req->wIndex);

    /* Re-activate the EP */
    DCD_EP_DeInit (pudev, (uint8_t)req->wIndex);

    if((((uint8_t)req->wIndex) & 0x80) == 0x80)
    {
        DCD_EP_Init(pudev,
                    ((uint8_t)req->wIndex),
                    USB_OTG_EPTYPE_BULK,
                    MSC_EPIN_SIZE);
    }
    else
    {
        DCD_EP_Init(pudev,
                    ((uint8_t)req->wIndex),
                    USB_OTG_EPTYPE_BULK,
                    MSC_EPOUT_SIZE);
    }

    MSC_BBB_CplClrFeature(pudev, (uint8_t)req->wIndex);

    return USBD_OK;
}

/**
  * @brief  Handle data IN Stage
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval status
  */
static uint8_t  USBD_MSC_DataIn (void *pudev, uint8_t EpID)
{
    MSC_BBB_DataIn(pudev, EpID);

    return USBD_OK;
}

/**
  * @brief  Handle data OUT Stage
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval status
  */
static uint8_t  USBD_MSC_DataOut (void *pudev, uint8_t EpID)
{
    MSC_BBB_DataOut (pudev, EpID);

    return USBD_OK;
}

/**
  * @brief  Get configuration descriptor
  * @param  speed : current device speed
  * @param  len : pointer to data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_MSC_GetCfgDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen = sizeof (USBD_MSC_CfgDesc);

    return USBD_MSC_CfgDesc;
}

/**
  * @brief  Get other speed configuration descriptor 
  * @param  Speed: current device speed
  * @param  pLen: pointer data length
  * @retval pointer to descriptor buffer
  */
#ifdef USB_OTG_HS_CORE
static uint8_t  *USBD_MSC_GetOtherCfgDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen = sizeof (USBD_MSC_OtherCfgDesc);

    return USBD_MSC_OtherCfgDesc;
}
#endif

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
