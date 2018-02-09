/**
  ******************************************************************************
  * @file    usbd_cdc_core.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB CDC class device core functions.                               
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_core.h"
#include "usbd_desc.h"
#include "usbd_enum.h"
#include "usbd_cdc_vcp.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Class_Library
  * @{
  */

/** @addtogroup USBD_CDC
  * @{
  */

static uint8_t  USBD_CDC_Init               (void *pudev, uint8_t ConfigIndex);
static uint8_t  USBD_CDC_DeInit             (void *pudev, uint8_t ConfigIndex);
static uint8_t  USBD_CDC_GetClassDescriptor (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_CDC_ClassReqHandle     (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_CDC_GetInterface       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_CDC_SetInterface       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_CDC_EP0_RxReady        (void *pudev);
static uint8_t  USBD_CDC_DataIn             (void *pudev, uint8_t EpID);
static uint8_t  USBD_CDC_DataOut            (void *pudev, uint8_t EpID);
static uint8_t  USBD_CDC_SOF                (void *pudev);
static uint8_t* USBD_CDC_GetCfgDesc         (uint8_t Speed, uint16_t *pLen);

static void  USBD_CDC_AsynchXferHandle (void *pudev);

/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Variables
  * @{
  */
extern CDC_IF_Fop_TypeDef APP_FOPS;
extern uint8_t USBD_DeviceDesc[USB_DEVICE_DESC_SIZE];
static __IO uint32_t  USBD_CDC_AltSet = 0;

uint8_t USB_DATA_Buffer[USB_CDC_DATA_PACKET_SIZE];
uint8_t USB_CMD_Buffer[USB_CDC_CMD_PACKET_SIZE];
uint8_t APP_DATA_Buffer[APP_RX_DATA_SIZE];

__IO uint32_t end_packet = 0;

uint32_t AppBufInPtr  = 0;
uint32_t AppBufOutPtr = 0;
uint32_t AppRxLength  = 0;

uint8_t  USB_Tx_State = 0;

static uint32_t CDCCmd = 0xFF;
static uint32_t CDCLen = 0;

/* CDC interface class callbacks structure */
USBD_Class_cb_TypeDef  USBD_CDC_cb = 
{
    USBD_CDC_Init,
    USBD_CDC_DeInit,
    USBD_CDC_GetClassDescriptor,
    USBD_CDC_ClassReqHandle,
    USBD_CDC_GetInterface, 
    USBD_CDC_SetInterface,
    NULL,
    NULL,
    NULL,                 /* EP0_TxSent, */
    USBD_CDC_EP0_RxReady,
    USBD_CDC_DataIn,
    USBD_CDC_DataOut,
    USBD_CDC_SOF,
    NULL,
    NULL,     
    USBD_CDC_GetCfgDesc,
#ifdef USE_USB_OTG_HS   
    USBD_CDC_GetOtherCfgDesc, /* use same cobfig as per FS */
#endif /* USE_USB_OTG_HS  */
};

/* USB CDC device Configuration Descriptor */
uint8_t USBD_CDC_CfgDesc[USB_CDC_CONFIG_DESC_SIZE] =
{
    /*Configuration Descriptor*/
    0x09,   /* bLength: configuration descriptor size */
    USB_DESCTYPE_CONFIGURATION,   /* bDescriptorType: configuration descriptor type */
    USB_CDC_CONFIG_DESC_SIZE,     /* wTotalLength: configuration descriptor set total length */
    0x00,
    0x02,   /* bNumInterfaces: 2 interface */
    0x01,   /* bConfigurationValue: Configuration value */
    0x00,   /* iConfiguration: index of string descriptor describing the configuration */
    0x80,   /* bmAttributes: device attributes (bus powered and not support remote wakeup) */
    0x32,   /* bMaxPower 100 mA: this current is used for detectiong Vbus */

    /*Interface descriptor */
    0x09,   /* bLength: interface descriptor size */
    USB_DESCTYPE_INTERFACE,  /* bDescriptorType: interface descriptor type */
    0x00,   /* bInterfaceNumber: number of interface */
    0x00,   /* bAlternateSetting: alternate setting */
    0x01,   /* bNumEndpoints: 1 endpoint used for command IN */
    0x02,   /* bInterfaceClass: CDC class */
    0x02,   /* bInterfaceSubClass: abstract control model */
    0x01,   /* nInterfaceProtocol: common AT commands */
    0x00,   /* iInterface: index of interface string descriptor */

    /* Header functional descriptor */
    0x05,   /* bFunctionLength: the descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x00,   /* bDescriptorSubtype: header function descriptor */
    0x10,   /* bcdCDC: spec release number (CDC1.10) */
    0x01,

    /* Call management functional descriptor */
    0x05,   /* bFunctionLength: the descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00,   /* bmCapabilities: D0 is reset, D1 is ignored */
    0x01,   /* bDataInterface: 1 interface used for call management */

    /* ACM functional descriptor */
    0x04,   /* bFunctionLength: the dsecriptor length */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities: D1 */

    /* Union functional descriptor */
    0x05,   /* bFunctionLength: the descriptor length */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    0x00,   /* bMasterInterface: Communication class interface */
    0x01,   /* bSlaveInterface0: Data Class Interface */

    /* Command endpoint descriptor */
    0x07,                           /* bLength: endpoint descriptor size */
    USB_DESCTYPE_ENDPOINT,          /* bDescriptorType: endpoint descriptor type */
    CDC_CMD_EP,                     /* bEndpointAddress: endpoint address(EP2_IN) */
    0x03,                           /* bmAttributes: interrupt endpoint */
    LOWBYTE(USB_CDC_CMD_PACKET_SIZE),    /* wMaxPacketSize: 8 bytes max */
    HIGHBYTE(USB_CDC_CMD_PACKET_SIZE),
    0x0A,                           /* bInterval: polling interval(10ms) */

    /* Data class interface descriptor */
    0x09,   /* bLength: interface descriptor size */
    USB_DESCTYPE_INTERFACE,  /* bDescriptorType: interface descriptor type */
    0x01,   /* bInterfaceNumber: number of interface */
    0x00,   /* bAlternateSetting: alternate setting */
    0x02,   /* bNumEndpoints: 2 endpoints used */
    0x0A,   /* bInterfaceClass: CDC class */
    0x00,   /* bInterfaceSubClass: no set */
    0x00,   /* bInterfaceProtocol: no set */
    0x00,   /* iInterface: no set */

    /* Data OUT endpoint descriptor */
    0x07,                              /* bLength: endpoint descriptor size */
    USB_DESCTYPE_ENDPOINT,             /* bDescriptorType: endpoint descriptor type */
    CDC_DATA_OUT_EP,                   /* bEndpointAddress: endpoint address(EP3_OUT) */
    0x02,                              /* bmAttributes: bulk endpoint */
    LOWBYTE(USB_CDC_DATA_PACKET_SIZE), /* wMaxPacketSize: 64 bytes max */
    HIGHBYTE(USB_CDC_DATA_PACKET_SIZE),
    0x00,                              /* bInterval: ignore for Bulk transfer */

    /* Data IN endpoint descriptor */
    0x07,                              /* bLength: endpoint descriptor size */
    USB_DESCTYPE_ENDPOINT,             /* bDescriptorType: endpoint descriptor type */
    CDC_DATA_IN_EP,                    /* bEndpointAddress: endpoint address(EP1_IN) */
    0x02,                              /* bmAttributes: bulk endpoint */
    LOWBYTE(USB_CDC_DATA_PACKET_SIZE), /* wMaxPacketSize: 64 bytes max */
    HIGHBYTE(USB_CDC_DATA_PACKET_SIZE),
    0x00                               /* bInterval: ignore for bulk transfer */
};

#ifdef USE_USB_OTG_HS
__ALIGN_BEGIN uint8_t USBD_CDC_OtherCfgDesc[USB_CDC_CONFIG_DESC_SIZ]  __ALIGN_END =
{
    0x09,   /* bLength: Configuation Descriptor size */
    USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION,   
    USB_CDC_CONFIG_DESC_SIZ,
    0x00,
    0x02,   /* bNumInterfaces: 2 interfaces */
    0x01,   /* bConfigurationValue: */
    0x04,   /* iConfiguration: */
    0xC0,   /* bmAttributes: */
    0x32,   /* MaxPower 100 mA */  

    /*Interface Descriptor */
    0x09,   /* bLength: Interface Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
    /* Interface descriptor type */
    0x00,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x01,   /* bNumEndpoints: One endpoints used */
    0x02,   /* bInterfaceClass: Communication Interface Class */
    0x02,   /* bInterfaceSubClass: Abstract Control Model */
    0x01,   /* bInterfaceProtocol: Common AT commands */
    0x00,   /* iInterface: */

    /*Header Functional Descriptor*/
    0x05,   /* bLength: Endpoint Descriptor size */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x00,   /* bDescriptorSubtype: Header Func Desc */
    0x10,   /* bcdCDC: spec release number */
    0x01,

    /*Call Management Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x01,   /* bDescriptorSubtype: Call Management Func Desc */
    0x00,   /* bmCapabilities: D0+D1 */
    0x01,   /* bDataInterface: 1 */

    /*ACM Functional Descriptor*/
    0x04,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
    0x02,   /* bmCapabilities */

    /*Union Functional Descriptor*/
    0x05,   /* bFunctionLength */
    0x24,   /* bDescriptorType: CS_INTERFACE */
    0x06,   /* bDescriptorSubtype: Union func desc */
    0x00,   /* bMasterInterface: Communication class interface */
    0x01,   /* bSlaveInterface0: Data Class Interface */

    /*Endpoint 2 Descriptor*/
    0x07,                           /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
    CDC_CMD_EP,                     /* bEndpointAddress */
    0x03,                           /* bmAttributes: Interrupt */
    LOBYTE(CDC_CMD_PACKET_SZE),     /* wMaxPacketSize: */
    HIBYTE(CDC_CMD_PACKET_SZE),
    0xFF,                           /* bInterval: */

    /*Data class interface descriptor*/
    0x09,   /* bLength: Endpoint Descriptor size */
    USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
    0x01,   /* bInterfaceNumber: Number of Interface */
    0x00,   /* bAlternateSetting: Alternate setting */
    0x02,   /* bNumEndpoints: Two endpoints used */
    0x0A,   /* bInterfaceClass: CDC */
    0x00,   /* bInterfaceSubClass: */
    0x00,   /* bInterfaceProtocol: */
    0x00,   /* iInterface: */

    /*Endpoint OUT Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType: Endpoint */
    CDC_OUT_EP,                       /* bEndpointAddress */
    0x02,                             /* bmAttributes: Bulk */
    0x40,                             /* wMaxPacketSize: */
    0x00,
    0x00,                             /* bInterval: ignore for Bulk transfer */

    /*Endpoint IN Descriptor*/
    0x07,   /* bLength: Endpoint Descriptor size */
    USB_ENDPOINT_DESCRIPTOR_TYPE,     /* bDescriptorType: Endpoint */
    CDC_IN_EP,                        /* bEndpointAddress */
    0x02,                             /* bmAttributes: Bulk */
    0x40,                             /* wMaxPacketSize: */
    0x00,
    0x00                              /* bInterval */
};
#endif /* USE_USB_OTG_HS */

/**
  * @}
  */

/** @defgroup USBD_CDC_Private_Functions
  * @{
  */

/**
  * @brief  Initialize the CDC interface
  * @param  pudev: pointer to usb device instance
  * @param  ConfigIndex: configuration index
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_Init (void *pudev, uint8_t ConfigIndex)
{
//    uint8_t *pbuf;

    /* Initialize the data Tx endpoint */
    DCD_EP_Init(pudev,
                CDC_DATA_IN_EP,
                USB_OTG_EPTYPE_BULK,
                CDC_DATA_IN_PACKET_SIZE);

    /* Initialize the data Rx endpoint */
    DCD_EP_Init(pudev,
                CDC_DATA_OUT_EP,
                USB_OTG_EPTYPE_BULK,
                CDC_DATA_OUT_PACKET_SIZE);

    /* Initialize the command Tx endpoint */
    DCD_EP_Init(pudev,
                CDC_CMD_EP,
                USB_OTG_EPTYPE_INT,
                USB_CDC_CMD_PACKET_SIZE);

//    pbuf = (uint8_t *)USBD_DeviceDesc;
//    pbuf[4] = DEVICE_CLASS_CDC;
//    pbuf[5] = DEVICE_SUBCLASS_CDC;

    /* Initialize the Interface physical components */
    APP_FOPS.pIf_Init(DEFAULT_CONFIG);

    /* Enable OUT endpoint prepare to receive next packet */
    DCD_EP_Rx(pudev,
              CDC_DATA_OUT_EP,
              (uint8_t*)(USB_DATA_Buffer),
              CDC_DATA_OUT_PACKET_SIZE);


    return USBD_OK;
}

/**
  * @brief  De-Initialize the CDC interface
  * @param  pudev: pointer to usb device instance
  * @param  ConfigIndex: configuration index
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_DeInit (void *pudev, uint8_t ConfigIndex)
{
    /* Deinitialize the data Tx/Rx endpoint */
    DCD_EP_DeInit (pudev, CDC_DATA_IN_EP);
    DCD_EP_DeInit (pudev, CDC_DATA_OUT_EP);

    /* Deinitialize the command Tx endpoint */
    DCD_EP_DeInit (pudev, CDC_CMD_EP);

    /* Restore default state of the interface physical components */
    APP_FOPS.pIf_DeInit();

    return USBD_OK;
}

/**
  * @brief  Get CDC class descriptor
  * @param  pudev: pointer to usb device instance
  * @param  req: device request
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_GetClassDescriptor (void *pudev, USB_DEVICE_REQ *req)
{
    uint16_t len = USB_CDC_DESC_SIZE;
    uint8_t  *pbuf= (uint8_t*)USBD_CDC_CfgDesc + 9;

    if((req->wValue >> 8) == CDC_DESC_TYPE)
    {
        len = MIN(USB_CDC_DESC_SIZE, req->wLength);
        pbuf = (uint8_t*)USBD_CDC_CfgDesc + 9 + (9 * USBD_ITF_MAX_NUM);
    }

    USBD_CtlTx (pudev, pbuf, len);

    return USBD_OK;
}

/**
  * @brief  Handle the CDC class-specific request
  * @param  pudev: pointer to usb device instance
  * @param  req: device class-specific request
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_ClassReqHandle (void *pudev, USB_DEVICE_REQ *req)
{
    if (req->wLength)
    {
        if (req->bmRequestType & 0x80)
        {
            /* Request is from device to host */

            /* Get the request data from media access layer */
            APP_FOPS.pIf_Ctrl(req->bRequest, USB_CMD_Buffer, req->wLength);

            /* Send the request data to the host */
            USBD_CtlTx (pudev, USB_CMD_Buffer, req->wLength);
        }
        else
        {
            /* Request is from host to device */

            /* Set the value of the current command to be processed */
            CDCCmd = req->bRequest;
            CDCLen = req->wLength;

            /* Enable EP0 prepare to receive command data packet */
            USBD_CtlRx (pudev, USB_CMD_Buffer, req->wLength);
        }
    }
    else
    {
        /* The command to be handled in the media access layer */
        APP_FOPS.pIf_Ctrl(req->bRequest, NULL, 0);
    }

    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Get Interface
  * @param  pudev: pointer to usb device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_GetInterface (void *pudev, USB_DEVICE_REQ *req)
{
    USBD_CtlTx (pudev, (uint8_t *)&USBD_CDC_AltSet, 1);

    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Set Interface
  * @param  pudev: pointer to usb device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_SetInterface (void *pudev, USB_DEVICE_REQ *req)
{
    if ((uint8_t)(req->wValue) < USBD_ITF_MAX_NUM)
    {
        USBD_CDC_AltSet = (uint8_t)(req->wValue);
    }
    else
    {
        /* Call the error management function (command will be nacked */
        USBD_CtlError (pudev, req);
    }

    return USBD_OK;
}

/**
  * @brief  Command data received on control endpoint
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_EP0_RxReady (void  *pudev)
{
    if (CDCCmd != NO_CMD)
    {
        /* Process the command data */
        APP_FOPS.pIf_Ctrl(CDCCmd, USB_CMD_Buffer, CDCLen);

        CDCCmd = NO_CMD;
        CDCLen = 0;
    }

    return USBD_OK;
}

