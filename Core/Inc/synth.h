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
#include "envelope.h"

#define MAX_OPERATORS 4
#define MAX_VOICES 8
#define MAX_VOLUME 0x80		//changing this may require various changes in code
#define MAX_VOLUME_BITS 7	//max volume represented by bit place
#define MAX_RATIO 0x80
#define MAX_POS_DETUNE 0x7F
#define MAX_NEG_DETUNE 0x80
#define MAX_ALGO 0x0B

typedef struct OPERATOR {
	uint8_t note_value;
	uint16_t freq;
	uint8_t volume;				//final output volume. (op_amp * env_amp / MAX_VOLUME)

	//PHASE
	uint16_t delta;				//fractional value for index increments in the wave table based of samp. freq
	uint16_t phase;				//current fractional value index in the wave table

	//ENVELOPE
	ADSR_STATES adsr_state;
	uint16_t env_amp;			//envelope amplitude (8 MSB integer, 8 LSB decimal)
} OPERATOR;

OPERATOR op[MAX_OPERATORS][MAX_VOICES];
uint8_t op_amp[MAX_OPERATORS];			//operator amplitude
uint8_t op_ratio[MAX_OPERATORS];		//operator pitch (frequency) multiplier
int8_t op_detune[MAX_OPERATORS];		//operator pitch detune
int8_t op_pitch_bend[MAX_OPERATORS];	//midi pitch bend
uint8_t op_attack[MAX_OPERATORS];		//attack rate
uint8_t op_decay[MAX_OPERATORS];		//decay rate
uint8_t op_sustain[MAX_OPERATORS];		//sustain level
uint8_t op_release[MAX_OPERATORS];		//release rate
uint16_t op_attack_inc[MAX_OPERATORS];	//attack rate converted to increments (8 MSB integer, 8 LSB decimal)
uint16_t op_decay_inc[MAX_OPERATORS];	//decay rate converted to increments (8 MSB integer, 8 LSB decimal)
uint16_t op_release_inc[MAX_OPERATORS];	//release rate converted to increments (8 MSB integer, 8 LSB decimal)
uint8_t algo;

void init_synth();
void clear_voices();
void add_voice(uint8_t note_value);
void release_voice(uint8_t note_value);
int16_t synth_sample();
static inline void delete_voice(uint8_t voice_index) { op[0][voice_index].note_value = -1; };

static inline void cycle_envelope(uint8_t voice_index) {
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
		switch (op[op_index][voice_index].adsr_state) {
		case ATTACK:
			if (op[op_index][voice_index].env_amp + op_attack_inc[op_index] >= MAX_VOLUME << 8) {
				op[op_index][voice_index].env_amp = MAX_VOLUME << 8;
				op[op_index][voice_index].adsr_state = DECAY;
			}
			else {
				op[op_index][voice_index].env_amp += op_attack_inc[op_index];
			}
			break;
		case DECAY:
			if (op[op_index][voice_index].env_amp - op_decay_inc[op_index] <= (op_sustain[op_index] << 8)
					|| op[op_index][voice_index].env_amp < op_decay_inc[op_index]) {
				op[op_index][voice_index].env_amp = (op_sustain[op_index] << 8);
				if (op_sustain[op_index] == 0x00) {
					op[op_index][voice_index].adsr_state = SILENT;
				}
				else {
					op[op_index][voice_index].adsr_state = SUSTAIN;
				}
			}
			else {
				op[op_index][voice_index].env_amp -= op_decay_inc[op_index];
			}
			break;
		case SUSTAIN:
			break;
		case RELEASE:
			if (op[op_index][voice_index].env_amp < op_release_inc[op_index]) {
				op[op_index][voice_index].env_amp = 0;
				op[op_index][voice_index].adsr_state = SILENT;
			}
			else {
				op[op_index][voice_index].env_amp -= op_release_inc[op_index];
			}
			break;
		case SILENT:
			if (op_index == 0x00) {
				delete_voice(voice_index);
				return;
			}
			break;
		}
		op[op_index][voice_index].volume = ((uint8_t)(op[op_index][voice_index].env_amp >> 8) * op_amp[op_index]) >> MAX_VOLUME_BITS;
	}
}

