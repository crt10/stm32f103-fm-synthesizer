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
		op_amp[i] = MAX_VOLUME;						//default amplitude 128
		op_ratio[i] = 0x10;							//default multiplier 1.0 (4 MSB integer, 4 LSB decimal)
		op_attack[i] = 96;//ENV_MAX_RATE;				//default attack 128
		op_decay[i] = 96;//ENV_MAX_RATE;					//default decay 128
		op_sustain[i] = 64;//MAX_VOLUME;					//default sustain 128
		op_release[i] = 96;//ENV_MAX_RATE;				//default release 128
		op_attack_inc[i] = calculate_env_inc(op_attack[i]);//ENV_MAX_RATE << 8;
		op_decay_inc[i] = calculate_env_inc(op_decay[i]);//ENV_MAX_RATE << 8;
		op_release_inc[i] = calculate_env_inc(op_release[i]);//ENV_MAX_RATE << 8;
		for (uint8_t o = 0; o < MAX_VOICES; o++) {
			op[i][o] = (OPERATOR) {-1, -1, MAX_VOLUME, -1, -1, SILENT, MAX_VOLUME << 8};
		}
	}
	env_period = ENV_MAX_PERIOD;
}

void add_voice(uint8_t note_value) {
	uint8_t voice_index;
	for (voice_index = 0; voice_index < MAX_VOICES; voice_index++) {
		if (op[0][voice_index].note_value == (uint8_t)-1) {
			break;
		}
	}
	if (voice_index == MAX_VOICES) {
		return;
	}
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
		op[op_index][voice_index].note_value = note_value;										//store note
		op[op_index][voice_index].phase = 0;													//reset the phase (wave table index)
		op[op_index][voice_index].freq = (op_ratio[op_index] * note_to_freq(note_value)) >> 4;	//calculate and store frequency (>> 4 for integer)
		if (op_index != 0)	{
			op[op_index][voice_index].delta = (op_ratio[op_index] * op[0][voice_index].delta) >> 4;	//multiply delta based off of ratio of carrier
		}
		else {
			op[op_index][voice_index].delta = calculate_delta(op[0][voice_index].freq);				//calculate and store delta
		}

		op[op_index][voice_index].volume = 0x00;
		op[op_index][voice_index].env_amp = op_attack_inc[op_index];							//start envelope amp with attack inc
		if (op_attack_inc[op_index] == MAX_VOLUME << 8) {										//if attack rate is max
			op[op_index][voice_index].adsr_state = DECAY;											//start on decay state
		}
		else {
			op[op_index][voice_index].adsr_state = ATTACK;											//else, start on attack state
		}
	}
}

void release_voice(uint8_t note_value) {
	uint8_t voice_index;
	for (voice_index = 0; voice_index < MAX_VOICES; voice_index++) {
		if (op[0][voice_index].note_value == note_value) {
			break;
		}
	}
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
		op[op_index][voice_index].adsr_state = RELEASE;
	}
}

int16_t synth_sample() {
	int16_t output_volume = 0x0800;											//default output volume 2048
	if (--env_period == 0x00) {												//decrement and check envelope period
		env_period = ENV_MAX_PERIOD;											//reset envelope period
		for (uint8_t voice_index = 0; voice_index < MAX_VOICES; voice_index++) {//loop through all voices
			if (op[0][voice_index].note_value != (uint8_t)-1) {						//if the voice is active
				cycle_envelope(voice_index);
				output_volume += modulate(voice_index);
			}
		}
	}
	else {
		for (uint8_t voice_index = 0; voice_index < MAX_VOICES; voice_index++) {//loop through all voices
			if (op[0][voice_index].note_value != (uint8_t)-1) {						//if the voice is active
				output_volume += modulate(voice_index);
			}
		}
	}
	return output_volume;
}
