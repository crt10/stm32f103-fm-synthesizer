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

#define BUFFER_SIZE 128

void init_audio_out(SPI_HandleTypeDef* hspi, TIM_HandleTypeDef* htim);
void update_volume(SPI_HandleTypeDef* hspi);


#endif /* INC_AUDIO_OUT_H_ */