static inline int8_t modulate(uint8_t voice_index) {
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {			//loop through all operators of voice
		/*update phase of active operator*/
		op[op_index][voice_index].phase = update_phase(op[op_index][voice_index].delta + op_pitch_bend[op_index], op[op_index][voice_index].phase);
	}

	switch (algo) {
	int8_t op3, op2, op1, op0;
	case 0x00:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_mod_sample((op[2][voice_index].phase >> 8) + op3)*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_mod_sample((op[1][voice_index].phase >> 8) + op2)*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8) + op1)*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return op0;
		break;
	case 0x01:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_mod_sample((op[2][voice_index].phase >> 8))*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_mod_sample((op[1][voice_index].phase >> 8) + op3 + op2)*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8) + op1)*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return op0;
		break;
	case 0x02:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_mod_sample((op[2][voice_index].phase >> 8))*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_mod_sample((op[1][voice_index].phase >> 8) + op2)*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8) + op3 + op1)*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return op0;
		break;
	case 0x03:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_mod_sample((op[2][voice_index].phase >> 8) + op3)*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_mod_sample((op[1][voice_index].phase >> 8) + op3)*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8) + op2 + op1)*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return op0;
		break;
	case 0x04:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_mod_sample((op[2][voice_index].phase >> 8))*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_mod_sample((op[1][voice_index].phase >> 8))*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8) + op3 + op2 + op1)*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return op0;
		break;
	case 0x05:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_mod_sample((op[2][voice_index].phase >> 8) + op3)*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_sample((op[1][voice_index].phase >> 8) + op2)*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8))*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return ((int16_t)op1 + op0) >> 1;
		break;
	case 0x06:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_mod_sample((op[2][voice_index].phase >> 8) + op3)*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_sample((op[1][voice_index].phase >> 8) + op2)*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8) + op2)*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return ((int16_t)op1 + op0) >> 1;
		break;
	case 0x07:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_sample((op[2][voice_index].phase >> 8) + op3)*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_mod_sample((op[1][voice_index].phase >> 8))*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8) + op1)*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return ((int16_t)op2 + op0) >> 1;
		break;
	case 0x08:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_sample((op[2][voice_index].phase >> 8) + op3)*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_sample((op[1][voice_index].phase >> 8) + op3)*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8) + op3)*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return ((int16_t)op2 + op1 + op0) / 3;
		break;
	case 0x09:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_sample((op[2][voice_index].phase >> 8) + op3)*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_sample((op[1][voice_index].phase >> 8) + op3)*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8))*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return ((int16_t)op2 + op1 + op0) / 3;
		break;
	case 0x0A:
		op3 = get_mod_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_sample((op[2][voice_index].phase >> 8) + op3)*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_sample((op[1][voice_index].phase >> 8))*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8))*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return ((int16_t)op2 + op1 + op0) / 3;
		break;
	case 0x0B:
		op3 = get_sample((op[3][voice_index].phase >> 8))*op[3][voice_index].volume >> MAX_VOLUME_BITS;
		op2 = get_sample((op[2][voice_index].phase >> 8))*op[2][voice_index].volume >> MAX_VOLUME_BITS;
		op1 = get_sample((op[1][voice_index].phase >> 8))*op[1][voice_index].volume >> MAX_VOLUME_BITS;
		op0 = get_sample((op[0][voice_index].phase >> 8))*op[0][voice_index].volume >> MAX_VOLUME_BITS;
		return ((int16_t)op3 + op2 + op1 + op0) >> 2;
		break;
	default:
		break;
	}
	return 0;
}

#endif /* INC_SYNTH_H_ */
