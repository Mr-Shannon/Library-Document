/**
  ******************************************************************************
  * @file    usbd_dfu_core.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB DFU class core functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_dfu_core.h"
#include "usbd_desc.h"
#include "usbd_enum.h"
#include "usb_hwp.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_Device_Class_Library
  * @{
  */

/** @defgroup USBD_DFU
  * @{
  */

/** @defgroup USBD_DFU_Core_Private_FunctionPrototypes
  * @{
  */

/* DFU Device library callbacks */
static uint8_t  USBD_DFU_Init               (void *pudev, uint8_t ConfigIndex);
static uint8_t  USBD_DFU_DeInit             (void *pudev, uint8_t ConfigIndex);
static uint8_t  USBD_DFU_GetClassDescriptor (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_DFU_ClassReqHandle     (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_DFU_GetInterface       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_DFU_SetInterface       (void *pudev, USB_DEVICE_REQ *req);
static uint8_t  USBD_DFU_EP0_TxSent         (void *pudev);
static uint8_t  USBD_DFU_EP0_RxReady        (void *pudev);
static uint8_t* USBD_DFU_GetCfgDesc         (uint8_t Speed, uint16_t *pLen);
static uint8_t* USBD_DFU_GetUsrStringDesc   (uint8_t Speed, uint8_t Index, uint16_t *pLen);

/* DFU Requests management functions */
static void DFU_Req_DETACH    (void *pudev, USB_DEVICE_REQ *req);
static void DFU_Req_DNLOAD    (void *pudev, USB_DEVICE_REQ *req);
static void DFU_Req_UPLOAD    (void *pudev, USB_DEVICE_REQ *req);
static void DFU_Req_GETSTATUS (void *pudev);
static void DFU_Req_CLRSTATUS (void *pudev);
static void DFU_Req_GETSTATE  (void *pudev);
static void DFU_Req_ABORT     (void *pudev);
static void DFU_LeaveDFUMode  (void *pudev); 

/**
  * @}
  */

/** @defgroup USBD_DFU_Core_Private_Variables
  * @{
  */
extern const uint8_t* USBD_DFU_StringDesc[];
extern uint8_t MAL_Buffer[];

/* State Machine variables */
uint8_t DeviceState = STATE_dfuIDLE;
uint8_t DeviceStatus[6] =
{
    STATUS_OK,     /* bStatus: device status is OK */
    0x00,          /* bwPollTimeout: 0ms */
    0x00,
    0x00,
    STATE_dfuIDLE, /* bState: device state is dfuIDLE */
    0x00           /* iString: index is 0 */
};

uint32_t Manifest_State = Manifest_complete;

/* Data Management variables */
static uint16_t BlockNum = 0;
static uint16_t Length = 0;
static uint32_t BaseAddress = APP_LOADED_ADDR;  /*!< User application base address to erase, program or read */
static __IO uint32_t  USBD_DFU_AltSet = 0;

/* DFU interface class callbacks structure */
USBD_Class_cb_TypeDef  USBD_DFU_cb = 
{
    USBD_DFU_Init,
    USBD_DFU_DeInit,
    USBD_DFU_GetClassDescriptor,
    USBD_DFU_ClassReqHandle,
    USBD_DFU_GetInterface,
    USBD_DFU_SetInterface,
    NULL,
    NULL,
    USBD_DFU_EP0_TxSent,
    USBD_DFU_EP0_RxReady,
    NULL, /* DataIn, */
    NULL, /* DataOut, */
    NULL, /*SOF */
    NULL,
    NULL,
    USBD_DFU_GetCfgDesc,
#ifdef USB_OTG_HS_CORE  
    USBD_DFU_GetOtherCfgDesc, /* use same cobfig as per FS */
#endif  
    USBD_DFU_GetUsrStringDesc,
};

/* USB DFU device Configuration Descriptor */
uint8_t USBD_DFU_CfgDesc[USB_DFU_CONFIG_DESC_SIZE] =
{
    0x09,         /* bLength: configuration descriptor size */
    USB_DESCTYPE_CONFIGURATION, /* bDescriptorType: configuration descriptor type */
    USB_DFU_CONFIG_DESC_SIZE,   /* wTotalLength: configuration descriptor set total length */
    0x00,
    0x01,         /* bNumInterfaces: 1 interface */
    0x01,         /* bConfigurationValue: configuration value */
    0x02,         /* iConfiguration: index of string descriptor describing the configuration */
    0x80,         /* bmAttributes: bus powered and not supports remote wakeup */
    0x32,         /* MaxPower 100 mA: this current is used for detecting Vbus */

    /**********  Descriptor of DFU interface 0 Alternate setting 0 **************/  
    USBD_DFU_IF_DESC(0), /* This interface is mandatory for all devices */

#if (USBD_ITF_MAX_NUM > 1)
    /**********  Descriptor of DFU interface 0 Alternate setting 1 **************/ 
    USBD_DFU_IF_DESC(1),
#endif

#if (USBD_ITF_MAX_NUM > 2)
    /**********  Descriptor of DFU interface 0 Alternate setting 2 **************/ 
    USBD_DFU_IF_DESC(2),
#endif

#if (USBD_ITF_MAX_NUM > 3)
    /**********  Descriptor of DFU interface 0 Alternate setting 3 **************/ 
    USBD_DFU_IF_DESC(3),
#endif

#if (USBD_ITF_MAX_NUM > 4)
    /**********  Descriptor of DFU interface 0 Alternate setting 4 **************/ 
    USBD_DFU_IF_DESC(4),
#endif

#if (USBD_ITF_MAX_NUM > 5)
    /**********  Descriptor of DFU interface 0 Alternate setting 5 **************/ 
    USBD_DFU_IF_DESC(5),
#endif

#if (USBD_ITF_MAX_NUM > 6)
    #error "ERROR: usbd_dfu_core.c: Modify the file to support more descriptors!"
#endif

    /******************** DFU functional descriptor********************/
    0x09,   /* blength: 9 bytes */
    DFU_DESC_TYPE,   /* bDescriptorType: DFU functional descriptor type */
    0x0B,   /* bmAttributes:
               bitCanDnload             = 1      (bit 0)
               bitCanUpload             = 1      (bit 1)
               bitManifestationTolerant = 0      (bit 2)
               bitWillDetach            = 1      (bit 3)
               Reserved                          (bit4-6)
               bitAcceleratedST         = 0      (bit 7) */
    0xFF,   /* wDetachTimeOut: 255 ms */
    0x00,
    TRANSFER_SIZE_BYTES(TRANSFER_SIZE),  /* TransferSize = 1024 Byte*/
    0x1A,                                /* bcdDFUVersion*/
    0x01
};

#ifdef USE_USB_OTG_HS
__ALIGN_BEGIN uint8_t usbd_dfu_OtherCfgDesc[USB_DFU_CONFIG_DESC_SIZ] __ALIGN_END =
{
    0x09, /* bLength: Configuation Descriptor size */
    USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION, /* bDescriptorType: Configuration */
    USB_DFU_CONFIG_DESC_SIZ,
    /* wTotalLength: Bytes returned */
    0x00,
    0x01,         /*bNumInterfaces: 1 interface*/
    0x01,         /*bConfigurationValue: Configuration value*/
    0x02,         /*iConfiguration: Index of string descriptor describing the configuration*/
    0xC0,         /*bmAttributes: bus powered and Supprts Remote Wakeup */
    0x32,         /*MaxPower 100 mA: this current is used for detecting Vbus*/

    /**********  Descriptor of DFU interface 0 Alternate setting 0 **************/  
    USBD_DFU_IF_DESC(0), /* This interface is mandatory for all devices */

#if (USBD_ITF_MAX_NUM > 1)
    /**********  Descriptor of DFU interface 0 Alternate setting 1 **************/ 
    USBD_DFU_IF_DESC(1),
#endif /* (USBD_ITF_MAX_NUM > 1) */

#if (USBD_ITF_MAX_NUM > 2)
    /**********  Descriptor of DFU interface 0 Alternate setting 2 **************/ 
    USBD_DFU_IF_DESC(2),
#endif /* (USBD_ITF_MAX_NUM > 2) */

#if (USBD_ITF_MAX_NUM > 3)
    /**********  Descriptor of DFU interface 0 Alternate setting 3 **************/ 
    USBD_DFU_IF_DESC(3),
#endif /* (USBD_ITF_MAX_NUM > 3) */

#if (USBD_ITF_MAX_NUM > 4)
    /**********  Descriptor of DFU interface 0 Alternate setting 4 **************/ 
    USBD_DFU_IF_DESC(4),
#endif /* (USBD_ITF_MAX_NUM > 4) */

#if (USBD_ITF_MAX_NUM > 5)
    /**********  Descriptor of DFU interface 0 Alternate setting 5 **************/ 
    USBD_DFU_IF_DESC(5),
#endif /* (USBD_ITF_MAX_NUM > 5) */

#if (USBD_ITF_MAX_NUM > 6)
    #error "ERROR: usbd_dfu_core.c: Modify the file to support more descriptors!"
#endif /* (USBD_ITF_MAX_NUM > 6) */

    /******************** DFU Functional Descriptor********************/
    0x09,   /*blength = 9 Bytes*/
    DFU_DESCRIPTOR_TYPE,   /* DFU Functional Descriptor*/
    0x0B,   /*bmAttribute
              bitCanDnload             = 1      (bit 0)
              bitCanUpload             = 1      (bit 1)
              bitManifestationTolerant = 0      (bit 2)
              bitWillDetach            = 1      (bit 3)
              Reserved                          (bit4-6)
              bitAcceleratedST         = 0      (bit 7)*/
    0xFF,   /*DetachTimeOut= 255 ms*/
    0x00,
    /*WARNING: In DMA mode the multiple MPS packets feature is still not supported
      In this case, when using DMA XFERSIZE should be set to 64 in usbd_conf.h */
    TRANSFER_SIZE_BYTES(XFERSIZE),       /* TransferSize = 1024 Byte*/         
    0x1A,                                /* bcdDFUVersion*/
    0x01
};
#endif /* USE_USB_OTG_HS */

/**
  * @}
  */

/** @defgroup USBD_DFU_Core_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the DFU interface.
  * @param  pudev: pointer to usb device instance
  * @param  ConfigIndex: configuration index
  * @retval usb device operation status
  */
static uint8_t  USBD_DFU_Init (void *pudev, uint8_t ConfigIndex)
{
    /* Initilialize the MAL(Media Access Layer) */
    DFU_MAL_Init();

    return USBD_OK;
}

/**
  * @brief  De-Initializes the DFU layer.
  * @param  pudev: pointer to device instance
  * @param  ConfigIndex: configuration index
  * @retval usb device operation status
  */
static uint8_t  USBD_DFU_DeInit (void *pudev, uint8_t ConfigIndex)
{
    /* Restore default state */
    DeviceState = STATE_dfuIDLE;
    DeviceStatus[0] = STATUS_OK;
    DeviceStatus[4] = DeviceState;
    BlockNum = 0;
    Length = 0;

    /* DeInitilialize the MAL(Media Access Layer) */
    DFU_MAL_DeInit();

    return USBD_OK;
}

/**
  * @brief  Get DFU class descriptor
  * @param  pudev: pointer to usb device instance
  * @param  req: device request
  * @retval usb device operation status
  */
static uint8_t  USBD_DFU_GetClassDescriptor (void *pudev, USB_DEVICE_REQ *req)
{
    uint16_t len = 0;
    uint8_t *pbuf = NULL;

    if((req->wValue >> 8) == DFU_DESC_TYPE)
    {
        pbuf = (uint8_t*)USBD_DFU_CfgDesc + 9 + (9 * USBD_ITF_MAX_NUM);
        len = MIN(USB_DFU_DESC_SIZE, req->wLength);
    }

    USBD_CtlTx (pudev, pbuf, len);

    return USBD_OK;
}

/**
  * @brief  Handle the DFU class request
  * @param  pudev: pointer to usb device instance
  * @param  req: device class request
  * @retval usb device operation status
  */
static uint8_t  USBD_DFU_ClassReqHandle (void *pudev, USB_DEVICE_REQ *req)
{
    switch (req->bRequest)
    {
        case DFU_DNLOAD:
            DFU_Req_DNLOAD(pudev, req);
            break;

        case DFU_UPLOAD:
            DFU_Req_UPLOAD(pudev, req);
            break;

        case DFU_GETSTATUS:
            DFU_Req_GETSTATUS(pudev);
            break;

        case DFU_CLRSTATUS:
            DFU_Req_CLRSTATUS(pudev);
            break;

        case DFU_GETSTATE:
            DFU_Req_GETSTATE(pudev);
            break;

        case DFU_ABORT:
            DFU_Req_ABORT(pudev);
            break;

        case DFU_DETACH:
            DFU_Req_DETACH(pudev, req);
            break;

        default:
            USBD_CtlError(pudev, req);
            return USBD_FAIL;
    }

    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Get Interface
  * @param  pudev: pointer to usb device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_DFU_GetInterface (void *pudev, USB_DEVICE_REQ *req)
{
    USBD_CtlTx (pudev, (uint8_t *)&USBD_DFU_AltSet, 1);

    return USBD_OK;
}

/**
  * @brief  Handle standard device request--Set Interface
  * @param  pudev: pointer to usb device instance
  * @param  req: standard device request
  * @retval usb device operation status
  */
static uint8_t  USBD_DFU_SetInterface (void *pudev, USB_DEVICE_REQ *req)
{
    if ((uint8_t)(req->wValue) < USBD_ITF_MAX_NUM)
    {
        USBD_DFU_AltSet = (uint8_t)(req->wValue);
    }
    else
    {
        USBD_CtlError (pudev, req);
    }

    return USBD_OK;
}

/**
  * @brief  Handles the DFU control endpoint data IN stage.
  * @param  pudev: device instance
  * @retval status
  */
static uint8_t  USBD_DFU_EP0_TxSent (void *pudev)
{
    uint32_t Addr;
    USB_DEVICE_REQ req;

    if (DeviceState == STATE_dfuDNBUSY)
    {
        /* Decode the Special Command*/
        if (BlockNum == 0)
        {
            if ((MAL_Buffer[0] == GET_COMMANDS) && (Length == 1))
            {}
            else if ((MAL_Buffer[0] == SET_ADDRESS_POINTER) && (Length == 5))
            {
                BaseAddress  = MAL_Buffer[1];
                BaseAddress += MAL_Buffer[2] << 8;
                BaseAddress += MAL_Buffer[3] << 16;
                BaseAddress += MAL_Buffer[4] << 24;
            }
            else if ((MAL_Buffer[0] == ERASE) && (Length == 5))
            {
                BaseAddress  = MAL_Buffer[1];
                BaseAddress += MAL_Buffer[2] << 8;
                BaseAddress += MAL_Buffer[3] << 16;
                BaseAddress += MAL_Buffer[4] << 24;
                DFU_MAL_Erase(BaseAddress);
            }
            else
            {
                /* Call the error management function (command will be NACKed) */
                req.bmRequestType = 0;
                req.wLength = 1;
                USBD_CtlError (pudev, &req);
            }
        }
        /* Regular Download Command */
        else if (BlockNum > 1)
        {
            /* Decode the required address */
            Addr = ((BlockNum - 2) * TRANSFER_SIZE) + BaseAddress;

            /* Preform the write operation */
            DFU_MAL_Write(Addr, Length);
            BlockNum = 0;
        }

        Length = 0;

        /* Update the state machine */
        DeviceState =  STATE_dfuDNLOAD_SYNC;
        DeviceStatus[4] = DeviceState;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;

        return USBD_OK;
    }
    else if (DeviceState == STATE_dfuMANIFEST)/* Manifestation in progress*/
    {
        /* Start leaving DFU mode */
        DFU_LeaveDFUMode(pudev);
    }

    return USBD_OK;
}

/**
  * @brief  Handle data OUT stage in control endpoint 0
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t  USBD_DFU_EP0_RxReady (void  *pudev)
{
    return USBD_OK;
}

/**
  * @brief  Handle the DFU_DETACH request
  * @param  pudev: pointer to usb device instance
  * @param  req: DFU class request
  * @retval None.
  */
static void DFU_Req_DETACH(void *pudev, USB_DEVICE_REQ *req)
{
    switch(DeviceState)
    {
        case STATE_dfuIDLE:
        case STATE_dfuDNLOAD_SYNC:
        case STATE_dfuDNLOAD_IDLE:
        case STATE_dfuMANIFEST_SYNC:
        case STATE_dfuUPLOAD_IDLE:
            /* Update the state machine */
            DeviceState = STATE_dfuIDLE;
            DeviceStatus[0] = STATUS_OK;
            DeviceStatus[1] = 0;
            DeviceStatus[2] = 0;
            DeviceStatus[3] = 0; /* bwPollTimeout = 0ms */
            DeviceStatus[4] = DeviceState;
            DeviceStatus[5] = 0; /* iString */
            BlockNum = 0;
            Length = 0;
            break;

        default:
            break;
    }

    /* Check the detach capability in the DFU functional descriptor */
    if ((USBD_DFU_CfgDesc[12 + (9 * USBD_ITF_MAX_NUM)]) & DFU_DETACH_MASK)
    {
        /* Perform an Attach-Detach operation on USB bus */
        DCD_DevDisconnect (pudev);
        DCD_DevConnect (pudev);
    }
    else
    {
        /* Wait for the period of time specified in detach request */
        USB_OTG_HWP_mDelay (req->wValue);
    }
}

/**
  * @brief  Handle the DFU_DNLOAD request
  * @param  pudev: pointer to usb device instance
  * @param  req: DFU class request
  * @retval None
  */
static void DFU_Req_DNLOAD(void *pudev, USB_DEVICE_REQ *req)
{
    /* Data setup request */
    if (req->wLength > 0)
    {
        if ((DeviceState == STATE_dfuIDLE) || (DeviceState == STATE_dfuDNLOAD_IDLE))
        {
            /* Update the global length and block number */
            BlockNum = req->wValue;
            Length = req->wLength;

            /* Update the state machine */
            DeviceState = STATE_dfuDNLOAD_SYNC;
            DeviceStatus[4] = DeviceState;

            /* Prepare the reception of the buffer over EP0 */
            USBD_CtlRx (pudev,
                        (uint8_t*)MAL_Buffer,
                        Length);
        }
        /* Unsupported state */
        else
        {
            /* Call the error management function (command will be nacked */
            USBD_CtlError (pudev, req);
        }
    }
    /* 0 Data DNLOAD request */
    else
    {
        /* End of DNLOAD operation*/
        if (DeviceState == STATE_dfuDNLOAD_IDLE || DeviceState == STATE_dfuIDLE)
        {
            Manifest_State = Manifest_In_Progress;
            DeviceState = STATE_dfuMANIFEST_SYNC;
            DeviceStatus[1] = 0;
            DeviceStatus[2] = 0;
            DeviceStatus[3] = 0;
            DeviceStatus[4] = DeviceState;
        }
        else
        {
            USBD_CtlError (pudev, req);
        }
    }
}

/**
  * @brief  Handles the DFU UPLOAD request.
  * @param  pudev: pointer to usb device instance
  * @param  req: DFU class request
  * @retval None
  */
static void DFU_Req_UPLOAD(void *pudev, USB_DEVICE_REQ *req)
{
    uint8_t *Phy_Addr = NULL;
    uint32_t Addr = 0;

    /* Data setup request */
    if (req->wLength > 0)
    {
        if ((DeviceState == STATE_dfuIDLE) || (DeviceState == STATE_dfuUPLOAD_IDLE))
        {
            /* Update the global langth and block number */
            BlockNum = req->wValue;
            Length = req->wLength;

            /* Set device poll timeout */
            DeviceStatus[1] = 0;
            DeviceStatus[2] = 0;
            DeviceStatus[3] = 0;

            /* DFU Get Command */
            if (BlockNum == 0)
            {
                /* Update the state machine */
                DeviceState = (Length > 3)? STATE_dfuIDLE : STATE_dfuUPLOAD_IDLE;
                DeviceStatus[4] = DeviceState;

                /* Store the values of all supported commands */
                MAL_Buffer[0] = GET_COMMANDS;
                MAL_Buffer[1] = SET_ADDRESS_POINTER;
                MAL_Buffer[2] = ERASE;

                /* Send the status data over EP0 */
                USBD_CtlTx (pudev,
                            (uint8_t *)(&(MAL_Buffer[0])),
                            3);
            }
            else if (BlockNum > 1)
            {
                DeviceState = STATE_dfuUPLOAD_IDLE ;
                DeviceStatus[4] = DeviceState;
                /* Change is Accelerated */
                Addr = ((BlockNum - 2) * TRANSFER_SIZE) + BaseAddress;

                /* Return the physical address where data are stored */
                Phy_Addr = DFU_MAL_Read(Addr, Length);

                /* Send the status data over EP0 */
                USBD_CtlTx (pudev, Phy_Addr, Length);
            }
            else  /* unsupported wBlockNum */
            {
                DeviceState = STATUS_errSTALLEDPKT;
                DeviceStatus[4] = DeviceState;

                USBD_CtlError (pudev, req); 
            }
        }
        else
        {
            Length = 0;
            BlockNum = 0;

            USBD_CtlError (pudev, req);
        }
    }
    else
    {
        DeviceState = STATE_dfuIDLE;
        DeviceStatus[1] = 0;
        DeviceStatus[2] = 0;
        DeviceStatus[3] = 0;
        DeviceStatus[4] = DeviceState;
    }
}

/**
  * @brief  Handle the DFU_GETSTATUS request
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
static void DFU_Req_GETSTATUS(void *pudev)
{
    switch (DeviceState)
    {
        case STATE_dfuDNLOAD_SYNC:
            if (Length != 0)
            {
                DeviceState = STATE_dfuDNBUSY;
                DeviceStatus[4] = DeviceState;

                if (BlockNum == 0)
                {
                    if(MAL_Buffer[0] == ERASE)
                    {
                        DFU_MAL_GetStatus(BaseAddress, CMD_ERASE, DeviceStatus);
                    }
                    else
                    {
                        DFU_MAL_GetStatus(BaseAddress, CMD_WRITE, DeviceStatus);
                    }
                }
            }
            else  /* (wlength==0)*/
            {
                DeviceState = STATE_dfuDNLOAD_IDLE;
                DeviceStatus[4] = DeviceState;
                DeviceStatus[1] = 0;
                DeviceStatus[2] = 0;
                DeviceStatus[3] = 0;
            }
            break;

        case STATE_dfuMANIFEST_SYNC:
            if (Manifest_State == Manifest_In_Progress)
            {
                DeviceState = STATE_dfuMANIFEST;
                DeviceStatus[4] = DeviceState;
                DeviceStatus[1] = 1;
                DeviceStatus[2] = 0;
                DeviceStatus[3] = 0; /* bwPollTimeout = 1ms */
            }
            else if ((Manifest_State == Manifest_complete) && \
                        ((USBD_DFU_CfgDesc[(11 + (9 * USBD_ITF_MAX_NUM))]) & 0x04))
            {
                DeviceState = STATE_dfuIDLE;
                DeviceStatus[4] = DeviceState;
                DeviceStatus[1] = 0;
                DeviceStatus[2] = 0;
                DeviceStatus[3] = 0;
            }
            break;

        default:
            break;
    }

    /* Send the status data of DFU interface to host over EP0 */
    USBD_CtlTx (pudev,
                (uint8_t *)(&(DeviceStatus[0])),
                6);
}

/**
  * @brief  Handle the DFU_CLRSTATUS request
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
static void DFU_Req_CLRSTATUS(void *pudev)
{
    if (DeviceState == STATE_dfuERROR)
    {
        DeviceState = STATE_dfuIDLE;
        DeviceStatus[0] = STATUS_OK;
    }
    else
    {
        /*State Error*/
        DeviceState = STATE_dfuERROR;
        DeviceStatus[0] = STATUS_errUNKNOWN;
    }

    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0; /* bwPollTimeout = 0ms */
    DeviceStatus[4] = DeviceState;
    DeviceStatus[5] = 0; /* iString: index = 0 */
}

