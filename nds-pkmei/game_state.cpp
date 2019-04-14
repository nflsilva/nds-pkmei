#include "game_state.h"


game_state::game_state(u8* memory_pointer) : game_save_section(memory_pointer) {

	_mystery_gift = &memory_pointer[0x67];



}

game_state::~game_state() {




}


u8 game_state::get_mystery_gift() {
	return *_mystery_gift;
}
void game_state::set_mystery_gift(u8 mystery_gift) {
	*_mystery_gift = mystery_gift;
}
