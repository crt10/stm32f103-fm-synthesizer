/*
 * synth.h
 *
 *  Created on: Jun 17, 2021
 *      Author: Tennyson Cheng
 */

#ifndef INC_SYNTH_H_
#define INC_SYNTH_H_

#include "stm32f1xx_hal.h"
#include "phase.h"

#define MAX_OPERATORS 4
#define MAX_VOICES 16

typedef struct OPERATOR {
	uint8_t note_value;
	uint16_t freq;

	//PHASE
	uint16_t delta;				//fractional value for index increments in the wave table based of samp. freq
	uint16_t phase;				//current fractional value index in the wave table

	//ENVELOPE
} OPERATOR;

OPERATOR op[MAX_OPERATORS][MAX_VOICES];
uint8_t op_amp[MAX_OPERATORS];	//operator amplitude
uint8_t op_ratio[MAX_OPERATORS];	//operator pitch (frequency) multiplier
uint8_t algo;

void init_synth();
void add_voice(uint8_t note_value);
void delete_voice(uint8_t note_value);
int16_t synth_sample();

static inline int16_t modulate(uint8_t voice_index) {
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

#endif /* INC_SYNTH_H_ */