/**
  * @brief  Data sent on non-control IN endpoint
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_DataIn (void *pudev, uint8_t EpID)
{
    uint16_t USB_Tx_length;

    if (USB_Tx_State == 1)
    {
        if (AppRxLength == 0) 
        {
            if (end_packet == 1)
            {
                end_packet = 0;

                /*Send zero-length packet*/
                DCD_EP_Tx (pudev, CDC_DATA_IN_EP, 0, 0);
            }
            else
            {
                USB_Tx_State = 0;
            }
        }
        else
        {
            if (AppRxLength > CDC_DATA_IN_PACKET_SIZE)
            {
                USB_Tx_length = CDC_DATA_IN_PACKET_SIZE;
            }
            else
            {
                USB_Tx_length = AppRxLength;
                if (AppRxLength == CDC_DATA_IN_PACKET_SIZE) end_packet = 1;
            }

            /* Prepare the available data buffer to be sent on IN endpoint */
            DCD_EP_Tx (pudev,
                       CDC_DATA_IN_EP,
                       &APP_DATA_Buffer[AppBufOutPtr],
                       USB_Tx_length);

            AppBufOutPtr += USB_Tx_length;
            AppRxLength  -= USB_Tx_length;
        }
    }

    return USBD_OK;
}

/**
  * @brief  Data received on non-control Out endpoint
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_DataOut (void *pudev, uint8_t EpID)
{
    uint16_t USB_Rx_Cnt;

    /* Update the received data counter */
    USB_Rx_Cnt = ((USB_OTG_CORE_HANDLE*)pudev)->dev.OutEp[EpID].xfer_count;

    /* USB received data will be immediately processed in media access layer */
    APP_FOPS.pIf_DataRx(COM2, USB_DATA_Buffer, USB_Rx_Cnt);

    /* Enable Out endpoint prepare to receive next data packet */
    DCD_EP_Rx(pudev,
              CDC_DATA_OUT_EP,
              USB_DATA_Buffer,
              CDC_DATA_OUT_PACKET_SIZE);
	
    return USBD_OK;
}

