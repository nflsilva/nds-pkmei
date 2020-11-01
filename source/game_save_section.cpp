#include "game_save_section.h"


game_save_section::game_save_section(u8* memory_pointer) : _data(NULL), _id(NULL), _checksum(NULL), _index(NULL) {

	_data = &memory_pointer[GAME_SAVE_SECTION_DATA_OFFSET];
	_id = (u16*)&memory_pointer[GAME_SAVE_SECTION_ID_OFFSET];
	_checksum = (u16*)&memory_pointer[GAME_SAVE_SECTION_CHECKSUM_OFFSET];
	_index = (u32*)&memory_pointer[GAME_SAVE_SECTION_INDEX_OFFSET];

}

game_save_section::~game_save_section() {



}


void game_save_section::set_data_block(u8* data, int size, int offset) {
	memcpy(&_data[offset], data, size * sizeof(u8));
}


u16 game_save_section::get_id() {
	return *_id;
}
void game_save_section::set_id(u16 id) {
	*_id = id;
}


u16 game_save_section::get_checksum() {
	return *_checksum;
}
void game_save_section::set_checksum(u16 checksum) {
	*_checksum = checksum;
}


u32 game_save_section::get_index() {
	return *_index;
}
void game_save_section::set_index(u32 index) {
	*_index = index;
}


u16 game_save_section::compute_checksum() {

	int size = 0;
	switch (*_id) {
	case 13:
		size = TYPE_A_CHECKSUM_SIZE;
		break;
	case 4:
		size = TYPE_B_CHECKSUM_SIZE;
		break;
	case 0:
		size = TYPE_C_CHECKSUM_SIZE;
		break;
	case 1: 
	case 2:
	case 3:
	case 5:
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
	case 11:
	case 12:
		size = TYPE_D_CHECKSUM_SIZE;
		break;
	}

	u32 sum = 0;
	for (int i = 0; i < (size / 4); i++) {

		sum += ((u32*)_data)[i];
	}
	u16 checksum = sum;
	checksum += sum>>16;

	return checksum;
}
