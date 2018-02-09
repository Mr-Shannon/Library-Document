/**
  ******************************************************************************
  * @file    usbh_msc_core.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file implements the MSC class driver functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_msc_core.h"
#include "usbh_msc_scsi.h"
#include "usbh_msc_bot.h"
#include "usbh_core.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Class_Library
  * @{
  */

/** @addtogroup USBH_MSC_CLASS
  * @{
  */

/** @defgroup USBH_MSC_CORE 
  * @{
  */

/** @defgroup USBH_MSC_CORE_Private_Defines
  * @{
  */
#define USBH_MSC_ERROR_RETRY_LIMIT 10

/**
  * @}
  */

/** @defgroup USBH_MSC_CORE_Private_Variables
  * @{
  */
uint8_t MSCErrorCount = 0;

MSC_Machine_TypeDef MSC_Machine;
USB_Setup_TypeDef MSC_Setup;

/**
  * @}
  */ 


/** @defgroup USBH_MSC_CORE_Private_FunctionPrototypes
  * @{
  */ 
static void USBH_MSC_InterfaceDeInit        (USB_OTG_CORE_HANDLE *pudev, void *puhost);
static USBH_STATUS USBH_MSC_InterfaceInit   (USB_OTG_CORE_HANDLE *pudev, void *puhost);
static USBH_STATUS USBH_MSC_Handle          (USB_OTG_CORE_HANDLE *pudev, void *puhost);
static USBH_STATUS USBH_MSC_ClassRequest    (USB_OTG_CORE_HANDLE *pudev, void *puhost);
static USBH_STATUS USBH_MSC_BOTReset        (USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost);
static USBH_STATUS USBH_MSC_GETMaxLUN       (USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost);

USBH_Class_cb_TypeDef  USBH_MSC_cb = 
{
    USBH_MSC_InterfaceInit,
    USBH_MSC_InterfaceDeInit,
    USBH_MSC_ClassRequest,
    USBH_MSC_Handle,
};

void USBH_MSC_ErrorHandle(uint8_t status);

/**
  * @}
  */

/** @defgroup USBH_MSC_CORE_Private_Functions
  * @{
  */

/**
  * @brief  Interface initialization for MSC class.
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @retval status of class request handled.
  */
static USBH_STATUS USBH_MSC_InterfaceInit (USB_OTG_CORE_HANDLE *pudev, void *puhost)
{
    USBH_HOST *pphost = puhost;

    if((pphost->DeviceProp.ItfDesc[0].bInterfaceClass == MSC_CLASS) && \
            (pphost->DeviceProp.ItfDesc[0].bInterfaceProtocol == MSC_PROTOCOL))
    {
        if(pphost->DeviceProp.EpDesc[0][0].bEndpointAddress & 0x80)
        {
            MSC_Machine.MSBulkInEp = (pphost->DeviceProp.EpDesc[0][0].bEndpointAddress);
            MSC_Machine.MSBulkInEpSize  = pphost->DeviceProp.EpDesc[0][0].wMaxPacketSize;
        }
        else
        {
            MSC_Machine.MSBulkOutEp = (pphost->DeviceProp.EpDesc[0][0].bEndpointAddress);
            MSC_Machine.MSBulkOutEpSize  = pphost->DeviceProp.EpDesc[0] [0].wMaxPacketSize;
        }

        if(pphost->DeviceProp.EpDesc[0][1].bEndpointAddress & 0x80)
        {
            MSC_Machine.MSBulkInEp = (pphost->DeviceProp.EpDesc[0][1].bEndpointAddress);
            MSC_Machine.MSBulkInEpSize  = pphost->DeviceProp.EpDesc[0][1].wMaxPacketSize;
        }
        else
        {
            MSC_Machine.MSBulkOutEp = (pphost->DeviceProp.EpDesc[0][1].bEndpointAddress);
            MSC_Machine.MSBulkOutEpSize  = pphost->DeviceProp.EpDesc[0][1].wMaxPacketSize;
        }

        MSC_Machine.hc_num_out = USBH_Alloc_Channel(pudev, 
                                                    MSC_Machine.MSBulkOutEp);

        MSC_Machine.hc_num_in = USBH_Alloc_Channel(pudev,
                                                   MSC_Machine.MSBulkInEp);  

        /* Open the new channels */
        USBH_Open_Channel (pudev,
                           MSC_Machine.hc_num_out,
                           pphost->DeviceProp.Address,
                           pphost->DeviceProp.Speed,
                           USB_EPTYPE_BULK,
                           MSC_Machine.MSBulkOutEpSize);  

        USBH_Open_Channel (pudev,
                           MSC_Machine.hc_num_in,
                           pphost->DeviceProp.Address,
                           pphost->DeviceProp.Speed,
                           USB_EPTYPE_BULK,
                           MSC_Machine.MSBulkInEpSize);
    }
    else
    {
        pphost->usr_cb->DeviceNotSupported(); 
    }

    return USBH_OK;
}


