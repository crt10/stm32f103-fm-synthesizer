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
		op_ratio[i] = 0x01;							//default multiplier 1
		for (uint8_t o = 0; o < MAX_VOICES; o++) {
			op[i][o] = (OPERATOR) {-1, -1, -1, -1};
		}
	}
}

void add_voice(uint8_t note_value) {
	uint8_t voice_index;
//	for (voice_index = 0; voice_index < MAX_VOICES; voice_index++) {
//		if (op[0][voice_index].note_value == note_value) {
//			return;
//		}
//	}
	for (voice_index = 0; voice_index < MAX_VOICES; voice_index++) {
		if (op[0][voice_index].note_value == (uint8_t)-1) {
			break;
		}
	}
	if (voice_index == MAX_VOICES) {
		return;
	}
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
		op[op_index][voice_index].note_value = note_value;								//store note
		op[op_index][voice_index].freq = op_ratio[op_index] * note_to_freq(note_value);	//store frequency
		if (op_index != 0)	{
			op[op_index][voice_index].delta = op_ratio[op_index] * op[0][voice_index].delta;//multiply delta based off of ratio of carrier
		}
		else {
			op[op_index][voice_index].delta = calculate_delta(op[0][voice_index].freq);						//calculate and store delta
		}
		op[op_index][voice_index].phase = 0;											//reset the phase (wave table index)
	}
}

void delete_voice(uint8_t note_value) {
	uint8_t voice_index;
	for (voice_index = 0; voice_index < MAX_VOICES; voice_index++) {
		if (op[0][voice_index].note_value == note_value) {
			op[0][voice_index].note_value = -1;
		}
	}
//	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
//		op[op_index][voice_index].note_value = -1;
//	}
}

int16_t synth_sample() {
	int16_t output_volume = 0x0800;
	for (uint8_t voice_index = 0; voice_index < MAX_VOICES; voice_index++) {	//loop through all voices
		if (op[0][voice_index].note_value != (uint8_t)-1) {							//if the voice is active
			output_volume += modulate(voice_index);
		}
	}
	return output_volume;
}

int16_t modulate(uint8_t voice_index) {
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {			//loop through all operators of voice
		/*update phase of active operator*/
		op[op_index][voice_index].phase = update_phase(op[op_index][voice_index].delta, op[op_index][voice_index].phase);
	}

	switch (algo) {
	int8_t op3, op2, op1, op0;
	case 0x00:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_mod_sample((op[2][voice_index].phase >> 8) + op3)*op_amp[2] >> 8;
		op1 = (int16_t)get_mod_sample((op[1][voice_index].phase >> 8) + op2)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample((op[0][voice_index].phase >> 8) + op1)*op_amp[0] >> 8;
		return op0;
		break;
	case 0x01:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_mod_sample(op[2][voice_index].phase >> 8)*op_amp[2] >> 8;
		op1 = (int16_t)get_mod_sample((op[1][voice_index].phase >> 8) + op3 + op2)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample((op[0][voice_index].phase >> 8) + op1)*op_amp[0] >> 8;
		return op0;
		break;
	case 0x02:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_mod_sample(op[2][voice_index].phase >> 8)*op_amp[2] >> 8;
		op1 = (int16_t)get_mod_sample((op[1][voice_index].phase >> 8) + op2)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample((op[0][voice_index].phase >> 8) + op3 + op1)*op_amp[0] >> 8;
		return op0;
		break;
	case 0x03:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_mod_sample((op[2][voice_index].phase >> 8) + op3)*op_amp[2] >> 8;
		op1 = (int16_t)get_mod_sample((op[1][voice_index].phase >> 8) + op3)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample((op[0][voice_index].phase >> 8) + op2 + op1)*op_amp[0] >> 8;
		return op0;
		break;
	case 0x04:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_mod_sample(op[2][voice_index].phase >> 8)*op_amp[2] >> 8;
		op1 = (int16_t)get_mod_sample(op[1][voice_index].phase >> 8)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample((op[0][voice_index].phase >> 8) + op3 + op2 + op1)*op_amp[0] >> 8;
		return op0;
		break;
	case 0x05:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_mod_sample((op[2][voice_index].phase >> 8) + op3)*op_amp[2] >> 8;
		op1 = (int16_t)get_sample((op[1][voice_index].phase >> 8) + op2)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample(op[0][voice_index].phase >> 8)*op_amp[0] >> 8;
		return ((int16_t)op1 + op0) >> 1;
		break;
	case 0x06:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_mod_sample((op[2][voice_index].phase >> 8) + op3)*op_amp[2] >> 8;
		op1 = (int16_t)get_sample((op[1][voice_index].phase >> 8) + op2)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample((op[0][voice_index].phase >> 8) + op2)*op_amp[0] >> 8;
		return ((int16_t)op1 + op0) >> 1;
		break;
	case 0x07:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_sample((op[2][voice_index].phase >> 8) + op3)*op_amp[2] >> 8;
		op1 = (int16_t)get_mod_sample(op[1][voice_index].phase >> 8)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample((op[0][voice_index].phase >> 8) + op1)*op_amp[0] >> 8;
		return ((int16_t)op2 + op0) >> 1;
		break;
	case 0x08:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_sample((op[2][voice_index].phase >> 8) + op3)*op_amp[2] >> 8;
		op1 = (int16_t)get_sample((op[1][voice_index].phase >> 8) + op3)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample((op[0][voice_index].phase >> 8) + op3)*op_amp[0] >> 8;
		return ((int16_t)op2 + op1 + op0) / 3;
		break;
	case 0x09:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_sample((op[2][voice_index].phase >> 8) + op3)*op_amp[2] >> 8;
		op1 = (int16_t)get_sample((op[1][voice_index].phase >> 8) + op3)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample(op[0][voice_index].phase >> 8)*op_amp[0] >> 8;
		return ((int16_t)op2 + op1 + op0) / 3;
		break;
	case 0x0A:
		op3 = (int16_t)get_mod_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_sample((op[2][voice_index].phase >> 8) + op3)*op_amp[2] >> 8;
		op1 = (int16_t)get_sample(op[1][voice_index].phase >> 8)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample(op[0][voice_index].phase >> 8)*op_amp[0] >> 8;
		return ((int16_t)op2 + op1 + op0) / 3;
		break;
	case 0x0B:
		op3 = (int16_t)get_sample(op[3][voice_index].phase >> 8)*op_amp[3] >> 8;
		op2 = (int16_t)get_sample(op[2][voice_index].phase >> 8)*op_amp[2] >> 8;
		op1 = (int16_t)get_sample(op[1][voice_index].phase >> 8)*op_amp[1] >> 8;
		op0 = (int16_t)get_sample(op[0][voice_index].phase >> 8)*op_amp[0] >> 8;
		return ((int16_t)op3 + op2 + op1 + op0) >> 2;
		break;
	default:
		break;
	}
	return 0;
}
