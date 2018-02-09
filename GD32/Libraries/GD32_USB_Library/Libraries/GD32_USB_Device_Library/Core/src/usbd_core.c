/**
  ******************************************************************************
  * @file    usbd_core.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS device-mode core functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_core.h"
#include "usbd_enum.h"
#include "usbd_ctlx.h"
#include "usb_dcd_int.h"
#include "usb_hwp.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Core_Library
  * @{
  */

/** @defgroup USBD_CORE
  * @{
  */

/** @defgroup USBD_CORE_Private_Function_Prototypes
  * @{
  */

#ifdef VBUS_SENSING_ENABLED
static uint8_t USBD_DevConnected    (USB_OTG_CORE_HANDLE *pudev);
static uint8_t USBD_DevDisconnected (USB_OTG_CORE_HANDLE *pudev);
#endif

static uint8_t USBD_SetupStage      (USB_OTG_CORE_HANDLE *pudev);
static uint8_t USBD_DataOutStage    (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID);
static uint8_t USBD_DataInStage     (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID);
static uint8_t USBD_SOF             (USB_OTG_CORE_HANDLE *pudev);
static uint8_t USBD_Reset           (USB_OTG_CORE_HANDLE *pudev);
static uint8_t USBD_Suspend         (USB_OTG_CORE_HANDLE *pudev);
static uint8_t USBD_Resume          (USB_OTG_CORE_HANDLE *pudev);

static uint8_t USBD_IsoINIncomplete (USB_OTG_CORE_HANDLE *pudev);
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE *pudev);
static uint8_t USBD_RunTestMode     (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */

/** @defgroup USBD_CORE_Private_Variables
  * @{
  */

//__IO USB_OTG_DCTL_TypeDef SET_TEST_MODE;

USBD_DCD_INT_cb_TypeDef USBD_DCD_INT_cb = 
{
    USBD_DataOutStage,
    USBD_DataInStage,
    USBD_SetupStage,
    USBD_SOF,
    USBD_Reset,
    USBD_Suspend,
    USBD_Resume,
    USBD_IsoINIncomplete,
    USBD_IsoOUTIncomplete,

#ifdef VBUS_SENSING_ENABLED
    USBD_DevConnected,
    USBD_DevDisconnected,
#endif
};

USBD_DCD_INT_cb_TypeDef  *USBD_DCD_INT_fops = &USBD_DCD_INT_cb;

/**
  * @}
  */

/** @defgroup USBD_CORE_Private_Functions
  * @{
  */

/**
  * @brief  Initailizes the OTG_FS device-mode stack and load the class driver
  * @param  pudev: pointer to usb device instance
  * @param  CoreID: USB OTG core ID
  * @param  desc_cb: device descriptor callback structure pointer
  * @param  class_cb: class callback structure pointer
  * @param  usr_cb: user callback structure pointer
  * @retval None
  */
void USBD_Init(USB_OTG_CORE_HANDLE *pudev,
               USB_OTG_CORE_ID_TypeDef CoreID,
               USBD_Desc_cb_TypeDef *desc_cb,
               USBD_Class_cb_TypeDef *class_cb,
               USBD_User_cb_TypeDef *user_cb)
{
    /* Hardware Init */
    USB_OTG_HWP_Init(pudev);

    USBD_DeInit(pudev);

    /* Device descriptor, class and user callbacks */
    pudev->dev.class_cb = class_cb;
    pudev->dev.usr_cb = user_cb;
    pudev->dev.dev_desc = desc_cb;

    /* Set USB OTG_FS core registers and params */
    DCD_Init(pudev, CoreID);

    /* Callback user function */
    pudev->dev.usr_cb->Init();

    /* Enable OTG_FS Interrupts */
    USB_OTG_HWP_EnableInterrupt(pudev);
}

/**
  * @brief  De-Initialize USB OTG_FS device-mode stack
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
USBD_Status USBD_DeInit(USB_OTG_CORE_HANDLE *pudev)
{
    /* Software Init */

    return USBD_OK;
}

/**
  * @brief  USB setup stage processing
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_SetupStage(USB_OTG_CORE_HANDLE *pudev)
{
    USB_DEVICE_REQ req;

    USBD_ParseSetupRequest(pudev, &req);

    switch (req.bmRequestType & USB_REQ_MASK)
    {
        /* Standard device request */
        case USB_STANDARD_REQ:
            USBD_StdDevReq(pudev, &req);
            break;

        /* Device class request */
        case USB_CLASS_REQ:
            USBD_DevClsReq(pudev, &req);
            break;

        /* Vendor defined request */
        case USB_VENDOR_REQ:
            USBD_VdrDefReq(pudev, &req);
            break;

        default:
            DCD_EP_SetStall(pudev, req.bmRequestType & 0x80);
            break;
    }

    return USBD_OK;
}

