
#include "stage_game.h"
#include "../engine/engine.h"
#include "../resources/dronehud.h"
#include "game_map.h"

u16 drone_hover;

world_entity game_player;
world_entity game_hq_area;
world_entity game_target_area;

void game_updatemapicons();
void game_updateplayer();
void game_updatecamera();

void game_start()
{
    drone_hover = 0;
    gamemap_configuredisplay();

    for(int y = 0; y < 20; ++y)
    {
        for(int x = 0; x < 30; ++x)
        {
            graphics_tilemap_tilemap_settile(2, x, y, dronehud_hudlayer_data[(y * 30) + x]);
        }
    }

    game_player.position.x = 120;
    game_player.position.y = 60;
    game_player.position.z = 0;
    game_player.velocity.x = 0;
    game_player.velocity.y = 0;
    game_player.velocity.z = 0;

    gamemap_targetcamera(game_player.position.x - 120, game_player.position.y - 80, game_player.position.z, 1);
}

void game_finish()
{


}

void game_update()
{
    game_updateplayer();
    game_updatemapicons();

    /*
    if (game_player.velocity.x != 0 || game_player.velocity.y != 0)
    {
        gamemap_targetcamera(game_player.position.x - 120, game_player.position.y - 80, game_player.position.z, 28);
    }
    */

    int xdif = game_player.position.x - game_map_camera_position.x;
    int ydif = game_player.position.y - game_map_camera_position.y;
    if(game_map_camera_duration == 0) //if (xdif <  70 || xdif > 170 || ydif < 50 || ydif > 110 )
    {
        char logmsg[300];
        sprintf((char*)&logmsg, "CD(%d, %d)", 
            xdif, ydif);
        platform_logtext((const char*)&logmsg);

        gamemap_targetcamera(
            game_player.position.x + (game_player.velocity.x * 2.0f) - 120, 
            game_player.position.y + (game_player.velocity.y * 2.0f) -  80, 
            game_player.position.z, 40);
    }

    game_updatecamera();
}

void game_updatemapicons()
{
    u16 animframe = ((engine_stage_time >> 2) % 4) << 1;

    // Draw Player
    graphics_sprites_configure(81, 120 + animframe, COLOURS_8BPP, 0, SIZE_8x8);
    graphics_sprites_position(81, 2, 1, 
        180 + ((s32)game_player.position.x >> 5),
        114 + ((s32)game_player.position.y >> 5), 
        MIRROR_NONE);

    // Draw HQ
    graphics_sprites_configure(82, 128 + animframe, COLOURS_8BPP, 0, SIZE_8x8);
    graphics_sprites_position(82, 2, 1, 184 + ((s32)game_hq_area.position.x >> 4),
        120 + ((s32)game_hq_area.position.y >> 4), MIRROR_NONE);

    // Draw Target
    graphics_sprites_configure(80, 136 + animframe, COLOURS_8BPP, 0, SIZE_8x8);
    graphics_sprites_position(80, 2, 1, 184 + ((s32)game_target_area.position.x >> 4),
        120 + ((s32)game_target_area.position.y >> 4), MIRROR_NONE);
}

void game_updateplayer()
{
    if( input_button_held( BUTTON_R ) && game_player.velocity.z < 3 )
    {
        game_player.velocity.z += 0.5f;
    }
    if( input_button_held( BUTTON_L ) && game_player.velocity.z > -3 )
    {
        game_player.velocity.z -= 0.5f;
    }
    if(game_player.position.z > 0)
    {
        if( input_button_held(DPAD_Right) && game_player.velocity.x < 3 )
        {
            game_player.velocity.x += 0.5f;
        }
        if( input_button_held( DPAD_Left ) && game_player.velocity.x > -3 )
        {
            game_player.velocity.x -= 0.5f;
        }
        if( input_button_held( DPAD_Down ) && game_player.velocity.y < 3 )
        {
            game_player.velocity.y += 0.5f;
        }
        if( input_button_held( DPAD_Up ) && game_player.velocity.y > -3 )
        {
            game_player.velocity.y -= 0.5f;
        }
    }

    game_player.position.x += game_player.velocity.x;
    game_player.position.y += game_player.velocity.y;
    game_player.position.z += game_player.velocity.z;
    if( game_player.velocity.z >= 0.1f )
    {
        game_player.velocity.z -= 0.2f;
    } 
    else if( game_player.velocity.z <= -0.1f )
    {
        game_player.velocity.z += 0.2f;
    }
    else
    {
        game_player.velocity.z = 0;
    }

    if(game_player.position.x < 0)
    {
        game_player.position.x = 0;
        game_player.velocity.x = 0;
    }
    if (game_player.position.x >= MAP_WIDTH * 16)
    {
        game_player.position.x = (MAP_WIDTH * 16) - 1;
        game_player.velocity.x = 0;
    }

    if (game_player.position.y < 0)
    {
        game_player.position.y = 0;
        game_player.velocity.y = 0;
    }
    if (game_player.position.y >= MAP_HEIGHT * 16)
    {
        game_player.position.y = (MAP_HEIGHT * 16) - 1;
        game_player.velocity.y = 0;
    }

    if(game_player.position.z < 0)
    {
        game_player.position.z = 0;
        game_player.velocity.x = 0;
        game_player.velocity.y = 0;
        game_player.velocity.z = 0;
        // TODO: Handle ground collisions
    }
    if(game_player.position.z > 50)
    {
        game_player.position.z = 50;
        game_player.velocity.z = 0;
        // TODO: Handle ground collisions
    }

}

void game_updatecamera()
{
    gamemap_update();

    u32 viewx;
    u32 viewy;
    s32 scrollx = 0;
    s32 scrolly = 0;
    mapex_currentview(0, &viewx, &viewy);

    if (game_player.position.x >= viewx && game_player.position.x < viewx + 240 &&
        game_player.position.y >= viewy && game_player.position.y < viewy + 160 )
    {
        // Player is flying
        if (game_player.position.z != 0)
        {
            game_player.state_time = (game_player.state_time + 1) % 4;
        }
        graphics_sprites_configure(0, 240 + (game_player.state_time << 4), COLOURS_8BPP, 0, SIZE_32x16);
        graphics_sprites_configure(1, 224, COLOURS_8BPP, 0, SIZE_16x8);
        graphics_sprites_position(0, 0, 1, (s16)game_player.position.x - viewx - 16, (s16)game_player.position.y - viewy - 8 - (s16)game_player.position.z, MIRROR_NONE);
        graphics_sprites_position(1, 0, 1, (s16)game_player.position.x - viewx -  8, (s16)game_player.position.y - viewy - 4, MIRROR_NONE);
    }
    else
    {
        graphics_sprites_position(0, 0, 0, 0, 0, MIRROR_NONE);
        graphics_sprites_position(1, 0, 0, 0, 0, MIRROR_NONE);
    }


    // TODO: work out scroll from viewx,viewy to camera_target

    // TODO: Loop through sprites
    //       Do sprites fit (partially) inside camera? If so, animate and render

    
}