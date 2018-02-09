/* -----------------------------------------------------------------------------    
* Copyright (C) 2012 STMicroelectronics. All rights reserved.    
*    
* $Date:        28 November 2012  
* $Revision: 	V1.0.0  
*    
* Project: 	    MCD Audio Library    
* Title:	    biquad_df1_cascade_glo.h    
*    
* Description:	Global header the Q31 Fast Biquad cascade DirectFormI(DF1) filter.
*               This header includes variables and functions wihch are visible from the audio FW.
*    
* Target Processor: Cortex-M4
*  
*   Version     Date           Author              Comment
*   -------     --------      ---------            ---------------------------------
*   1.0.0       28 Nov 2012   F. Ober              - Creation. Library includes all function needed
*                                                    to be integrated in the audio FW. 
*   1.0.1       23 Mar 2013   F. Ober              - Removed macros and buffer structure.
*   1.0.2       04 Apr 2013   F. Ober              - Cleanup.
*    
* --------------------------------------------------------------------------- */


#ifndef BIQUAD_DF1_CASCADE_GLO_H
#define BIQUAD_DF1_CASCADE_GLO_H


/*============================================================================*/
/*      E R R O R   V A L U E S                                               */
/*============================================================================*/
#define BIQ_ERROR_NONE                           0
#define BIQ_UNSUPPORTED_INTERLEAVING_MODE       -1
#define BIQ_UNSUPPORTED_NUMBER_OF_CHANNELS      -2
#define BIQ_UNSUPPORTED_NUMBER_OF_BYTEPERSAMPLE -3


/*============================================================================*/
/*      D Y N A M I C   P A R A M E T E R S   S T R U C T U R E               */
/*============================================================================*/
struct biquad_df1_cascade_dynamic_param {
    uint8_t enable;
    uint8_t nb_sos;
    int32_t biquad_coeff[60];   /* 6*MAX_NB_SOS */
};
typedef struct biquad_df1_cascade_dynamic_param biquad_df1_cascade_dynamic_param_t;


/*============================================================================*/
/*      S T A T I C   P A R A M E T E R S   S T R U C T U R E                 */
/*============================================================================*/
struct biquad_df1_cascade_static_param {
  uint8_t    nb_sos;  
  int32_t    biquad_coeff[60];  /* 6*MAX_NB_SOS */
};
typedef struct biquad_df1_cascade_static_param biquad_df1_cascade_static_param_t;


/*============================================================================*/
/*      I N T E R N A L   S T A T I C   M E M O R Y                           */
/*============================================================================*/
extern const uint32_t biquad_df1_cascade_dynamic_mem_size;


/*============================================================================*/
/*      I N T E R N A L   D Y N A M I C   M E M O R Y                         */
/*============================================================================*/
extern const uint32_t biquad_df1_cascade_static_mem_size;


/*============================================================================*/
/*      I N T E R F A C E S   D E C L A R A T I O N S                         */
/*============================================================================*/
/* biquad_df1_cascade_reset() : initializes static memory, states machines, ... */
extern int32_t biquad_df1_cascade_reset(void *static_mem_ptr, void *dynamic_mem_ptr);

/* biquad_df1_cascade_setConfig() : use to change dynamically some parameters */
extern int32_t biquad_df1_cascade_setConfig(biquad_df1_cascade_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);

/* biquad_df1_cascade_getConfig() : use to get values of dynamic parameters */
extern int32_t biquad_df1_cascade_getConfig(biquad_df1_cascade_dynamic_param_t *input_dynamic_param_ptr, void *static_mem_ptr);

/* biquad_df1_cascade_setParam() : use to set parameters that won't change during processing */
extern int32_t biquad_df1_cascade_setParam(biquad_df1_cascade_static_param_t *input_static_param_ptr, void *static_mem_ptr);

/* biquad_df1_cascade_getParam() : use to get values of static parameters */
extern int32_t biquad_df1_cascade_getParam(biquad_df1_cascade_static_param_t *input_static_param_ptr, void *static_mem_ptr);

/* biquad_df1_cascade_process() : this is the main processing routine */
extern int32_t biquad_df1_cascade_process(buffer_t *input_buffer, buffer_t *output_buffer, void *static_mem_ptr);


#endif // BIQUAD_DF1_CASCADE_GLO_H
