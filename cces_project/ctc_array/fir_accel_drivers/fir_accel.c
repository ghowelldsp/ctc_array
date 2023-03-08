/**
 * @file    fir_accel.h
 *
 * @brief
 *  
 * @par
 * Created On: 28/09/22
 * Created By: G. Howell
 */

/*------------------- INCLUDES -----------------------------------*/

#include "fir_accel.h"
#include "fir_accel_tcb.h"
#include <sys/platform.h>
#include <services/int/adi_int.h>

/*------------------- EXTERN VARIABLES ---------------------------*/
/*------------------- PRIVATE MACROS AND DEFINES -----------------*/
/*------------------- PRIVATE TYPEDEFS ---------------------------*/
/*------------------- STATIC VARIABLES ---------------------------*/
/*------------------- GLOBAL VARIABLES ---------------------------*/
/*------------------- STATIC FUNCTION PROTOTYPES -----------------*/
/*------------------- STATIC FUNCTIONS ---------------------------*/

static void InterruptHandler (uint32_t iid, void* pParam);

static uint32_t TranslateAddr (uint32_t  MemAddr);

/*------------------- GLOBAL FUNCTIONS ---------------------------*/

/**
 * @brief 	Initalises the fir accelerator for a single configuration
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fir_accel_init(FIRA_CONFIG *pFirAccConfig,
								  FIRA_TCB *pTCB,
								  FIRA_BUFFERS *pBuffers,
								  FIRA_INFO *pFirInfo)
{
	// write control 1 data TODO - this writes multiple times
	*pREG_FIR0_CTL1 = firCtrl1;

	/*
	 * TCB
	 */
	for (int chIdx=0; chIdx<FIR_ACC_NUM_CHANNELS; chIdx++)
	{
		// load in the default settings
		pTCB[chIdx] = firAccDefaultTcb;

		// set up next tcb
		if (chIdx==(FIR_ACC_NUM_CHANNELS-1))
		{
			pTCB[chIdx].pNextTCB = 0;
		}
		else
		{
			pTCB[chIdx].pNextTCB = (void*)TranslateAddr((uint32_t)&pTCB[chIdx+1].nFirControl2);
		}

		// update base address of registers
		pTCB[chIdx].pCoeffIndex = (void*)TranslateAddr((uint32_t)*(pBuffers->pCoeffBuffers+chIdx));
		pTCB[chIdx].pOutputBase = (void*)TranslateAddr((uint32_t)*(pBuffers->pOutputBuffers+chIdx));
		pTCB[chIdx].pOutputIndex = (void*)TranslateAddr((uint32_t)*(pBuffers->pOutputBuffers+chIdx));
		pTCB[chIdx].pInputBase = (void*)TranslateAddr((uint32_t)pBuffers->pInputBuffer);
		pTCB[chIdx].pInputIndex = (void*)TranslateAddr((uint32_t)pBuffers->pInputBuffer);
	};

	// add buffers and tcb to config
	pFirAccConfig->pInfo = pFirInfo;
	pFirAccConfig->pBuffers = pBuffers;
	pFirAccConfig->pTCB = pTCB;

	return ADI_FIR_ACC_RESULT_SUCCESS;
}

