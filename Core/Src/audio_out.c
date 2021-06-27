/*
 * audio_out.c
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#include "audio_out.h"

void init_audio_out(SPI_HandleTypeDef* hspi, TIM_HandleTypeDef *htim) {
	DMA1_Channel3->CPAR = (uint32_t)&SPI1->DR;
	DMA1_Channel3->CMAR = (uint32_t)&output_volume;
	DMA1_Channel3->CCR = DMA_CCR_PSIZE_0 | DMA_CCR_MSIZE_0 | DMA_CCR_DIR | DMA_CCR_TCIE;
	SPI1->CR1 |= SPI_CR1_SPE;
	HAL_TIM_Base_Start_IT(htim);
	output_volume = 0x3800;
}

void dma_volume_out() {
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);	//pull CS pin low
	DMA1_Channel3->CNDTR = (uint32_t)1;						//write 1 to DMA size
	DMA1_Channel3->CCR |= DMA_CCR_EN;						//enable DMA channel 3
	SPI1->CR2 |= SPI_CR2_TXDMAEN;							//enable SPI DMA transfer
}

void update_volume() {
	dma_volume_out();
	output_volume = synth_sample();
	if (output_volume > 0x0FFF) {				//max possible volume with 12-bit dac
		output_volume = 0x0FFF;
	}
	output_volume |= 0b0011000000000000;		//MCP4921 DAC: bit 12 = on/off, bit 13 = gain 1x/2x
	//HAL_SPI_Transmit(hspi, (uint8_t*)&output_volume, 1, 0xFF);
}
