/*
 * synth.c
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#include "synth.h"

void init_synth() {
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		algo = 0x00;								//default algorithm 0
		op_amp[i] = 0xFF;							//default amplitude 255
		op_mul[i] = 0x01;							//default multiplier 1
		for (uint8_t o = 0; o < MAX_VOICES; o++) {
			op[i][o] = (OPERATOR) {-1, -1, -1, -1};
		}
	}
}

void add_voice(uint8_t note_value) {
	uint8_t voice_index;
	for (voice_index = 0; voice_index < MAX_VOICES; voice_index++) {
		if (op[0][voice_index].note_value == (uint8_t)-1) {
			break;
		}
	}
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
		op[op_index][voice_index].note_value = note_value;							//store note
		uint16_t freq = op_mul[op_index] * note_to_freq(note_value);				//NOTE: overflow is not accounted for
		op[op_index][voice_index].freq = freq;										//store frequency
		op[op_index][voice_index].delta = calculate_delta(freq);					//store delta
		op[op_index][voice_index].phase = 0;										//reset the phase (wave table index)
	}
}

void delete_voice(uint8_t note_value) {
	uint8_t voice_index;
	for (voice_index = 0; voice_index < MAX_VOICES; voice_index++) {
		if (op[0][voice_index].note_value == note_value) {
			break;
		}
	}
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
		op[op_index][voice_index].note_value = -1;
	}
}

uint16_t synth_sample() {
	uint16_t output_volume = 0;
	for (uint8_t voice_index = 0; voice_index < MAX_VOICES; voice_index++) {	//loop through all voices
		if (op[0][voice_index].note_value != (uint8_t)-1) {							//if the voice is active
			for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {			//loop through all operators of voice
				OPERATOR* operator = &op[op_index][voice_index];							//create pointer to active voice
				operator->phase = update_phase(operator->delta, operator->phase);			//update phase of active voice
				output_volume += get_sample(operator->phase >> 8);				//add current sample of wave based on phase
			}
		}
	}
	return output_volume;
}