/**
  * @brief  Handle start of frame event
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t  USBD_CDC_SOF (void *pudev)
{
    static uint32_t FrameCount = 0;

    if (FrameCount++ == CDC_IN_FRAME_INTERVAL)
    {
        /* Frame counter is reset */
        FrameCount = 0;

        /* Handle asynchronous transmission between USB endpoint and Usart */
        USBD_CDC_AsynchXferHandle(pudev);
    }

    return USBD_OK;
}

/**
  * @brief  Prepare send data over CDC IN endpoint
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
static void USBD_CDC_AsynchXferHandle (void *pudev)
{
    uint16_t USB_Tx_length;

    if(USB_Tx_State != 1)
    {
        if(AppBufOutPtr == AppBufInPtr) 
        {
            /* No data received by real usart */
            return;
        }

        if(AppBufOutPtr > AppBufInPtr) /* Need rollback */
        {
            AppRxLength = APP_RX_DATA_SIZE - AppBufOutPtr;
        }
        else
        {
            AppRxLength = AppBufInPtr - AppBufOutPtr;
        }

        if (AppRxLength > CDC_DATA_IN_PACKET_SIZE)
        {
            USB_Tx_length = CDC_DATA_IN_PACKET_SIZE;
        }
        else
        {
            USB_Tx_length = AppRxLength;
            if (USB_Tx_length == CDC_DATA_IN_PACKET_SIZE) end_packet = 1; /* Last packet will be sent */
        }


        DCD_EP_Tx (pudev,
                   CDC_DATA_IN_EP,
                   &APP_DATA_Buffer[AppBufOutPtr],
                   USB_Tx_length);

        /* Update Tx parameters */
        AppBufOutPtr += USB_Tx_length;
        AppRxLength  -= USB_Tx_length;

        USB_Tx_State = 1;

        if (AppBufOutPtr == APP_RX_DATA_SIZE)
        {
            AppBufOutPtr = 0;
        }
    }
}

/**
  * @brief  Getting configuration descriptor
  * @param  Speed: current device speed
  * @param  pLen: pointer to data length 
  * @retval Pointer to configuration descriptor buffer
  */
static uint8_t  *USBD_CDC_GetCfgDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen = sizeof (USBD_CDC_CfgDesc);

    return USBD_CDC_CfgDesc;
}

/**
  * @brief  Getting other configuration descriptor
  * @param  Speed: current device speed
  * @param  pLen: pointer data length
  * @retval pointer to descriptor buffer
  */
#ifdef USE_USB_OTG_HS
static uint8_t  *USBD_CDC_GetOtherCfgDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen = sizeof (USBD_CDC_OtherCfgDesc);

    return USBD_CDC_OtherCfgDesc;
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

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
