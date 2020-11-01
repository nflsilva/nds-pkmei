#include "gen3_save.h"



gen3_save::gen3_save() : _data(NULL), _game_save_a(NULL), _game_save_b(NULL) {

	_data = (u8*)malloc(GEN3_SAVE_FILE_BYTES * sizeof(u8));
	memset(_data, 0, GEN3_SAVE_FILE_BYTES * sizeof(u8));

	for (int bank = 0, bi = 0; bank < 2; bank++) {

		slot2_cartridge::change_sram_bank(bank);

		for (int ri = 0; ri < (GEN3_SAVE_FILE_BYTES / 2); ri++, bi++) {
			sysSetBusOwners(true, true);
			_data[bi] = SRAM[ri];
			swiDelay(10);
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

bool gen3_save::inject_wondercard(u8* wondercard) {

	// Find the latest game save
	game_save* current_game = get_game_save_a();
	if (current_game->get_trainer_info()->get_index() < get_game_save_b()->get_trainer_info()->get_index()) {
		current_game = get_game_save_b();
	}
	iprintf("Found out that %lu is the current save.\n", current_game->get_trainer_info()->get_index());

	// Find the game code
	u32 wc_offset = E_WC_OFFSET;
	u32 wc_script_offset = E_WC_SCRIPT_OFFSET;

	trainer_info* current_info = current_game->get_trainer_info();
	if (current_info->get_game_code() == RS_GAME_CODE) {
		// Ruby/Sapphire does not support Wondercards
		iprintf("Found it's a RS game.\n");
		return false;
	}
	else if (current_info->get_game_code() == FRLG_GAME_CODE) {
		wc_offset = FRLG_WC_OFFSET;
		wc_script_offset = FRLG_WC_SCRIPT_OFFSET;
		iprintf("Found out that it's FRLG\n");
	}
	rival_info* ri = current_game->get_rival_info();

	// Checksum + WC
	ri->set_data_block(wondercard, 0x4 + 0x14C, wc_offset);
	// Icon
	ri->set_data_block(&wondercard[0x4 + 0x14C + 0xA], 0x2, wc_offset + 0x4 + 0x14C + 0xA);
	// Data
	ri->set_data_block(&wondercard[0x4 + 0x14C + 0x28 + 0x28], 1004, wc_script_offset);

	// Update Checksumn
	ri->set_checksum(ri->compute_checksum());

	return true;
}

bool gen3_save::write_save_to_cartridge() {

	slot2_cartridge::erase_sram();

	// Write all data in both banks
	for (int bank = 0, bi = 0; bank < 2; bank++) {

		slot2_cartridge::change_sram_bank(bank);

		for (uint32 ri = 0; ri < (GEN3_SAVE_FILE_BYTES / 2); ri++, bi++) {
			slot2_cartridge::write_sram_byte(_data[bi], ri);
		}

	}
		
	return true;

}

