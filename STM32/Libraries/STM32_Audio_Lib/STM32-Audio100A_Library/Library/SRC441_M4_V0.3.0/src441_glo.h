/**
  ******************************************************************************
  * @file    src441glo.h
  * @author  MCD Application Team
  * @version V0.1.0
  * @date    05-April-2013
  * @brief   This file contains SRC441 module definitions to export
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
#ifndef SRC441_GLO_H
#define SRC441_GLO_H

/* Includes ------------------------------------------------------------------*/


/*============================================================================*/
/*      C O D E   C O N F I G U R A T I O N                                   */
/*============================================================================*/



/*============================================================================*/
/*      E R R O R   V A L U E S                                               */
/*============================================================================*/
#define SRC441_ERROR_NONE                           0
#define SRC441_UNSUPPORTED_MODE                     -1
#define SRC441_BAD_FRAME_SIZE                       -2
#define SRC441_WRONG_NBBYTES_PER_SAMPLES            -3
#define SRC441_UNSUPPORTED_NB_CHANNELS              -4
#define SRC441_UNSUPPORTED_INPLACE_PROCESSING       -5
#define SRC441_BAD_HW                               -6


/*============================================================================*/
/*      D Y N A M I C   P A R A M E T E R S   S T R U C T U R E               */
/*============================================================================*/
struct src441_dynamic_param {
    /* To be filled (Need to have a non empty structure...) */
    int32_t   empty;
};
typedef struct src441_dynamic_param src441_dynamic_param_t;


/*============================================================================*/
/*      S T A T I C   P A R A M E T E R S   S T R U C T U R E                 */
/*============================================================================*/
struct src441_static_param {
    /* To be filled (Need to have a non empty structure...) */
    int32_t   empty;
};
typedef struct src441_static_param src441_static_param_t;


/*============================================================================*/
/*      I N T E R N A L   S T A T I C   M E M O R Y                           */
/*============================================================================*/
extern const uint32_t src441_dynamic_mem_size;


/*============================================================================*/
/*      I N T E R N A L   D Y N A M I C   M E M O R Y                         */
/*============================================================================*/
extern const uint32_t src441_static_mem_size;


/*============================================================================*/
/*      I N T E R F A C E S   D E C L A R A T I O N S                         */
/*============================================================================*/
/* src441_reset() : initializes static memory, states machines, ... */
extern int32_t src441_reset(void *static_mem_ptr, void *dynamic_mem_ptr);
/* src441_setConfig() : use to change dynamically some parameters */
extern int32_t src441_setConfig(src441_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);
/* src441_getConfig() : use to get values of dynamic parameters */
extern int32_t src441_getConfig(src441_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);
/* src441_setParam() : use to set parameters that won't change during processing */
extern int32_t src441_setParam(src441_static_param_t *input_static_param_ptr, void *static_mem_ptr);
/* src441_getParam() : use to get values of static parameters */
extern int32_t src441_getParam(src441_static_param_t *input_static_param_ptr, void *static_mem_ptr);
/* src441_process() : this is the main processing routine */
extern int32_t src441_process(buffer_t *input_buffer, buffer_t *output_buffer, void *static_mem_ptr);


#endif  /* SRC441_GLO_H */

/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
