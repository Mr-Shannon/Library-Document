/**
  ******************************************************************************
  * @file    usbd_ctlx.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS device-mode control transfer core functions
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbd_ctlx.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Device_Library
  * @{
  */

/** @defgroup USBD_CTLX
  * @{
  */

/** @defgroup USBD_CTLX_Private_Functions
  * @{
  */

/**
  * @brief  Transmit data on the control channel
  * @param  pudev: pointer to usb device instance
  * @param  pbuf: data buffer pointer
  * @param  Len: data length
  * @retval usb device operation status
  */
USBD_Status  USBD_CtlTx (USB_OTG_CORE_HANDLE *pudev,
                                      uint8_t *pbuf,
                                      uint16_t Len)
{
    USBD_Status ret = USBD_OK;

    pudev->dev.InEp[0].total_data_len = Len;
    pudev->dev.InEp[0].rem_data_len = Len;
    pudev->dev.device_cur_state = USB_OTG_CTRL_DATA_IN;

    DCD_EP_Tx (pudev, 0, pbuf, Len);

    return ret;
}

/**
  * @brief  Continue transmitting data on the control channel
  * @param  pudev: pointer to usb otg device instance
  * @param  pbuf: data buffer pointer
  * @param  Len: data length
  * @retval usb device operation status
  */
USBD_Status  USBD_CtlContinueTx (USB_OTG_CORE_HANDLE *pudev,
                                              uint8_t *pbuf,
                                              uint16_t Len)
{
    USBD_Status ret = USBD_OK;

    DCD_EP_Tx (pudev, 0, pbuf, Len);

    return ret;
}

/**
  * @brief  Receive data on the control channel
  * @param  pudev: pointer to usb otg device instance
  * @param  pbuf: data buffer pointer
  * @param  Len: data length
  * @retval usb device operation status
  */
USBD_Status  USBD_CtlRx (USB_OTG_CORE_HANDLE *pudev,
                                      uint8_t *pbuf,
                                      uint16_t Len)
{
    USBD_Status ret = USBD_OK;

    pudev->dev.OutEp[0].total_data_len = Len;
    pudev->dev.OutEp[0].rem_data_len = Len;
    pudev->dev.device_cur_state = USB_OTG_CTRL_DATA_OUT;

    DCD_EP_Rx (pudev, 0, pbuf, Len);

    return ret;
}

/**
  * @brief  Continue receiving data on the control channel
  * @param  pudev: pointer to usb device instance
  * @param  pbuf: data buffer pointer
  * @param  Len: data length
  * @retval usb device operation status
  */
USBD_Status  USBD_CtlContinueRx (USB_OTG_CORE_HANDLE *pudev,
                                              uint8_t *pbuf,
                                              uint16_t Len)
{
    USBD_Status ret = USBD_OK;

    DCD_EP_Rx (pudev, 0, pbuf, Len);

    return ret;
}

/**
  * @brief  Transmit status on the control channel
  * @param  pudev: pointer to usb device instance
  * @retval usb device operation status
  */
USBD_Status  USBD_CtlTransmitStatus (USB_OTG_CORE_HANDLE *pudev)
{
    USBD_Status ret = USBD_OK;

    pudev->dev.device_cur_state = USB_OTG_CTRL_STATUS_IN;

    DCD_EP_Tx (pudev, 0, NULL, 0);

    USB_OTG_EP0_StartOut(pudev);

    return ret;
}

/**
  * @brief  Receive status on the control channel
  * @param  pudev: pointer to usb otg device instance
  * @retval usb device operation status
  */
USBD_Status  USBD_CtlReceiveStatus (USB_OTG_CORE_HANDLE  *pudev)
{
    USBD_Status ret = USBD_OK;

    pudev->dev.device_cur_state = USB_OTG_CTRL_STATUS_OUT;

    DCD_EP_Rx (pudev, 0, NULL, 0);

    USB_OTG_EP0_StartOut(pudev);

    return ret;
}

/**
  * @brief  Get the received data Length
  * @param  pudev: pointer to usb otg device instance
  * @param  EpID: endpoint identifier
  * @retval Received data length
  */
uint16_t  USBD_GetRxCount (USB_OTG_CORE_HANDLE *pudev, uint8_t EpID)
{
    return pudev->dev.OutEp[EpID].xfer_count;
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
