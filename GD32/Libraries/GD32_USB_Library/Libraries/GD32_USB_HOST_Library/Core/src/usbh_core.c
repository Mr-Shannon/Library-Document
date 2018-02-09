/**
  ******************************************************************************
  * @file    usbh_core.c 
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file implements the functions for the core state machine process
  *          the enumeration and the control transfer process
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_ctlx.h"
#include "usb_hwp.h"
#include "usbh_hcs.h"
#include "usbh_enum.h"
#include "usbh_core.h"
#include "usb_hcd_int.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_CORE
  * @{
  */ 

/** @defgroup USBH_CORE_Private_TypesDefinitions
  * @{
  */ 
uint8_t USBH_Disconnected (USB_OTG_CORE_HANDLE *pudev);
uint8_t USBH_Connected    (USB_OTG_CORE_HANDLE *pudev);
uint8_t USBH_SOF          (USB_OTG_CORE_HANDLE *pudev);

USBH_HCD_INT_cb_TypeDef USBH_HCD_INT_cb = 
{
    USBH_SOF,
    USBH_Connected,
    USBH_Disconnected,
};

USBH_HCD_INT_cb_TypeDef  *USBH_HCD_INT_fops = &USBH_HCD_INT_cb;

/**
  * @}
  */

/** @defgroup USBH_CORE_Private_FunctionPrototypes
  * @{
  */
static USBH_STATUS USBH_HandleEnum  (USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost);
USBH_STATUS USBH_HandleControl      (USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost);

/**
  * @}
  */

/** @defgroup USBH_CORE_Private_Functions
  * @{
  */

/**
  * @brief  USB Connect callback function from the Interrupt. 
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
uint8_t USBH_Connected (USB_OTG_CORE_HANDLE *pudev)
{
    pudev->host.ConnectStatus = 1;

    return 0;
}

/**
  * @brief  USB Disconnect callback function from the Interrupt. 
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
uint8_t USBH_Disconnected (USB_OTG_CORE_HANDLE *pudev)
{
    pudev->host.ConnectStatus = 0;

    return 0;
}

/**
  * @brief  USB SOF callback function from the Interrupt. 
  * @param  pudev: pointer to selected usb otg device
  * @retval operation tatus
  */
uint8_t USBH_SOF (USB_OTG_CORE_HANDLE *pudev)
{
    /* This callback could be used to implement a scheduler process */
    return 0;  
}

/**
  * @brief  Host hardware and stack initializations 
  * @param  pudev: pointer to selected usb otg device
  * @param  CoreID: OTG Full-Speed or High-Speed core
  * @param  puhost: pointer to usb host
  * @param  class_cb: Class callback structure address
  * @param  usr_cb: User callback structure address
  * @retval None
  */
void USBH_Init(USB_OTG_CORE_HANDLE *pudev,
               USB_OTG_CORE_ID_TypeDef CoreID,
               USBH_HOST *puhost,
               USBH_Class_cb_TypeDef *class_cb,
               USBH_Usr_cb_TypeDef *usr_cb)
{
    /* Hardware Init */
    USB_OTG_HWP_Init(pudev);

    /* configure GPIO pin used for switching VBUS power */
    USB_OTG_HWP_ConfigVBUS(0);

    /* Host de-initializations */
    USBH_DeInit(pudev, puhost);

    /*Register class and user callbacks */
    puhost->class_cb = class_cb;
    puhost->usr_cb = usr_cb;

    /* Start the USB OTG core */
    HCD_Init(pudev, CoreID);

    /* Upon Init call usr call back */
    puhost->usr_cb->Init();

    /* Enable Interrupts */
    USB_OTG_HWP_EnableInterrupt(pudev);
}

/**
  * @brief  De-Initialize Host
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @retval host status
  */
