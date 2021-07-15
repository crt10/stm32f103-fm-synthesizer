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

const char* HEX_TO_STRING_RATIO[] = {
	"0.00", "0.07", "0.13", "0.19", "0.25", "0.32", "0.38", "0.44", "0.5 ", "0.57", "0.63", "0.69", "0.75", "0.82", "0.88", "0.94",
	"1.00", "1.07", "1.13", "1.19", "1.25", "1.32", "1.38", "1.44", "1.5 ", "1.57", "1.63", "1.69", "1.75", "1.82", "1.88", "1.94",
	"2.00", "2.07", "2.13", "2.19", "2.25", "2.32", "2.38", "2.44", "2.5 ", "2.57", "2.63", "2.69", "2.75", "2.82", "2.88", "2.94",
	"3.00", "3.07", "3.13", "3.19", "3.25", "3.32", "3.38", "3.44", "3.5 ", "3.57", "3.63", "3.69", "3.75", "3.82", "3.88", "3.94",
	"4.00", "4.07", "4.13", "4.19", "4.25", "4.32", "4.38", "4.44", "4.5 ", "4.57", "4.63", "4.69", "4.75", "4.82", "4.88", "4.94",
	"5.00", "5.07", "5.13", "5.19", "5.25", "5.32", "5.38", "5.44", "5.5 ", "5.57", "5.63", "5.69", "5.75", "5.82", "5.88", "5.94",
	"6.00", "6.07", "6.13", "6.19", "6.25", "6.32", "6.38", "6.44", "6.5 ", "6.57", "6.63", "6.69", "6.75", "6.82", "6.88", "6.94",
	"7.00", "7.07", "7.13", "7.19", "7.25", "7.32", "7.38", "7.44", "7.5 ", "7.57", "7.63", "7.69", "7.75", "7.82", "7.88", "7.94",
	"8.00"
};

const char* HEX_TO_STRING_DETUNE[] = {
	"0   ", "+1  ", "+2  ", "+3  ", "+4  ", "+5  ", "+6  ", "+7  ", "+8  ", "+9  ", "+10 ", "+11 ", "+12 ", "+13 ", "+14 ", "+15 ",
	"+16 ", "+17 ", "+18 ", "+19 ", "+20 ", "+21 ", "+22 ", "+23 ", "+24 ", "+25 ", "+26 ", "+27 ", "+28 ", "+29 ", "+30 ", "+31 ",
	"+32 ", "+33 ", "+34 ", "+35 ", "+36 ", "+37 ", "+38 ", "+39 ", "+40 ", "+41 ", "+42 ", "+43 ", "+44 ", "+45 ", "+46 ", "+47 ",
	"+48 ", "+49 ", "+50 ", "+51 ", "+52 ", "+53 ", "+54 ", "+55 ", "+56 ", "+57 ", "+58 ", "+59 ", "+60 ", "+61 ", "+62 ", "+63 ",
	"+64 ", "+65 ", "+66 ", "+67 ", "+68 ", "+69 ", "+70 ", "+71 ", "+72 ", "+73 ", "+74 ", "+75 ", "+76 ", "+77 ", "+78 ", "+79 ",
	"+80 ", "+81 ", "+82 ", "+83 ", "+84 ", "+85 ", "+86 ", "+87 ", "+88 ", "+89 ", "+90 ", "+91 ", "+92 ", "+93 ", "+94 ", "+95 ",
	"+96 ", "+97 ", "+98 ", "+99 ", "+100", "+101", "+102", "+103", "+104", "+105", "+106", "+107", "+108", "+109", "+110", "+111",
	"+112", "+113", "+114", "+115", "+116", "+117", "+118", "+119", "+120", "+121", "+122", "+123", "+124", "+125", "+126", "+127",
	"-128", "-127", "-126", "-125", "-124", "-123", "-122", "-121", "-120", "-119", "-118", "-117", "-116", "-115", "-114", "-113",
	"-112", "-111", "-110", "-109", "-108", "-107", "-106", "-105", "-104", "-103", "-102", "-101", "-100", "-99 ", "-98 ", "-97 ",
	"-96 ", "-95 ", "-94 ", "-93 ", "-92 ", "-91 ", "-90 ", "-89 ", "-88 ", "-87 ", "-86 ", "-85 ", "-84 ", "-83 ", "-82 ", "-81 ",
	"-80 ", "-79 ", "-78 ", "-77 ", "-76 ", "-75 ", "-74 ", "-73 ", "-72 ", "-71 ", "-70 ", "-69 ", "-68 ", "-67 ", "-66 ", "-65 ",
	"-64 ", "-63 ", "-62 ", "-61 ", "-60 ", "-59 ", "-58 ", "-57 ", "-56 ", "-55 ", "-54 ", "-53 ", "-52 ", "-51 ", "-50 ", "-49 ",
	"-48 ", "-47 ", "-46 ", "-45 ", "-44 ", "-43 ", "-42 ", "-41 ", "-40 ", "-39 ", "-38 ", "-37 ", "-36 ", "-35 ", "-34 ", "-33 ",
	"-32 ", "-31 ", "-30 ", "-29 ", "-28 ", "-27 ", "-26 ", "-25 ", "-24 ", "-23 ", "-22 ", "-21 ", "-20 ", "-19 ", "-18 ", "-17 ",
	"-16 ", "-15 ", "-14 ", "-13 ", "-12 ", "-11 ", "-10 ", "-9  ", "-8  ", "-7  ", "-6  ", "-5  ", "-4  ", "-3  ", "-2  ", "-1  ",
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
