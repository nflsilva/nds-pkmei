#include "trainer_info.h"


trainer_info::trainer_info(u8* memory_pointer) : game_save_section(memory_pointer),  _player_gender(NULL), _game_code(NULL) {

	_player_gender = &memory_pointer[TRAINER_INFO_GENDER_OFFSET];
	_game_code = (u32*)&memory_pointer[TRAINER_INFO_GAME_CODE_OFFSET];

}

trainer_info::~trainer_info() {




}


u8 trainer_info::get_player_gender() {
	return *_player_gender;
}
void trainer_info::set_player_gender(u8 gender) {
	*_player_gender = gender;
}


u32 trainer_info::get_game_code() {
	return *_game_code;
}

