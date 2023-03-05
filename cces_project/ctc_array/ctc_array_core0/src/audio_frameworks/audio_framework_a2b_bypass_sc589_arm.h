/*
 * Copyright (c) 2018 Analog Devices, Inc.  All rights reserved.
 *
 * See .c file for more information.
 */
#ifndef _AUDIO_FRAMEWORK_A2B_BYPASS_SC589_ARM_H
#define _AUDIO_FRAMEWORK_A2B_BYPASS_SC589_ARM_H

#include "drivers/bm_gpio_driver/bm_gpio.h" // Driver for GPIO functionality

#if defined(SAM_AUDIOPROJ_FIN_BOARD_PRESENT) && SAM_AUDIOPROJ_FIN_BOARD_PRESENT

// LEDs by PCB designator
#define    GPIO_AUDIOPROJ_FIN_LED_3       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 0)
#define    GPIO_AUDIOPROJ_FIN_LED_4       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 1)
#define    GPIO_AUDIOPROJ_FIN_LED_5       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 2)
#define    GPIO_AUDIOPROJ_FIN_LED_6       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 3)
#define    GPIO_AUDIOPROJ_FIN_LED_7       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 4)
#define    GPIO_AUDIOPROJ_FIN_LED_8       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 5)
#define    GPIO_AUDIOPROJ_FIN_LED_9       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 6)
#define    GPIO_AUDIOPROJ_FIN_LED_10      BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 7)

// LEDs beneath the Switches / push buttons
#define    GPIO_AUDIOPROJ_FIN_LED_SW1     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 3)
#define    GPIO_AUDIOPROJ_FIN_LED_SW2     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 2)
#define    GPIO_AUDIOPROJ_FIN_LED_SW3     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 1)
#define    GPIO_AUDIOPROJ_FIN_LED_SW4     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 0)

// Four VU (volume unit) LEDs in center of Audio Project Fin
#define    GPIO_AUDIOPROJ_FIN_LED_VU1     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 4)
#define    GPIO_AUDIOPROJ_FIN_LED_VU2     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 5)
#define    GPIO_AUDIOPROJ_FIN_LED_VU3     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 6)
#define    GPIO_AUDIOPROJ_FIN_LED_VU4     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 7)

// External LED 100mil header (P15-P12)
#define    GPIO_AUDIOPROJ_FIN_EXT_LED_1   BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_D, 4)
#define    GPIO_AUDIOPROJ_FIN_EXT_LED_2   BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_D, 5)
#define    GPIO_AUDIOPROJ_FIN_EXT_LED_3   BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_D, 6)
#define    GPIO_AUDIOPROJ_FIN_EXT_LED_4   BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_D, 7)

// Switches / push buttons
#define    GPIO_AUDIOPROJ_FIN_SW_1        BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 8)
#define    GPIO_AUDIOPROJ_FIN_SW_2        BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 9)
#define    GPIO_AUDIOPROJ_FIN_SW_3        BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 10)
#define    GPIO_AUDIOPROJ_FIN_SW_4        BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 12)

#define    GPIO_AUDIOPROJ_FIN_PB_1        BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 8)
#define    GPIO_AUDIOPROJ_FIN_PB_2        BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 9)
#define    GPIO_AUDIOPROJ_FIN_PB_3        BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 10)
#define    GPIO_AUDIOPROJ_FIN_PB_4        BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_E, 12)

#endif // __AUDIOPROJ_DAUGHTER_BOARD__

// LEDs on SHARC Audio Module
#define    GPIO_SHARC_SAM_LED10     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_D, 1)
#define    GPIO_SHARC_SAM_LED11     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_D, 2)
#define    GPIO_SHARC_SAM_LED12     BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_D, 3)

// Push buttons (PBs) on SHARC Audio Module
#define    GPIO_SHARC_SAM_PB1       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_F, 0)
#define    GPIO_SHARC_SAM_PB2       BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_F, 1)

// TWI mux on v1.4 SAM boards and later
#define    GPIO_SAM_TWI_MUX         BM_GPIO_PORTPIN_MAKE(ADI_GPIO_PORT_B, 8)

void audioframework_initialize(void);
void audioframework_background_loop(void);
void audioframework_wait_for_sharcs(void);

#endif    //_AUDIO_FRAMEWORK_A2B_BYPASS_SC589_ARM_H