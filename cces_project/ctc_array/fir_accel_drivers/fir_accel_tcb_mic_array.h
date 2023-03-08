/**
 * @file    fir_accel.h
 *
 * @brief   Description
 *  
 * @par
 * Created On: 28/09/22
 * Created By: G. Howell
 */
#pragma once
//#ifndef __FIR_ACCEL_TCB_DEF_H__
//#define __FIR_ACCEL_TCB_DEF_H__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------- INCLUDES -----------------------------------*/

#include "fir_accel.h"
#include <sys/platform.h>

/*------------------- MACROS AND DEFINES -------------------------*/

#define FIR_ACC_AUDIO_BLOCK_SIZE 			32u

#define FIR_ACC_NUM_CHANNELS 				20u

#define FIR_ACC_TAP_LENGTH 					11u
#define FIR_ACC_WINDOW_SIZE 				FIR_ACC_AUDIO_BLOCK_SIZE
#define FIR_ACC_SAMPLE_RATIO 				1u

#define FIR_CTR1_CAI_VAL 					0u
#define FIR_CTR1_CCINTR_VAL					0u
#define FIR_CTR1_FXD_VAL					0u
#define FIR_CTR1_TC_VAL						0u
#define FIR_CTR1_RND_VAL					0u

/*------------------- TYPEDEFS -----------------------------------*/
/*------------------- EXPORTED VARIABLES -------------------------*/
/*------------------- STATIC VARIABLES ---------------------------*/

static ADI_FIR_ACC_HANDLER firHandler = {0u};

static uint32_t firCtrl1 =  (FIR_CTR1_CH_VAL << FIR_POS_FIRCTL1_CH) |
						    (FIR_CTR1_CAI_VAL << FIR_POS_FIRCTL1_CAI) |
							(FIR_CTR1_CCINTR_VAL << FIR_POS_FIRCTL1_CCINTR) |
							(FIR_CTR1_FXD_VAL << FIR_POS_FIRCTL1_FXD) |
							(FIR_CTR1_TC_VAL << FIR_POS_FIRCTL1_TC) |
							(FIR_CTR1_RND_VAL << FIR_POS_FIRCTL1_RND) |
							0;

static FIRA_TCB firAccDefaultTcb =
{
	/* Coefficient Buffer */
	.nCoeffCount	= FIR_ACC_TAP_LENGTH,
	.nCoeffModify	= 1,

	/* Output Circular Buffer */
	.nOutputCount	= FIR_ACC_WINDOW_SIZE,
	.nOutputModify	= 1,

	/* Input Circular Buffer */
	.nInputCount	= FIR_ACC_TAP_LENGTH+(FIR_ACC_WINDOW_SIZE)-1u,
	.nInputModify	= 1,

	/* Registers */
	.nFirControl2	= 0 << FIR_POS_FIRCTL2_FIR_UPSAMP |							/* upsampling disabled */
					  0 << FIR_POS_FIRCTL2_FIR_SRCEN |							/* sample rate conversion disabled */
					  (FIR_ACC_SAMPLE_RATIO-1) << FIR_POS_FIRCTL2_FIR_RATIO |	/* sampling ratio */
					  (FIR_ACC_WINDOW_SIZE-1) << FIR_POS_FIRCTL2_WINDOW |		/* window length */
					  (FIR_ACC_TAP_LENGTH-1) << FIR_POS_FIRCTL2_TAPLEN |		/* tap length */
					  0
};

static FIRA_TCB firAccTcbs[FIR_ACC_NUM_CHANNELS];

#ifdef __cplusplus
}
#endif

//#endif /* __FIR_ACCEL_TCB_DEF_H__ */
