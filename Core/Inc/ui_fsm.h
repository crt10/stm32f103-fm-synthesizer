/*
 * ui_fsm.h
 *
 *  Created on: Jul 6, 2021
 *      Author: Tennyson Cheng
 */

#ifndef INC_UI_FSM_H_
#define INC_UI_FSM_H_

#include "stm32f1xx_hal.h"
#include "string.h"
#include "ui.h"
#include "synth.h"

#define NUM_OF_STATES 13
#define MAX_PARAMS 4
#define NUM_OF_UI_STRINGS 12
/* NOTE: 6 is the number of messages needed in order to send 1 data/cmd to the LCD
 * In order to send 1 character, we send 6 messages, hence the *6.
 * 2 cmds are neede to update the DDRAM address, hence the (2*6). */
#define UI_STRING_CONVERTED_SIZE DISPLAY_MAX_LINES*DISPLAY_MAX_PHYSICAL_LENGTH*6 + (2*6)
#define SECOND_LINE_START_INDEX DISPLAY_MAX_PHYSICAL_LENGTH*6 + (2*6)
#define INC_DEC_DEFAULT 0x01
#define INC_DEC_ALT 0x08

typedef void (* transition_fn) ();
typedef enum {
	menu_amp, menu_ratio, menu_detune, menu_env, menu_env_op, menu_algo, menu_instr,
	set_amp, set_ratio, set_detune, set_env, set_algo, set_instr
} state;
typedef enum {
	pb_0, pb_1, pb_2,
	rot_inc, rot_dec,
	invalid
} input;
typedef struct {
	input key;
	state next_state;
	transition_fn transition_task;
} transition;

/*transition state functions*/
void input_invalid();
void change_inc_dec();

void select_menu_amp();
void select_menu_ratio();
void select_menu_detune();
void select_menu_env();
void select_menu_algo();
void select_menu_instr();

void enter_menu_amp();
void enter_menu_ratio();
void enter_menu_detune();
void enter_menu_env();
void enter_menu_env_op();
void enter_menu_algo();
void enter_menu_instr();

void inc_menu_env_op();
void inc_set_amp();
void inc_set_ratio();
void inc_set_detune();
void inc_set_env();
void inc_set_algo();
void inc_set_instr();

void dec_set_amp();
void dec_set_ratio();
void dec_set_detune();
void dec_set_env();
void dec_set_instr();

void temp_set_amp();
void temp_set_ratio();
void temp_set_detune();
void temp_set_env();
void temp_set_algo();
void temp_set_instr();

extern const transition menu_amp_transitions[];
extern const transition menu_ratio_transitions[];
extern const transition menu_detune_transitions[];
extern const transition menu_env_transitions[];
extern const transition menu_env_op_transitions[];
extern const transition menu_algo_transitions[];
extern const transition menu_instr_transitions[];
extern const transition set_amp_transitions[];
extern const transition set_ratio_transitions[];
extern const transition set_detune_transitions[];
extern const transition set_env_transitions[];
extern const transition set_algo_transitions[];
extern const transition set_instr_transitions[];
extern const transition* fsm_transition_table[NUM_OF_STATES];

extern const uint8_t ui_menu_amp[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_menu_ratio[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_menu_detune[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_menu_env[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_menu_env_op[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_menu_algo[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_menu_instr[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_set[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_set_algo[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t ui_set_env[DISPLAY_MAX_PHYSICAL_LENGTH];
extern const uint8_t* ui_string_table[NUM_OF_UI_STRINGS];
uint8_t ui_menu_amp_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_menu_ratio_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_menu_detune_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_menu_env_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_menu_algo_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_menu_instr_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_menu_env_op_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_set_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_set_algo_converted[UI_STRING_CONVERTED_SIZE];
uint8_t ui_set_env_converted[UI_STRING_CONVERTED_SIZE];
uint8_t* ui_string_table_converted[NUM_OF_UI_STRINGS];

state present_state;
uint8_t fsm_op;
uint8_t fsm_env_op;
uint8_t inc_dec;
uint8_t temp[MAX_PARAMS];

void init_ui(I2C_HandleTypeDef* hi2c);
void fsm(input key);
static inline void fsm_change_op(uint8_t op) { fsm_op = op; };
void display_update_menu_amp();
void display_update_menu_ratio();
void display_update_menu_detune();
void display_update_menu_env();
void display_update_menu_env_op();
void display_update_menu_algo();
void display_update_menu_instr();

#endif /* INC_UI_FSM_H_ */
