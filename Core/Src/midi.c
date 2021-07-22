/*
 * midi.c
 *
 *  Created on: Jun 15, 2021
 *      Author: Tennyson Cheng
 */

#include "midi.h"

void init_midi(UART_HandleTypeDef* huart) {
	data[0] = -1;
	data[1] = -1;
	for (uint8_t i = 0; i < RING_BUFFER_SIZE; i++) {
		midi_buffer[i] = -1;
	}
	midi_buffer_read = 0;
	midi_buffer_write = 0;
	HAL_UART_Receive_IT(huart, &midi_buffer[midi_buffer_write], 1);
}

void process_midi_byte() {
	uint8_t midi_in = midi_buffer[midi_buffer_read];
	if ((midi_in & 0x80) == 0x80) {	//if byte received was status byte
		status = midi_in;
		data[0] = -1;
		data[1] = -1;
	}
	else {													//if byte received was data byte
		switch (status & 0xF0) {
		case 0x80:												//note off
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				note_off();
			}
			break;
		case 0x90:												//note on
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				/*some midi devices send velocity 0 to turn off notes*/
				if (data[1] == 0) {
					note_off();
				}
				else {
					note_on();
				}
			}
			break;
		case 0xA0:												//polyphonic pressure (unimplemented)
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				polyphonic_pressure();
			}
			break;
		case 0xB0:												//control change (unimplemented)
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				control_change();
			}
			break;
		case 0xC0:												//program change (unimplemented)
			data[0] = midi_in;
			program_change();
			break;
		case 0xD0:												//channel pressure (unimplemented)
			data[0] = midi_in;
			channel_pressure();
			break;
		case 0xE0:												//pitch bend (unimplemented)
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				pitch_bend();
			}
			break;
		case 0xF0:												//system (unimplemented)
			reset();
			break;
		default:
			reset();
		}
	}
	midi_buffer_read = (midi_buffer_read + 1) & (RING_BUFFER_SIZE - 1);
}

void reset() {
	data[0] = -1;
	data[1] = -1;
	midi_buffer_read = 0;
	midi_buffer_write = 0;
	clear_voices();
}

void note_off() {
	release_voice(data[0]);
	data[0] = -1;
	data[1] = -1;
}

void note_on() {
	add_voice(data[0]);
	data[0] = -1;
	data[1] = -1;
}

void polyphonic_pressure() {
	reset();
}

void control_change() {
	reset();
}

void program_change() {
	reset();
}

void channel_pressure() {
	reset();
}

void pitch_bend() {
	int16_t pitch = ((int16_t)data[1] << 7) | data[0];
	pitch -= MIDI_PITCH_ZERO;
	pitch >>= 7;
	for (uint8_t op_index = 0; op_index < MAX_OPERATORS; op_index++) {
		op_pitch_bend[op_index] = (op_ratio[op_index] * pitch) >> 4;
	}
}