USBH_STATUS USBH_DeInit(USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost)
{
    /* Software Init */
    puhost->State = HOST_IDLE;
    puhost->BackupState = HOST_IDLE;
    puhost->EnumState = ENUM_IDLE;
    puhost->RequestState = CMD_SEND;

    puhost->Control.State = CTRL_SETUP;
    puhost->Control.Ep0Size = USB_OTG_FS_MAX_EP0_SIZE;

    puhost->DeviceProp.Address = USBH_DEVICE_ADDRESS_DEFAULT;
    puhost->DeviceProp.Speed = HPRT0_PRTSPD_FULL_SPEED;

    USBH_Free_Channel(pudev, puhost->Control.HcInNum);
    USBH_Free_Channel(pudev, puhost->Control.HcOutNum);

    return USBH_OK;
}

/**
  * @brief  USB Host core main state machine process
  * @param  pudev: pointer to select usb otg device
  * @param  puhost: pointer to usb host
  * @retval None
  */
void USBH_Process(USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost)
{
    volatile USBH_STATUS status = USBH_FAIL;

    /* check for Host port events */
    if ((HCD_IsDeviceConnected(pudev) == 0) && (puhost->State != HOST_IDLE))
    {
        if(puhost->State != HOST_DEV_DETACHED)
        {
            puhost->State = HOST_DEV_DETACHED;
        }
    }

    switch (puhost->State)
    {
        case HOST_IDLE:
            if (HCD_IsDeviceConnected(pudev))
            {
                puhost->State = HOST_DEV_ATTACHED;
                USB_OTG_HWP_mDelay(100);
            }
            break;

        case HOST_DEV_ATTACHED:
            puhost->usr_cb->DeviceConnected();
            puhost->Control.HcOutNum = USBH_Alloc_Channel(pudev, 0x00);
            puhost->Control.HcInNum = USBH_Alloc_Channel(pudev, 0x80);

            /* Reset USB Device */
            if (HCD_ResetPort(pudev) == 0)
            {
                puhost->usr_cb->DeviceReset();

                /* Wait for USB USBH_ISR_PrtEnDisableChange()
                 * Host is Now ready to start the Enumeration
                 */
                puhost->DeviceProp.Speed = HCD_GetCurrentSpeed(pudev);
                puhost->State = HOST_ENUMERATION;
                puhost->usr_cb->DeviceSpeedDetected(puhost->DeviceProp.Speed);

                /* Open IN Control pipes */
                USBH_Open_Channel (pudev,
                                   puhost->Control.HcInNum,
                                   puhost->DeviceProp.Address,
                                   puhost->DeviceProp.Speed,
                                   USB_EPTYPE_CTRL,
                                   puhost->Control.Ep0Size);

                /* Open OUT Control pipes */
                USBH_Open_Channel (pudev,
                                   puhost->Control.HcOutNum,
                                   puhost->DeviceProp.Address,
                                   puhost->DeviceProp.Speed,
                                   USB_EPTYPE_CTRL,
                                   puhost->Control.Ep0Size);
            }
            break;

        case HOST_ENUMERATION:
            /* Check for enumeration status */
            if (USBH_HandleEnum(pudev, puhost) == USBH_OK)
            {
                /* The function shall return USBH_OK when full enumeration is complete */

                /* user callback for end of device basic enumeration */
                puhost->usr_cb->EnumerationFinish();
                puhost->State = HOST_USER_INPUT;
            }
            break;

        case HOST_USER_INPUT:
            /*The function should return user response true to move to class state */
            if (puhost->usr_cb->UserInput() == USBH_USER_RESP_OK)
            {
                if((puhost->class_cb->Init(pudev, puhost)) == USBH_OK)
                {
                    puhost->State = HOST_CLASS_REQUEST;
                }
            }
            break;

        case HOST_CLASS_REQUEST:
            /* process class standard contol requests state machine */ 
            status = puhost->class_cb->Requests(pudev, puhost);

            if(status == USBH_OK)
            {
                puhost->State = HOST_CLASS;
            }
            else
            {
                USBH_ErrorHandle (puhost, status);
            }
            break;

        case HOST_CLASS:
            /* process class state machine */
            status = puhost->class_cb->Machine(pudev, puhost);
            USBH_ErrorHandle (puhost, status);
            break;

        case HOST_CTRL_XFER:
            /* process control transfer state machine */
            USBH_HandleControl (pudev, puhost);
            break;

        case HOST_SUSPENDED:
            break;

        case HOST_ERROR:
            /* Re-Initilaize Host for new Enumeration */
            USBH_DeInit (pudev, puhost);
            puhost->usr_cb->DeInit();
            puhost->class_cb->DeInit(pudev, &puhost->DeviceProp);
            break;

        case HOST_DEV_DETACHED:
            /* Manage User disconnect operations*/
            puhost->usr_cb->DeviceDisconnected();

            /* Re-Initilaize Host for new Enumeration */
            USBH_DeInit(pudev, puhost);
            puhost->usr_cb->DeInit();
            puhost->class_cb->DeInit(pudev, &puhost->DeviceProp);
            USBH_DeAlloc_AllChannel(pudev);
            puhost->State = HOST_IDLE;
            break;

        default:
            break;
    }
}

