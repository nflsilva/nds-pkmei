#pragma once

#include "app.h"
#include "game_save_section.h"

class game_state : public game_save_section {

public:
	game_state(u8* memory_pointer);
	~game_state();

	u8 get_mystery_gift();
	void set_mystery_gift(u8 mystery_gift);

private:

	u8* _mystery_gift;


};