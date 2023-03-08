/**
 * @file    fir_accel_mic_array.h
 *
 * @brief
 *  
 * @par
 * Created On: 29/10/22
 * Created By: G. Howell
 */

#ifndef __FIR_ACCEL_MIC_ARRAY_H__
#define __FIR_ACCEL_MIC_ARRAY_H__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------- INCLUDES -----------------------------------*/

#include "fir_accel.h"

/*------------------- MACROS AND DEFINES -------------------------*/
/*------------------- TYPEDEFS -----------------------------------*/
/*------------------- EXPORTED VARIABLES -------------------------*/

extern FIRA_CONFIG *pfirAccConfigL1;

/*------------------- GLOBAL FUNCTION PROTOTYPES -----------------*/

void fir_accelerator_initialise(void);
void fir_accelerator_run(FIRA_CONFIG *pFirAccConfig, float *pFirInputData);

#ifdef __cplusplus
}
#endif

#endif /* __FIR_ACCEL_MIC_ARRAY_H__ */