/**
  * @brief  Handle the Error on Host side
  * @param  puhost: pointer to usb host
  * @param  ErrType: type of Error or Busy/OK state
  * @retval None
  */
void USBH_ErrorHandle(USBH_HOST *puhost, USBH_STATUS ErrType)
{
    /* Error unrecovered or not supported device speed */
    if ((ErrType == USBH_SPEED_UNKNOWN_ERROR) ||
            (ErrType == USBH_UNRECOVERED_ERROR))
    {
        puhost->usr_cb->UnrecoveredError();
        puhost->State = HOST_ERROR;
    }
    /* USB host restart requested from application layer */
    else if(ErrType == USBH_APPLY_DEINIT)
    {
        puhost->State = HOST_ERROR;

        /* user callback for initalization */
        puhost->usr_cb->Init();
    }
}

/**
  * @brief  Complete enumeration process
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to host
  * @retval host status
  */
static USBH_STATUS USBH_HandleEnum(USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost)
{
    USBH_STATUS Status = USBH_BUSY;
    uint8_t Local_Buffer[64];

    switch (puhost->EnumState)
    {
        case ENUM_IDLE:
            /* Get Device Desc for only 1st 8 bytes : To get EP0 MaxPacketSize */
            if (USBH_GetDevDesc(pudev, puhost, 8) == USBH_OK)
            {
                puhost->Control.Ep0Size = puhost->DeviceProp.DevDesc.bMaxPacketSize0;

                /* Issue Reset */
                HCD_ResetPort(pudev);
                puhost->EnumState = ENUM_GET_FULL_DEV_DESC;

                /* modify control channels configuration for MaxPacket size */
                USBH_Modify_Channel (pudev,
                                     puhost->Control.HcOutNum,
                                     0,
                                     0,
                                     0,
                                     puhost->Control.Ep0Size);

                USBH_Modify_Channel (pudev,
                                     puhost->Control.HcInNum,
                                     0,
                                     0,
                                     0,
                                     puhost->Control.Ep0Size);
            }
            break;

        case ENUM_GET_FULL_DEV_DESC:
            /* Get FULL Device Desc  */
            if (USBH_GetDevDesc(pudev, puhost, USB_DEVDESC_SIZE) == USBH_OK)
            {
                /* user callback for device descriptor available */
                puhost->usr_cb->DeviceDescAvailable(&puhost->DeviceProp.DevDesc);
                puhost->EnumState = ENUM_SET_ADDR;
            }
            break;

        case ENUM_SET_ADDR: 
            /* set address */
            if (USBH_SetAddress(pudev, puhost, USBH_DEVICE_ADDRESS) == USBH_OK)
            {
                USB_OTG_HWP_mDelay(2);
                puhost->DeviceProp.Address = USBH_DEVICE_ADDRESS;

                /* user callback for device address assigned */
                puhost->usr_cb->DeviceAddressSet();
                puhost->EnumState = ENUM_GET_CFG_DESC;

                /* modify control channels to update device address */
                USBH_Modify_Channel (pudev,
                                     puhost->Control.HcInNum,
                                     puhost->DeviceProp.Address,
                                     0,
                                     0,
                                     0);

                USBH_Modify_Channel (pudev,
                                     puhost->Control.HcOutNum,
                                     puhost->DeviceProp.Address,
                                     0,
                                     0,
                                     0);
            }
            break;

        case ENUM_GET_CFG_DESC:
            /* get standard configuration descriptor */
            if (USBH_GetCfgDesc(pudev,
                                 puhost,
                                 USB_CFGDESC_SIZE) == USBH_OK)
            {
                puhost->EnumState = ENUM_GET_FULL_CFG_DESC;
            }
            break;

        case ENUM_GET_FULL_CFG_DESC:  
            /* get FULL config descriptor (config, interface, endpoints) */
            if (USBH_GetCfgDesc(pudev, 
                                puhost,
                                puhost->DeviceProp.CfgDesc.wTotalLength) == USBH_OK)
            {
                /* User callback for configuration descriptors available */
                puhost->usr_cb->ConfigurationDescAvailable(&puhost->DeviceProp.CfgDesc,
                                                          puhost->DeviceProp.ItfDesc,
                                                          puhost->DeviceProp.EpDesc[0]);

                puhost->EnumState = ENUM_GET_MFC_STRING_DESC;
            }
            break;

        case ENUM_GET_MFC_STRING_DESC:  
            if (puhost->DeviceProp.DevDesc.iManufacturer != 0)
            {
                /* Check that Manufacturer String is available */
                if (USBH_GetStrDesc(pudev,
                                    puhost,
                                    puhost->DeviceProp.DevDesc.iManufacturer, 
                                    Local_Buffer,
                                    0xff) == USBH_OK)
                {
                    /* User callback for Manufacturing string */
                    puhost->usr_cb->ManufacturerString(Local_Buffer);
                    puhost->EnumState = ENUM_GET_PRODUCT_STRING_DESC;
                }
            }
            else
            {
                puhost->usr_cb->ManufacturerString("N/A");
                puhost->EnumState = ENUM_GET_PRODUCT_STRING_DESC;
            }
            break;

        case ENUM_GET_PRODUCT_STRING_DESC:
            if (puhost->DeviceProp.DevDesc.iProduct != 0)
            {
                /* Check that Product string is available */
                if (USBH_GetStrDesc(pudev, 
                                    puhost, 
                                    puhost->DeviceProp.DevDesc.iProduct, 
                                    Local_Buffer, 
                                    0xff) == USBH_OK)
                {
                    /* User callback for Product string */
                    puhost->usr_cb->ProductString(Local_Buffer);
                    puhost->EnumState = ENUM_GET_SERIALNUM_STRING_DESC;
                }
            }
            else
            {
                puhost->usr_cb->ProductString("N/A");
                puhost->EnumState = ENUM_GET_SERIALNUM_STRING_DESC;
            }
            break;

        case ENUM_GET_SERIALNUM_STRING_DESC:
            if (puhost->DeviceProp.DevDesc.iSerialNumber != 0)
            {
                /* Check that Serial number string is available */
                if (USBH_GetStrDesc(pudev, 
                                    puhost, 
                                    puhost->DeviceProp.DevDesc.iSerialNumber,
                                    Local_Buffer, 
                                    0xff) == USBH_OK)
                {
                    /* User callback for Serial number string */
                    puhost->usr_cb->SerialNumString(Local_Buffer);
                    puhost->EnumState = ENUM_SET_CONFIGURATION;
                }
            }
            else
            {
                puhost->usr_cb->SerialNumString("N/A");
                puhost->EnumState = ENUM_SET_CONFIGURATION;
            }
            break;

        case ENUM_SET_CONFIGURATION:
            /* set configuration (default config) */
            if (USBH_SetCfg(pudev, 
                            puhost,
                            puhost->DeviceProp.CfgDesc.bConfigurationValue) == USBH_OK)
            {
                puhost->EnumState = ENUM_DEV_CONFIGURED;
            }
            break;

        case ENUM_DEV_CONFIGURED:
            /* user callback for enumeration done */
            Status = USBH_OK;
            break;

        default:
            break;
    }

    return Status;
}

