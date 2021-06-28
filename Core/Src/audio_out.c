/*
 * audio_out.c
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#include "audio_out.h"

void init_audio_out(SPI_HandleTypeDef* hspi, TIM_HandleTypeDef *htim) {
	SPI1->CR1 |= SPI_CR1_SPE;
	HAL_TIM_Base_Start_IT(htim);
	output_volume = 0x3800;
}

void update_volume() {
	GPIOA->BSRR = GPIO_BSRR_BR4;					//pull CS pin low
	SPI1->DR = output_volume;						//write out data to SPI (assume SPI is not busy)
	output_volume = synth_sample();
	if (output_volume > 0x0FFF) {					//max possible volume with 12-bit dac
		output_volume = 0x0FFF;
	}
	output_volume |= 0b0011000000000000;			//MCP4921 DAC: bit 12 = on/off, bit 13 = gain 1x/2x
	GPIOA->BSRR = GPIO_BSRR_BS4;					//pull CS pin high
}
