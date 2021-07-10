/*
 * ui.c
 *
 *  Created on: Jul 4, 2021
 *      Author: Tennyson Cheng
 */

#include "ui.h"

const char* HEX_TO_STRING[] = {
		"0   ", "1   ", "2   ", "3   ", "4   ", "5   ", "6   ", "7   ", "8   ", "9   ", "10  ", "11  ", "12  ", "13  ", "14  ", "15  ",
		"16  ", "17  ", "18  ", "19  ", "20  ", "21  ", "22  ", "23  ", "24  ", "25  ", "26  ", "27  ", "28  ", "29  ", "30  ", "31  ",
		"32  ", "33  ", "34  ", "35  ", "36  ", "37  ", "38  ", "39  ", "40  ", "41  ", "42  ", "43  ", "44  ", "45  ", "46  ", "47  ",
		"48  ", "49  ", "50  ", "51  ", "52  ", "53  ", "54  ", "55  ", "56  ", "57  ", "58  ", "59  ", "60  ", "61  ", "62  ", "63  ",
		"64  ", "65  ", "66  ", "67  ", "68  ", "69  ", "70  ", "71  ", "72  ", "73  ", "74  ", "75  ", "76  ", "77  ", "78  ", "79  ",
		"80  ", "81  ", "82  ", "83  ", "84  ", "85  ", "86  ", "87  ", "88  ", "89  ", "90  ", "91  ", "92  ", "93  ", "94  ", "95  ",
		"96  ", "97  ", "98  ", "99  ", "100 ", "101 ", "102 ", "103 ", "104 ", "105 ", "106 ", "107 ", "108 ", "109 ", "110 ", "111 ",
		"112 ", "113 ", "114 ", "115 ", "116 ", "117 ", "118 ", "119 ", "120 ", "121 ", "122 ", "123 ", "124 ", "125 ", "126 ", "127 ",
		"128 "
};

void init_display(I2C_HandleTypeDef* hi2c) {
	display_i2c = hi2c;
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
	//turn on display
	display_send_command(DISPLAY_CMD_ON_OFF | DISPLAY_BIT_ON_DISPLAY | DISPLAY_BIT_ON_CURSOR);
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
	HAL_I2C_Master_Transmit(display_i2c, DISPLAY_ADDR_I2C_WRITE, &byte, 1, 0xFF);
}

void display_i2c_dma_write(uint8_t* buffer, uint8_t size) {
	HAL_I2C_Master_Transmit_DMA(display_i2c, DISPLAY_ADDR_I2C_WRITE, buffer, size);
}

void display_convert_cmd(uint8_t cmd, uint8_t* buffer, uint8_t* pos){
	uint8_t cmd_MSB = (cmd & 0xF0) | DISPLAY_BIT_BACKLIGHT;
	uint8_t cmd_LSB = (cmd << 4) | DISPLAY_BIT_BACKLIGHT;
	*(buffer + (*pos)++) = cmd_MSB;
	*(buffer + (*pos)++) = cmd_MSB | DISPLAY_BIT_E;
	*(buffer + (*pos)++) = cmd_MSB;
	*(buffer + (*pos)++) = cmd_LSB;
	*(buffer + (*pos)++) = cmd_LSB | DISPLAY_BIT_E;
	*(buffer + (*pos)++) = cmd_LSB;
}

void display_convert_data(uint8_t data, uint8_t* buffer, uint8_t* pos) {
	uint8_t data_MSB = (data & 0xF0) | DISPLAY_BIT_RS | DISPLAY_BIT_BACKLIGHT;
	uint8_t data_LSB = (data << 4) | DISPLAY_BIT_RS | DISPLAY_BIT_BACKLIGHT;
	*(buffer + (*pos)++) = data_MSB;
	*(buffer + (*pos)++) = data_MSB | DISPLAY_BIT_E;
	*(buffer + (*pos)++) = data_MSB;
	*(buffer + (*pos)++) = data_LSB;
	*(buffer + (*pos)++) = data_LSB | DISPLAY_BIT_E;
	*(buffer + (*pos)++) = data_LSB;
}
