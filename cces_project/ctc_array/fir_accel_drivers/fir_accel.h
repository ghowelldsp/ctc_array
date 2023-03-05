/**
 * @file    fir_accel.h
 *
 * @brief   Description
 *  
 * @par
 * Created On: 28/09/22
 * Created By: G. Howell
 */

#ifndef __FIR_ACCEL_H__
#define __FIR_ACCEL_H__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------- INCLUDES -----------------------------------*/

#include <stdint.h>

/*------------------- MACROS AND DEFINES -------------------------*/

#define FIR_CTR1_CH_VAL 					(FIR_ACC_NUM_CHANNELS-1)

#define FIR_POS_FIRCTL2_TAPLEN         		(0u)
#define FIR_POS_FIRCTL2_WINDOW           	(14u)
#define FIR_POS_FIRCTL2_FIR_RATIO       	(25u)
#define FIR_POS_FIRCTL2_FIR_SRCEN        	(29u)
#define FIR_POS_FIRCTL2_FIR_UPSAMP       	(30u)

#define FIR_POS_FIRCTL1_EN         			(0u)
#define FIR_POS_FIRCTL1_CH         			(1u)
#define FIR_POS_FIRCTL1_DMAEN      			(8u)
#define FIR_POS_FIRCTL1_CAI        			(9u)
#define FIR_POS_FIRCTL1_CCINTR     			(11u)
#define FIR_POS_FIRCTL1_FXD        			(12u)
#define FIR_POS_FIRCTL1_TC        			(13u)
#define FIR_POS_FIRCTL1_RND        			(14u)

#define FIR_POS_FIRDMASTAT_DMAWDONE       	(5u)
#define FIR_POS_FIRDMASTAT_DMAACDONE      	(6u)

#define FIR_FIRCTL1_EN_DISABLE            	(0ul << FIR_POS_FIRCTL1_EN)
#define FIR_FIRCTL1_EN_ENABLE            	(1ul << FIR_POS_FIRCTL1_EN)
#define FIR_FIRCTL1_DMAEN_ENABLE         	(1ul << FIR_POS_FIRCTL1_DMAEN)

/*------------------- TYPEDEFS -----------------------------------*/

typedef enum
{
    ADI_FIR_ACC_RESULT_SUCCESS,                /*!< API call was successful */
    ADI_FIR_ACC_RESULT_FAILED,                 /*!< API call failed */
} ADI_FIR_ACC_RESULT;

typedef struct AdiFirAccBuffers
{
	void*          	pCoeffBuff;
	void*          	pOutputBuff;
	void*          	pInputBuff;

} ADI_FIR_ACC_BUFF;

typedef struct FirAInfo
{
	uint32_t 		windowLen;
	uint32_t 		delayLen;
	uint32_t       	inputBuffLen;

} FIRA_INFO;

typedef struct AdiFirAccInputData
{
	void*          pInputData;

} ADI_FIR_ACC_IN_DATA;

typedef struct AdiFirAccTCB
{
    /* Chain Pointer */
    void*          pNextTCB;

    /* Coefficient Buffer */
    uint32_t       nCoeffCount;
    int32_t        nCoeffModify;
    void*          pCoeffIndex;

    /* Output Circular Buffer */
    void*          pOutputBase;
    uint32_t       nOutputCount;
    int32_t        nOutputModify;
    void* volatile pOutputIndex;

    /* Input Circular Buffer */
    void*          pInputBase;
    uint32_t       nInputCount;
    int32_t        nInputModify;
    void* volatile pInputIndex;

    /* Registers */
    uint32_t       nFirControl2;
} ADI_FIR_ACC_TCB;

typedef struct AdiFirAccConfig
{
	FIRA_INFO			*pInfo;
	ADI_FIR_ACC_BUFF 	*pBuffers;
	ADI_FIR_ACC_TCB 	*pTCB;
} ADI_FIR_ACC_CONFIG;

typedef struct _AdiFirAccItHandler
{
    uint32_t iid;
    void *pFirHandle;
} ADI_FIR_ACC_HANDLER;

/*------------------- EXPORTED VARIABLES -------------------------*/
/*------------------- GLOBAL FUNCTION PROTOTYPES -----------------*/

int fir_test(void);

ADI_FIR_ACC_RESULT fir_accel_init(ADI_FIR_ACC_BUFF *pBuffers,
								  ADI_FIR_ACC_TCB *pTCB,
								  FIRA_INFO *pFirInfo1,
								  ADI_FIR_ACC_CONFIG *pFirAccConfig);
ADI_FIR_ACC_RESULT fira_add_input_data(ADI_FIR_ACC_CONFIG *pFirAccConfig,
									   ADI_FIR_ACC_IN_DATA *pFirInputData);
ADI_FIR_ACC_RESULT fir_enable_it(void);
ADI_FIR_ACC_RESULT fir_enable(ADI_FIR_ACC_CONFIG *pFirAccConfig);
ADI_FIR_ACC_RESULT fir_disable(void);
ADI_FIR_ACC_RESULT fir_accel_reset(ADI_FIR_ACC_CONFIG *pFirAccConfig);
ADI_FIR_ACC_RESULT fir_wait_till_done(void);

#ifdef __cplusplus
}
#endif

#endif /* __FIR_ACCEL_H__ */
