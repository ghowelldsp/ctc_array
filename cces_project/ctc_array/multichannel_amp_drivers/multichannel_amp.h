/**
 * @file    multichannel_amp.h
 *
 * @brief   Description
 *  
 * @par
 * Created On: 02/07/2022
 * Created By: G. Howell
 */

#ifndef __MULTICHANNEL_AMP_H__
#define __MULTICHANNEL_AMP_H__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------- INCLUDES -----------------------------------*/

#include <stdint.h>

/*------------------- MACROS AND DEFINES -------------------------*/

#define MCAMP_I2C_MUX_DEV_ADDR			((uint8_t)0x70)
#define MCAMP_I2C_SPEED					100000


/*------------------- TYPEDEFS -----------------------------------*/

typedef enum
{
	MCAMP_SUCCESS,
	MCAMP_ERROR
} MCAMP_STATUS;

/*------------------- EXPORTED VARIABLES -------------------------*/
/*------------------- GLOBAL FUNCTION PROTOTYPES -----------------*/

MCAMP_STATUS McAmp_Initialise(void);
MCAMP_STATUS McAmp_Device_Check(void);
MCAMP_STATUS McAmp_Enable(void);

#ifdef __cplusplus
}
#endif

#endif /* __MULTICHANNEL_AMP_H__ */
