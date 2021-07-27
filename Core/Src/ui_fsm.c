/*
 * ui_fsm.c
 *
 *  Created on: Jul 8, 2021
 *      Author: Tennyson Cheng
 */

#include "ui_fsm.h"

const transition menu_amp_transitions[] = {
	{pb_0				, menu_instr		, select_menu_instr		},
	{pb_1				, set_amp			, enter_menu_amp		},
	{pb_2				, menu_ratio		, select_menu_ratio		},
	{invalid			, menu_amp			, input_invalid			}
};
const transition menu_ratio_transitions[] = {
	{pb_0				, menu_amp			, select_menu_amp		},
	{pb_1				, set_ratio			, enter_menu_ratio		},
	{pb_2				, menu_detune		, select_menu_detune	},
	{invalid			, menu_ratio		, input_invalid			}
};
const transition menu_detune_transitions[] = {
	{pb_0				, menu_ratio		, select_menu_ratio		},
	{pb_1				, set_detune		, enter_menu_detune		},
	{pb_2				, menu_env			, select_menu_env		},
	{invalid			, menu_detune		, input_invalid			}
};
const transition menu_env_transitions[] = {
	{pb_0				, menu_detune		, select_menu_detune	},
	{pb_1				, menu_env_op		, enter_menu_env		},
	{pb_2				, menu_algo			, select_menu_algo		},
	{invalid			, menu_env			, input_invalid			}
};
const transition menu_env_op_transitions[] = {
	{pb_0				, menu_env			, select_menu_env		},
	{pb_1				, set_env			, enter_menu_env_op		},
	{pb_2				, menu_env_op		, inc_menu_env_op		},
	{invalid			, menu_env_op		, input_invalid			}
};
const transition menu_algo_transitions[] = {
	{pb_0				, menu_env			, select_menu_env		},
	{pb_1				, set_algo			, enter_menu_algo		},
	{pb_2				, menu_instr		, select_menu_instr		},
	{invalid			, menu_algo			, input_invalid			}
};
const transition menu_instr_transitions[] = {
	{pb_0				, menu_algo			, select_menu_algo		},
	{pb_1				, set_instr			, enter_menu_instr		},
	{pb_2				, menu_amp			, select_menu_amp		},
	{invalid			, menu_instr		, input_invalid			}
};
const transition set_amp_transitions[] = {
	{pb_0				, menu_amp			, select_menu_amp		},
	{pb_1				, set_amp			, temp_set_amp			},
	{pb_2				, set_amp			, change_inc_dec		},
	{rot_inc			, set_amp			, inc_set_amp			},
	{rot_dec			, set_amp			, dec_set_amp			},
	{invalid			, set_amp			, input_invalid			}
};
const transition set_ratio_transitions[] = {
	{pb_0				, menu_ratio		, select_menu_ratio		},
	{pb_1				, set_ratio			, temp_set_ratio		},
	{pb_2				, set_ratio			, change_inc_dec		},
	{rot_inc			, set_ratio			, inc_set_ratio			},
	{rot_dec			, set_ratio			, dec_set_ratio			},
	{invalid			, set_ratio			, input_invalid			}
};
const transition set_detune_transitions[] = {
	{pb_0				, menu_detune		, select_menu_detune	},
	{pb_1				, set_detune		, temp_set_detune		},
	{pb_2				, set_detune		, change_inc_dec		},
	{rot_inc			, set_detune		, inc_set_detune		},
	{rot_dec			, set_detune		, dec_set_detune		},
	{invalid			, set_detune		, input_invalid			}
};
const transition set_env_transitions[] = {
	{pb_0				, menu_env_op		, enter_menu_env		},
	{pb_1				, set_env			, temp_set_env			},
	{pb_2				, set_env			, change_inc_dec		},
	{rot_inc			, set_env			, inc_set_env			},
	{rot_dec			, set_env			, dec_set_env			},
	{invalid			, set_env			, input_invalid			}
};
const transition set_algo_transitions[] = {
	{pb_0				, menu_algo			, select_menu_algo		},
	{pb_1				, set_algo			, temp_set_algo			},
	{pb_2				, set_algo			, inc_set_algo			},
	{invalid			, set_algo			, input_invalid			}
};
const transition set_instr_transitions[] = {
	{pb_0				, menu_instr		, select_menu_instr		},
	{pb_1				, set_instr			, dec_set_instr			},
	{pb_2				, set_instr			, inc_set_instr			},
	{invalid			, set_instr			, input_invalid			}
};
const transition* fsm_transition_table[NUM_OF_STATES] = {
	menu_amp_transitions,
	menu_ratio_transitions,
	menu_detune_transitions,
	menu_env_transitions,
	menu_env_op_transitions,
	menu_algo_transitions,
	menu_instr_transitions,
	set_amp_transitions,
	set_ratio_transitions,
	set_detune_transitions,
	set_env_transitions,
	set_algo_transitions,
	set_instr_transitions,
};

