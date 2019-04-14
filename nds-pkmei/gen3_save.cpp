#include "gen3_save.h"



gen3_save::gen3_save() : _data(NULL), _game_save_a(NULL), _game_save_b(NULL) {

	_data = (u8*)malloc(GEN3_SAVE_FILE_BYTES * sizeof(u8));
	memset(_data, 0, GEN3_SAVE_FILE_BYTES * sizeof(u8));

	for (int bank = 0, bi = 0; bank < 2; bank++) {

		SRAM[0x5555] = 0xAA;
		swiDelay(5);
		SRAM[0x2AAA] = 0x55;
		swiDelay(5);
		SRAM[0x5555] = 0xB0;
		swiDelay(5);
		*SRAM = (u8)bank;
		swiDelay(5);

		for (int ri = 0; ri < (GEN3_SAVE_FILE_BYTES / 2); ri++, bi++) {
			sysSetBusOwners(true, true);
			_data[bi] = SRAM[ri];
		}

	}

	_game_save_a = new game_save(&_data[GAME_SAVE_A_OFFSET]);
	_game_save_b = new game_save(&_data[GAME_SAVE_B_OFFSET]);

}

gen3_save::~gen3_save() {
	
	if (_data != NULL) {
		free(_data);
	}

}

game_save* gen3_save::get_game_save_a() {
	return _game_save_a;
}
game_save* gen3_save::get_game_save_b() {
	return _game_save_b;
}


bool gen3_save::write_save_to_cartridge() {

	// Erase all data
	SRAM[0x5555] = 0xAA;
	swiDelay(5);
	SRAM[0x2AAA] = 0x55;
	swiDelay(5);
	SRAM[0x5555] = 0x80;
	swiDelay(5);
	SRAM[0x5555] = 0xAA;
	swiDelay(5);
	SRAM[0x2AAA] = 0x55;
	swiDelay(5);
	SRAM[0x5555] = 0x10;
	swiDelay(5);
	while (*SRAM != 0xFF)
		swiDelay(5);

	// Write all data in both banks
	for (int bank = 0, bi = 0; bank < 2; bank++) {

		SRAM[0x5555] = 0xAA;
		swiDelay(5);
		SRAM[0x2AAA] = 0x55;
		swiDelay(5);
		SRAM[0x5555] = 0xB0;
		swiDelay(5);
		*SRAM = (u8)bank;
		swiDelay(5);

		sysSetBusOwners(true, true);
		for (int ri = 0; ri < (GEN3_SAVE_FILE_BYTES / 2); ri++, bi++) {

			SRAM[0x5555] = 0xAA;
			swiDelay(5);
			SRAM[0x2AAA] = 0x55;
			swiDelay(5);
			SRAM[0x5555] = 0xA0;
			swiDelay(5);

			SRAM[ri] = _data[bi];
			while (SRAM[ri] != _data[bi]) { swiDelay(5); }

		}

	}
		
	return true;

}


