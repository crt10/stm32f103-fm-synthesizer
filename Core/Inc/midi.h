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

#define RING_BUFFER_SIZE 128	//should be exponentials of 2 and lower than 256. EX: 4, 8, 16, 32, etc.

uint8_t status;
uint8_t data[2];
uint8_t midi_buffer[RING_BUFFER_SIZE];
volatile uint8_t midi_buffer_read;
volatile uint8_t midi_buffer_write;


void init_midi(UART_HandleTypeDef* huart);
void process_midi_byte();

void note_off();
void note_on();
void polyphonic_pressure();
void control_change();
void program_change();
void channel_pressure();
void pitch_bend();

#endif /* INC_MIDI_H_ */
