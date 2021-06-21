/*
 * midi.h
 *
 *  Created on: Jun 15, 2021
 *      Author: Tennyson Cheng
 */

#ifndef INC_MIDI_H_
#define INC_MIDI_H_

#include "stm32f1xx_hal.h"
#include "synth.h"

uint8_t midi_in;
uint8_t status;
uint8_t data[2];

void init_midi(UART_HandleTypeDef* huart);
void process_midi_byte(UART_HandleTypeDef* huart);

void note_off();
void note_on();
void polyphonic_pressure();
void control_change();
void program_change();
void channel_pressure();
void pitch_bend();

#endif /* INC_MIDI_H_ */
