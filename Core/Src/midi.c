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
	HAL_UART_Receive_IT(huart, &midi_in, 1);
}

void process_midi_byte(UART_HandleTypeDef* huart) {
	HAL_UART_Receive_IT(huart, &midi_in, 1);	//re-enable interrupt for next byte
	if ((midi_in & 0x80) == 0x80) {				//if byte received was status byte
		status = midi_in;
		data[0] = -1;
		data[1] = -1;
	}
	else {										//if byte received was data byte
		switch (status) {
		case 0x80:									//note off
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				note_off();
			}
			break;
		case 0x90:									//note on
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				if (data[1] == 0) {					//some midi devices send velocity 0 to turn off notes
					note_off();
				}
				else {
					note_on();
				}
			}
			break;
		case 0xA0:									//polyphonic pressure (unimplemented)
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				polyphonic_pressure();
			}
			break;
		case 0xB0:									//control change (unimplemented)
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				control_change();
			}
			break;
		case 0xC0:									//program change (unimplemented)
			data[0] = midi_in;
			program_change();
			break;
		case 0xD0:									//channel pressure (unimplemented)
			data[0] = midi_in;
			channel_pressure();
			break;
		case 0xE0:									//pitch bend
			if (data[0] == (uint8_t)-1) {
				data[0] = midi_in;
			}
			else {
				data[1] = midi_in;
				pitch_bend();
			}
			break;
		case 0xF0:									//system (unimplemented)
			break;
		default:
			__NOP();
		}
	}
}

void note_off() {
	release_voice(data[0]);
}

void note_on() {
	add_voice(data[0]);
}

void polyphonic_pressure() {

}

void control_change() {

}

void program_change() {

}

void channel_pressure() {

}

void pitch_bend() {

}