/**
  * @brief  Data out stage processing
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval usb device operation status
  */
static uint8_t USBD_DataOutStage(USB_OTG_CORE_HANDLE *pudev , uint8_t EpID)
{
    USB_OTG_EP *ep;

    if(EpID == 0) 
    {
        ep = &pudev->dev.OutEp[0];

        if (pudev->dev.device_cur_state == USB_OTG_CTRL_DATA_OUT)
        {
            if(ep->rem_data_len > ep->MaxPacket)
//            if(ep->xfer_len > ep->MaxPacket)
            {
                ep->rem_data_len -= ep->MaxPacket;
//                ep->xfer_len -= ep->MaxPacket;

                USBD_CtlContinueRx (pudev,
                                    ep->xfer_buff,
                                    MIN( ep->rem_data_len  /*ep->xfer_len*/, ep->MaxPacket));
            }
            else
            {
                if((pudev->dev.class_cb->EP0_RxReady != NULL)&&
                    (pudev->dev.device_cur_status == USB_OTG_STATUS_CONFIGURED))
                {
                    pudev->dev.class_cb->EP0_RxReady(pudev);
                }
                USBD_CtlTransmitStatus(pudev);
            }
        }
    }
    else if((pudev->dev.class_cb->DataOut != NULL)&&
            (pudev->dev.device_cur_status == USB_OTG_STATUS_CONFIGURED))
    {
        pudev->dev.class_cb->DataOut(pudev, EpID); 
    }

    return USBD_OK;
}

/**
  * @brief  Data in stage processing
  * @param  pudev: pointer to usb device instance
  * @param  EpID: endpoint identifier
  * @retval usb device operation status
  */
static uint8_t USBD_DataInStage(USB_OTG_CORE_HANDLE *pudev, uint8_t EpID)
{
    USB_OTG_EP *ep;

    if(EpID == 0) 
    {
        ep = &pudev->dev.InEp[0];

        if (pudev->dev.device_cur_state == USB_OTG_CTRL_DATA_IN)
        {
            if(ep->rem_data_len > ep->MaxPacket)
//            if(ep->xfer_len > ep->MaxPacket)
            {
                ep->rem_data_len -= ep->MaxPacket;
//                ep->xfer_len -= ep->MaxPacket;

                USBD_CtlContinueTx (pudev, 
                                    ep->xfer_buff,
                                     ep->rem_data_len /* ep->xfer_len */);
            }
            else
            {
                /* last packet is MPS multiple, so send ZLP packet */
                if((ep->total_data_len % ep->MaxPacket == 0) &&
                    (ep->total_data_len >= ep->MaxPacket) &&
                        (ep->total_data_len < ep->ctl_data_len ))
//                if(ep->xfer_len == ep->MaxPacket)
                {
//                    ep->xfer_len = 0;
                    USBD_CtlContinueTx (pudev, NULL, 0);
                    ep->ctl_data_len = 0;
                }
                else
                {
                    if((pudev->dev.class_cb->EP0_TxSent != NULL)&&
                        (pudev->dev.device_cur_status == USB_OTG_STATUS_CONFIGURED))
                    {
                        pudev->dev.class_cb->EP0_TxSent(pudev);
                    }

                    USBD_CtlReceiveStatus(pudev);
                }
            }
        }

        if (pudev->dev.TestMode == 1)
        {
            USBD_RunTestMode(pudev);
            pudev->dev.TestMode = 0;
        }
    }
    else if((pudev->dev.class_cb->DataIn != NULL)&& 
            (pudev->dev.device_cur_status == USB_OTG_STATUS_CONFIGURED))
//    else
    {
//        ep->xfer_len -= ep->xfer_count;

//        if(ep->xfer_len == 0)
//        {
//            if((pudev->dev.class_cb->DataIn != NULL) && 
//                (pudev->dev.device_cur_state == USB_OTG_STATUS_CONFIGURED))
//            {
//                pudev->dev.class_cb->DataIn(pudev, EpID);
//            }
//        }
//        else
//        {
//            DCD_EP_Tx(pudev, EpID, ep->xfer_buff, ep->xfer_len);
//        }
        pudev->dev.class_cb->DataIn(pudev, EpID);
    }

    return USBD_OK;
}

