#pragma once

#include "app.h"



class slot2_cartridge {

public:

	static bool change_sram_bank(u8 bank);
	static bool erase_sram();
	static bool write_sram_byte(u8 byte, u32 offset);

private:


};