#pragma once

#include <math.h>
#include "app.h"
#include "game_save.h"


// 128KByte size
#define GEN3_SAVE_FILE_BYTES (128 * 1024)

// Offsets
#define GAME_SAVE_A_OFFSET 0x000000
#define GAME_SAVE_B_OFFSET 0x00E000
#define HALL_OF_FAME_OFFSET 0x01C000
#define MYSTERY_GIFT_OFFSET 0x01E000
#define RECORDED_BATTLE_OFFSET 0x01F000


class gen3_save {

public:

	gen3_save();
	~gen3_save();


	game_save* get_game_save_a();
	game_save* get_game_save_b();

	bool write_save_to_cartridge();



private:

	u8* _data;

	game_save* _game_save_a;
	game_save* _game_save_b;



};

