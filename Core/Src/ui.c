/*
 * ui.c
 *
 *  Created on: Jul 4, 2021
 *      Author: Tennyson Cheng
 */

#include "ui.h"

void init_ui(I2C_HandleTypeDef* hi2c) {
	ui_i2c = hi2c;
	//4 function set commands needed to determine bus mode
	uint8_t init_4_bit_cmd = (DISPLAY_CMD_FUNC | DISPLAY_BIT_FUNC_BUS) | ((DISPLAY_CMD_FUNC | DISPLAY_BIT_FUNC_BUS) >> 4);
	display_send_command(init_4_bit_cmd);
	init_4_bit_cmd = (DISPLAY_CMD_FUNC | DISPLAY_BIT_FUNC_BUS) | (DISPLAY_CMD_FUNC >> 4);
	display_send_command(init_4_bit_cmd);
	//4th func set to finalize 4-bit mode and double line mode
	display_send_command(DISPLAY_CMD_FUNC |  DISPLAY_BIT_FUNC_DOUBLE);
	//clear the display
	display_send_command(DISPLAY_CMD_CLEAR);
	//set cursor to move to right along with DDRAM increment when written
	display_send_command(DISPLAY_CMD_ENTRY | DISPLAY_BIT_ENTRY_INC);
	//turn on display and cursor
	display_send_command(DISPLAY_CMD_ON_OFF | DISPLAY_BIT_ON_DISPLAY | DISPLAY_BIT_ON_CURSOR);
	display_send_data('t');
	display_send_data('e');
	display_send_data('s');
	display_send_data('t');
}

void display_send_command(uint8_t cmd) {
	uint8_t cmd_MSB = (cmd & 0xF0) | DISPLAY_BIT_BACKLIGHT;
	uint8_t cmd_LSB = (cmd << 4) | DISPLAY_BIT_BACKLIGHT;
	display_i2c_write(cmd_MSB);						//load 4 MSB into DR
	display_i2c_write(cmd_MSB | DISPLAY_BIT_E);		//pull enable bit HIGH
	display_i2c_write(cmd_MSB);						//pull enable bit LOW

	HAL_Delay(5);									//wait 5 ms

	display_i2c_write(cmd_LSB);						//load 4 LSB into DR
	display_i2c_write(cmd_LSB | DISPLAY_BIT_E);		//pull enable bit HIGH
	display_i2c_write(cmd_LSB);						//pull enable bit LOW
}

void display_send_data(uint8_t data) {
	uint8_t data_MSB = (data & 0xF0) | DISPLAY_BIT_RS | DISPLAY_BIT_BACKLIGHT;
	uint8_t data_LSB = (data << 4) | DISPLAY_BIT_RS| DISPLAY_BIT_BACKLIGHT;
	display_i2c_write(data_MSB);					//load 4 MSB into DR + write bit
	display_i2c_write(data_MSB | DISPLAY_BIT_E);	//pull enable bit HIGH
	display_i2c_write(data_MSB);					//pull enable bit LOW

	display_i2c_write(data_LSB);					//load 4 LSB into DR + write bit
	display_i2c_write(data_LSB | DISPLAY_BIT_E);	//pull enable bit HIGH
	display_i2c_write(data_LSB);					//pull enable bit LOW
}

void display_i2c_write(uint8_t byte) {
	HAL_I2C_Master_Transmit(ui_i2c, DISPLAY_ADDR_I2C_WRITE, &byte, 1, 0xFF);
}
