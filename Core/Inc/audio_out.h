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

#define AUDIO_GPIO GPIOA
#define AUDIO_SPI_CS_PIN GPIO_PIN_4

SPI_TypeDef* audio_spi;

uint16_t output_volume;

void init_audio_out(SPI_HandleTypeDef* hspi, TIM_HandleTypeDef* htim);
void dma_volume_out();
void update_volume();


#endif /* INC_AUDIO_OUT_H_ */
