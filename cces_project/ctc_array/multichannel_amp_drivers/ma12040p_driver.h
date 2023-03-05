/**
 * @file    ma12040p_driver.h
 *
 * @brief   Description
 *  
 * @par
 * Created On: 02/07/2022
 * Created By: G. Howell
 */

#ifndef __MA12040P_DRIVER_H__
#define __MA12040P_DRIVER_H__

#ifdef __cplusplus
extern "C" {
#endif

/*------------------- INCLUDES -----------------------------------*/

#include "ma12040p.h"
#include "drivers/bm_twi_driver/bm_twi.h"

/*------------------- MACROS AND DEFINES -------------------------*/
/*------------------- TYPEDEFS -----------------------------------*/

typedef enum
{
	MA12040P_SUCCESS,
	MA12040P_ERROR
} MA12040P_RESULT;

/*------------------- EXPORTED VARIABLES -------------------------*/
/*------------------- GLOBAL FUNCTION PROTOTYPES -----------------*/

MA12040P_RESULT ma12040p_write_reg(BM_TWI *twi, uint8_t reg_addr, uint8_t reg_value);
MA12040P_RESULT ma12040p_read_reg(BM_TWI *twi, uint8_t reg_addr, uint8_t *reg_value);

#ifdef __cplusplus
}
#endif

#endif /* __MA12040P_DRIVER_H__ */
