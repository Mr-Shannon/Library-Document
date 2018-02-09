/**
  ******************************************************************************
  * @file    usb_hcd.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB OTG_FS host mode driver Layer
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"
#include "usb_hcd.h"
#include "usb_conf.h"
#include "usb_hwp.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */

/** @defgroup USB_OTG_FS_HCD
  * @{
  */

/** @defgroup USB_HCD_Private_Functions
  * @{
  */ 

/**
  * @brief  Initialize the HOST portion of the driver.
  * @param  pudev: pointer to selected usb otg device
  * @param  CoreID: usb otg core identifier(High-Speed or Full-Speed)
  * @retval operation status
  */
uint32_t HCD_Init(USB_OTG_CORE_HANDLE *pudev, USB_OTG_CORE_ID_TypeDef CoreID)
{
    uint8_t i = 0;

    pudev->host.ConnectStatus = 0;

    for (i = 0; i < USB_OTG_FS_MAX_TX_FIFOS; i++)
    {
        pudev->host.ErrCount[i] = 0;
        pudev->host.XferCount[i] = 0;
        pudev->host.HC_Status[i] = HC_IDLE;
    }

    pudev->host.HC[0].Mps = 8;

    USB_OTG_SelectCore(pudev, CoreID);

#ifndef DUAL_ROLE_MODE_ENABLED

    _DisableGlobalInt();

    USB_OTG_CoreInit(pudev);

    /* Force Host Mode*/
    USB_OTG_SetCurrentMode(pudev, HOST_MODE);

    USB_OTG_CoreInitHost(pudev);

    _EnableGlobalInt();

#endif

    return 0;
}

/**
  * @brief  Get current device speed
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
uint32_t HCD_GetCurrentSpeed (USB_OTG_CORE_HANDLE *pudev)
{
    return (_GetHPCSR() & HPCSR_PS) >> 17;
}

/**
  * @brief  Issues the reset command to device
  * @param  pudev: pointer to selected usb otg device
  * @retval operation status
  */
uint32_t HCD_ResetPort(USB_OTG_CORE_HANDLE *pudev)
{
    /*
     * Before starting to drive a USB reset, the application waits for the OTG 
     * interrupt triggered by the debounce done bit (DBCDNE bit in OTG_FS_GOTGINT), 
     * which indicates that the bus is stable again after the electrical debounce 
     * caused by the attachment of a pull-up resistor on DP (FS) or DM (LS).
     */
    USB_OTG_ResetPort(pudev);

    return 0;
}

/**
  * @brief  Check if the device is connected.
  * @param  pudev: pointer to selected usb otg device
  * @retval Device connection status. 1 -> connected and 0 -> disconnected
  */
uint32_t HCD_IsDeviceConnected(USB_OTG_CORE_HANDLE *pudev)
{
    return (pudev->host.ConnectStatus);
}

/**
  * @brief  This function returns the frame number for sof packet
  * @param  pudev: pointer to selected usb otg device
  * @retval frame number
  */
uint32_t HCD_GetCurrentFrame (USB_OTG_CORE_HANDLE *pudev) 
{
    return (_GetHFINF() & 0xFFFF);
}

/**
  * @brief  This function returns the last URBstate
  * @param  pudev: pointer to selected usb otg device
  * @param  ChannelNum: host channel number
  * @retval URB_STATE
  */
URB_STATE HCD_GetURB_State (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum) 
{
    return pudev->host.URB_State[ChannelNum];
}

/**
  * @brief  This function returns the last URBstate
  * @param  pudev: pointer to selected usb otg device
  * @param  ChannelNum: host channel number
  * @retval No. of data bytes transferred
  */
uint32_t HCD_GetXferCount (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum) 
{
    return pudev->host.XferCount[ChannelNum];
}

/**
  * @brief  This function returns the HC Status 
  * @param  pudev: pointer to selected usb otg device
  * @param  ChannelNum: host channel number
  * @retval host channel status
  */
HC_STATUS HCD_GetHCState (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum) 
{
    return pudev->host.HC_Status[ChannelNum];
}

/**
  * @brief  This function prepare a HC and start a transfer
  * @param  pudev: pointer to selected usb otg device
  * @param  ChannelNum: host channel number
  * @retval operation status
  */
uint32_t HCD_HC_Init (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum) 
{
    return USB_OTG_HC_Init(pudev, ChannelNum);
}

/**
  * @brief  This function prepare a HC and start a transfer
  * @param  pudev: pointer to selected usb otg device
  * @param  ChannelNum: host channel number
  * @retval operation status
  */
uint32_t HCD_SubmitRequest (USB_OTG_CORE_HANDLE *pudev, uint8_t ChannelNum) 
{
    pudev->host.URB_State[ChannelNum] = URB_IDLE;
    pudev->host.HC[ChannelNum].xfer_count = 0;

    return USB_OTG_HC_StartXfer(pudev, ChannelNum);
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
