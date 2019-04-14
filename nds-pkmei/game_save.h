#pragma once


#include "app.h"
#include "game_save_section.h"
#include "trainer_info.h"
#include "team_items.h"
#include "game_state.h"
#include "misc_data.h"
#include "rival_info.h"
#include "pc_buffer.h"

// 56 KByte
#define GAME_SAVE_BYTES 56*1024


class game_save {

public:
	game_save(u8* memory_pointer);
	~game_save();

	trainer_info* get_trainer_info();
	team_items* get_team_items();
	game_state* get_game_state();
	misc_data* get_misc_data();
	rival_info* get_rival_info();

private:

	trainer_info* _trainer_info;
	team_items* _team_items;
	game_state* _game_state;
	misc_data* _misc_data;
	rival_info* _rival_info;
	pc_buffer** _pc_buffers;

};

