#ifndef __CAN
#define __CAN


/******************************************************************************
 * Заголовочник CAN
 ******************************************************************************/
#include "gd32f10x.h"
#include "gd32f10x_gpio.h"
#include "gd32f10x_rcc.h"
#include "gd32f10x_can.h"
#include "gd32f10x_misc.h"
 
 
//#define MODE CAN_MODE_SILENT_LOOPBACK
//#define MODE CAN_MODE_SILENT
//#define MODE CAN_MODE_LOOPBACK
#define MODE CAN_MODE_NORMAL
 
#define CAN1_ReMap // Переназначение CAN1 на нужные выводы

#ifndef CAN1_ReMap
	#define CAN1_GPIO_PORT			GPIOA
	#define CAN1_RX_SOURCE			GPIO_PIN_11				// RX
	#define CAN1_TX_SOURCE			GPIO_PIN_12				// TX
	#define CAN1_Periph				RCC_APB2PERIPH_GPIOA	// Тактирование блока переферии A
#else
	#define CAN1_GPIO_PORT			GPIOB
	#define CAN1_RX_SOURCE			GPIO_PIN_8				// RX
	#define CAN1_TX_SOURCE			GPIO_PIN_9				// TX
	#define CAN1_Periph				RCC_APB2PERIPH_GPIOB	// Тактирпование блока переферии B
#endif


// #define CAN1_SPEED_PRESCALE			2						// 750 Kb
// #define CAN1_SPEED_PRESCALE			3					  // 500 Kb
 #define CAN1_SPEED_PRESCALE	 		5						// 300 Kb
// #define CAN1_SPEED_PRESCALE			10					// 150 Kb
// #define CAN1_SPEED_PRESCALE			15					// 100 Kb
// #define CAN1_SPEED_PRESCALE			30					// 50 Kb
// #define CAN1_SPEED_PRESCALE			75					// 20 Kb
// #define CAN1_SPEED_PRESCALE			150					// 10 Kb
// #define CAN1_SPEED_PRESCALE getSpeed();



/******************************************************************************
 * 
 ******************************************************************************/
#define CAN_CMD_Test_Send			0x0001		// ??????? ???????? ????????? ?????????
#define CAN_CMD_Test_Ok				0x0002		// ??????? ????????????? ????????? ?????????




/******************************************************************************
 * 
 ******************************************************************************/
void init_CAN(void);
void CAN1_RX0_IRQHandler(void);

void CAN_Send_Test(void);
void CAN_Send_Ok(void);

#endif //__CAN
