#pragma once

#include "app.h"
#include "game_save_section.h"

#define TRAINER_INFO_GENDER_OFFSET 0x0008
#define TRAINER_INFO_GAME_CODE_OFFSET 0x00AC

class trainer_info : public game_save_section {

public:
	trainer_info(u8* memory_pointer);
	~trainer_info();

	u8 get_player_gender();
	u32 get_game_code();

	void set_player_gender(u8 gender);

private:

	u8* _player_name;
	u8* _player_gender;
	u32* _game_code;


};
