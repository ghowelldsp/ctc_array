/**
 * @file    fir_accel.h
 *
 * @brief   Description
 *  
 * @par
 * Created On: 28/09/22
 * Created By: G. Howell
 */

#ifndef __FIR_ACCEL_TCB_H__
#define __FIR_ACCEL_TCB_H__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------- INCLUDES -----------------------------------*/

#ifdef FIR_ACC_SINGLE_CH
#include "fir_accel_tcb_singleCh.h"
#endif

#ifdef FIR_ACC_TWO_CH
#include "fir_accel_tcb_twoCh.h"
#endif

#ifdef FIR_ACC_CONT_FLOW
#include "fir_accel_tcb_continuousFlow.h"
#endif

#ifdef FIR_ACC_BAREMETAL
#include "fir_accel_tcb_baremetal.h"
#endif

#ifdef FIR_ACC_DECIMATE
#include "fir_accel_tcb_decimate.h"
#endif

#ifdef FIR_ACC_MIC_ARRAY
#include "fir_accel_tcb_mic_array.h"
#endif

#ifdef FIR_ACC_MULTICHANNEL_AMP
#include "fir_accel_tcb_mic_array.h"
#endif

/*------------------- MACROS AND DEFINES -------------------------*/
/*------------------- TYPEDEFS -----------------------------------*/
/*------------------- EXPORTED VARIABLES -------------------------*/
/*------------------- STATIC VARIABLES ---------------------------*/

#ifdef __cplusplus
}
#endif

#endif /* __FIR_ACCEL_TCB_H__ */
