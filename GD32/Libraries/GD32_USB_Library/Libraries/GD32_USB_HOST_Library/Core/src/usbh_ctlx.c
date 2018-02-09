/** 
  ******************************************************************************
  * @file    usbh_ctlx.c 
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file handles the issuing of the USB transactions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbh_ctlx.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_CTLX
  * @{
  */

/** @defgroup USBH_CTLX_Private_FunctionPrototypes
  * @{
  */
static USBH_STATUS USBH_SubmitSetupRequest(USBH_HOST *puhost,
                                               uint8_t* buf,
                                               uint16_t length);

/**
  * @}
  */


/** @defgroup USBH_CTLX_Private_Functions
  * @{
  */

/**
  * @brief  Send a control request and provide the status after completion of the request
  * @param  pudev: pointer to selected usb otg device
  * @param  puhost: pointer to usb host
  * @param  buf: data buffer address to store the response
  * @param  Len: length of the response
  * @retval host operation status
  */
USBH_STATUS USBH_CtlReq (USB_OTG_CORE_HANDLE *pudev, 
                                  USBH_HOST *puhost, 
                                  uint8_t *buf,
                                  uint16_t Len)
{
    USBH_STATUS status = USBH_BUSY;

    switch (puhost->RequestState)
    {
        case CMD_SEND:
            /* Start a SETUP transfer */
            USBH_SubmitSetupRequest(puhost, buf, Len);
            puhost->RequestState = CMD_WAIT;
            status = USBH_BUSY;
            break;

        case CMD_WAIT:
            if (puhost->Control.State == CTRL_COMPLETE)
            {
                /* Commands successfully sent and Response Received  */
                puhost->RequestState = CMD_SEND;
                puhost->Control.State = CTRL_IDLE;
                status = USBH_OK;
            }
            else if (puhost->Control.State == CTRL_ERROR)
            {
                /* Failure Mode */
                puhost->RequestState = CMD_SEND;
                status = USBH_FAIL;
            }
            else if (puhost->Control.State == CTRL_STALLED)
            {
                /* Commands successfully sent and Response Received */
                puhost->RequestState = CMD_SEND;
                status = USBH_NOT_SUPPORTED;
            }
            break;

        default:
            break; 
    }

    return status;
}

/**
  * @brief  Send the Setup Packet to the Device
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: buffer pointer from which the Data will be send to Device
  * @param  HcNum: Host channel Number
  * @retval host operation status
  */
USBH_STATUS USBH_CtlTxSetup (USB_OTG_CORE_HANDLE *pudev, 
                                           uint8_t *buf, 
                                           uint8_t  HcNum)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 0;
    pHC->DPID = HC_PID_SETUP;
    pHC->xfer_buff = buf;
    pHC->xfer_len = USBH_SETUP_PACKET_SIZE;

    return (USBH_STATUS)HCD_SubmitRequest (pudev, HcNum);
}

/**
  * @brief  Send a data packet to the Device
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: buffer pointer from which the Data will be sent to Device
  * @param  Len: length of the data to be sent
  * @param  HcNum: host channel number
  * @retval host operation status
  */
USBH_STATUS USBH_CtlTxData (USB_OTG_CORE_HANDLE *pudev, 
                                          uint8_t *buf, 
                                          uint8_t  HcNum,
                                          uint16_t Len)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 0;
    pHC->xfer_buff = buf;
    pHC->xfer_len = Len;

    if (Len == 0)
    {
        /* For status OUT stage, length = 0, Status Out PID = 1 */
        pHC->OutToggle = 1;
    }

    /* Set the Data Toggle bit as per the Flag */
    if (pHC->OutToggle == 0)
    {
        /* Put the PID 0 */
        pHC->DPID = HC_PID_DATA0;
    }
    else
    {
        /* Put the PID 1 */
        pHC->DPID = HC_PID_DATA1;
    }

    HCD_SubmitRequest (pudev, HcNum);

    return USBH_OK;
}

/**
  * @brief  Receives the device response to the Setup Packet
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: buffer pointer in which the response needs to be copied
  * @param  Len: length of the data to be received
  * @param  HcNum: Host channel Number
  * @retval host operation status
  */
USBH_STATUS USBH_CtlRxData (USB_OTG_CORE_HANDLE *pudev, 
                                          uint8_t *buf, 
                                          uint8_t  HcNum,
                                          uint16_t Len)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 1;
    pHC->DPID = HC_PID_DATA1;
    pHC->xfer_buff = buf;
    pHC->xfer_len = Len;

    HCD_SubmitRequest (pudev, HcNum);

    return USBH_OK;
}

/**
  * @brief  Sends the Bulk Packet to the device
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: Buffer pointer from which the Data will be sent to Device
  * @param  Len: Length of the data to be sent
  * @param  HcNum: Host channel Number
  * @retval Status
  */