const uint8_t ui_menu_amp[DISPLAY_MAX_PHYSICAL_LENGTH] = "instr ~amp ratio";
const uint8_t ui_menu_ratio[DISPLAY_MAX_PHYSICAL_LENGTH] = " amp ~ratio detu";
const uint8_t ui_menu_detune[DISPLAY_MAX_PHYSICAL_LENGTH] = "tio ~detune env ";
const uint8_t ui_menu_env[DISPLAY_MAX_PHYSICAL_LENGTH] = "etune ~env algo ";
const uint8_t ui_menu_env_op[DISPLAY_MAX_PHYSICAL_LENGTH] = "SELECT OPERATOR:";
const uint8_t ui_menu_algo[DISPLAY_MAX_PHYSICAL_LENGTH] = " env ~algo instr";
const uint8_t ui_menu_instr[DISPLAY_MAX_PHYSICAL_LENGTH] = "algo ~instr amp ";
const uint8_t ui_set[DISPLAY_MAX_PHYSICAL_LENGTH] = "OP1 OP2 OP3 OP4 ";
const uint8_t ui_set_algo[DISPLAY_MAX_PHYSICAL_LENGTH] = "SELECT ALGO:    ";
const uint8_t ui_set_env[DISPLAY_MAX_PHYSICAL_LENGTH] = "ATK DEC SUS REL ";
const uint8_t ui_set_instr[DISPLAY_MAX_PHYSICAL_LENGTH] = "SELECT INSTR:   ";

const uint8_t* ui_string_table[NUM_OF_UI_STRINGS] = {
	ui_menu_amp,
	ui_menu_ratio,
	ui_menu_detune,
	ui_menu_env,
	ui_menu_env_op,
	ui_menu_algo,
	ui_menu_instr,
	ui_set,
	ui_set_algo,
	ui_set_env,
	ui_set_instr
};
uint8_t* ui_string_table_converted[NUM_OF_UI_STRINGS] = {
	ui_menu_amp_converted,
	ui_menu_ratio_converted,
	ui_menu_detune_converted,
	ui_menu_env_converted,
	ui_menu_env_op_converted,
	ui_menu_algo_converted,
	ui_menu_instr_converted,
	ui_set_converted,
	ui_set_algo_converted,
	ui_set_env_converted,
	ui_set_instr_converted
};

const INSTRUMENT instruments[MAX_INSTRUMENTS] = {
		{
				"Default         ",
				{MAX_VOLUME, MAX_VOLUME, MAX_VOLUME, MAX_VOLUME},
				{0x10, 0x10, 0x10, 0x10},
				{0x00, 0x00, 0x00, 0x00},
				{ENV_MAX_RATE, ENV_MAX_RATE, ENV_MAX_RATE, ENV_MAX_RATE},
				{ENV_MAX_RATE, ENV_MAX_RATE, ENV_MAX_RATE, ENV_MAX_RATE},
				{MAX_VOLUME, MAX_VOLUME, MAX_VOLUME, MAX_VOLUME},
				{ENV_MAX_RATE, ENV_MAX_RATE, ENV_MAX_RATE, ENV_MAX_RATE},
				0x00
		},
		{
				"TEST INSTR      ",
				{128, 96, 104, 80},
				{0x08, 0x20, 0x18, 0x08},
				{1, 2, 0, -4},
				{124, 124, 126, 128},
				{64, 128, 128, 128},
				{96, 128, 128, 128},
				{120, 128, 126, 128},
				7
		}
};

