/*
 * ui.h
 *
 *  Created on: Jul 4, 2021
 *      Author: Tennyson Cheng
 */

#ifndef INC_UI_H_
#define INC_UI_H_

#include "stm32f1xx_hal.h"

#define DISPLAY_ADDR_I2C_WRITE 0x27 << 1
#define DISPLAY_ADDR_I2C_READ 0x2F << 1
#define DISPLAY_CMD_CLEAR 0x01
#define DISPLAY_CMD_RETURN 0x02
#define DISPLAY_CMD_ENTRY 0x04			//bit 0: shift, bit 1: inc/dec
#define DISPLAY_CMD_ON_OFF 0x08			//bit 0: cursor blink, bit 1: cursor off/on, bit 2: display off/on
#define DISPLAY_CMD_SHIFT 0x10			//bit 2: left/right, bit 3: cursor/display
#define DISPLAY_CMD_FUNC 0x20			//bit 2: font, bit 3: single/double line, bit 4: bus mode
#define DISPLAY_CMD_CGRAM_ADDR 0x40		//bit 0-5: CGRAM address
#define DISPLAY_CMD_DDRAM_ADDR 0x80		//bit 0-6: DDRAM address
#define DISPLAY_ADDR_LINE_1 0x00		//single line mode: 0x00-0x4F, double line mode: 0x00-0x27
#define DISPLAY_ADDR_LINE_2 0x40		//double line mode: 0x40-0x67

#define DISPLAY_BIT_ENTRY_SHIFT 0x01			//enable display shifting
#define DISPLAY_BIT_ENTRY_INC 0x02				//increment DDRAM when written and cursor moves to right
#define DISPLAY_BIT_SHIFT_CURSOR_LEFT 0x00		//shift cursor to the left
#define DISPLAY_BIT_SHIFT_CURSOR_RIGHT 0x04		//shift cursor to the right
#define DISPLAY_BIT_SHIFT_DISPLAY_LEFT 0x08		//shift display to the left
#define DISPLAY_BIT_SHIFT_DISPLAY_RIGHT 0x0C	//shift display to the right
#define DISPLAY_BIT_FUNC_FONT 0x04				//5x11 dot font
#define DISPLAY_BIT_FUNC_DOUBLE 0x08			//double line mode
#define DISPLAY_BIT_FUNC_BUS 0x10				//8-bit bus
#define DISPLAY_BIT_ON_BLINK 0x01				//cursor blink on
#define DISPLAY_BIT_ON_CURSOR 0x02				//cursor on
#define DISPLAY_BIT_ON_DISPLAY 0x04				//display on
#define DISPLAY_BIT_RS 0x01
#define DISPLAY_BIT_RW 0x02
#define DISPLAY_BIT_E 0x04
#define DISPLAY_BIT_BACKLIGHT 0x08		//PCF8574 LCD I2C module uses pin 3 for backlight control

#define DISPLAY_MAX_LENGTH 40			//1602 LCD can hold 40 characters per line in RAM
#define DISPLAY_MAX_PHYSICAL_LENGTH 16	//1602 LCD can physically display 16 characters per line
#define DISPLAY_MAX_LINES 2				//double line mode of 1602 LCD is used

#define UI_PB0 GPIO_PIN_7
#define UI_PB1 GPIO_PIN_8
#define UI_PB2 GPIO_PIN_9
#define UI_ROT_GPIO GPIOB
#define UI_ROT0_A GPIO_PIN_12
#define UI_ROT0_B GPIO_PIN_13
#define UI_ROT1_A GPIO_PIN_14
#define UI_ROT1_B GPIO_PIN_15
#define UI_ROT2_A GPIO_PIN_3
#define UI_ROT2_B GPIO_PIN_4
#define UI_ROT3_A GPIO_PIN_5
#define UI_ROT3_B GPIO_PIN_6

extern const char* HEX_TO_STRING[];

I2C_HandleTypeDef* display_i2c;

void init_display(I2C_HandleTypeDef* hi2c);
void display_send_command(uint8_t cmd);
void display_send_data(uint8_t data);
void display_i2c_write(uint8_t byte);
void display_i2c_dma_write(uint8_t* buffer, uint8_t size);

/*used to convert bytes for 4-bit transmission*/
void display_convert_cmd(uint8_t cmd, uint8_t* buffer, uint8_t* pos);
void display_convert_data(uint8_t data, uint8_t* buffer, uint8_t* pos);

#endif /* INC_UI_H_ */
