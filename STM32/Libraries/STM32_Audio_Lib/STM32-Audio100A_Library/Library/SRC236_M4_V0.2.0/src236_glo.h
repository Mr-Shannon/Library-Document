/**
  ******************************************************************************
  * @file    src236glo.h
  * @author  MCD Application Team
  * @version V0.1.0
  * @date    04-April-2013
  * @brief   This file contains SRC236 module definitions to export
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
#ifndef SRC236_GLO_H
#define SRC236_GLO_H

/* Includes ------------------------------------------------------------------*/


/*============================================================================*/
/*      C O D E   C O N F I G U R A T I O N                                   */
/*============================================================================*/

#define SRC_RATIO_2			0	// Means FS_Out = FS_In * 2
#define SRC_RATIO_3			1	// Means FS_Out = FS_In * 3
#define SRC_RATIO_6			2	// Means FS_Out = FS_In * 6
#define SRC_RATIO_1_2		3	// Means FS_Out = FS_In / 2
#define SRC_RATIO_1_3		4	// Means FS_Out = FS_In / 3
#define SRC_RATIO_1_6		5	// Means FS_Out = FS_In / 6
#define SRC_RATIO_3_2		6	// Means FS_Out = FS_In * 3/2
#define SRC_RATIO_2_3		7	// Means FS_Out = FS_In * 2/3


/*============================================================================*/
/*      E R R O R   V A L U E S                                               */
/*============================================================================*/
#define SRC236_ERROR_NONE                              0
#define SRC236_UNSUPPORTED_MODE                        -1
#define SRC236_WRONG_NBBYTES_PER_SAMPLES               -2
#define SRC236_UNSUPPORTED_NB_CHANNELS                 -3
#define SRC236_UNSUPPORTED_INPLACE_PROCESSING          -4
#define SRC236_INPUT_BUFFERS_TOO_BIG                   -5
#define SRC236_BAD_HW                                  -6


/*============================================================================*/
/*      D Y N A M I C   P A R A M E T E R S   S T R U C T U R E               */
/*============================================================================*/
struct src236_dynamic_param {
    /* To be filled (Need to have a non empty structure...) */
    int32_t   empty;
};
typedef struct src236_dynamic_param src236_dynamic_param_t;


/*============================================================================*/
/*      S T A T I C   P A R A M E T E R S   S T R U C T U R E                 */
/*============================================================================*/
struct src236_static_param {
    /* To be filled (Need to have a non empty structure...) */
    int32_t   src_mode;
};
typedef struct src236_static_param src236_static_param_t;


/*============================================================================*/
/*      I N T E R N A L   S T A T I C   M E M O R Y                           */
/*============================================================================*/
extern const uint32_t src236_dynamic_mem_size;


/*============================================================================*/
/*      I N T E R N A L   D Y N A M I C   M E M O R Y                         */
/*============================================================================*/
extern const uint32_t src236_static_mem_size;


/*============================================================================*/
/*      I N T E R F A C E S   D E C L A R A T I O N S                         */
/*============================================================================*/
/* src236_reset() : initializes static memory, states machines, ... */
extern int32_t src236_reset(void *static_mem_ptr, void *dynamic_mem_ptr);
/* src236_setConfig() : use to change dynamically some parameters */
extern int32_t src236_setConfig(src236_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);
/* src236_getConfig() : use to get values of dynamic parameters */
extern int32_t src236_getConfig(src236_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);
/* src236_setParam() : use to set parameters that won't change during processing */
extern int32_t src236_setParam(src236_static_param_t *input_static_param_ptr, void *static_mem_ptr);
/* src236_getParam() : use to get values of static parameters */
extern int32_t src236_getParam(src236_static_param_t *input_static_param_ptr, void *static_mem_ptr);
/* src236_process() : this is the main processing routine */
extern int32_t src236_process(buffer_t *input_buffer, buffer_t *output_buffer, void *static_mem_ptr);


#endif  /* SRC236_GLO_H */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