/**
  * @brief  De-Initialize interface by freeing host channels allocated to interface
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @retval None
  */
void USBH_MSC_InterfaceDeInit (USB_OTG_CORE_HANDLE *pudev, void *puhost)
{
    if (MSC_Machine.hc_num_out)
    {
        USB_OTG_HC_Halt(pudev, MSC_Machine.hc_num_out);
        USBH_Free_Channel (pudev, MSC_Machine.hc_num_out);
        MSC_Machine.hc_num_out = 0;
    }

    if (MSC_Machine.hc_num_in)
    {
        USB_OTG_HC_Halt(pudev, MSC_Machine.hc_num_in);
        USBH_Free_Channel (pudev, MSC_Machine.hc_num_in);
        MSC_Machine.hc_num_in = 0;
    }
}

/**
  * @brief  Initialize the MSC state machine
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @retval status of class request handled.
  */
static USBH_STATUS USBH_MSC_ClassRequest(USB_OTG_CORE_HANDLE *pudev, void *puhost)
{
    USBH_STATUS status = USBH_OK;
    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOT_INIT_STATE;

    return status; 
}

/**
  * @brief  MSC state machine handler 
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @retval host operation status
  */
static USBH_STATUS USBH_MSC_Handle (USB_OTG_CORE_HANDLE *pudev, void *puhost)
{
    USBH_HOST *pphost = puhost;
    USBH_STATUS status = USBH_BUSY;
    uint8_t mscStatus = USBH_MSC_BUSY;
    uint8_t appliStatus = 0;

    static uint8_t maxLunExceed = FALSE;

    if(HCD_IsDeviceConnected(pudev))
    {
        switch(USBH_MSC_BOTXferParam.MSCState)
        {
            case USBH_MSC_BOT_INIT_STATE:
                USBH_MSC_Init(pudev);
                USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOT_RESET;
                break;

            case USBH_MSC_BOT_RESET:
                /* Issue BOT RESET request */
                status = USBH_MSC_BOTReset(pudev, puhost);
                if(status == USBH_OK )
                {
                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_GET_MAX_LUN;
                }

                if(status == USBH_NOT_SUPPORTED )
                {
                    /* If the Command has failed, then we need to move to Next State, after
                       STALL condition is cleared by Control-Transfer */
                    USBH_MSC_BOTXferParam.MSCStateBkp = USBH_MSC_GET_MAX_LUN; 

                    /* a Clear Feature should be issued here */
                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_CTRL_ERROR_STATE;
                }
                break;

            case USBH_MSC_GET_MAX_LUN:
                /* Issue GetMaxLUN request */
                status = USBH_MSC_GETMaxLUN(pudev, puhost);

                if(status == USBH_OK)
                {
                    MSC_Machine.maxLun = *(MSC_Machine.buff);

                    /* If device has more that one logical unit then it is not supported */
                    if((MSC_Machine.maxLun > 0) && (maxLunExceed == FALSE))
                    {
                        maxLunExceed = TRUE;
                        pphost->usr_cb->DeviceNotSupported();

                        break;
                    }

                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_TEST_UNIT_READY;
                }

                if(status == USBH_NOT_SUPPORTED )
                {
                    /* If the Command has failed, then we need to move to Next State, after
                       STALL condition is cleared by Control-Transfer */
                    USBH_MSC_BOTXferParam.MSCStateBkp = USBH_MSC_TEST_UNIT_READY; 

                    /* A Clear Feature should be issued here */
                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_CTRL_ERROR_STATE;
                }
                break;

            case USBH_MSC_CTRL_ERROR_STATE:

                /* Issue Clearfeature request */
                status = USBH_ClrFeature(pudev,
                                         puhost,
                                         0x00,
                                         pphost->Control.HcOutNum);

                if(status == USBH_OK)
                {
                    /* If GetMaxLun Request not support, assume Single LUN configuration */
                    MSC_Machine.maxLun = 0;  

                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOTXferParam.MSCStateBkp;     
                }
                break;

            case USBH_MSC_TEST_UNIT_READY:

                /* Issue SCSI command TestUnitReady */ 
                mscStatus = USBH_MSC_TestUnitReady(pudev);

                if(mscStatus == USBH_MSC_OK )
                {
                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_READ_CAPACITY10;
                    MSCErrorCount = 0;
                    status = USBH_OK;
                }
                else
                {
                    USBH_MSC_ErrorHandle(mscStatus);
                }
                break;

            case USBH_MSC_READ_CAPACITY10:

                /* Issue READ_CAPACITY10 SCSI command */
                mscStatus = USBH_MSC_ReadCapacity10(pudev);
                if(mscStatus == USBH_MSC_OK )
                {
                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_MODE_SENSE6;
                    MSCErrorCount = 0;
                    status = USBH_OK;
                }
                else
                {
                    USBH_MSC_ErrorHandle(mscStatus);
                }
                break;

            case USBH_MSC_MODE_SENSE6:

                /* Issue ModeSense6 SCSI command for detecting if device is write-protected */
                mscStatus = USBH_MSC_ModeSense6(pudev);

                if(mscStatus == USBH_MSC_OK )
                {
                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_DEFAULT_APPLI_STATE;
                    MSCErrorCount = 0;
                    status = USBH_OK;
                }
                else
                {
                    USBH_MSC_ErrorHandle(mscStatus);
                }
                break;

            case USBH_MSC_REQUEST_SENSE:
                /* Issue RequestSense SCSI command for retreiving error code */
                mscStatus = USBH_MSC_RequestSense(pudev);
                if(mscStatus == USBH_MSC_OK )
                {
                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_BOTXferParam.MSCStateBkp;
                    status = USBH_OK;
                }
                else
                {
                    USBH_MSC_ErrorHandle(mscStatus);
                }
                break;

            case USBH_MSC_BOT_USB_TRANSFERS:
                /* Process the BOT state machine */
                USBH_MSC_HandleBOTXfer(pudev , puhost);
                break;

            case USBH_MSC_DEFAULT_APPLI_STATE:
                /* Process Application callback for MSC */
                appliStatus = pphost->usr_cb->UserApplication();
                if(appliStatus == 0)
                {
                    USBH_MSC_BOTXferParam.MSCState = USBH_MSC_DEFAULT_APPLI_STATE;
                }
                else if (appliStatus == 1) 
                {
                    /* De-init requested from application layer */
                    status = USBH_APPLY_DEINIT;
                }
                break;

            case USBH_MSC_UNRECOVERED_STATE:
                status = USBH_UNRECOVERED_ERROR;
                break;

            default:
                break; 
        }
    }

    return status;
}

