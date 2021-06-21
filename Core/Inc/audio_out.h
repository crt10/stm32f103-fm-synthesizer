/*
 * audio_out.h
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#ifndef INC_AUDIO_OUT_H_
#define INC_AUDIO_OUT_H_
#include "stm32f1xx_hal.h"
#include "synth.h"

void init_audio_out(TIM_HandleTypeDef *htim);
void update_volume(TIM_HandleTypeDef *htim);


#endif /* INC_AUDIO_OUT_H_ */
