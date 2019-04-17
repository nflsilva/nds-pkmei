#include "app.h"
#include "gen3_save.h"

static PrintConsole topScreen;
static PrintConsole bottomScreen;

char hexToChar(u8 hex) {
	if (hex > 186 && hex < 213) {
		return 65 + (hex - 186);
	}
	if (hex > 212 && hex < 239) {
		return 97 + (hex - 186);
	}
}

u8 hexToUINT(char hex) {
	if (hex > '/' && hex < ':') {
		return (hex - '0');
	}
	else if (hex > '\'' && hex < 'g') {
		return 10 + (hex - 'a');
	}
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

				u8 frlg_aurora_raw[] = "6cd00000e803ffff000000000800000000000000000000b1bbcfccc9ccbb00cec3bdc5bfceb2000000000000000000000000000000000000000000bfecd7dcd5e2dbd900bdd5e6d800a3a1a1a500c0bbc6c6000000000000000000000000c1e300e8e300e8dcd900e7d9d7e3e2d800dae0e3e3e600e3da00e8dcd900cac9c51bc7c9c8000000bdbfc8cebfcc00d5e2d800e1d9d9e800e8dcd900d8d9e0ddead9e6ed00e4d9e6e7e3e200dde20000dbe6d9d9e2ad00ccd9d7d9ddead900e8dcd900bbcfccc9ccbb00cec3bdc5bfce00d5e2d800000000e8dcd9e200e7d5ead900e8dcd900dbd5e1d9abab0000000000000000000000000000000000000000bee300e2e3e800e8e3e7e700e8dcdde700bfecd7dcd5e2dbd900bdd5e6d800000000000000000000d6d9dae3e6d900e6d9d7d9ddeadde2db00e8dcd900bbcfccc9ccbb00cec3bdc5bfceabab00000000000000000000000000000000ffff0000000000000000000000000000000000000000000000000000000004dfc9e8163102ed1a2d941a000000000000000000000000000000000000000000000000000000005b1c000033ffffffb8282701086a5a2ba702bb018c2701082be402bb018c2701084773010100210d800100bb018c270108bd95270108666d4673010100210d800000bb01832701081a008073011a018001000900294b0829a702bdf2270108666d6c02bd84280108666d6c02bd57280108666d6c02cedcd5e2df00ede3e900dae3e600e9e7dde2db00e8dcd900c7d3cdcebfccd3fec1c3c0ce00cdede7e8d9e1adfbd3e3e900e1e9e7e800d6d900fd01adfecedcd9e6d900dde700d500e8ddd7dfd9e800dcd9e6d900dae3e600ede3e9adffc3e800d5e4e4d9d5e6e700e8e300d6d900dae3e600e9e7d900d5e800e8dcd9fed0bfccc7c3c6c3c9c800bdc3ced300e4e3e6e8adfbd1dced00e2e3e800dbddead900dde800d500e8e6ed00d5e2d800e7d9d900ebdcd5e8fedde800dde700d5d6e3e9e8acffcedcd5e2df00ede3e900dae3e600e9e7dde2db00e8dcd900c7d3cdcebfccd3fec1c3c0ce00cdede7e8d9e1adffc9dcb800c3b4e100e7e3e6e6edb800fd01ad00d3e3e9e600bcbbc1b4e7fec5bfd300c3cebfc7cd00cac9bdc5bfce00dde700dae9e0e0adfbcae0d9d5e7d900e7e8e3e6d900e7e3e1d9e8dcdde2db00e3e200ede3e9e600cabdb8fee8dcd9e200d7e3e1d900d6d5d7df00dae3e600e8dcdde7adff02000200ffff000000000000000000000000000000000000000000000000000000000000dd36000800000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000101004155524f000000000000000000000000000000000000000000000000000000000000000000000000000010100001101000010000000100000011000000011100000100000001000000001010001010100000010010001000100010000100000101000001010000010000101010001010100000001011110110000000100000001000000000111101000010100000101011111101000000010000100000001000000001001011000000001010000010100001000010100000010001000000100000000001000000000000101000101010111111010010000010101000101000010110000101100001000010100000111000011000111100010001010110000100010001000011000000001010001010101011110000000100001111010000100010000000001111000000101000001010000001000010000000010000001000000000010000001000000010100000001010101101100010001000100010001000000110000000010000001010000010100011110000000001000010000000000010000000001111010000101000101010101111000000010000001000000010001000000000111100";
				u8* frlg_aurora = (u8*)malloc(1420 * sizeof(u8));

				for (int r = 0, p = 0; p < 1420; r += 2, p++) {
					char k0 = frlg_aurora_raw[r];
					char k1 = frlg_aurora_raw[r + 1];
					u8 n0 = hexToUINT(k0);
					u8 n1 = hexToUINT(k1);
					frlg_aurora[p] = ((n0 * 16) + n1);;
					iprintf("%c%c %u %u = %u \n", k0, k1, n0, n1, (u8)frlg_aurora[p]);
				}


				int WC_OFFSET_FRLG = 0x460;
				int WC_SCRIPT_OFFSET_FRLG = 0x79C;

				rival_info* ria = s->get_game_save_a()->get_rival_info();
				rival_info* rib = s->get_game_save_b()->get_rival_info();

				// Checksum + WC?
				ria->set_data_block(frlg_aurora, 0x4 + 0x14C, WC_OFFSET_FRLG);
				rib->set_data_block(frlg_aurora, 0x4 + 0x14C, WC_OFFSET_FRLG);

				// Icon?
				ria->set_data_block(&frlg_aurora[0x4 + 0x14C + 0xA], 0x2, WC_OFFSET_FRLG + 0x4 + 0x14C + 0xA);
				rib->set_data_block(&frlg_aurora[0x4 + 0x14C + 0xA], 0x2, WC_OFFSET_FRLG + 0x4 + 0x14C + 0xA);

				// Data?
				ria->set_data_block(&frlg_aurora[0x4 + 0x14C + 0x28 + 0x28], 4 + 1000, WC_SCRIPT_OFFSET_FRLG);
				rib->set_data_block(&frlg_aurora[0x4 + 0x14C + 0x28 + 0x28], 4 + 1000, WC_SCRIPT_OFFSET_FRLG);

				// Checksumn
				ria->set_checksum(ria->compute_checksum());
				rib->set_checksum(rib->compute_checksum());

				iprintf("Done!\n");
				break;
			}


		}


	}

	return 0;
}