/**
  * @brief  Handles the USB control transfer state machine
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to host
  * @retval host status
  */
USBH_STATUS USBH_HandleControl (USB_OTG_CORE_HANDLE *pudev, USBH_HOST *puhost)
{
    uint8_t direction;  
    static uint16_t timeout = 0;
    USBH_STATUS status = USBH_OK;
    URB_STATE URB_Status = URB_IDLE;

    puhost->Control.Status = CTRL_START;

    switch (puhost->Control.State)
    {
        case CTRL_SETUP:
            /* send a SETUP packet */
            USBH_CtlTxSetup (pudev, 
                             puhost->Control.Setup.Data, 
                             puhost->Control.HcOutNum);

            puhost->Control.State = CTRL_SETUP_WAIT;
            break;

        case CTRL_SETUP_WAIT:
            URB_Status = HCD_GetURB_State(pudev, puhost->Control.HcOutNum);

            /* case SETUP packet sent successfully */
            if(URB_Status == URB_DONE)
            {
                direction = (puhost->Control.Setup.b.bmRequestType & USB_REQ_DIR_MASK);

                /* check if there is a data stage */
                if (puhost->Control.Setup.b.wLength != 0)
                {
                    timeout = DATA_STAGE_TIMEOUT;

                    if (direction == USB_REQ_DIR_D2H)
                    {
                        /* Data Direction is IN */
                        puhost->Control.State = CTRL_DATA_IN;
                    }
                    else
                    {
                        /* Data Direction is OUT */
                        puhost->Control.State = CTRL_DATA_OUT;
                    }
                }
                /* No DATA stage */
                else
                {
                    timeout = NODATA_STAGE_TIMEOUT;

                    /* If there is No Data Transfer Stage */
                    if (direction == USB_REQ_DIR_D2H)
                    {
                        /* Data Direction is IN */
                        puhost->Control.State = CTRL_STATUS_OUT;
                    }
                    else
                    {
                        /* Data Direction is OUT */
                        puhost->Control.State = CTRL_STATUS_IN;
                    }
                }

                /* Set the delay timer to enable timeout for data stage completion */
                puhost->Control.Timer = HCD_GetCurrentFrame(pudev);
            }
            else if(URB_Status == URB_ERROR)
            {
                puhost->Control.State = CTRL_ERROR;
                puhost->Control.Status = CTRL_TRACERR;
            }
            break;

        case CTRL_DATA_IN:
            /* Issue an IN token */ 
            USBH_CtlRxData(pudev,
                           puhost->Control.buff,
                           puhost->Control.HcInNum,
                           puhost->Control.Length);

            puhost->Control.State = CTRL_DATA_IN_WAIT;
            break;

        case CTRL_DATA_IN_WAIT:
            URB_Status = HCD_GetURB_State(pudev, puhost->Control.HcInNum); 

            /* check is DATA packet transfered successfully */
            if (URB_Status == URB_DONE)
            {
                puhost->Control.State = CTRL_STATUS_OUT;
            }

            /* manage error cases*/
            if (URB_Status == URB_STALL) 
            {
                /* In stall case, return to previous machine state*/
                puhost->State = puhost->BackupState;
            }
            else if (URB_Status == URB_ERROR)
            {
                /* Device error */
                puhost->Control.State = CTRL_ERROR;
            }
            else if ((HCD_GetCurrentFrame(pudev)- puhost->Control.Timer) > timeout)
            {
                /* timeout for IN transfer */
                puhost->Control.State = CTRL_ERROR; 
            }
            break;

        case CTRL_DATA_OUT:
            /* Start DATA out transfer (only one DATA packet)*/
            pudev->host.HC[puhost->Control.HcOutNum].OutToggle = 1; 

            USBH_CtlTxData (pudev,
                            puhost->Control.buff,
                            puhost->Control.HcOutNum,
                            puhost->Control.Length);

            puhost->Control.State = CTRL_DATA_OUT_WAIT;
            break;

        case CTRL_DATA_OUT_WAIT:
            URB_Status = HCD_GetURB_State(pudev , puhost->Control.HcOutNum);

            if (URB_Status == URB_DONE)
            {
                /* If the Setup Pkt is sent successful, then change the state */
                puhost->Control.State = CTRL_STATUS_IN;
            }
            /* handle error cases */
            else if (URB_Status == URB_STALL) 
            {
                /* In stall case, return to previous machine state*/
                puhost->State = puhost->BackupState;
                puhost->Control.State = CTRL_STALLED;
            }
            else if (URB_Status == URB_NOTREADY)
            {
                /* Nack received from device */
                puhost->Control.State = CTRL_DATA_OUT;
            }
            else if (URB_Status == URB_ERROR)
            {
                /* device error */
                puhost->Control.State = CTRL_ERROR;      
            }
            break;

        case CTRL_STATUS_IN:
            /* Send 0 bytes out packet */
            USBH_CtlRxData (pudev,
                            0,
                            puhost->Control.HcInNum,
                            0);

            puhost->Control.State = CTRL_STATUS_IN_WAIT;
            break;

        case CTRL_STATUS_IN_WAIT:
            URB_Status = HCD_GetURB_State(pudev, puhost->Control.HcInNum); 

            if ( URB_Status == URB_DONE)
            {
                /* Control transfers completed, Exit the State Machine */
                puhost->State = puhost->BackupState;
                puhost->Control.State = CTRL_COMPLETE;
            }
            else if (URB_Status == URB_ERROR)
            {
                puhost->Control.State = CTRL_ERROR;  
            }
            else if((HCD_GetCurrentFrame(pudev) - puhost->Control.Timer) > timeout)
            {
                puhost->Control.State = CTRL_ERROR; 
            }
            else if(URB_Status == URB_STALL)
            {
                /* Control transfers completed, Exit the State Machine */
                puhost->State = puhost->BackupState;
                puhost->Control.Status = CTRL_STALL;
                status = USBH_NOT_SUPPORTED;
            }
            break;

        case CTRL_STATUS_OUT:
            pudev->host.HC[puhost->Control.HcOutNum].OutToggle ^= 1;
            USBH_CtlTxData (pudev,
                            0,
                            puhost->Control.HcOutNum,
                            0);

            puhost->Control.State = CTRL_STATUS_OUT_WAIT;
            break;

        case CTRL_STATUS_OUT_WAIT:
            URB_Status = HCD_GetURB_State(pudev, puhost->Control.HcOutNum);

            if (URB_Status == URB_DONE)
            {
                puhost->State = puhost->BackupState;
                puhost->Control.State = CTRL_COMPLETE;
            }
            else if (URB_Status == URB_NOTREADY)
            {
                puhost->Control.State = CTRL_STATUS_OUT;
            }
            else if (URB_Status == URB_ERROR)
            {
                puhost->Control.State = CTRL_ERROR;
            }
            break;

        case CTRL_ERROR:
            /* 
              After a halt condition is encountered or an error is detected by the 
              host, a control endpoint is allowed to recover by accepting the next Setup 
              PID; i.e., recovery actions via some other pipe are not required for control
              endpoints. For the Default Control Pipe, a device reset will ultimately be 
              required to clear the halt or error condition if the next Setup PID is not 
              accepted.
            */
            if (++puhost->Control.ErrorCount <= USBH_MAX_ERROR_COUNT)
            {
                /* Do the transmission again, starting from SETUP Packet */
                puhost->Control.State = CTRL_SETUP; 
            }
            else
            {
                puhost->Control.Status = CTRL_FAIL;
                puhost->State = puhost->BackupState;
                status = USBH_FAIL;
            }
            break;

        default:
            break;
    }

    return status;
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