void init_ui(I2C_HandleTypeDef* hi2c) {
	init_display(hi2c);
	for (uint8_t i = 0; i < NUM_OF_UI_STRINGS; i++) {	//loop through all UI strings to convert
		uint8_t index = 0;
		display_convert_cmd(							//start DDRAM address at beginning of first line
			DISPLAY_CMD_DDRAM_ADDR | DISPLAY_ADDR_LINE_1, ui_string_table_converted[i], &index
		);
		for (uint8_t o = 0; o < DISPLAY_MAX_PHYSICAL_LENGTH; o++) {
			display_convert_data(						//convert chars to 4-bit bus data for 1602 LCD
				ui_string_table[i][o], ui_string_table_converted[i], &index
			);
		}
		display_convert_cmd(							//move DDRAM address at beginning of second line
			DISPLAY_CMD_DDRAM_ADDR | DISPLAY_ADDR_LINE_2, ui_string_table_converted[i], &index
		);
		for (uint8_t o = 0; o < DISPLAY_MAX_PHYSICAL_LENGTH; o++) {
			display_convert_data(						//write spaces for second line
				' ', ui_string_table_converted[i], &index
			);
		}
	}
	present_state = menu_amp;				//set state to menu_amp
	fsm_op = 0;
	fsm_env_op = 0;
	select_menu_amp();						//update display to show menu amp selection
	inc_dec = 0x01;
}

void fsm(input key) {
	uint8_t i;
	for (i = 0; fsm_transition_table[present_state][i].key != key &&
		fsm_transition_table[present_state][i].key != invalid; i++);	//point i to the correct transition
	fsm_transition_table[present_state][i].transition_task();			//execute associated transition function
	present_state = fsm_transition_table[present_state][i].next_state;	//update state
}

void input_invalid() {
}

void change_inc_dec() {
	if (inc_dec == INC_DEC_DEFAULT) {
		inc_dec = INC_DEC_ALT;
	}
	else {
		inc_dec = INC_DEC_DEFAULT;
	}
}

void select_menu_amp() {
	display_i2c_dma_write(ui_menu_amp_converted, UI_STRING_CONVERTED_SIZE);
}

void select_menu_ratio() {
	display_i2c_dma_write(ui_menu_ratio_converted, UI_STRING_CONVERTED_SIZE);
}
void select_menu_detune() {
	display_i2c_dma_write(ui_menu_detune_converted, UI_STRING_CONVERTED_SIZE);
}

void select_menu_env() {
	display_i2c_dma_write(ui_menu_env_converted, UI_STRING_CONVERTED_SIZE);
}

void select_menu_algo() {
	display_i2c_dma_write(ui_menu_algo_converted, UI_STRING_CONVERTED_SIZE);
}

void select_menu_instr() {
	display_i2c_dma_write(ui_menu_instr_converted, UI_STRING_CONVERTED_SIZE);
}

void enter_menu_amp() {
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		temp[i] = op_amp[i];
	}
	display_update_menu_amp();
}

void enter_menu_ratio() {
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		temp[i] = op_ratio[i];
	}
	display_update_menu_ratio();
}

void enter_menu_detune() {
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		temp[i] = op_detune[i];
	}
	display_update_menu_detune();
}

void enter_menu_env() {
	display_update_menu_env();
}

void enter_menu_env_op() {
	temp[0] = op_attack[fsm_env_op];
	temp[1] = op_decay[fsm_env_op];
	temp[2] = op_sustain[fsm_env_op];
	temp[3] = op_release[fsm_env_op];
	display_update_menu_env_op();
}

void enter_menu_algo() {
	temp[0] = algo;
	display_update_menu_algo();
}

void enter_menu_instr() {
	display_update_menu_instr();
}

void inc_menu_env_op() {
	if (++fsm_env_op == MAX_OPERATORS) {
		fsm_env_op = 0x00;
	}
	display_update_menu_env();
}

