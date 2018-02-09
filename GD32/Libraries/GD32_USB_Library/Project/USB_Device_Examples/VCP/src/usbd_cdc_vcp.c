/**
  ******************************************************************************
  * @file    usbd_cdc_vcp.c
  * @author  MCU SD
  * @version V1.0.0
  * @date    26-Dec-2014
  * @brief   USB virtual ComPort media access layer functions
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usbd_cdc_vcp.h"
#include "usb_conf.h"
#include "periph.h"
#include <stdio.h>

#define APP_TX_DATA_SIZE  1000
uint32_t ptrWriteUserTxBufferFS = 0 ; 
uint32_t ptrReadUserTxBufferFS = 0 ; 
char 		sdstring[64];
char 		strhex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};



/* Private variables ---------------------------------------------------------*/
LINE_CODING linecoding =
{
    115200, /* Baud rate     */
    0x00,   /* Stop bits - 1 */
    0x00,   /* Parity - none */
    0x08    /* Num of bits 8 */
};


USART_InitPara USART_InitStructure;

/* Application buffer which's data will be sent over USB CDC device IN endpoint */
void	usb_out(char *pString);
void USB_send_one(uint8_t data);
extern uint8_t  APP_DATA_Buffer[];

/* Increment this pointer or roll it back to start pointer when writing received data to the APP_Buffer */
extern uint32_t AppBufInPtr;

extern CanTxMessage TxMessage;
extern CanRxMessage RxMessage;
extern void Delay(uint32_t ms);

/* Private function prototypes ----------------------------------------------- */
static uint16_t  VCP_Init      (uint8_t Config);
static uint16_t  VCP_DeInit    (void);
static uint16_t  VCP_Ctrl      (uint8_t Cmd, uint8_t* CmdBuf, uint16_t Len);
static uint16_t  VCP_DataTx    (/*COM_TypeDef Com, */uint8_t* DataBuf, uint16_t Len);
static uint16_t  VCP_DataRx    (COM_TypeDef Com, uint8_t* DataBuf, uint16_t Len);
static uint16_t  VCP_COMConfig (uint8_t Config);

CDC_IF_Fop_TypeDef VCP_fops = 
{
    VCP_Init,
    VCP_DeInit,
    VCP_Ctrl,
    VCP_DataTx,
    VCP_DataRx
};

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Initializes the virtual ComPort on the GD32
  * @param  Config: configuration type
  * @retval USBD_OK if configuration type is right, USBD_FAIL else
  */
static uint16_t VCP_Init(uint8_t Config)
{
    if(Config == DEFAULT_CONFIG)
    {
        NVIC_InitPara NVIC_InitStructure;

        USART_InitStructure.USART_BRR = 115200;
        USART_InitStructure.USART_WL = USART_WL_8B;
        USART_InitStructure.USART_STBits = USART_STBITS_1;
        USART_InitStructure.USART_Parity = USART_PARITY_RESET;
        USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
        USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;

        /* Configure and enable the USART */
        GD_EVAL_COMInit(COM2, &USART_InitStructure);

        /* Enable the USART Receive interrupt */
        USART_INT_Set(EVAL_COM2, USART_INT_RBNE, ENABLE);

        /* Enable USART Interrupt */
        NVIC_InitStructure.NVIC_IRQ = EVAL_COM2_IRQn;
        NVIC_InitStructure.NVIC_IRQPreemptPriority = 0;
        NVIC_InitStructure.NVIC_IRQSubPriority = 0;
        NVIC_InitStructure.NVIC_IRQEnable = ENABLE;
        NVIC_Init(&NVIC_InitStructure);

        return USBD_OK;
    }
    else
    {
        return USBD_FAIL;
    }
}

/**
  * @brief  Deinitializes the virtual ComPort on the GD32
  * @param  None
  * @retval USBD_OK
  */
static uint16_t VCP_DeInit(void)
{
    return USBD_OK;
}

/**
  * @brief  Handle the CDC device class requests
  * @param  Cmd: command code
  * @param  CmdBuf: command data buffer pointer
  * @param  Len: count of data to be sent (in bytes)
  * @retval USBD_OK
  */
