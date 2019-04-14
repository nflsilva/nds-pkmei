#pragma once
#include "app.h"

// 4 KByte
#define GAME_SAVE_SECTION_BYTES 4*1024

#define GAME_SAVE_SECTION_DATA_OFFSET 0x0000
#define GAME_SAVE_SECTION_ID_OFFSET 0x0FF4
#define GAME_SAVE_SECTION_CHECKSUM_OFFSET 0x0FF6
#define GAME_SAVE_SECTION_INDEX_OFFSET 0x0FFC

#define TYPE_A_CHECKSUM_SIZE 2000
#define TYPE_B_CHECKSUM_SIZE 3848
#define TYPE_C_CHECKSUM_SIZE 3884
#define TYPE_D_CHECKSUM_SIZE 3968

class game_save_section {

public:
	game_save_section(u8* memory_pointer);
	~game_save_section();

	u16 get_id();
	u16 get_checksum();
	u32 get_index();

	void set_data_block(u8* data, int size, int offset);

	void set_id(u16 id);
	void set_checksum(u16 checksum);
	void set_index(u32 index);

	u16 compute_checksum();

private:

	u8* _data;
	u16* _id;
	u16* _checksum;
	u32* _index;

};
