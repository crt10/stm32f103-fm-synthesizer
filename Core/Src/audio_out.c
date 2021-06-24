/*
 * audio_out.c
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#include "audio_out.h"

void init_audio_out(SPI_HandleTypeDef* hspi, TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_Start_IT(htim);
}

void update_volume(SPI_HandleTypeDef* hspi) {
	uint16_t output_volume = synth_sample();
	if (output_volume > 0x0FFF) {				//max possible volume with 12-bit dac
		output_volume = 0x0FFF;
	}
	output_volume <<= 2;
	output_volume |= 0b0011000000000000;		//MCP4921 DAC: bit 12 = on/off, bit 13 = gain 1x/2x
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_SPI_Transmit(hspi, (uint8_t*)&output_volume, 1, 0xFF);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
}
