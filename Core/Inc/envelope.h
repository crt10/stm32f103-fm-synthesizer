/*
 * envelope.h
 *
 *  Created on: Jun 16, 2021
 *      Author: Tennyson Cheng
 */

#ifndef INC_ENVELOPE_H_
#define INC_ENVELOPE_H_

#include "stm32f1xx_hal.h"
#include "phase.h"

/* The value of env_period should be SAMPLING_FREQ / ENV_SAMPLING_FREQ.
 * At SAMPLING_FREQ = 48000 and ENV_SAMPLING_FREQ = 32, the max env_time value should be 1500.
 * env_time decrements every sample cycle. env_period will decrement from 1500 to 0.
 * Once env_period hits 0, the next envelope cycle will be sampled.
 */
#define ENV_SAMPLING_FREQ 32
#define ENV_MAX_PERIOD (SAMPLING_FREQ / ENV_SAMPLING_FREQ)
#define ENV_MAX_RATE 0x80

typedef enum ADSR_STATES {
	ATTACK,
	DECAY,
	SUSTAIN,
	RELEASE,
	SILENT
} ADSR_STATES;

uint16_t env_period;

static inline uint16_t calculate_env_inc(uint8_t rate) { return (ENV_MAX_RATE << 8) / (uint16_t)(ENV_MAX_RATE - rate + 1); };

#endif /* INC_ENVELOPE_H_ */
