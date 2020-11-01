#include "video_manager.h"


video_manager& video_manager::getInstance() {
	static video_manager instance;
	return instance;
}

void video_manager::init(){
	init_video();
	init_backgrounds();
	init_oam();
	init_sprites();
}

video_manager::video_manager(): BG_DMA_CHANNEL(3), SPRITE_DMA_CHANNEL(3) {
	
	
	
	
	
}

void video_manager::init_video(){
	
	/* Configure the VRAM and background control registers. */
	powerOn(POWER_ALL_2D);
	
	/* Our main screen will be at the bottom */
	lcdMainOnBottom();
	
	/*  Map VRAM to display a background on the main and sub screens. */
	vramSetBankA(VRAM_A_MAIN_BG);
	vramSetBankB(VRAM_B_MAIN_SPRITE);
	vramSetBankF(VRAM_F_MAIN_SPRITE);
	
	vramSetBankC(VRAM_C_SUB_BG);


	/*  Set the video mode on the main screen. 
	 *  We need 1 background and a few sprites
	 */
	videoSetMode(
				MODE_5_2D | 
				DISPLAY_BG3_ACTIVE |
				DISPLAY_SPR_ACTIVE |
				DISPLAY_SPR_1D);

	/*  Set the video mode on the sub screen. 
	 *  Just a console screen for now
	 */
	videoSetModeSub(MODE_0_2D);
	consoleInit(&topConsole, 3, BgType_Text4bpp, BgSize_T_256x256, 31, 0, false, true);
	
}

void video_manager::init_backgrounds(){
	
	/* Setup affine background 3 on main screen */
	REG_BG3CNT = BG_BMP16_256x256 |
			BG_BMP_BASE(0) |
			BG_PRIORITY(3);

	/* Setup affine transformation as identity matrix */
    REG_BG3PA = 1 << 8;
    REG_BG3PB = 0;
    REG_BG3PC = 0;
    REG_BG3PD = 1 << 8;
	
	/* X and Y positions as top left (0, 0) */ 
    REG_BG3X = 0;
    REG_BG3Y = 0;
	
}

void video_manager::display_bottom_background() {
	dmaCopyHalfWords(BG_DMA_CHANNEL,
		 bottomBackgroundBitmap,
		 (uint16*) BG_BMP_RAM(0),
		 bottomBackgroundBitmapLen);
}

void video_manager::init_oam(){
	
	/* Initialize all structures with zeros */
	oam = new OAMTable();
    for (int i = 0; i < SPRITE_COUNT; i++) {
        oam->oamBuffer[i].attribute[0] = ATTR0_DISABLED;
        oam->oamBuffer[i].attribute[1] = 0;
        oam->oamBuffer[i].attribute[2] = 0;
    }
	
	/* Initialize with identity matrix */
    for (int i = 0; i < MATRIX_COUNT; i++) {
        oam->matrixBuffer[i].hdx = 1 << 8;
        oam->matrixBuffer[i].hdy = 0;
        oam->matrixBuffer[i].vdx = 0;
        oam->matrixBuffer[i].vdy = 1 << 8;
    }
	
	this->update_oam();
	
}

void video_manager::init_sprites(){
	
	/*  Define some sprite configuration specific constants.
	 * 
	 *  We will use these to compute the proper index into memory for certain
	 *  tiles or palettes.
	 *
	 *  OFFSET_MULTIPLIER is calculated based on the following formula from
	 *  GBATEK (http://nocash.emubase.de/gbatek.htm#dsvideoobjs):
	 *      TileVramAddress = TileNumber * BoundaryValue
	 *  Since SPRITE_GFX is a uint16*, the compiler will increment the address
	 *  it points to by 2 for each change in 1 of the array index into
	 *  SPRITE_GFX. (The compiler does pointer arithmetic.)
	 */
    static const int BYTES_PER_16_COLOR_TILE = 32;
    static const int COLORS_PER_PALETTE = 16;
    static const int BOUNDARY_VALUE = 32; /* This is the default boundary value (can be set in REG_DISPCNT) */
    static const int OFFSET_MULTIPLIER = BOUNDARY_VALUE / sizeof(SPRITE_GFX[0]);

    /* Keep track of the available tiles */
    int next_available_tile_idx = 0;
	
	/* Setup sprites info */
	sprite_info* sprites[] = {
		new sprite_info(auroraTicketPal, auroraTicketPalLen, auroraTicketTiles, auroraTicketTilesLen, 27, 26),
		new sprite_info(eonTicketPal, eonTicketPalLen, eonTicketTiles, eonTicketTilesLen, 147, 26),
		new sprite_info(mysticTicketPal, mysticTicketPalLen, mysticTicketTiles, mysticTicketTilesLen, 27, 113),
		new sprite_info(oldSeaMapPal, oldSeaMapPalLen, oldSeaMapTiles, oldSeaMapTilesLen, 146, 113),
	};
 
    /* Create the ticket sprites. */
	for(int oam_id = 0; oam_id < 4; oam_id++){
		
		SpriteEntry* se = &oam->oamBuffer[oam_id];

		/* Configure attribute 0 */
		se->y = sprites[oam_id]->y;
		se->isRotateScale = false;
		se->isSizeDouble = false;
		se->blendMode = OBJMODE_NORMAL;
		se->isMosaic = false;
		se->colorMode = OBJCOLOR_16;
		se->shape = OBJSHAPE_SQUARE;

		/* Configure attribute 1 */
		se->x = sprites[oam_id]->x;
		se->hFlip = false;
		se->vFlip = false;
		se->size = OBJSIZE_16;

		/* Configure attribute 2 */
		se->gfxIndex = next_available_tile_idx;
		next_available_tile_idx += sprites[oam_id]->tiles_len / BYTES_PER_16_COLOR_TILE;
		se->priority = OBJPRIORITY_0;
		se->palette = oam_id;
		
		/* Copy over the sprite palettes */
		dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
						 sprites[oam_id]->pallet_ptr,
						 &SPRITE_PALETTE[oam_id * COLORS_PER_PALETTE],
						 sprites[oam_id]->pallet_len);
		
		/* Copy the sprite graphics to sprite graphics memory */
		dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
						 sprites[oam_id]->tiles_ptr,
						 &SPRITE_GFX[se->gfxIndex * OFFSET_MULTIPLIER],
						 sprites[oam_id]->tiles_len);
	}
	



}

void video_manager::update_oam(){
    DC_FlushAll();
    dmaCopyHalfWords(SPRITE_DMA_CHANNEL,
                     oam->oamBuffer,
                     OAM,
                     SPRITE_COUNT * sizeof(SpriteEntry));
}
