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

/*------------------- EXTERN VARIABLES ---------------------------*/
/*------------------- PRIVATE MACROS AND DEFINES -----------------*/
/*------------------- PRIVATE TYPEDEFS ---------------------------*/
/*------------------- STATIC VARIABLES ---------------------------*/
/*------------------- GLOBAL VARIABLES ---------------------------*/

FIRA_INFO firInfo1[FIR_ACC_NUM_CHANNELS];

ADI_FIR_ACC_CONFIG firAccConfig1[FIR_ACC_NUM_CHANNELS];
ADI_FIR_ACC_CONFIG *pfirAccConfig1 = firAccConfig1;

ADI_FIR_ACC_BUFF firAccBuff1[FIR_ACC_NUM_CHANNELS];

ADI_FIR_ACC_IN_DATA firInputData[2];
ADI_FIR_ACC_IN_DATA *pFirInputData = firInputData;

// left and right input buffers
float firInputBuff1[FIR_ACC_TAP_LENGTH+FIR_ACC_WINDOW_SIZE-1u] = {0};
float firInputBuff2[FIR_ACC_TAP_LENGTH+FIR_ACC_WINDOW_SIZE-1u] = {0};

// fir output buffers
float firOutputBuff1[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff2[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff3[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff4[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff5[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff6[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff7[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff8[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff9[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff10[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff11[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff12[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff13[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff14[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff15[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff16[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff17[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff18[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff19[FIR_ACC_WINDOW_SIZE] = {0};
float firOutputBuff20[FIR_ACC_WINDOW_SIZE] = {0};

// coefficient buffer
float firCoeffBuff1[FIR_ACC_TAP_LENGTH] =
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

	// set up fir 1 buffers
	firAccBuff1[0].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[0].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[0].pOutputBuff = (void*)firOutputBuff1;

	firAccBuff1[1].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[1].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[1].pOutputBuff = (void*)firOutputBuff2;

	firAccBuff1[2].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[2].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[2].pOutputBuff = (void*)firOutputBuff3;

	firAccBuff1[3].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[3].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[3].pOutputBuff = (void*)firOutputBuff4;

	firAccBuff1[4].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[4].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[4].pOutputBuff = (void*)firOutputBuff5;

	firAccBuff1[5].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[5].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[5].pOutputBuff = (void*)firOutputBuff6;

	firAccBuff1[6].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[6].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[6].pOutputBuff = (void*)firOutputBuff7;

	firAccBuff1[7].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[7].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[7].pOutputBuff = (void*)firOutputBuff8;

	firAccBuff1[8].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[8].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[8].pOutputBuff = (void*)firOutputBuff9;

	firAccBuff1[9].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[9].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[9].pOutputBuff = (void*)firOutputBuff10;

	firAccBuff1[10].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[10].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[10].pOutputBuff = (void*)firOutputBuff11;

	firAccBuff1[11].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[11].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[11].pOutputBuff = (void*)firOutputBuff12;

	firAccBuff1[12].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[12].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[12].pOutputBuff = (void*)firOutputBuff13;

	firAccBuff1[13].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[13].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[13].pOutputBuff = (void*)firOutputBuff14;

	firAccBuff1[14].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[14].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[14].pOutputBuff = (void*)firOutputBuff15;

	firAccBuff1[15].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[15].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[15].pOutputBuff = (void*)firOutputBuff16;

	firAccBuff1[16].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[16].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[16].pOutputBuff = (void*)firOutputBuff17;

	firAccBuff1[17].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[17].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[17].pOutputBuff = (void*)firOutputBuff18;

	firAccBuff1[18].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[18].pInputBuff = (void*)firInputBuff1;
	firAccBuff1[18].pOutputBuff = (void*)firOutputBuff19;

	firAccBuff1[19].pCoeffBuff = (void*)firCoeffBuff1;
	firAccBuff1[19].pInputBuff = (void*)firInputBuff2;
	firAccBuff1[19].pOutputBuff = (void*)firOutputBuff20;

	// info
	firInfo1[0].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[0].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[0].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[1].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[1].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[1].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[2].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[2].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[2].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[3].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[3].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[3].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[4].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[4].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[4].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[5].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[5].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[5].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[6].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[6].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[6].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[7].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[7].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[7].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[8].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[8].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[8].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[9].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[9].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[9].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[10].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[10].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[10].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[11].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[11].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[11].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[12].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[12].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[12].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[13].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[13].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[13].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[14].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[14].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[14].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[15].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[15].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[15].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[16].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[16].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[16].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[17].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[17].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[17].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[18].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[18].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[18].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	firInfo1[19].delayLen = FIR_ACC_TAP_LENGTH-1;
	firInfo1[19].windowLen = FIR_ACC_AUDIO_BLOCK_SIZE;
	firInfo1[19].inputBuffLen = FIR_ACC_AUDIO_BLOCK_SIZE+FIR_ACC_TAP_LENGTH-1u;

	// set up fir accel
	fir_accel_init(firAccBuff1, firAccTcbs, firInfo1, firAccConfig1);
	//	fir_enable_it();
}

void fir_accelerator_run(ADI_FIR_ACC_CONFIG *pFirAccConfig, ADI_FIR_ACC_IN_DATA *pFirInputData)
{
	// run fir accel
	fira_add_input_data(pFirAccConfig, pFirInputData);
	fir_enable(pFirAccConfig);
	fir_wait_till_done();
	fir_accel_reset(pFirAccConfig);
}

