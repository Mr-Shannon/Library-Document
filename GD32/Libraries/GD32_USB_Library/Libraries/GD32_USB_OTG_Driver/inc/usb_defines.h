/**
  ******************************************************************************
  * @file    usb_defines.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Header file for USB OTG_FS core defines
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_DEF_H
#define __GD32F10X_USB_DEF_H

/* Includes ------------------------------------------------------------------*/
#include  "usb_conf.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */
  
/** @defgroup USB_OTG_FS_DEFINES
  * @{
  */

/** @defgroup USB_OTG_FS_CORE_Defines
  * @{
  */
#define USB_OTG_SPEED_PARAM_HIGH                0
#define USB_OTG_SPEED_PARAM_HIGH_IN_FULL        1
#define USB_OTG_SPEED_PARAM_FULL                3

#define USB_OTG_SPEED_HIGH                      0
#define USB_OTG_SPEED_FULL                      1

#define USB_OTG_ULPI_PHY                        1
#define USB_OTG_EMBEDDED_PHY                    2

/**
  * @}
  */


/** @defgroup USB_OTG_FS_Global_Defines
  * @{
  */
#define GAHBCSR_TXFEL_EMPTY                     1
#define GAHBCSR_TXFEL_HALFEMPTY                 0

#define GRXSTS_PKTSTS_IN                        2
#define GRXSTS_PKTSTS_IN_XFER_COMP              3
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR           5
#define GRXSTS_PKTSTS_CH_HALTED                 7

/**
  * @}
  */

/** @defgroup USB_OTG_FS_OTG_Mode_Defines
  * @{
  */
#define MODE_HNP_SRP_CAPABLE                    0
#define MODE_SRP_ONLY_CAPABLE                   1
#define MODE_NO_HNP_SRP_CAPABLE                 2
#define MODE_SRP_CAPABLE_DEVICE                 3
#define MODE_NO_SRP_CAPABLE_DEVICE              4
#define MODE_SRP_CAPABLE_HOST                   5
#define MODE_NO_SRP_CAPABLE_HOST                6
#define A_HOST                                  1
#define A_SUSPEND                               2
#define A_PERIPHERAL                            3
#define B_PERIPHERAL                            4
#define B_HOST                                  5
#define DEVICE_MODE                             0
#define HOST_MODE                               1
#define OTG_MODE                                2

/**
  * @}
  */

/** @defgroup USB_OTG_FS_Device_Mode_Defines
  * @{
  */
#define DSTR_ENUMSPD_HS_PHY_30MHZ_OR_60MHZ      0
#define DSTR_ENUMSPD_FS_PHY_30MHZ_OR_60MHZ      1
#define DSTR_ENUMSPD_LS_PHY_6MHZ                2
#define DSTR_ENUMSPD_FS_PHY_48MHZ               3

#define DCG_FRAME_INTERVAL_80                   0
#define DCG_FRAME_INTERVAL_85                   1
#define DCG_FRAME_INTERVAL_90                   2
#define DCG_FRAME_INTERVAL_95                   3

#define DEP0CTLR_MPL_64                         0
#define DEP0CTLR_MPL_32                         1
#define DEP0CTLR_MPL_16                         2
#define DEP0CTLR_MPL_8                          3

#define USB_EPTYPE_CTRL                         0
#define USB_EPTYPE_ISOC                         1
#define USB_EPTYPE_BULK                         2
#define USB_EPTYPE_INTR                         3
#define USB_EPTYPE_MSK                          3

#define RXSTR_GOUT_NAK                          1
#define RXSTR_DATA_UPDT                         2
#define RXSTR_XFER_COMP                         3
#define RXSTR_SETUP_COMP                        4
#define RXSTR_SETUP_UPDT                        6

/**
  * @}
  */


/** @defgroup USB_OTG_FS_Host_Mode_Defines
  * @{
  */
#define HC_PID_DATA0                           0
#define HC_PID_DATA2                           1
#define HC_PID_DATA1                           2
#define HC_PID_SETUP                           3

#define HPRT0_PRTSPD_HIGH_SPEED                0
#define HPRT0_PRTSPD_FULL_SPEED                1
#define HPRT0_PRTSPD_LOW_SPEED                 2

#define HCFG_30_60_MHZ                         0
#define HCFG_48_MHZ                            1
#define HCFG_6_MHZ                             2

#define HCCHAR_CTRL                            0
#define HCCHAR_ISOC                            1
#define HCCHAR_BULK                            2
#define HCCHAR_INTR                            3

#define  MIN(a, b)      (((a) < (b)) ? (a) : (b))

/**
  * @}
  */

/** @defgroup USB_OTG_FS_Defines_Exported_Types
  * @{
  */
typedef enum
{
    USB_OTG_HS_CORE_ID = 0,
    USB_OTG_FS_CORE_ID = 1
}USB_OTG_CORE_ID_TypeDef;

/**
  * @brief USB enumeration speed
  */
enum USB_OTG_SPEED {
    USB_SPEED_UNKNOWN = 0,
    USB_SPEED_LOW,
    USB_SPEED_FULL,
    USB_SPEED_HIGH
};

/**
  * @}
  */

#endif /* __GD32F10X_USB_DEF_H */


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

