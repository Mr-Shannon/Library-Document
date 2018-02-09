/**
  ******************************************************************************
  * @file    usb_conf.h
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB device driver basic configuration
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_USB_CONF_H
#define __GD32F10X_USB_CONF_H

/* Includes ------------------------------------------------------------------*/
#include "gd32f10x.h"
#include "gd3210c_eval.h" 
#include "gd3210c_eval_lcd.h"

/** @addtogroup GD32F10x_Firmware
  * @{
  */

/** @addtogroup USB_OTG_FS
  * @{
  */

/** @addtogroup USB_OTG_FS_DRIVER
  * @{
  */
  
/** @defgroup USB_OTG_FS_CONF
  * @{
  */

/** @defgroup USB_OTG_FS_CONF_Exported_Defines
  * @{
  */ 

/* USB Core and PHY interface configuration */

/****************** USB OTG FS PHY CONFIGURATION *******************************
 *  The USB OTG FS Core supports one on-chip Full Speed PHY.
 *  The USE_EMBEDDED_PHY symbol is defined in the project compiler preprocessor
 *  when FS core is used.
*******************************************************************************/

#ifdef USE_USB_OTG_FS
    #define USB_OTG_FS_CORE
#endif

/*******************************************************************************
 *                      FIFO Size Configuration in Device mode
 *
 *  (i) Receive data FIFO size = RAM for setup packets + 
 *                   OUT endpoint control information +
 *                   data OUT packets + miscellaneous
 *      Space = ONE 32-bits words
 *      --> RAM for setup packets = 10 spaces
 *          (n is the nbr of CTRL EPs the device core supports)
 *      --> OUT EP CTRL info = 1 space
 *          (one space for status information written to the FIFO along with each 
 *          received packet)
 *      --> Data OUT packets = (Largest Packet Size / 4) + 1 spaces 
 *          (MINIMUM to receive packets)
 *      --> OR data OUT packets  = at least 2* (Largest Packet Size / 4) + 1 spaces 
 *          (if high-bandwidth EP is enabled or multiple isochronous EPs)
 *      --> Miscellaneous = 1 space per OUT EP
 *          (one space for transfer complete status information also pushed to the 
 *          FIFO with each endpoint's last packet)
 *
 *  (ii) MINIMUM RAM space required for each IN EP Tx FIFO = MAX packet size for 
 *       that particular IN EP. More space allocated in the IN EP Tx FIFO results
 *       in a better performance on the USB and can hide latencies on the AHB.
 *
 *  (iii) TXn min size = 16 words. (n:Transmit FIFO index)
 *
 *  (iv) When a TxFIFO is not used, the Configuration should be as follows:
 *       case 1: n > m and Txn is not used (n,m:Transmit FIFO indexes)
 *       --> Txm can use the space allocated for Txn.
 *       case 2: n < m and Txn is not used (n,m:Transmit FIFO indexes)
 *       --> Txn should be configured with the minimum space of 16 words
 *
 *  (v) The FIFO is used optimally when used TxFIFOs are allocated in the top 
 *      of the FIFO.Ex: use EP1 and EP2 as IN instead of EP1 and EP3 as IN ones.
 *
 *  (vi) In HS case12 FIFO locations should be reserved for internal DMA registers
 *       so total FIFO size should be 1012 Only instead of 1024
*******************************************************************************/

/****************** USB OTG FS CONFIGURATION **********************************/
#ifdef USB_OTG_FS_CORE
 #define RX_FIFO_SIZE                               128
 #define TX0_FIFO_FS_SIZE                           32
 #define TX1_FIFO_FS_SIZE                           128
 #define TX2_FIFO_FS_SIZE                           32
 #define TX3_FIFO_FS_SIZE                           0

// #define USB_OTG_FS_LOW_PWR_MGMT_SUPPORT
// #define USB_OTG_FS_SOF_OUTPUT_ENABLED
#endif

/****************** USB OTG MISC CONFIGURATION ********************************/
#define VBUS_SENSING_ENABLED

/****************** USB OTG MODE CONFIGURATION ********************************/
//#define USE_HOST_MODE
#define USE_DEVICE_MODE
//#define USE_OTG_MODE

#ifndef USB_OTG_FS_CORE
    #ifndef USB_OTG_HS_CORE
        #error  "USB_OTG_HS_CORE or USB_OTG_FS_CORE should be defined!"
    #endif
#endif

#ifndef USE_DEVICE_MODE
    #ifndef USE_HOST_MODE
        #error  "USE_DEVICE_MODE or USE_HOST_MODE should be defined!"
    #endif
#endif

#ifndef USE_USB_OTG_HS
    #ifndef USE_USB_OTG_FS
        #error  "USE_USB_OTG_HS or USE_USB_OTG_FS should be defined!"
    #endif
#endif

/****************** C Compilers dependant keywords ****************************/

/* In HS mode and when the DMA is used, all variables and data structures dealing
   with the DMA during the transaction process should be 4-bytes aligned */

#ifdef USB_OTG_HS_INTERNAL_DMA_ENABLED
    #if defined (__GNUC__)         /* GNU Compiler */
        #define __ALIGN_END __attribute__ ((aligned (4)))
        #define __ALIGN_BEGIN
    #else
        #define __ALIGN_END

        #if defined (__CC_ARM)     /* ARM Compiler */
            #define __ALIGN_BEGIN __align(4)  
        #elif defined (__ICCARM__) /* IAR Compiler */
            #define __ALIGN_BEGIN 
        #elif defined (__TASKING__)/* TASKING Compiler */
            #define __ALIGN_BEGIN __align(4) 
        #endif /* __CC_ARM */  
    #endif /* __GNUC__ */ 
#else
    #define __ALIGN_BEGIN
    #define __ALIGN_END
#endif /* USB_OTG_HS_INTERNAL_DMA_ENABLED */

/* __packed keyword used to decrease the data type alignment to 1-byte */
#if defined (__CC_ARM)         /* ARM Compiler */
    #define __packed __packed
#elif defined (__ICCARM__)     /* IAR Compiler */
    #define __packed __packed
#elif defined (__GNUC__)       /* GNU Compiler */
    #define __packed __attribute__ ((__packed__))
#elif defined (__TASKING__)    /* TASKING Compiler */
    #define __packed __unaligned
#endif /* __CC_ARM */


/**
  * @}
  */

#endif /* __GD32F10X_USB_CONF_H */

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
