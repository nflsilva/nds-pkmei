#pragma once

#include "app.h"

//Backgrounds
#include "bottomBackground.h"


//Sprites
#include "auroraTicket.h"
#include "eonTicket.h"
#include "mysticTicket.h"
#include "oldSeaMap.h"

class video_manager {
	
private:

	static video_manager& instance;
	PrintConsole topConsole;
	
	video_manager();
	
public:

	static video_manager& getInstance();
	const int BG_DMA_CHANNEL;
	const int SPRITE_DMA_CHANNEL;
    OAMTable* oam;

	void init();
	void update_oam();
	void display_bottom_background();
	
	
private:
	
	void init_video();
	void init_backgrounds();
	void init_sprites();
	void init_oam();
	
	
	class sprite_info {
		public:
			const unsigned short* pallet_ptr;
			int pallet_len;
			const unsigned int* tiles_ptr;
			int tiles_len;
			
			int x;
			int y;
			
			sprite_info(const unsigned short* p_ptr, int p_len,
						const unsigned int* t_ptr, int t_len, 
						int s_x, int s_y) : 
						
						pallet_ptr(p_ptr), pallet_len(p_len), 
						tiles_ptr(t_ptr), tiles_len(t_len),
						x(s_x), y(s_y){}
			
	};

	






};

