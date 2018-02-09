/**
  ******************************************************************************
  * @file    cdc_glo.h
  * @author  MCD Application Team
  * @version V0.1.0
  * @date    05-April-2013
  * @brief   This file contains OmniSurround module definitions to export
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  ******************************************************************************
  */ 

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _OMNI2_GLO_H_
#define _OMNI2_GLO_H_

/* Includes ------------------------------------------------------------------*/


/*============================================================================*/
/*      D E F I N I T I O N S                                                 */
/*============================================================================*/
enum eAcMode_Supported
{
	AMODE20t = 0x0,          /*  Stereo channels for dolby pro logic */
    AMODE10 = 0x1,           /*  Mono channel (1.0) */
	AMODE20 = 0x2,           /*  Stereo channels (2.0) */
	AMODE30 = 0x3,           /*  Stereo + Center channel (3.0) */
    AMODE32 = 0x7,           /*  Stereo + Center channel + Surround Channels (5.0) */
    AMODE34 = 0xB,           /*  Stereo + Center channel + Surround Channels + Center Surround Channels (7.0) */
    AMODE20t_LFE = 0x80,     /*  Stereo channels for dolby pro logic + LFE channel */
    AMODE20_LFE = 0x82,      /*  Stereo + LFE channel (2.1) */
    AMODE30_LFE = 0x83,      /*  Stereo + Center channel + LFE channel (3.1) */
    AMODE32_LFE = 0x87,      /*  Stereo + Center channel + LFE channel + Surround Channels (5.1) */
    AMODE34_LFE = 0x8B,      /*  Stereo + Center channel + LFE channel + Surround Channels + Center Surround Channels (7.1) */
    AMODE_ID  = 0xFF         /*  End of supported configurations */ 
};


/*============================================================================*/
/*      C O D E   C O N F I G U R A T I O N                                   */
/*============================================================================*/



/*============================================================================*/
/*      E R R O R   V A L U E S                                               */
/*============================================================================*/
#define	OMNI2_ERROR_NONE            0
#define	OMNI2_ERROR                 -1
#define	OMNI2_ERROR_PARSE_COMMAND   -2
#define	OMNI2_BAD_HW                -3


/*============================================================================*/
/*      D Y N A M I C   P A R A M E T E R S   S T R U C T U R E               */
/*============================================================================*/
struct omni2_dynamic_param {
    int32_t             Omni2Enable;
    int32_t             Omni2Strength;
    int32_t             Omni2DownFiringSpeakers;
};
typedef struct omni2_dynamic_param omni2_dynamic_param_t;


/*============================================================================*/
/*      S T A T I C   P A R A M E T E R S   S T R U C T U R E                 */
/*============================================================================*/
struct omni2_static_param {
    int32_t             Omni2CentreOutput;
    int32_t             AudioMode;
    int32_t             SamplingFreq;
};
typedef struct omni2_static_param omni2_static_param_t;


/*============================================================================*/
/*      I N T E R N A L   S T A T I C   M E M O R Y                           */
/*============================================================================*/
extern const uint32_t omni2_dynamic_mem_size;


/*============================================================================*/
/*      I N T E R N A L   D Y N A M I C   M E M O R Y                         */
/*============================================================================*/
extern const uint32_t omni2_static_mem_size;


/*============================================================================*/
/*      I N T E R F A C E S   D E C L A R A T I O N S                         */
/*============================================================================*/
/* omni2_reset() : initializes static memory, states machines, ... */
extern int32_t omni2_reset(void *static_mem_ptr, void *dynamic_mem_ptr);
/* omni2_setConfig() : use to change dynamically some parameters */
extern int32_t omni2_setConfig(omni2_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);
/* omni2_getConfig() : use to get values of dynamic parameters */
extern int32_t omni2_getConfig(omni2_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);
/* omni2_setParam() : use to set parameters that won't change during processing */
extern int32_t omni2_setParam(omni2_static_param_t *input_static_param_ptr, void *static_mem_ptr);
/* omni2_getParam() : use to get values of static parameters */
extern int32_t omni2_getParam(omni2_static_param_t *input_static_param_ptr, void *static_mem_ptr);
/* omni2_process() : this is the main processing routine */
extern int32_t omni2_process(buffer_t *input_buffer, buffer_t *output_buffer, void *static_mem_ptr);


#endif /*_OMNI2_GLO_H_*/

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
