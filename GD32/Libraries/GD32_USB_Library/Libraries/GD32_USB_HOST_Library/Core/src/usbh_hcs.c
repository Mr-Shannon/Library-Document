/**
  ******************************************************************************
  * @file    usbh_hcs.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file implements functions for opening and closing host channels
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "usbh_hcs.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_Host_Core_Library
  * @{
  */

/** @defgroup USBH_HCS
  * @{
  */

/** @defgroup USBH_HCS_Private_FunctionPrototypes
  * @{
  */
static uint16_t USBH_GetFreeChannel (USB_OTG_CORE_HANDLE *pudev);

/**
  * @}
  */

/** @defgroup USBH_HCS_Private_Functions
  * @{
  */

/**
  * @brief  Open a channel
  * @param  pudev: pointer to selected usb otg device
  * @param  HcNum: host channel number
  * @param  DevAddress: USB Device address allocated to attached device
  * @param  Speed: USB device speed (Full/Low)
  * @param  EpType: endpoint type (bulk/int/ctl)
  * @param  Mps: max packet size
  * @retval host operation status
  */
uint8_t USBH_Open_Channel  (USB_OTG_CORE_HANDLE *pudev,
                            uint8_t  HcNum,
                            uint8_t  DevAddress,
                            uint8_t  Speed,
                            uint8_t  EpType,
                            uint16_t Mps)
{
    uint16_t ChInfo = pudev->host.ChannelInfo[HcNum];
    USB_OTG_HC *pHC = &pudev->host.HC[HcNum];

    pHC->EpID = ChInfo & 0x7F;
    pHC->EpIn = (ChInfo & 0x80) == 0x80;
    pHC->DevAddr = DevAddress;
    pHC->EpType = EpType;
    pHC->Mps = Mps;
    pHC->Speed = Speed;
    pHC->InToggle = 0;
    pHC->OutToggle = 0;

    if(Speed == HPRT0_PRTSPD_HIGH_SPEED)
    {
        pHC->do_ping = 1;
    }

    USB_OTG_HC_Init(pudev, HcNum);

    return HC_OK;
}

/**
  * @brief  Modify a channel
  * @param  pudev: pointer to selected usb otg device
  * @param  HcNum: host channel number
  * @param  DevAddress: USB Device address allocated to attached device
  * @param  Speed: USB device speed (Full/Low)
  * @param  EpType: endpoint type (Bulk/int/ctl)
  * @param  Mps: max packet size
  * @retval host operation status
  */
uint8_t USBH_Modify_Channel (USB_OTG_CORE_HANDLE *pudev,
                             uint8_t  HcNum,
                             uint8_t  DevAddress,
                             uint8_t  Speed,
                             uint8_t  EpType,
                             uint16_t Mps)
{
    if(DevAddress != 0)
    {
        pudev->host.HC[HcNum].DevAddr = DevAddress;
    }

    if((pudev->host.HC[HcNum].Mps != Mps) && (Mps != 0))
    {
        pudev->host.HC[HcNum].Mps = Mps; 
    }

    if((pudev->host.HC[HcNum].Speed != Speed ) && (Speed != 0))
    {
        pudev->host.HC[HcNum].Speed = Speed;
    }

    USB_OTG_HC_Init(pudev, HcNum);

    return HC_OK;
}

/**
  * @brief  Allocate a new channel for the pipe
  * @param  pudev: pointer to selected usb otg device
  * @param  EpAddr: endpoint for which the channel to be allocated
  * @retval host channel number
  */
uint8_t USBH_Alloc_Channel (USB_OTG_CORE_HANDLE *pudev, uint8_t EpAddr)
{
    uint16_t HcNum = USBH_GetFreeChannel(pudev);

    if (HcNum != HC_ERROR)
    {
        pudev->host.ChannelInfo[HcNum] = HC_USED | EpAddr;
    }

    return HcNum;
}

/**
  * @brief  Free the USB host channel
  * @param  Idx: channel number to be freed 
  * @retval host operation status
  */
uint8_t USBH_Free_Channel (USB_OTG_CORE_HANDLE *pudev, uint8_t Idx)
{
    if(Idx < HC_MAX)
    {
        pudev->host.ChannelInfo[Idx] &= HC_USED_MASK;
    }

    return USBH_OK;
}

/**
  * @brief  Free all USB host channel
  * @param  pudev: pointer to usb otg core instance
  * @retval host operation status
  */
uint8_t USBH_DeAlloc_AllChannel (USB_OTG_CORE_HANDLE *pudev)
{
    uint8_t idx;

    for (idx = 2; idx < HC_MAX; idx ++)
    {
        pudev->host.ChannelInfo[idx] = 0;
    }

    return USBH_OK;
}

/**
  * @brief  Get a free channel number for allocation to a device endpoint
  * @param  pudev: pointer to usb otg core instance
  * @retval free channel number
  */
static uint16_t USBH_GetFreeChannel (USB_OTG_CORE_HANDLE *pudev)
{
    uint8_t Idx = 0;

    for (Idx = 0; Idx < HC_MAX; Idx++)
    {
        if ((pudev->host.ChannelInfo[Idx] & HC_USED) == 0)
        {
            return Idx;
        }
    }

    return HC_ERROR;
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
