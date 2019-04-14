#include "app.h"
#include "gen3_save.h"

static PrintConsole topScreen;
static PrintConsole bottomScreen;


int main() {

	// Initialize consoles
	videoSetMode(MODE_0_2D);
	videoSetModeSub(MODE_0_2D);

	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankC(VRAM_C_SUB_BG);

	consoleInit(&topScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, true, true);
	consoleInit(&bottomScreen, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);

	// Enable accessing slot 2:
	sysSetCartOwner(true);


	gen3_save* s = new gen3_save();

	int offset = 0;
	while (true) {


		while (true) {

			swiWaitForVBlank();

			scanKeys();
			uint32 input = keysDownRepeat();

			if (input & KEY_A) {
				iprintf("Hello Patricia!\n");
				break;
			}

			if (input & KEY_START) {
				u8 gender = s->get_game_save_a()->get_trainer_info()->get_player_gender();
				iprintf("%d \n", gender);
				break;
			}
			if (input & KEY_SELECT) {

				trainer_info* ia = s->get_game_save_a()->get_trainer_info();
				trainer_info* ib = s->get_game_save_b()->get_trainer_info();

				iprintf("Changing gender\n");
				ia->set_player_gender(0x02);
				ib->set_player_gender(0x02);

				iprintf("Computing checksum\n");
				u16 checksuma = ia->compute_checksum();
				u16 checksumb = ib->compute_checksum();

				iprintf("Changing checksum\n");
				ia->set_checksum(checksuma);
				ib->set_checksum(checksumb);

				iprintf("Done.\n");
				break;
			}
			if (input & KEY_X) {
				iprintf("Writing to cartridge.\n");
				bool result = s->write_save_to_cartridge();
				iprintf("%d \n", result);
				break;
			}


		}


	}

	return 0;
}