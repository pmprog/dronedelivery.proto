
#include "../engine/engine.h"
#include "../resources/jamfox_gba.h"
#include "../resources/tiles_game.h"
#include "stage_pmprog.h"
#include "stage_game.h"

int main(void)
{
	platform_init();

	graphics_palette_set( (Colour*)jamfox_gba, 0, jamfox_gba_len);
	//graphics_palette_set( (Colour*)&jamfox_gba[1], 0, 1);	// Force black override
	graphics_mode_tilemap( 3, COLOURS_8BPP );	// Three planes. Bottom = Ground, Mid = Buildings infront, Upper = HUD
	graphics_sprites_clear();

	// Use code as a test character set
	graphics_tilemap_characterset_set((u8*)tiles_game, 0, tiles_game_tile_count, COLOURS_8BPP);

    random_init( (u8)(engine_stage_time & 0x00FF) );

	// Execute
	//engine_execute((StageRecord *)&pmprog_stage);
    engine_execute((StageRecord *)&game_stage);

	audio_music_stop();
	platform_shutdown();
}
