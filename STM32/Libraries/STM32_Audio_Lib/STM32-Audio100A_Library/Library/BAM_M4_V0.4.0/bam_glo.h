/* -----------------------------------------------------------------------------    
* Copyright (C) 2013 STMicroelectronics. All rights reserved.    
*    
* $Date:        13 September 2013  
* $Revision: 	V1.0.2  
*    
* Project: 	    MCD Audio Library    
* Title:	    bam_glo.h    
*    
* Description:	Global header for the Bass Manager module.
*               This header includes variables and functions which are visible from the audio FW.
*    
* Target Processor: Cortex-M4
*  
*   Version     Date           Author              Comment
*   -------     --------      ---------            ---------------------------------
*   1.0.0       24 May 2013   F. Ober              - Creation.
*   1.0.1       14 Jun 2013   F. Ober              - First release on SVN
*   1.0.2       13 Sep 2013   F. Ober              - Add error for UNSUPPORTED_MODE
*   1.0.3       22 Oct 2014   F. Ober              - Removed enable, bass_Vol and Xover from static parameters
*    
* --------------------------------------------------------------------------- */

#include <stdio.h>

#ifndef BAM_GLO_H
#define BAM_GLO_H


/*============================================================================*/
/*      E R R O R   V A L U E S                                               */
/*============================================================================*/
#define BAM_ERROR_NONE                           0
#define BAM_UNSUPPORTED_INTERLEAVING_MODE       -1
#define BAM_UNSUPPORTED_NUMBER_OF_CHANNELS      -2
#define BAM_UNSUPPORTED_NUMBER_OF_BYTEPERSAMPLE -3
#define BAM_UNSUPPORTED_XOVER_FREQUENCY         -4
#define BAM_UNSUPPORTED_MODE                    -5
#define BAM_BAD_HW                              -6


#define BAM_OUTPUT_MODE_2_0                   1
#define BAM_OUTPUT_MODE_2_1_WITH_LFE_SPLIT    2
#define BAM_OUTPUT_MODE_2_1_WITHOUT_LFE_SPLIT 3


/*============================================================================*/
/*      D Y N A M I C   P A R A M E T E R S   S T R U C T U R E               */
/*============================================================================*/
struct bam_dynamic_param {
    int32_t limiter_release_time;
    int16_t bass_vol;
    int16_t freq_xover;
    int16_t enable;
    int16_t limiter_enable;
};
typedef struct bam_dynamic_param bam_dynamic_param_t;


/*============================================================================*/
/*      S T A T I C   P A R A M E T E R S   S T R U C T U R E                 */
/*============================================================================*/
struct bam_static_param {
    int16_t mode;
};
typedef struct bam_static_param bam_static_param_t;


/*============================================================================*/
/*      I N T E R N A L   S T A T I C   M E M O R Y                           */
/*============================================================================*/
extern const uint32_t bam_dynamic_mem_size;


/*============================================================================*/
/*      I N T E R N A L   D Y N A M I C   M E M O R Y                         */
/*============================================================================*/
extern const uint32_t bam_static_mem_size;


/*============================================================================*/
/*      I N T E R F A C E S   D E C L A R A T I O N S                         */
/*============================================================================*/
/* bam_reset() : initializes static memory, states machines, ... */
extern int32_t bam_reset(void *static_mem_ptr, void *dynamic_mem_ptr);

/* bam_setConfig() : use to change dynamically some parameters */
extern int32_t bam_setConfig(bam_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);

/* bam_getConfig() : use to get values of dynamic parameters */
extern int32_t bam_getConfig(bam_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);

/* bam_setParam() : use to set parameters that won't change during processing */
extern int32_t bam_setParam(bam_static_param_t *input_static_param_ptr, void *static_mem_ptr);

/* bam_getParam() : use to get values of static parameters */
extern int32_t bam_getParam(bam_static_param_t *input_static_param_ptr, void *static_mem_ptr);

/* bam_process() : this is the main processing routine */
extern int32_t bam_process(buffer_t *input_buffer, buffer_t *output_buffer, void *static_mem_ptr);


#endif // BAM_GLO_H
