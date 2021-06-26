/*
 * phase.c
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#include "phase.h"

uint16_t note_to_freq(uint8_t note_value) {
	return NOTE_FREQ_TABLE[note_value];
}

uint16_t calculate_delta(uint16_t freq) {
	uint32_t total_samples = TABLE_SAMPLE_SIZE * freq;		//# of samples to map one second of the target frequency
	total_samples <<= 8;									//shift left 8 bits for fractional bits (8 LSB)
	uint16_t delta = total_samples / SAMPLING_FREQ;			//8 MSB = integer, 8 LSB = fractional. integer represents index in wave table.
	return delta;
}
