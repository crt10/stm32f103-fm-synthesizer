/*
 * audio_out.c
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#include "audio_out.h"

void init_audio_out(SPI_HandleTypeDef* hspi, TIM_HandleTypeDef *htim) {
	audio_spi = hspi->Instance;
	audio_spi->CR1 |= SPI_CR1_SPE;					//enable SPI
	HAL_TIM_Base_Start_IT(htim);
	output_volume = 0x3800;
}

void update_volume() {
	AUDIO_GPIO->BSRR = (AUDIO_SPI_CS_PIN << 16);	//pull CS pin low
	audio_spi->DR = output_volume;					//write out data to SPI (assume SPI is not busy)
	output_volume = synth_sample();
	output_volume |= 0b0011000000000000;			//MCP4921 DAC: bit 12 = on/off, bit 13 = gain 1x/2x
	AUDIO_GPIO->BSRR = AUDIO_SPI_CS_PIN;			//pull CS pin high
}
