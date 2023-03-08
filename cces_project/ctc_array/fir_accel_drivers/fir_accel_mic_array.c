/**
 * @file    fir_accel_mic_array.c
 *
 * @brief
 *  
 * @par
 * Created On: 29/10/22
 * Created By: G. Howell
 */

/*------------------- INCLUDES -----------------------------------*/

#include "fir_accel_mic_array.h"
#include "fir_accel_tcb.h"

#include <stdlib.h>

/*------------------- EXTERN VARIABLES ---------------------------*/
/*------------------- PRIVATE MACROS AND DEFINES -----------------*/
/*------------------- PRIVATE TYPEDEFS ---------------------------*/
/*------------------- STATIC VARIABLES ---------------------------*/
/*------------------- GLOBAL VARIABLES ---------------------------*/

FIRA_INFO firInfo;

FIRA_CONFIG firAccConfigL1;
FIRA_CONFIG *pfirAccConfigL1 = &firAccConfigL1;

FIRA_BUFFERS firaBuffL1;
FIRA_BUFFERS firaBuffR1;

// left and right input buffers
float firaInputBuffL1[FIR_ACC_TAP_LENGTH+FIR_ACC_WINDOW_SIZE-1u] = {0};
float firaInputBuffR1[FIR_ACC_TAP_LENGTH+FIR_ACC_WINDOW_SIZE-1u] = {0};

// fir output buffers
float firOutputBuffL1[FIR_ACC_NUM_CHANNELS][FIR_ACC_WINDOW_SIZE] = {0};

// coefficient buffer
float firCoeffBuffL1[FIR_ACC_TAP_LENGTH] =
{
	#include "fir_accel_drivers/firCoeff.dat"
};

/*------------------- STATIC FUNCTION PROTOTYPES -----------------*/
/*------------------- STATIC FUNCTIONS ---------------------------*/
/*------------------- GLOBAL FUNCTIONS ---------------------------*/

/**
 * @brief 	Initalises the fir accelerator
 *
 * @param
 *
 * @return  Fir accel result
 */
void fir_accelerator_initialise(void)
{
	*pREG_SPU0_SECUREP155 = 2u;

	// reserve memory for coefficient and output buffers
	firaBuffL1.pOutputBuffers = (float**)malloc(sizeof(float*)*FIR_ACC_NUM_CHANNELS);
	firaBuffL1.pCoeffBuffers = (float**)malloc(sizeof(float*)*FIR_ACC_NUM_CHANNELS);

	// set up input buffers
	firaBuffL1.pInputBuffer = firaInputBuffL1;

	// set up coefficient and output buffers for each channel
	for (int i=0; i<FIR_ACC_NUM_CHANNELS; i++)
	{
		*(firaBuffL1.pCoeffBuffers+i) = firCoeffBuffL1;
		*(firaBuffL1.pOutputBuffers+i) = firOutputBuffL1[i];
	}

	// info
	firInfo.delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo.windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo.inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	// set up fir accel
	fir_accel_init(&firAccConfigL1, firAccTcbs, &firaBuffL1, &firInfo);
	//	fir_enable_it();
}

void fir_accelerator_run(FIRA_CONFIG *pFirAccConfig, float *pFirInputData)
{
	// run fir accel
//	fira_add_input_data(pFirAccConfig, pFirInputData);
	fira_add_input_data(pFirAccConfig, pFirInputData);
	fir_enable(pFirAccConfig);
	fir_wait_till_done();
	fir_disable();
}

