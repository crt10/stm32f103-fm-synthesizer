/*
 * audio_out.c
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#include "audio_out.h"

void init_audio_out(TIM_HandleTypeDef *htim) {
	HAL_TIM_Base_Start_IT(htim);
	HAL_TIM_PWM_Start_IT(htim, TIM_CHANNEL_1);
}

void update_volume(TIM_HandleTypeDef *htim) {
	uint16_t output_volume = synth_sample();
	if (output_volume >= (uint16_t)1023) {
		__NOP();
	}
	__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, output_volume);
}