USBH_STATUS USBH_BulkTxData (USB_OTG_CORE_HANDLE *pudev, 
                                           uint8_t *buf, 
                                           uint8_t  HcNum, 
                                           uint16_t Len)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 0;
    pHC->xfer_buff = buf;
    pHC->xfer_len = Len;

    /* Set the Data Toggle bit as per the Flag */
    if (pHC->OutToggle == 0)
    {
        /* Put the PID 0 */
        pHC->DPID = HC_PID_DATA0;
    }
    else
    {
        /* Put the PID 1 */
        pHC->DPID = HC_PID_DATA1;
    }

    HCD_SubmitRequest (pudev, HcNum);

    return USBH_OK;
}

/**
  * @brief  Receives IN bulk packet from device
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: buffer pointer in which the received data packet to be copied
  * @param  Len: Length of the data to be received
  * @param  HcNum: host channel number
  * @retval host operation status
  */
USBH_STATUS USBH_BulkRxData (USB_OTG_CORE_HANDLE *pudev, 
                                           uint8_t *buf, 
                                           uint8_t  HcNum, 
                                           uint16_t Len)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 1;
    pHC->xfer_buff = buf;
    pHC->xfer_len = Len;

    if( pHC->InToggle == 0)
    {
        pHC->DPID = HC_PID_DATA0;
    }
    else
    {
        pHC->DPID = HC_PID_DATA1;
    }

    HCD_SubmitRequest (pudev, HcNum);

    return USBH_OK;
}


/**
  * @brief  Receives the device response to the Interrupt IN token
  * @param  pudev: pointer to selected usb otg device
  * @param  buff: Buffer pointer in which the response needs to be copied
  * @param  Len: Length of the data to be received
  * @param  HcNum: host channel number
  * @retval host operation status
  */
USBH_STATUS USBH_InterruptRxData (USB_OTG_CORE_HANDLE *pudev, 
                                                uint8_t *buf, 
                                                uint8_t  HcNum, 
                                                uint8_t  Len)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 1;
    pHC->xfer_buff = buf;
    pHC->xfer_len = Len;

    if(pHC->InToggle == 0)
    {
        pHC->DPID = HC_PID_DATA0;
    }
    else
    {
        pHC->DPID = HC_PID_DATA1;
    }

    /* Toggle DATA PID */
    pHC->InToggle ^= 1;

    HCD_SubmitRequest (pudev, HcNum);

    return USBH_OK;
}

/**
  * @brief  Send the data on Interrupt OUT Endpoint
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: buffer pointer from where the data needs to be copied
  * @param  Len: length of the data to be sent
  * @param  HcNum: host channel Number
  * @retval host operation status 
  */
USBH_STATUS USBH_InterruptTxData(USB_OTG_CORE_HANDLE *pudev, 
                                               uint8_t *buf, 
                                               uint8_t  HcNum,
                                               uint8_t  Len)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 0;
    pHC->xfer_buff = buf;
    pHC->xfer_len = Len;

    if(pHC->InToggle == 0)
    {
        pHC->DPID = HC_PID_DATA0;
    }
    else
    {
        pHC->DPID = HC_PID_DATA1;
    }

    pHC->InToggle ^= 1;

    HCD_SubmitRequest (pudev, HcNum);

    return USBH_OK;
}


/**
  * @brief  Start a setup transfer by changing the state-machine and 
  *         initializing  the required variables needed for the Control Transfer
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: Buffer used for setup request
  * @param  Len: Length of the data
  * @retval host operation status
*/
static USBH_STATUS USBH_SubmitSetupRequest(USBH_HOST *puhost,
                                               uint8_t *buf, 
                                               uint16_t Len)
{
    /* Save Global State */
    puhost->BackupState = puhost->State;

    /* Prepare the Transactions */
    puhost->State = HOST_CTRL_XFER;
    puhost->Control.buff = buf;
    puhost->Control.Length = Len;
    puhost->Control.State = CTRL_SETUP;

    return USBH_OK;
}


/**
  * @brief  Receives the Device Response to the Isochronous IN token
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: buffer pointer in which the response needs to be copied
  * @param  Len: length of the data to be received
  * @param  HcNum: host channel number
  * @retval host operation status
  */
USBH_STATUS USBH_IsocRxData (USB_OTG_CORE_HANDLE *pudev, 
                                           uint8_t *buf, 
                                           uint8_t  HcNum,
                                           uint32_t Len)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 1;
    pHC->xfer_buff = buf;
    pHC->xfer_len = Len;
    pHC->DPID = HC_PID_DATA0;

    HCD_SubmitRequest (pudev, HcNum);

    return USBH_OK;
}

/**
  * @brief  Send the data on Isochronous OUT Endpoint
  * @param  pudev: pointer to selected usb otg device
  * @param  buf: buffer pointer from where the data needs to be copied
  * @param  Len: length of the data to be sent
  * @param  HcNum: host channel number
  * @retval host operation status
  */
USBH_STATUS USBH_IsocTxData(USB_OTG_CORE_HANDLE *pudev, 
                                          uint8_t *buf, 
                                          uint8_t  HcNum,
                                          uint32_t Len)
{
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpIn = 0;
    pHC->xfer_buff = buf;
    pHC->xfer_len = Len;
    pHC->DPID = HC_PID_DATA0;

    HCD_SubmitRequest (pudev, HcNum);

    return USBH_OK;
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
