#include "rival_info.h"


rival_info::rival_info(u8* memory_pointer) : game_save_section(memory_pointer) {

	_name = &memory_pointer[0x0BCC];

}

rival_info::~rival_info() {




}

u8* rival_info::get_name() {
	return _name;
}