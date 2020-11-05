#include "app.h"

#include "gen3_save.h"
#include "ticket.h"




char hexToChar(u8 hex) {
	if (hex > 186 && hex < 213) {
		return 65 + (hex - 186);
	}
	if (hex > 212 && hex < 239) {
		return 97 + (hex - 186);
	}
	return '0';
}



void init_bottom_sprites(){
	
	NF_InitSpriteSys(1);
	
	NF_LoadSpriteGfx("sprite/auroraTicket", 0, 16, 16);
	NF_LoadSpritePal("sprite/auroraTicket", 0);
	
	NF_LoadSpriteGfx("sprite/eonTicket", 1, 16, 16);
	NF_LoadSpritePal("sprite/eonTicket", 1);
	
	NF_LoadSpriteGfx("sprite/mysticTicket", 2, 16, 16);
	NF_LoadSpritePal("sprite/mysticTicket", 2);
	
	NF_LoadSpriteGfx("sprite/oldSeaMap", 3, 16, 16);
	NF_LoadSpritePal("sprite/oldSeaMap", 3);
	
	NF_VramSpriteGfx(1, 0, 0, true);
	NF_VramSpritePal(1, 0, 0);
	
	NF_VramSpriteGfx(1, 1, 1, true);
	NF_VramSpritePal(1, 1, 1);
	
	NF_VramSpriteGfx(1, 2, 2, true);
	NF_VramSpritePal(1, 2, 2);
	
	NF_VramSpriteGfx(1, 3, 3, true);
	NF_VramSpritePal(1, 3, 3);
	
}
void draw_bottom_sprites(){
	NF_CreateSprite(1, 0, 0, 0, 26, 26);
	NF_CreateSprite(1, 1, 1, 1, 146, 26);
	NF_CreateSprite(1, 2, 2, 2, 26, 113);
	NF_CreateSprite(1, 3, 3, 3, 146, 113);
}
void cleanup_bottom_sprite_ram(){
	
	NF_UnloadSpriteGfx(0);
	NF_UnloadSpritePal(0);
	
	NF_UnloadSpriteGfx(1);
	NF_UnloadSpritePal(1);
	
	NF_UnloadSpriteGfx(2);
	NF_UnloadSpritePal(2);
	
	NF_UnloadSpriteGfx(3);
	NF_UnloadSpritePal(3);
	
}




int main() {


	
	// Init section. Only consoles 
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);
	consoleDemoInit();
	
	iprintf("nds-pkmei 0.1\n");
	iprintf("NitroFS init.");
	
	swiWaitForVBlank();
	NF_SetRootFolder("NITROFS");
	
	
	// Post-Init section
	NF_Set2D(0, 0);
	NF_Set2D(1, 0);

	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(1);
	NF_InitTextSys(1);
	
	// Init backgrounds (layer 3)
	NF_LoadTiledBg("bg/bottomBackground", "bbg", 256, 256);
	NF_CreateTiledBg(1, 3, "bbg");

	// Init sprites
	NF_InitSpriteBuffers();
	init_bottom_sprites();
	draw_bottom_sprites();
	cleanup_bottom_sprite_ram();
	
	// Init texts (layer 1)
	NF_LoadTextFont16("fnt/font16", "bottom", 256, 256, 0);
	NF_CreateTextLayer16(1, 1, 0, "bottom");
	
	NF_DefineTextColor(1, 1, 0, 31, 31, 31);
	
	NF_SetTextColor(1, 1, 0);
	NF_WriteText16(1, 1, 2, 4, "Aurora Ticket");
	NF_WriteText16(1, 1, 19, 4, "Eon Ticket");
	NF_WriteText16(1, 1, 2, 9, "Mystic Ticket");
	NF_WriteText16(1, 1, 18, 9, "Old Sea Map");

	NF_UpdateTextLayers();
	//Enable accessing slot 2:
	//sysSetCartOwner(true);


	iprintf("Entering loop\n");
	while (true) {
		
	
		NF_SpriteOamSet(0);
		NF_SpriteOamSet(1);

		NF_UpdateTextLayers();

		swiWaitForVBlank();	

		oamUpdate(&oamMain);
		oamUpdate(&oamSub);
	}

	return 0;
}


void process_inputs(){
	
	gen3_save* s = 0;//new gen3_save();
	
	scanKeys();
	uint32 input = keysDownRepeat();
	if (input & KEY_A) {
		iprintf("Hello Patricia!\n");

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

	}
	if (input & KEY_X) {
		iprintf("Writing to cartridge.\n");
		bool result = s->write_save_to_cartridge();
		iprintf("%d \n", result);

	}
	if (input & KEY_Y) {
		iprintf("Injecting Aurora Ticket.\n");

		u8* event_data = ticket::get_e_mystic_wondercard();
		s->inject_wondercard(event_data);

		iprintf("Done!\n");

	}

	
}


