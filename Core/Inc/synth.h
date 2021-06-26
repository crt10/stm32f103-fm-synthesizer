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
int16_t modulate(uint8_t voice_index);

#endif /* INC_SYNTH_H_ */