/**
  * @brief  Handle the DFU_GETSTATE request
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
static void DFU_Req_GETSTATE(void *pudev)
{
    /* Send the current state of the DFU interface to host */
    USBD_CtlTx (pudev, &DeviceState, 1);
}

/**
  * @brief  Handle the DFU_ABORT request
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
static void DFU_Req_ABORT(void *pudev)
{
    switch(DeviceState)
    {
        case STATE_dfuIDLE:
        case STATE_dfuDNLOAD_SYNC:
        case STATE_dfuDNLOAD_IDLE:
        case STATE_dfuMANIFEST_SYNC:
        case STATE_dfuUPLOAD_IDLE:
            DeviceState = STATE_dfuIDLE;
            DeviceStatus[0] = STATUS_OK;
            DeviceStatus[1] = 0;
            DeviceStatus[2] = 0;
            DeviceStatus[3] = 0; /* bwPollTimeout = 0ms */
            DeviceStatus[4] = DeviceState;
            DeviceStatus[5] = 0; /* iString: index = 0 */
            BlockNum = 0;
            Length = 0;
            break;

        default:
            break;
    }
}

/**
  * @brief  Leave DFU mode and reset device to jump to user loaded code
  * @param  pudev: pointer to usb device instance
  * @retval None
  */
void DFU_LeaveDFUMode(void *pudev)
{
    Manifest_State = Manifest_complete;

    DeviceStatus[1] = 0;
    DeviceStatus[2] = 0;
    DeviceStatus[3] = 0;

    if ((USBD_DFU_CfgDesc[(11 + (9 * USBD_ITF_MAX_NUM))]) & 0x04)
    {
        DeviceState = STATE_dfuMANIFEST_SYNC;
        DeviceStatus[4] = DeviceState;

        return;
    }
    else
    {
        DeviceState = STATE_dfuMANIFEST_WAIT_RESET;
        DeviceStatus[4] = DeviceState;

        /* Disconnect the USB device */
        DCD_DevDisconnect (pudev);

        /* DeInitilialize the MAL(Media Access Layer) */
        DFU_MAL_DeInit();

        /* Generate system reset to allow jumping to the user code */
        NVIC_SystemReset();
       
        /* This instruction will not be reached (system reset) */
        return;
    }
}

