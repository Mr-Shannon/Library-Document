/**
  ******************************************************************************
  * @file    gd32f10x_it.h 
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   This file contains the headers of the interrupt handlers.
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __GD32F10X_IT_H
#define __GD32F10X_IT_H

#ifdef __cplusplus
extern "C" {
#endif 

/* Includes ------------------------------------------------------------------*/
#include "usb_conf.h"

/* Exported functions ------------------------------------------------------- */
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __GD32F10X_IT_H */

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