void inc_set_amp() {
	op_amp[fsm_op] += inc_dec;
	if (op_amp[fsm_op] > MAX_VOLUME) {
		op_amp[fsm_op] = MAX_VOLUME;
	}
	display_update_menu_amp();
}

void inc_set_ratio() {
	if (inc_dec == INC_DEC_DEFAULT) {
		op_ratio[fsm_op] += 0x01;
	}
	else {
		op_ratio[fsm_op] += 0x10;
	}
	if (op_ratio[fsm_op] > MAX_RATIO) {
		op_ratio[fsm_op] = MAX_RATIO;
	}
	display_update_menu_ratio();
}

void inc_set_detune() {
	if ((uint8_t)op_detune[fsm_op] > MAX_POS_DETUNE) {	//detune is negative
		op_detune[fsm_op] += inc_dec;
	}
	else {										//detune is positive
		op_detune[fsm_op] += inc_dec;
		if ((uint8_t)op_detune[fsm_op] > MAX_POS_DETUNE) {
			op_detune[fsm_op] = MAX_POS_DETUNE;
		}
	}
	display_update_menu_detune();
}

void inc_set_env() {
	switch (fsm_op) {
	case 0x00:
		op_attack[fsm_env_op] += inc_dec;
		if (op_attack[fsm_env_op] > ENV_MAX_RATE) {
			op_attack[fsm_env_op] = ENV_MAX_RATE;
		}
		op_attack_inc[fsm_env_op] = calculate_env_inc(op_attack[fsm_env_op]);
		break;
	case 0x01:
		op_decay[fsm_env_op] += inc_dec;
		if (op_decay[fsm_env_op] > ENV_MAX_RATE) {
			op_decay[fsm_env_op] = ENV_MAX_RATE;
		}
		op_decay_inc[fsm_env_op] = calculate_env_inc(op_decay[fsm_env_op]);
		break;
	case 0x02:
		op_sustain[fsm_env_op] += inc_dec;
		if (op_sustain[fsm_env_op] > MAX_VOLUME) {
			op_sustain[fsm_env_op] = MAX_VOLUME;
		}
		break;
	case 0x03:
		op_release[fsm_env_op] += inc_dec;
		if (op_release[fsm_env_op] > ENV_MAX_RATE) {
			op_release[fsm_env_op] = ENV_MAX_RATE;
		}
		op_release_inc[fsm_env_op] = calculate_env_inc(op_release[fsm_env_op]);
		break;
	default:
		break;
	}
	display_update_menu_env_op();
}

void inc_set_algo() {
	if (++algo == (MAX_ALGO + 1)) {
		algo = 0x00;
	}
	display_update_menu_algo();
}

void inc_set_instr() {
	if (++instrument == MAX_INSTRUMENTS) {
		instrument = 0x00;
	}
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		algo = instruments[instrument].algo;
		op_amp[i] = instruments[instrument].amp[i];
		op_ratio[i] = instruments[instrument].ratio[i];
		op_detune[i] = instruments[instrument].detune[i];
		op_attack[i] = instruments[instrument].attack[i];
		op_decay[i] = instruments[instrument].decay[i];
		op_sustain[i] = instruments[instrument].sustain[i];
		op_release[i] = instruments[instrument].release[i];
		op_attack_inc[i] = calculate_env_inc(op_attack[i]);
		op_decay_inc[i] = calculate_env_inc(op_decay[i]);
		op_release_inc[i] = calculate_env_inc(op_release[i]);
	}
	display_update_menu_instr();
}

void dec_set_amp() {
	op_amp[fsm_op] -= inc_dec;
	if (op_amp[fsm_op] > MAX_VOLUME) {
		op_amp[fsm_op] = 0x00;
	}
	display_update_menu_amp();
}

void dec_set_ratio() {
	if (inc_dec == INC_DEC_DEFAULT) {
		op_ratio[fsm_op] -= 0x01;
	}
	else {
		op_ratio[fsm_op] -= 0x10;
	}
	if (op_ratio[fsm_op] > MAX_RATIO || op_ratio[fsm_op] == 0x00) {
		op_ratio[fsm_op] = 0x01;
	}
	display_update_menu_ratio();
}