static uint16_t  VCP_Ctrl (uint8_t Cmd, uint8_t* CmdBuf, uint16_t Len)
{
    switch (Cmd)
    {
        case SEND_ENCAPSULATED_COMMAND:
            /* No operation for this driver */
            break;

        case GET_ENCAPSULATED_RESPONSE:
            /* No operation for this driver */
            break;

        case SET_COMM_FEATURE:
            /* No operation for this driver */
            break;

        case GET_COMM_FEATURE:
            /* No operation for this driver */
            break;

        case CLEAR_COMM_FEATURE:
            /* No operation for this driver */
            break;

        case SET_LINE_CODING:
            linecoding.dwDTERate = (uint32_t)(CmdBuf[0] | (CmdBuf[1] << 8) | (CmdBuf[2] << 16) | (CmdBuf[3] << 24));
            linecoding.bCharFormat = CmdBuf[4];
            linecoding.bParityType = CmdBuf[5];
            linecoding.bDataBits = CmdBuf[6];
            VCP_COMConfig(SETTING_CONFIG);  /* Set the new configuration */
            break;

        case GET_LINE_CODING:
            CmdBuf[0] = (uint8_t)(linecoding.dwDTERate);
            CmdBuf[1] = (uint8_t)(linecoding.dwDTERate >> 8);
            CmdBuf[2] = (uint8_t)(linecoding.dwDTERate >> 16);
            CmdBuf[3] = (uint8_t)(linecoding.dwDTERate >> 24);
            CmdBuf[4] = linecoding.bCharFormat;
            CmdBuf[5] = linecoding.bParityType;
            CmdBuf[6] = linecoding.bDataBits;
            break;

        case SET_CONTROL_LINE_STATE:
            /* No operation for this driver */
            break;

        case SEND_BREAK:
            /* No operation for this driver */
            break;

        default:
            break;
    }

    return USBD_OK;
}

/**
  * @brief  Real usart received data to be send over virtual usart(USB Tx endpoint)
  * @param  DataBuf: data buffer will be sent
  * @param  Len: count of data to be sent (in bytes)
  * @retval USBD_OK if all operation is right, else USBD_FAIL
  */
static uint16_t VCP_DataTx (/*COM_TypeDef Com,*/ uint8_t* DataBuf, uint16_t Len)
{
		uint8_t x;
	
		for (x=0; x<Len; x++)
		{
				APP_DATA_Buffer[AppBufInPtr] = DataBuf[x];
			  AppBufInPtr++;
		}
		
    /* To avoid buffer overflow */
    if(AppBufInPtr == APP_RX_DATA_SIZE)
    {
        AppBufInPtr = 0;
				
    }

    return USBD_OK;
}



//передача данных на комп
uint16_t CDC_DataTx (uint8_t* Buf, uint32_t Len)
{
	uint8_t x;
  for (x=0; x<Len; x++)
	{
		APP_DATA_Buffer[AppBufInPtr] = Buf[x];
    AppBufInPtr++;
				LEDToggle(LED1);
 
  /* Если приняли больше APP_RX_DATA */
  if(AppBufInPtr == APP_RX_DATA_SIZE)
		{
			AppBufInPtr = 0;
		}
	 }
  return USBD_OK;
}

/**
  * @brief  Virtual usart received over USB Rx endpoint are sent over real usart
  * @note   Before exiting this function, the function will block any OUT packet reception on USB endpoint 
  *         If exiting this function before transfer is complete on CDC interface, it will result in 
  *         receiving more data while previous ones are still not sent.
  * @param  Buf: data buffer will be received
  * @param  Len: length of data received (in bytes)
  * @retval USBD_OK
  */
static uint16_t VCP_DataRx (COM_TypeDef Com, uint8_t* DataBuf, uint16_t Len)
{
    uint32_t i;

    for (i = 0; i < Len; i++)
    {
        USART_DataSend(COM_USART[Com], *(DataBuf + i) );
        while(USART_GetBitState(COM_USART[Com], USART_FLAG_TBE) == RESET); 
    }

    return USBD_OK;
}

/**
  * @brief  Configure the ComPort with default values or values received from host
  * @param  Config: can be DEFAULT_CONFIG to have no operation or SETTING_CONFIG 
  *                 to set a configuration received from the host
  * @retval USBD_FAIL if received command data have errors, or will be USBD_OK 
  */
static uint16_t VCP_COMConfig(uint8_t Config)
{
    if (Config == SETTING_CONFIG)  
    {
        uint32_t USART_STBits = USART_InitStructure.USART_STBits;
        uint32_t USART_Parity = USART_InitStructure.USART_Parity;

        /* set the Stop bit*/
        switch (linecoding.bCharFormat)
        {
            case 0:
                USART_InitStructure.USART_STBits = USART_STBITS_1;
                break;

            case 1:
                USART_InitStructure.USART_STBits = USART_STBITS_1_5;
                break;

            case 2:
                USART_InitStructure.USART_STBits = USART_STBITS_2;
                break;

            default :
                return (USBD_FAIL);
        }

        /* set the parity bit*/
        switch (linecoding.bParityType)
        {
            case 0:
                USART_InitStructure.USART_Parity = USART_PARITY_RESET;
                break;

            case 1:
                USART_InitStructure.USART_Parity = USART_PARITY_SETEVEN;
                break;

            case 2:
                USART_InitStructure.USART_Parity = USART_PARITY_SETODD;
                break;

            default :
                USART_InitStructure.USART_STBits = USART_STBits;
                return (USBD_FAIL);
        }

        /*set the data type : only 8bits and 9bits is supported */
        switch (linecoding.bDataBits)
        {
            case 0x07:
                /* With this configuration a parity (Even or Odd) should be set */
                USART_InitStructure.USART_WL = USART_WL_8B;
                break;

            case 0x08:
                if (USART_InitStructure.USART_Parity == USART_PARITY_RESET)
                {
                    USART_InitStructure.USART_WL = USART_WL_8B;
                }
                else 
                {
                    USART_InitStructure.USART_WL = USART_WL_9B;
                }
                break;

            default :
                USART_InitStructure.USART_Parity = USART_Parity;
                USART_InitStructure.USART_STBits = USART_STBits;
                return (USBD_FAIL);
        }

        USART_InitStructure.USART_BRR = linecoding.dwDTERate;
        USART_InitStructure.USART_HardwareFlowControl = USART_HARDWAREFLOWCONTROL_NONE;
        USART_InitStructure.USART_RxorTx = USART_RXORTX_RX | USART_RXORTX_TX;

        /* Configure and enable the USART */
        GD_EVAL_COMInit(COM2, &USART_InitStructure);
    }
    return USBD_OK;
}

