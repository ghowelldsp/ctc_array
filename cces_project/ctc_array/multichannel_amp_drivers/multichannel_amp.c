/**
 * @file    multichannel_amp.c
 *
 * @brief
 *  
 * @par
 * Created On: 10/07/2022
 * Created By: G. Howell
 */

/*------------------- INCLUDES -----------------------------------*/

#include "multichannel_amp.h"

#include "ma12040p_driver.h"

#include <sruSC589.h>
#include "drivers/bm_twi_driver/bm_twi.h"
#include "drivers/bm_event_logging_driver/bm_event_logging.h"

/*------------------- EXTERN VARIABLES ---------------------------*/
/*------------------- PRIVATE MACROS AND DEFINES -----------------*/
/*------------------- PRIVATE TYPEDEFS ---------------------------*/
/*------------------- STATIC VARIABLES ---------------------------*/
/*------------------- GLOBAL VARIABLES ---------------------------*/

BM_TWI twi;
uint16_t ma12040pDevMap;

/*------------------- STATIC FUNCTION PROTOTYPES -----------------*/
/*------------------- STATIC FUNCTIONS ---------------------------*/

static MCAMP_STATUS i2c_init(BM_TWI *twi, BM_TWI_PERIPHERAL_NUMBER deviceNum, uint8_t devAddr);
static MCAMP_STATUS ma12040p_devices_on_bus(BM_TWI *twi, uint8_t i2cMuxBus);

/*------------------- GLOBAL FUNCTIONS ---------------------------*/

/**
 * @brief Initialises the MA12040P I2C connection
 *
 * @param   ma12040p_config  MA12040P configuration structure
 *
 * @return  MA12040P status
 */
MCAMP_STATUS McAmp_Initialise(void)
{
	char message[128];

	// set the I2C port with the device address of the I2C mux
	if (i2c_init(&twi, TWI2, MCAMP_I2C_MUX_DEV_ADDR) != MCAMP_SUCCESS)
	{
		log_event(EVENT_INFO, "   McAmp: I2C Setup Failure");
	} else
	{
		log_event(EVENT_INFO, "   McAmp: I2C Setup Success");
	}
	sprintf(message, "   McAmp: I2C Config, Bus: %d, Addr: 0x%.2x, Speed: %.3f KHz",
			TWI2, MCAMP_I2C_MUX_DEV_ADDR, (double)MCAMP_I2C_SPEED / 1000.0);
	log_event(EVENT_INFO, message);

	return MCAMP_SUCCESS;
}

/**
 * @brief Checks the devices on the I2C bus
 *
 * @param   ma12040p_config  MA12040P configuration structure
 *
 * @return  MA12040P status
 */
MCAMP_STATUS McAmp_Device_Check(void)
{
	char message[128];

	// check i2c mux busses for ma12040p devices present
	uint8_t busIdx;
	for (busIdx = 0; busIdx < 2; busIdx++)
	{
		if (ma12040p_devices_on_bus(&twi, busIdx) != MCAMP_SUCCESS)
		{
			sprintf(message, "   McAmp: Error checking devices on bus %d", busIdx);
			log_event(EVENT_INFO, message);
		}
	}

	if (ma12040pDevMap==0)
	{
		log_event(EVENT_INFO, "   McAmp: No MA12040P devices connected.");
	}

	return MCAMP_SUCCESS;
}

/**
 * @brief Initialises the MA12040P I2C connection
 *
 * @param   ma12040p_config  MA12040P configuration structure
 *
 * @return  MA12040P status
 */
MCAMP_STATUS McAmp_Enable(void)
{
	SRU2(LOW, DAI1_PB14_I);
	log_event(EVENT_INFO, "McAmp: Amp's Enabled");

	return MCAMP_SUCCESS;
}

/**
 * @brief Initialises the I2C connection
 *
 * @param   i2c_config  I2C configuration structure
 *
 * @return  framework status
 */
static MCAMP_STATUS i2c_init(BM_TWI *twi, BM_TWI_PERIPHERAL_NUMBER deviceNum, uint8_t devAddr)
{
	if (twi_initialize(twi, devAddr, TWI_TYPICAL_SCLK0_FREQ, deviceNum) != TWI_SIMPLE_SUCCESS)
	{
		return MCAMP_ERROR;
	}
	twi_set_clock(twi, MCAMP_I2C_SPEED);

	return MCAMP_SUCCESS;
}

/**
 * @brief Checks the I2C bus for MA12040P devices
 *
 * @param   i2cMuxBus  i2c mux bus number, from 0-7
 *
 * @return  MA12040P status
 */
static MCAMP_STATUS ma12040p_devices_on_bus(BM_TWI *twi, uint8_t i2cMuxBus)
{
	char message[128];

	uint8_t i2cMuxBusRegVal = (1<<i2cMuxBus);

	// create function to check ma12040p devices on bus
	if (twi_write_block(twi, &i2cMuxBusRegVal, 1) != TWI_SIMPLE_SUCCESS)
	{
		return MCAMP_ERROR;
	}
	uint8_t reg_val;
	if (twi_read(twi, &reg_val) != TWI_SIMPLE_SUCCESS)
	{
		return MCAMP_ERROR;
	}
	if (reg_val != i2cMuxBusRegVal)
	{
		return MCAMP_ERROR;
	}

	// check if MA12040P devices are detected
	uint8_t devAddrIdx;
	uint8_t regVal = 0;
	uint8_t devMapShift = 8*i2cMuxBus;
	for (devAddrIdx = 0; devAddrIdx<4; devAddrIdx++)
	{
		// set i2c address to that of one of the 4 possible ma12040p addresses
		twi_set_temporary_address(twi, MA12040P_DEVADDR_1+devAddrIdx);

		// read one register to confirm if device present
		if (ma12040p_read_reg(twi, 0x00, &regVal) == MA12040P_SUCCESS)
		{
			if (regVal == 0x3d)
			{
				ma12040pDevMap |= (1<<(devAddrIdx+devMapShift));

				sprintf(message, "   McAmp: MA12040P found on I2C mux bus %d with device address of 0x%.2x", i2cMuxBus, MA12040P_DEVADDR_1+devAddrIdx);
				log_event(EVENT_INFO, message);
			}

			regVal = 0x00;
		}
	}

	twi_restore_address(twi);

	return MCAMP_SUCCESS;
}
