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
		op_detune[i] = 0x00;						//default detune 0
		op_pitch_bend[i] = 0x00;					//default pitch bend 0
		op_attack[i] = ENV_MAX_RATE;				//default attack 128
		op_decay[i] = ENV_MAX_RATE;					//default decay 128
		op_sustain[i] = MAX_VOLUME;					//default sustain 128
		op_release[i] = ENV_MAX_RATE;				//default release 128
		op_attack_inc[i] = calculate_env_inc(op_attack[i]);
		op_decay_inc[i] = calculate_env_inc(op_decay[i]);
		op_release_inc[i] = calculate_env_inc(op_release[i]);
		for (uint8_t o = 0; o < MAX_VOICES; o++) {
			op[i][o] = (OPERATOR) {-1, -1, MAX_VOLUME, -1, -1, SILENT, MAX_VOLUME << 8};
		}
	}
	env_period = ENV_MAX_PERIOD;
}

void clear_voices() {
	for (uint8_t i = 0; i < MAX_VOICES; i++) {
		op[0][i].note_value = -1;
	}
}

void add_voice(uint8_t note_value) {
	uint8_t voice_index, released_voice = (uint8_t)-1;
	for (voice_index = 0; voice_index < MAX_VOICES; voice_index++) {
		if (op[0][voice_index].note_value == (uint8_t)-1) {										//find unused voice
			break;
		}
	}
	for (uint8_t i = 0; i < MAX_VOICES; i++) {
		if (op[0][i].note_value == note_value) {												//if the same note is active but released, reactivate it
			voice_index = i;
			break;
		}
		if (op[0][i].adsr_state == RELEASE) {											//keep track of any note in released state
			released_voice = i;
		}
	}
	if (voice_index == MAX_VOICES) {
		if (released_voice == (uint8_t)-1) {
			return;
		}
		else {
			voice_index = released_voice;
		}
	}
	uint16_t carrier_freq = note_to_freq(note_value);
	uint16_t carrier_delta = calculate_delta(carrier_freq);
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
		op[op_index][voice_index].note_value = note_value;										//store note
		op[op_index][voice_index].phase = 0;													//reset the phase (wave table index)
		op[op_index][voice_index].freq = (op_ratio[op_index] * carrier_freq) >> 4;				//calculate and store frequency (>> 4 for integer)
		op[op_index][voice_index].delta = (op_ratio[op_index] * carrier_delta) >> 4;			//multiply delta based off of ratio of carrier
		op[op_index][voice_index].delta += (int16_t)op_detune[op_index];						//offset with detune param
		op[op_index][voice_index].volume = 0x00;												//reset volume to 0
		op[op_index][voice_index].env_amp = op_attack_inc[op_index];							//start envelope amp with attack increment
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
	if (voice_index == MAX_VOICES) {
		return;
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