void dec_set_detune() {
	if ((uint8_t)op_detune[fsm_op] < MAX_NEG_DETUNE) {	//detune is positive
		op_detune[fsm_op] -= inc_dec;
	}
	else {										//detune is negative
		op_detune[fsm_op] -= inc_dec;
		if ((uint8_t)op_detune[fsm_op] < MAX_NEG_DETUNE) {
			op_detune[fsm_op] = MAX_NEG_DETUNE;
		}
	}
	display_update_menu_detune();
}

void dec_set_env() {
	switch (fsm_op) {
	case 0x00:
		op_attack[fsm_env_op] -= inc_dec;
		if (op_attack[fsm_env_op] > ENV_MAX_RATE || op_attack[fsm_env_op] == 0x00) {
			op_attack[fsm_env_op] = 0x01;
		}
		op_attack_inc[fsm_env_op] = calculate_env_inc(op_attack[fsm_env_op]);
		break;
	case 0x01:
		op_decay[fsm_env_op] -= inc_dec;
		if (op_decay[fsm_env_op] > ENV_MAX_RATE || op_decay[fsm_env_op] == 0x00) {
			op_decay[fsm_env_op] = 0x01;
		}
		op_decay_inc[fsm_env_op] = calculate_env_inc(op_decay[fsm_env_op]);
		break;
	case 0x02:
		op_sustain[fsm_env_op] -= inc_dec;
		if (op_sustain[fsm_env_op] > MAX_VOLUME) {
			op_sustain[fsm_env_op] = 0x00;
		}
		break;
	case 0x03:
		op_release[fsm_env_op] -= inc_dec;
		if (op_release[fsm_env_op] > ENV_MAX_RATE || op_release[fsm_env_op] == 0x00) {
			op_release[fsm_env_op] = 0x01;
		}
		op_release_inc[fsm_env_op] = calculate_env_inc(op_release[fsm_env_op]);
		break;
	default:
		break;
	}
	display_update_menu_env_op();
}

void dec_set_instr() {
	if (instrument == 0x00) {
		instrument = (MAX_INSTRUMENTS-1);
	}
	else {
		--instrument;
	}
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		algo = instruments[instrument].algo;
		op_amp[i] = instruments[instrument].amp[i];
		op_ratio[i] = instruments[instrument].ratio[i];
		op_detune[i] = instruments[instrument].detune[i];
		op_attack[i] = instruments[instrument].attack[i];
		op_decay[i] = instruments[instrument].decay[i];
		op_sustain[i] = instruments[instrument].sustain[i];
		op_release[i] = instruments[instrument].release[i];
		op_attack_inc[i] = calculate_env_inc(op_attack[i]);
		op_decay_inc[i] = calculate_env_inc(op_decay[i]);
		op_release_inc[i] = calculate_env_inc(op_release[i]);
	}
	display_update_menu_instr();
}

void temp_set_amp() {
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		uint8_t temp2 = temp[i];
		temp[i] = op_amp[i];
		op_amp[i] = temp2;
	}
	display_update_menu_amp();
}

void temp_set_ratio() {
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		uint8_t temp2 = temp[i];
		temp[i] = op_ratio[i];
		op_ratio[i] = temp2;
	}
	display_update_menu_ratio();
}

void temp_set_detune() {
	for (uint8_t i = 0; i < MAX_OPERATORS; i++) {
		uint8_t temp2 = temp[i];
		temp[i] = op_detune[i];
		op_detune[i] = temp2;
	}
	display_update_menu_detune();
}

