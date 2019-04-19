#include "app.h"
#include "gen3_save.h"
#include "ticket.h"

static PrintConsole topScreen;
static PrintConsole bottomScreen;


char hexToChar(u8 hex) {
	if (hex > 186 && hex < 213) {
		return 65 + (hex - 186);
	}
	if (hex > 212 && hex < 239) {
		return 97 + (hex - 186);
	}
	return '0';
}

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

				rival_info* ria = s->get_game_save_a()->get_rival_info();
				rival_info* rib = s->get_game_save_b()->get_rival_info();

				u8* rivalA = ria->get_name();
				u8* rivalB = rib->get_name();

				iprintf("Rival A: ");
				for (int i = 0; i < 8; i++) {
					iprintf("%c", hexToChar(rivalA[i]));
				}
				iprintf("\n");
				iprintf("Rival B: ");
				for (int i = 0; i < 8; i++) {
					iprintf("%c", hexToChar(rivalB[i]));
				}
				iprintf("\n");

				break;
			}
			if (input & KEY_SELECT) {

				trainer_info* ia = s->get_game_save_a()->get_trainer_info();
				trainer_info* ib = s->get_game_save_b()->get_trainer_info();

				iprintf("Changing gender\n");
				ia->set_player_gender(0x01);
				ib->set_player_gender(0x01);

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

			if (input & KEY_Y) {
				iprintf("Injecting Aurora Ticket.\n");

				u8* event_data = ticket::get_e_mystic_wondercard();
				s->inject_wondercard(event_data);

				iprintf("Done!\n");
				break;
			}


		}


	}

	return 0;
}


