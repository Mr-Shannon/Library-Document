/**
  ******************************************************************************
  * @file    app.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   Main routine
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/ 
#include "usbd_cdc_core.h"
#include "usbd_user.h"
#include "usb_conf.h"
#include "usbd_desc.h"
#include "can_init.h"
#include "periph.h"


/* Private variables ---------------------------------------------------------*/
USB_OTG_CORE_HANDLE USB_OTG_dev;
RCC_ClocksPara RCC_Clocks;
extern int	can1_resive0;				//признак что приняли в буфер и можно отправить на USB

/* Private functions ---------------------------------------------------------*/
extern void printcan1(void);
extern void printcan_test(void);
void Delay (uint32_t ms);

/**
  * @brief  Main routine will construct a USB virtual ComPort device
  * @param  None
  * @retval None
  */
int main(void)
{
		RCC_GetClocksFreq(&RCC_Clocks);
    USBD_Init(&USB_OTG_dev,
              USB_OTG_FS_CORE_ID,
              &USER_desc,
              &USBD_CDC_cb,
              &USER_cb);
		init_CAN();
		LEDInit(LED1);LEDInit(LED2);
		LEDToggle(LED2);LEDToggle(LED1);
		Delay(50);
		LEDToggle(LED2);LEDToggle(LED1);
		Delay(50);
	
    /* Main loop */
    while (1)
    {
				if (can1_resive0 == 1)
					{
						printcan_test();
						can1_resive0=0;
					}

    }
}


void Delay (uint32_t ms)
{
	volatile uint32_t nCount;
	RCC_ClocksPara RCC_Clocks;
	RCC_GetClocksFreq(&RCC_Clocks);
  nCount=(RCC_Clocks.AHB_Frequency/10000)*ms;
  for (; nCount!=0; nCount--);

}



/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