void temp_set_env() {
	uint8_t temp2;
	temp2 = temp[0];
	temp[0] = op_attack[fsm_env_op];
	op_attack[fsm_env_op] = temp2;
	op_attack_inc[fsm_env_op] = calculate_env_inc(op_attack[fsm_env_op]);

	temp2 = temp[1];
	temp[1] = op_decay[fsm_env_op];
	op_decay[fsm_env_op] = temp2;
	op_decay_inc[fsm_env_op] = calculate_env_inc(op_decay[fsm_env_op]);

	temp2 = temp[2];
	temp[2] = op_sustain[fsm_env_op];
	op_sustain[fsm_env_op] = temp2;

	temp2 = temp[3];
	temp[3] = op_release[fsm_env_op];
	op_release[fsm_env_op] = temp2;
	op_release_inc[fsm_env_op] = calculate_env_inc(op_release[fsm_env_op]);

	display_update_menu_env_op();
}

void temp_set_algo() {
	uint8_t temp2 = temp[0];
	temp[0] = algo;
	algo = temp2;
	display_update_menu_algo();
}

void temp_set_instr() {

}

void display_update_menu_amp() {
	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	for (uint8_t op = 0; op < MAX_OPERATORS; op++) {
		strcat(params, HEX_TO_STRING[op_amp[op]]);
	}
	uint8_t index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < DISPLAY_MAX_PHYSICAL_LENGTH; i++) {
		display_convert_data(
			params[i], ui_set_converted, &index
		);
	}
	display_i2c_dma_write(ui_set_converted, UI_STRING_CONVERTED_SIZE);
}

void display_update_menu_ratio() {
	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	for (uint8_t op = 0; op < MAX_OPERATORS; op++) {
		strcat(params, HEX_TO_STRING_RATIO[op_ratio[op]]);
	}
	uint8_t index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < DISPLAY_MAX_PHYSICAL_LENGTH; i++) {
		display_convert_data(
			params[i], ui_set_converted, &index
		);
	}
	display_i2c_dma_write(ui_set_converted, UI_STRING_CONVERTED_SIZE);
}

void display_update_menu_detune() {
	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	for (uint8_t op = 0; op < MAX_OPERATORS; op++) {
		strcat(params, HEX_TO_STRING_DETUNE[(uint8_t)op_detune[op]]);
	}
	uint8_t index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < DISPLAY_MAX_PHYSICAL_LENGTH; i++) {
		display_convert_data(
			params[i], ui_set_converted, &index
		);
	}
	display_i2c_dma_write(ui_set_converted, UI_STRING_CONVERTED_SIZE);
}

void display_update_menu_env() {
	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	strcat(params, HEX_TO_STRING[fsm_env_op]);
	uint8_t index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < 4; i++) {
		display_convert_data(
			params[i], ui_menu_env_op_converted, &index
		);
	}
	display_i2c_dma_write(ui_menu_env_op_converted, UI_STRING_CONVERTED_SIZE);
}

void display_update_menu_env_op() {
	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	strcat(params, HEX_TO_STRING[op_attack[fsm_env_op]]);
	strcat(params, HEX_TO_STRING[op_decay[fsm_env_op]]);
	strcat(params, HEX_TO_STRING[op_sustain[fsm_env_op]]);
	strcat(params, HEX_TO_STRING[op_release[fsm_env_op]]);
	uint8_t index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < DISPLAY_MAX_PHYSICAL_LENGTH; i++) {
		display_convert_data(
			params[i], ui_set_env_converted, &index
		);
	}
	display_i2c_dma_write(ui_set_env_converted, UI_STRING_CONVERTED_SIZE);
}

void display_update_menu_algo() {
	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	strcat(params, HEX_TO_STRING[algo]);
	uint8_t index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < 4; i++) {
		display_convert_data(
			params[i], ui_set_algo_converted, &index
		);
	}
	display_i2c_dma_write(ui_set_algo_converted, UI_STRING_CONVERTED_SIZE);
}

void display_update_menu_instr() {
	uint8_t index = SECOND_LINE_START_INDEX - (4*6);	//index set on first line, 15th position
	display_convert_data(
		*HEX_TO_STRING[instrument], ui_set_instr_converted, &index
	);

	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	strcat(params, instruments[instrument].name);
	index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < DISPLAY_MAX_PHYSICAL_LENGTH; i++) {
		display_convert_data(
			params[i], ui_set_instr_converted, &index
		);
	}
	display_i2c_dma_write(ui_set_instr_converted, UI_STRING_CONVERTED_SIZE);
}
