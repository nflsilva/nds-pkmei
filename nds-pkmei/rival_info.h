#pragma once

#include "app.h"
#include "game_save_section.h"

class rival_info : public game_save_section {

public:
	rival_info(u8* memory_pointer);
	~rival_info();

	u8* get_name();

private:

	u8* _name;


};