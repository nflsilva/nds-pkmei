#include "hardware_manager.h"



bool hardware_manager::change_sram_bank(u8 bank) {

	SRAM[0x5555] = 0xAA;
	swiDelay(10);
	SRAM[0x2AAA] = 0x55;
	swiDelay(10);
	SRAM[0x5555] = 0xB0;
	swiDelay(10);
	*SRAM = (u8)bank;
	swiDelay(10);

	return true;
}

bool hardware_manager::erase_sram() {

	// Erase all data
	SRAM[0x5555] = 0xAA;
	swiDelay(10);
	SRAM[0x2AAA] = 0x55;
	swiDelay(10);
	SRAM[0x5555] = 0x80;
	swiDelay(10);
	SRAM[0x5555] = 0xAA;
	swiDelay(10);
	SRAM[0x2AAA] = 0x55;
	swiDelay(10);
	SRAM[0x5555] = 0x10;
	swiDelay(10);
	while (*SRAM != 0xFF) { swiDelay(10); }

	return true;
}

bool hardware_manager::write_sram_byte(u8 byte, u32 offset) {

	sysSetBusOwners(true, true);

	SRAM[0x5555] = 0xAA;
	swiDelay(10);
	SRAM[0x2AAA] = 0x55;
	swiDelay(10);
	SRAM[0x5555] = 0xA0;
	swiDelay(10);

	SRAM[offset] = byte;
	swiDelay(10);

	while (SRAM[offset] != byte) { swiDelay(10); }

	return true;
}
