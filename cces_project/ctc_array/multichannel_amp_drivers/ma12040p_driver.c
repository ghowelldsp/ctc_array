/**
 * @file    ma12040p_driver.h
 *
 * @brief
 *  
 * @par
 * Created On: 02/07/2022
 * Created By: G. Howell
 */

/*------------------- INCLUDES -----------------------------------*/

#include "ma12040p_driver.h"

/*------------------- EXTERN VARIABLES ---------------------------*/
/*------------------- PRIVATE MACROS AND DEFINES -----------------*/
/*------------------- PRIVATE TYPEDEFS ---------------------------*/
/*------------------- STATIC VARIABLES ---------------------------*/
/*------------------- GLOBAL VARIABLES ---------------------------*/
/*------------------- STATIC FUNCTION PROTOTYPES -----------------*/
/*------------------- STATIC FUNCTIONS ---------------------------*/
/*------------------- GLOBAL FUNCTIONS ---------------------------*/

/**
 * @brief Writes a register value to the MA12040P
 *
 * @param   ma12040p_config  MA12040P configuration structure
 * @param   reg_addr  register address
 * @param   reg_value  register value to write
 *
 * @return  MA12040P status
 */
MA12040P_RESULT ma12040p_write_reg(BM_TWI *twi, uint8_t reg_addr, uint8_t reg_value)
{
	uint8_t data[2];
	data[0] = reg_addr;
	data[1] = reg_value;

	if (twi_write_block(twi, data, 2) != TWI_SIMPLE_SUCCESS)
	{
		return MA12040P_ERROR;
	}

	return MA12040P_SUCCESS;
}

/**
 * @brief Reads a register value from the MA12040P
 *
 * @param   ma12040p_config  MA12040P configuration structure
 *
 * @return  MA12040P status
 */
MA12040P_RESULT ma12040p_read_reg(BM_TWI *twi, uint8_t reg_addr, uint8_t *reg_value)
{
	if (twi_write_block(twi, &reg_addr, 1) != TWI_SIMPLE_SUCCESS)
	{
		return MA12040P_ERROR;
	}
	if (twi_read(twi, reg_value) != TWI_SIMPLE_SUCCESS)
	{
		return MA12040P_ERROR;
	}

	return MA12040P_SUCCESS;
}
