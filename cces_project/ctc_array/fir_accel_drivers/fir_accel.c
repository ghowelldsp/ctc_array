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
 * @brief 	Initalises the fir accelerator
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fir_accel_init(ADI_FIR_ACC_BUFF *pBuffers,
								  ADI_FIR_ACC_TCB *pTCB,
								  FIRA_INFO *pFirInfo1,
								  ADI_FIR_ACC_CONFIG *pFirAccConfig)
{
	// write control 1 data TODO - this writes multiple times
	*pREG_FIR0_CTL1 = firCtrl1;

	uint8_t chIdx;

	for (chIdx=0; chIdx<FIR_ACC_NUM_CHANNELS; chIdx++)
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
		pTCB[chIdx].pCoeffIndex = (void*)TranslateAddr((uint32_t)pBuffers[chIdx].pCoeffBuff);
		pTCB[chIdx].pOutputBase = (void*)TranslateAddr((uint32_t)pBuffers[chIdx].pOutputBuff);
		pTCB[chIdx].pOutputIndex = (void*)TranslateAddr((uint32_t)pBuffers[chIdx].pOutputBuff);
		pTCB[chIdx].pInputBase = (void*)TranslateAddr((uint32_t)pBuffers[chIdx].pInputBuff);
		pTCB[chIdx].pInputIndex = (void*)TranslateAddr((uint32_t)pBuffers[chIdx].pInputBuff);

		// add buffers and tcb to config
		pFirAccConfig[chIdx].pInfo = &pFirInfo1[chIdx];
		pFirAccConfig[chIdx].pBuffers = &pBuffers[chIdx];
		pFirAccConfig[chIdx].pTCB = &pTCB[chIdx];

	};

	return ADI_FIR_ACC_RESULT_SUCCESS;
}


#include "drivers/bm_event_logging_driver/bm_event_logging.h"

/**
 * @brief 	Enables the fir accelerator interrupt
 *
 * @param
 *
 * @return  Fir accel result
 */
ADI_FIR_ACC_RESULT fira_add_input_data(ADI_FIR_ACC_CONFIG *pFirAccConfig, ADI_FIR_ACC_IN_DATA *pFirInputData)
{
	uint32_t startIdx, endIdx;
	uint32_t diff = 0;

	float tempVal;

	for (int i=0; i<FIR_ACC_NUM_CHANNELS; i++)
	{
		// determine start address of x[n] and end index of buffer
		startIdx = ((uint32_t)pFirAccConfig[i].pTCB->pInputIndex) + pFirAccConfig[i].pInfo->delayLen;
		endIdx = ((uint32_t)pFirAccConfig[i].pTCB->pInputBase) + pFirAccConfig[i].pInfo->inputBuffLen;

		// check if start index is passed end of buffer, modify to start of buffer if so
		if (startIdx >= endIdx)
		{
			diff = startIdx - endIdx;
			startIdx = ((uint32_t)pFirAccConfig[i].pTCB->pInputBase) + diff;
		}
		else
		{
			diff = startIdx - (uint32_t)pFirAccConfig[i].pTCB->pInputBase;
		}

		float *pXnIdx = (float*)pFirAccConfig[i].pBuffers->pInputBuff + diff;
		float *pInputBase = (float*)pFirAccConfig[i].pBuffers->pInputBuff;
		float *pInputData = (float*)pFirInputData[0].pInputData;

		// determine if x[n] will run over end index limit
		if (startIdx+FIR_ACC_AUDIO_BLOCK_SIZE > endIdx)
		{
			diff = endIdx - startIdx;

			// add input data to input buffers
			for (int idx = 0; idx < FIR_ACC_AUDIO_BLOCK_SIZE; idx++)
			{
				if (idx>=diff)
				{
					pInputBase[idx-diff] = pInputData[idx];
				}
				else
				{

					pXnIdx[idx] = pInputData[idx];
				}
			}
		}
		else
		{
			// add input data to input buffers
			for (int idx = 0; idx < FIR_ACC_AUDIO_BLOCK_SIZE; idx++)
			{
				pXnIdx[idx] = pInputData[idx];
			}
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
ADI_FIR_ACC_RESULT fir_enable(ADI_FIR_ACC_CONFIG *pFirAccConfig)
{
	*pREG_FIR0_CHNPTR = TranslateAddr((uint32_t)&pFirAccConfig[0].pTCB->nFirControl2);

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
ADI_FIR_ACC_RESULT fir_accel_reset(ADI_FIR_ACC_CONFIG *pFirAccConfig)
{
	// disable the fir accelerator
	fir_disable();

	// reset the input buffer
	uint8_t chIdx;

//	for (chIdx=0; chIdx<FIR_ACC_NUM_CHANNELS; chIdx++)
//	{
//		// set up next tcb
//		pFirAccConfig[chIdx].pTCB->pInputIndex = (void*)TranslateAddr((uint32_t)pFirAccConfig[chIdx].pBuffers->pInputBuff);
//	};

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