/**
 * @brief 	Enables the fir accelerator interrupt
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fira_add_input_data(FIRA_CONFIG *pFiraConfig,
		 	 	 	 	 	 	 	   float *pFirInputData)
{
	uint32_t startIdx, endIdx;
	uint32_t diff = 0;

	// determine start address of x[n] and end index of buffer
	startIdx = ((uint32_t)pFiraConfig->pTCB[1].pInputIndex) + pFiraConfig->pInfo->delayLen;
	endIdx = ((uint32_t)pFiraConfig->pTCB[1].pInputBase) + pFiraConfig->pInfo->inputBuffLen;

	// check if start index is passed end of buffer, modify to start of buffer if so
	if (startIdx >= endIdx)
	{
		diff = startIdx - endIdx;
		startIdx = ((uint32_t)pFiraConfig->pTCB[0].pInputBase) + diff;
	}
	else
	{
		diff = startIdx - (uint32_t)pFiraConfig->pTCB[1].pInputBase;
	}

	float *pInputXn = pFiraConfig->pBuffers->pInputBuffer + diff;
	float *pInputBase = pFiraConfig->pBuffers->pInputBuffer;

	// determine if x[n] will run over end index limit
	if (startIdx+FIR_ACC_AUDIO_BLOCK_SIZE > endIdx)
	{
		diff = endIdx - startIdx;

		// add input data to input buffers
		for (int idx = 0; idx < FIR_ACC_AUDIO_BLOCK_SIZE; idx++)
		{
			if (idx>=diff)
			{
				pInputBase[idx-diff] = pFirInputData[idx];
			}
			else
			{

				pInputXn[idx] = pFirInputData[idx];
			}
		}
	}
	else
	{
		// add input data to input buffers
		for (int idx = 0; idx < FIR_ACC_AUDIO_BLOCK_SIZE; idx++)
		{
			pInputXn[idx] = pFirInputData[idx];
		}
	}

	return ADI_FIR_ACC_RESULT_SUCCESS;
}

/**
 * @brief 	Enables the fir accelerator interrupt
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fir_enable_it(void)
{
	// set up interrupt callback
	adi_int_InstallHandler((uint32_t)INTR_FIR0_DMA, &InterruptHandler, &firHandler, true);

	// enable interrupt in control1 register
	*pREG_FIR0_CTL1 |= 0x1 << FIR_POS_FIRCTL1_CCINTR;

	return ADI_FIR_ACC_RESULT_SUCCESS;
}

/**
 * @brief 	Enables the fir accelerator
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fir_enable(FIRA_CONFIG *pFirAccConfig)
{
	*pREG_FIR0_CHNPTR = TranslateAddr((uint32_t)&pFirAccConfig->pTCB[0].nFirControl2);

	*pREG_FIR0_CTL1 |= FIR_FIRCTL1_EN_ENABLE | FIR_FIRCTL1_DMAEN_ENABLE;

	return ADI_FIR_ACC_RESULT_SUCCESS;
}

/**
 * @brief 	Disables the fir accelerator
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fir_disable(void)
{
	*pREG_FIR0_CTL1 &= ~(FIR_FIRCTL1_EN_ENABLE | FIR_FIRCTL1_DMAEN_ENABLE);

	return ADI_FIR_ACC_RESULT_SUCCESS;
}

/**
 * @brief 	Resets the fir accelerator
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fir_accel_reset(FIRA_CONFIG *pFirAccConfig)
{
	// disable the fir accelerator
	fir_disable();

	// reset the input buffer
	for (int chIdx=0; chIdx<FIR_ACC_NUM_CHANNELS; chIdx++)
	{
		pFirAccConfig->pTCB[chIdx].pInputIndex = (void*)TranslateAddr((uint32_t)pFirAccConfig->pBuffers->pInputBuffer);
	};

	return ADI_FIR_ACC_RESULT_SUCCESS;
}

/**
 * @brief 	Poles DMA_STAT register for indicator that process is completed
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fir_wait_till_done(void)
{
	uint32_t dmaStat;
	uint32_t dmaDone = 0;

	// continually poles the dma stat register to see if all channels
	// are done indicating that the fir accel has completed
	while (!dmaDone)
	{
		dmaStat = *pREG_FIR0_DMASTAT;

		dmaDone = (dmaStat >> FIR_POS_FIRDMASTAT_DMAACDONE) & 0x1;
	}

	return ADI_FIR_ACC_RESULT_SUCCESS;
}

static uint32_t TranslateAddr (uint32_t  MemAddr)
{
    /* The TCB addresses have been divided by 4. Adjust them so the common TranslateAddr routine works */

#if defined(__ADSPSHARC__) && (__ADSPSHARC__ >= 0x200)
    MemAddr = adi_rtl_internal_to_system_addr(MemAddr, 1);
#endif

    return (MemAddr >> 2u);
}

static void InterruptHandler (uint32_t iid, void* pParam)
{

}
