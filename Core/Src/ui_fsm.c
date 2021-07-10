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
	{pb_2				, menu_fdbk			, select_menu_fdbk		},
	{invalid			, menu_env			, input_invalid			}
};
const transition menu_env_op_transitions[] = {
	{pb_0				, menu_env			, select_menu_env		},
	{pb_1				, set_env			, enter_menu_env_op		},
	{pb_2				, menu_env_op		, inc_menu_env_op		},
	{invalid			, menu_env_op		, input_invalid			}
};
const transition menu_fdbk_transitions[] = {
	{pb_0				, menu_env			, select_menu_env		},
	{pb_1				, set_fdbk			, enter_menu_fdbk		},
	{pb_2				, menu_lfo			, select_menu_lfo		},
	{invalid			, menu_fdbk			, input_invalid			}
};
const transition menu_lfo_transitions[] = {
	{pb_0				, menu_fdbk			, select_menu_fdbk		},
	{pb_1				, set_lfo			, enter_menu_lfo		},
	{pb_2				, menu_algo			, select_menu_algo		},
	{invalid			, menu_env			, input_invalid			}
};
const transition menu_algo_transitions[] = {
	{pb_0				, menu_lfo			, select_menu_lfo		},
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
const transition set_fdbk_transitions[] = {
	{pb_0				, menu_fdbk			, select_menu_fdbk		},
	{pb_1				, set_fdbk			, temp_set_fdbk			},
	{pb_2				, set_fdbk			, change_inc_dec		},
	{rot_inc			, set_fdbk			, inc_set_fdbk			},
	{rot_dec			, set_fdbk			, dec_set_fdbk			},
	{invalid			, set_fdbk			, input_invalid			}
};
const transition set_lfo_transitions[] = {
	{pb_0				, menu_lfo			, select_menu_lfo		},
	{pb_1				, set_lfo			, temp_set_lfo			},
	{pb_2				, set_lfo			, change_inc_dec		},
	{rot_inc			, set_lfo			, inc_set_lfo			},
	{rot_dec			, set_lfo			, dec_set_lfo			},
	{invalid			, set_lfo			, input_invalid			}
};
const transition set_algo_transitions[] = {
	{pb_0				, menu_algo			, select_menu_algo		},
	{pb_1				, set_algo			, temp_set_algo			},
	{pb_2				, set_algo			, change_inc_dec		},
	{rot_inc			, set_algo			, inc_set_algo			},
	{rot_dec			, set_algo			, dec_set_algo			},
	{invalid			, set_algo			, input_invalid			}
};
const transition set_instr_transitions[] = {
	{pb_0				, menu_instr		, select_menu_instr		},
	{pb_1				, set_instr			, temp_set_instr		},
	{pb_2				, set_instr			, change_inc_dec		},
	{rot_inc			, set_instr			, inc_set_instr			},
	{rot_dec			, set_instr			, dec_set_instr			},
	{invalid			, set_instr			, input_invalid			}
};
const transition* fsm_transition_table[NUM_OF_STATES] = {
	menu_amp_transitions,
	menu_ratio_transitions,
	menu_detune_transitions,
	menu_env_transitions,
	menu_env_op_transitions,
	menu_fdbk_transitions,
	menu_lfo_transitions,
	menu_algo_transitions,
	menu_instr_transitions,
	set_amp_transitions,
	set_ratio_transitions,
	set_detune_transitions,
	set_env_transitions,
	set_fdbk_transitions,
	set_lfo_transitions,
	set_algo_transitions,
	set_instr_transitions,
};

const uint8_t ui_menu_amp[DISPLAY_MAX_PHYSICAL_LENGTH] = "instr ~amp ratio";
const uint8_t ui_menu_ratio[DISPLAY_MAX_PHYSICAL_LENGTH] = " amp ~ratio detu";
const uint8_t ui_menu_detune[DISPLAY_MAX_PHYSICAL_LENGTH] = "tio ~detune env ";
const uint8_t ui_menu_env[DISPLAY_MAX_PHYSICAL_LENGTH] = "etune ~env fdbk ";
const uint8_t ui_menu_env_op[DISPLAY_MAX_PHYSICAL_LENGTH] = "SELECT OPERATOR:";
const uint8_t ui_menu_fdbk[DISPLAY_MAX_PHYSICAL_LENGTH] = " env ~fdbk lfo a";
const uint8_t ui_menu_lfo[DISPLAY_MAX_PHYSICAL_LENGTH] = " fdbk ~lfo algo ";
const uint8_t ui_menu_algo[DISPLAY_MAX_PHYSICAL_LENGTH] = " lfo ~algo instr";
const uint8_t ui_menu_instr[DISPLAY_MAX_PHYSICAL_LENGTH] = "algo ~instr amp ";
const uint8_t ui_set[DISPLAY_MAX_PHYSICAL_LENGTH] = "OP1 OP2 OP3 OP4 ";
const uint8_t ui_set_algo[DISPLAY_MAX_PHYSICAL_LENGTH] = "SELECT ALGO:    ";
const uint8_t ui_set_env[DISPLAY_MAX_PHYSICAL_LENGTH] = "ATK DEC SUS REL ";
const uint8_t* ui_string_table[NUM_OF_UI_STRINGS] = {
	ui_menu_amp,
	ui_menu_ratio,
	ui_menu_detune,
	ui_menu_env,
	ui_menu_env_op,
	ui_menu_fdbk,
	ui_menu_lfo,
	ui_menu_algo,
	ui_menu_instr,
	ui_set,
	ui_set_algo,
	ui_set_env
};
uint8_t* ui_string_table_converted[NUM_OF_UI_STRINGS] = {
	ui_menu_amp_converted,
	ui_menu_ratio_converted,
	ui_menu_detune_converted,
	ui_menu_env_converted,
	ui_menu_env_op_converted,
	ui_menu_fdbk_converted,
	ui_menu_lfo_converted,
	ui_menu_algo_converted,
	ui_menu_instr_converted,
	ui_set_converted,
	ui_set_algo_converted,
	ui_set_env_converted
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
	if (inc_dec == 0x01) {
		inc_dec = 0x08;
	}
	else {
		inc_dec = 0x01;
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

void select_menu_fdbk() {
	display_i2c_dma_write(ui_menu_fdbk_converted, UI_STRING_CONVERTED_SIZE);
}

void select_menu_lfo() {
	display_i2c_dma_write(ui_menu_lfo_converted, UI_STRING_CONVERTED_SIZE);
}

void select_menu_algo() {
	display_i2c_dma_write(ui_menu_algo_converted, UI_STRING_CONVERTED_SIZE);
}

void select_menu_instr() {
	display_i2c_dma_write(ui_menu_instr_converted, UI_STRING_CONVERTED_SIZE);
}

void enter_menu_amp() {
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

void enter_menu_ratio() {
	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	for (uint8_t op = 0; op < MAX_OPERATORS; op++) {
		strcat(params, HEX_TO_STRING[op_ratio[op]]);
	}
	uint8_t index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < DISPLAY_MAX_PHYSICAL_LENGTH; i++) {
		display_convert_data(
			params[i], ui_set_converted, &index
		);
	}
	display_i2c_dma_write(ui_set_converted, UI_STRING_CONVERTED_SIZE);
}

void enter_menu_detune() {
	char params[DISPLAY_MAX_PHYSICAL_LENGTH+1] = "";
	for (uint8_t op = 0; op < MAX_OPERATORS; op++) {
		strcat(params, HEX_TO_STRING[op_detune[op]]);
	}
	uint8_t index = SECOND_LINE_START_INDEX;
	for (uint8_t i = 0; i < DISPLAY_MAX_PHYSICAL_LENGTH; i++) {
		display_convert_data(
			params[i], ui_set_converted, &index
		);
	}
	display_i2c_dma_write(ui_set_converted, UI_STRING_CONVERTED_SIZE);
}

void enter_menu_env() {
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

void enter_menu_env_op() {
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

void enter_menu_fdbk() {

}

void enter_menu_lfo() {

}

void enter_menu_algo() {
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

void enter_menu_instr() {

}

void inc_menu_env_op() {

}
void inc_set_amp() {

}
void inc_set_ratio() {

}
void inc_set_detune() {

}
void inc_set_env() {

}
void inc_set_fdbk() {

}
void inc_set_lfo() {

}
void inc_set_algo() {

}
void inc_set_instr() {

}

void dec_set_amp() {

}
void dec_set_ratio() {

}
void dec_set_detune() {

}
void dec_set_env() {

}
void dec_set_fdbk() {

}
void dec_set_lfo() {

}
void dec_set_algo() {

}
void dec_set_instr() {

}

void temp_set_amp() {

}
void temp_set_ratio() {

}
void temp_set_detune() {

}
void temp_set_env() {

}
void temp_set_fdbk() {

}
void temp_set_lfo() {

}
void temp_set_algo() {

}
void temp_set_instr() {

}