/**
  * @brief  Get configuration descriptor
  * @param  Speed: current device speed
  * @param  length: pointer to data length
  * @retval Pointer to descriptor buffer
  */
static uint8_t  *USBD_DFU_GetCfgDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen = sizeof (USBD_DFU_CfgDesc);

    return USBD_DFU_CfgDesc;
}

#ifdef USB_OTG_HS_CORE
/**
  * @brief  Returns other speed configuration descriptor.
  * @param  Speed: current device speed
  * @param  pLen: pointer data length
  * @retval pointer to descriptor buffer
  */
static uint8_t  *USBD_DFU_GetOtherCfgDesc (uint8_t Speed, uint16_t *pLen)
{
    *pLen = sizeof (usbd_dfu_OtherCfgDesc);

    return usbd_dfu_OtherCfgDesc;
}
#endif

/**
  * @brief  Manages the transfer of memory interfaces string descriptors.
  * @param  Speed: current device speed
  * @param  Index: desciptor index
  * @param  pLen: pointer data length
  * @retval pointer to the descriptor table or NULL if the descriptor is not supported.
  */
static uint8_t* USBD_DFU_GetUsrStringDesc (uint8_t Speed, uint8_t Index, uint16_t *pLen)
{
    uint8_t StrIndex = Index - USBD_INTERFACE_STR_IDX - 1;

    /* Check if the requested interface string descriptor index is supported */
    if (Index <= (USBD_INTERFACE_STR_IDX + USBD_ITF_MAX_NUM))
    {
        USBD_GetUnicodeString ((uint8_t *)USBD_DFU_StringDesc[StrIndex], USBD_StrDesc, pLen);

        return USBD_StrDesc;
    }
    else
    {
        return NULL;
    }
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
