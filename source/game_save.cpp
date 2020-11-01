#include "game_save.h"


game_save::game_save(u8* memory_pointer) : _trainer_info(NULL), _team_items(NULL), _game_state(NULL), 
												_misc_data(NULL), _rival_info(NULL), _pc_buffers(NULL) {

	for (int s = 0; s < 14; s++) {
		    
		int current_section_offset = (s * GAME_SAVE_SECTION_BYTES);
		u16 id = memory_pointer[current_section_offset + GAME_SAVE_SECTION_ID_OFFSET];

		u8* section_pointer = &memory_pointer[current_section_offset];
		iprintf("%u ", id);
		if (id == 0) {
			_trainer_info = new trainer_info(section_pointer);
		}
		else if (id == 2) {
			_game_state = new game_state(section_pointer);
		}
		else if (id == 4) {
			_rival_info = new rival_info(section_pointer);
		}

	}
	iprintf("---\n");

}

game_save::~game_save() {




}



trainer_info* game_save::get_trainer_info () {
	return _trainer_info;
}
team_items* game_save::get_team_items() {
	return _team_items;
}
game_state* game_save::get_game_state() {
	return _game_state;
}
misc_data* game_save::get_misc_data() {
	return _misc_data;
}
rival_info* game_save::get_rival_info() {
	return _rival_info;
}