/**
  * @brief  Launch test mode process
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t  USBD_RunTestMode (USB_OTG_CORE_HANDLE  *pudev) 
{
//  USB_OTG_WRITE_REG32(&pudev->regs.DREGS->DCTL, SET_TEST_MODE.d32);
//  _SetDCTLR(SET_TEST_MODE.d32);
    return USBD_OK;  
}

/**
  * @brief  USB Reset event processing
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_Reset(USB_OTG_CORE_HANDLE  *pudev)
{
    /* Open EP0 OUT */
    DCD_EP_Init(pudev,
                EP0_OUT,
                USB_EPTYPE_CTRL,
                USB_OTG_FS_MAX_EP0_SIZE);

    /* Open EP0 IN */
    DCD_EP_Init(pudev,
                EP0_IN,
                USB_EPTYPE_CTRL,
                USB_OTG_FS_MAX_EP0_SIZE);

    /* Upon Reset call usr call back */
    pudev->dev.device_cur_status = USB_OTG_STATUS_DEFAULT;
    pudev->dev.usr_cb->DeviceReset(pudev->cfg.Speed);

    return USBD_OK;
}

/**
  * @brief  USB Resume event processing
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_Resume(USB_OTG_CORE_HANDLE  *pudev)
{
    /* Upon Resume call usr call back */
    pudev->dev.usr_cb->DeviceResumed();
    pudev->dev.device_cur_status = pudev->dev.device_old_status;
    pudev->dev.device_cur_status = USB_OTG_STATUS_CONFIGURED;

    return USBD_OK;
}

/**
  * @brief  USB Suspend event processing
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_Suspend(USB_OTG_CORE_HANDLE *pudev)
{
    pudev->dev.device_old_status = pudev->dev.device_cur_status;
    pudev->dev.device_cur_status = USB_OTG_STATUS_SUSPENDED;

    /* Upon Resume call usr call back */
    pudev->dev.usr_cb->DeviceSuspended();

    return USBD_OK;
}

/**
  * @brief  SOF event processing
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_SOF(USB_OTG_CORE_HANDLE *pudev)
{
    if(pudev->dev.class_cb->SOF)
    {
        pudev->dev.class_cb->SOF(pudev); 
    }

    return USBD_OK;
}

/**
  * @brief  Device configuration and interface setting
  * @param  pudev: pointer to usb device instance
  * @param  CfgIdx: configuration index
  * @retval usb device operation status
  */
USBD_Status USBD_SetCfg(USB_OTG_CORE_HANDLE *pudev, uint8_t CfgIdx)
{
    pudev->dev.class_cb->Init(pudev, CfgIdx);

    /* Upon set config call usr call back */
    pudev->dev.usr_cb->DeviceConfigured();

    return USBD_OK; 
}

/**
  * @brief  Clear current configuration
  * @param  pudev: pointer to usb device instance
  * @param  CfgIdx: configuration index
  * @retval usb device operation status
  */
USBD_Status USBD_ClrCfg(USB_OTG_CORE_HANDLE  *pudev, uint8_t CfgIdx)
{
    pudev->dev.class_cb->DeInit(pudev, CfgIdx);

    return USBD_OK;
}

/**
  * @brief  Handle iso in incomplete event
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_IsoINIncomplete(USB_OTG_CORE_HANDLE *pudev)
{
    pudev->dev.class_cb->IsoINIncomplete(pudev);

    return USBD_OK;
}

/**
  * @brief  Handle iso out incomplete event
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_IsoOUTIncomplete(USB_OTG_CORE_HANDLE *pudev)
{
    pudev->dev.class_cb->IsoOUTIncomplete(pudev);

    return USBD_OK;
}

#ifdef VBUS_SENSING_ENABLED

/**
  * @brief  Handle device connection event
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_DevConnected(USB_OTG_CORE_HANDLE *pudev)
{
    pudev->dev.usr_cb->DeviceConnected();

    pudev->dev.connection_status = 1;

    return USBD_OK;
}

/**
  * @brief  Handle device disconnection event
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
static uint8_t USBD_DevDisconnected(USB_OTG_CORE_HANDLE *pudev)
{
    pudev->dev.usr_cb->DeviceDisconnected();
    pudev->dev.class_cb->DeInit(pudev, 0);
    pudev->dev.connection_status = 0;

    return USBD_OK;
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