/**
  * @brief  Reset the mass storage device and its associated interface.
  *         This class-specific request shall ready the 
  *         device for the next CBW from the host.
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @retval status of class request handled.
  */
static USBH_STATUS USBH_MSC_BOTReset(USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost)
{
    USB_Setup_TypeDef *pSetup = &(puhost->Control.Setup);

    pSetup->b.bmRequestType = USB_REQ_DIR_H2D | USB_CLASS_REQ | USB_REQTYPE_INTERFACE;
    pSetup->b.bRequest = USB_REQ_BOT_RESET;
    pSetup->b.wValue = 0;
    pSetup->b.wIndex = 0;
    pSetup->b.wLength = 0;

    return USBH_CtlReq(pudev, puhost, 0, 0);
}

/**
  * @brief  Get max lun of the mass storage device 
  * @param  pudev: pointer to selected usb otg device
  * @param  phost: pointer to usb host
  * @retval USB control transfer status
  */
static USBH_STATUS USBH_MSC_GETMaxLUN(USB_OTG_CORE_HANDLE *pudev , USBH_HOST *puhost)
{
    USB_Setup_TypeDef *pSetup = &(puhost->Control.Setup);

    pSetup->b.bmRequestType = USB_REQ_DIR_D2H | USB_CLASS_REQ | USB_REQTYPE_INTERFACE;
    pSetup->b.bRequest = USB_REQ_GET_MAX_LUN;
    pSetup->b.wValue = 0;
    pSetup->b.wIndex = 0;
    pSetup->b.wLength = 1;

    return USBH_CtlReq(pudev, puhost, MSC_Machine.buff, 1);
}

/**
  * @brief  Handling errors occuring during the MSC state machine
  * @param  Status: error status
  * @retval None
  */
void USBH_MSC_ErrorHandle(uint8_t Status)
{
    if(Status == USBH_MSC_FAIL)
    {
        MSCErrorCount++;
        if(MSCErrorCount < USBH_MSC_ERROR_RETRY_LIMIT)
        {
            /* Try MSC level error recovery, Issue the request Sense to get Drive error reason  */
            USBH_MSC_BOTXferParam.MSCState = USBH_MSC_REQUEST_SENSE;
            USBH_MSC_BOTXferParam.CmdStateMachine = CMD_SEND_STATE;
        }
        else
        {
            /* Error trials exceeded the limit, go to unrecovered state */
            USBH_MSC_BOTXferParam.MSCState = USBH_MSC_UNRECOVERED_STATE;
        }
    }
    else if(Status == USBH_MSC_PHASE_ERROR)
    {
        /* Phase error, Go to Unrecoovered state */
        USBH_MSC_BOTXferParam.MSCState = USBH_MSC_UNRECOVERED_STATE;
    }
    else if(Status == USBH_MSC_BUSY)
    {
        /* No change in state */
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

/**
  * @}
  */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