void USB_send_one(uint8_t data)
{
  APP_DATA_Buffer[0] = data;
	CDC_DataTx(APP_DATA_Buffer, 1);	// отдаем по 1 элементу
}
//-------------------------------------------------------------------------------------------------------
//выведем строку в виртуальный порт usb
//пока не достигли конца строки вызываем USB_Send_Data, увеличивая указатель на элемент в строке
void	usb_out(char *pString)
{
	while (*pString != 0x00)
		{ 
			USB_send_one(*pString++);		//положим символ в буфер
		}
}

uint8_t halfbyte_to_hexascii(uint8_t _halfbyte)
	{
			 _halfbyte &= 0x0F ;
			 if(_halfbyte >= 10) return('A' + _halfbyte - 10) ; 
				else               return('0' + _halfbyte) ; 
	}

void printcan_test(void)
{
	char sdsh, sdsl;
	uint8_t	dshell, i;
	
	sprintf(sdstring, "\r\n");
	usb_out(sdstring);
	sprintf(sdstring, "%s", "STD_ID_HB: "); 	//старший байт стандартного заголовка
	usb_out(sdstring);
	dshell = (RxMessage.StdId & 0x0F00)>>8;
	sdsh=strhex[((dshell & 0xF0)>>4)];				//выделим старший символ
	sdsl=strhex[(dshell & 0x0F)];							//выделим младший символ
	USB_send_one(sdsh);												//отправим в usb
	USB_send_one(sdsl);												//отправим в usb
	USB_send_one(' ');												//напечатаем пробел


	sprintf(sdstring, "\x1b[31;1mSTD_ID_LB: \x1b[0m");		//младший байт стандартного заголовка
	usb_out(sdstring);
	dshell = RxMessage.StdId & 0xFF;
	sdsh=strhex[((dshell & 0xF0)>>4)];				//выделим старший символ
	sdsl=strhex[(dshell & 0x0F)];							//выделим младший символ
	USB_send_one(sdsh);												//отправим в usb
	USB_send_one(sdsl);												//отправим в usb
	USB_send_one(' ');												//напечатаем пробел
	
	
	sprintf(sdstring, "DLC: ");								//длинна
	usb_out(sdstring);
	dshell = RxMessage.DLC;
	sdsh = strhex[((dshell & 0xF0)>>4)];
	sdsl=strhex[(dshell & 0x0F)];	
	USB_send_one(sdsh);
	USB_send_one(sdsl);
	USB_send_one(' ');	
	
	
	sprintf(sdstring, "DATA: ");							//поле данных все 8 банок в цикле
	usb_out(sdstring);
	for(i=0; i<8; i++)
	{
		dshell = RxMessage.Data[i];
		sdsh = strhex[((dshell & 0xF0)>>4)];
		sdsl = strhex[(dshell & 0x0F)];
		USB_send_one(sdsh);
		USB_send_one(sdsl);
		USB_send_one(' ');
	}
	
}
	
	
void	printcan1(void)
	{
				uint32_t num_bytes ;
				uint8_t buf[512] ;	
				num_bytes = 0 ;
				buf[num_bytes++] = '\n' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.StdId)>>28);
				buf[num_bytes++] = halfbyte_to_hexascii(((RxMessage.StdId)>>24)&0xF);
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((((RxMessage.StdId)>>16)&0xF0)>>4);
				buf[num_bytes++] = halfbyte_to_hexascii(((RxMessage.StdId)>>16)&0xF);
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.DLC)>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.DLC));
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[0])>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[0]));
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[1])>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[1]));
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[2])>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[2]));
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[3])>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[3]));
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[4])>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[4]));
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[5])>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[5]));
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[6])>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[6]));
				buf[num_bytes++] = ' ' ;
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[7])>>4);
				buf[num_bytes++] = halfbyte_to_hexascii((RxMessage.Data[7]));
				buf[num_bytes++] = '\r';
				CDC_DataTx (buf, num_bytes);
	}

/************************ (C) COPYRIGHT 2014 GIGADEVICE *****END OF FILE****/
