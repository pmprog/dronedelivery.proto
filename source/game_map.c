
#include "game_map.h"
#include "../resources/tiles_game.h"


EWRAM_DATA u16 game_map_data_layer0[MAP_HEIGHT * MAP_WIDTH];
EWRAM_DATA u16 game_map_data_layer1[MAP_HEIGHT * MAP_WIDTH];
vec3 game_map_camera_position;
vec3 game_map_camera_start;
vec3 game_map_camera_target;
u16 game_map_camera_duration;
u16 game_map_camera_current_time;

void gamemap_generate()
{
    for(u16 y = 0; y < MAP_HEIGHT; ++y)
    {
        for(u16 x = 0; x < MAP_WIDTH; ++x)
        {
            u16 edittile = 24 + (((y >> 4) * 6) + (x >> 4));
            game_map_data_layer0[(y * MAP_WIDTH) + x] = 12  + (y == 0 || y == (MAP_HEIGHT - 1) ? 2 : 0) + (x == 0 || x == (MAP_WIDTH - 1) ? 1 : 0);
            game_map_data_layer1[(y * MAP_WIDTH) + x] = 0;
            graphics_tilemap_characterset_setpixel(edittile, COLOURS_8BPP, x % 8, y % 8, game_map_data_layer0[(y * MAP_WIDTH) + x]);
        }
    }
}

void gamemap_configuredisplay()
{
    graphics_sprites_characterset_set((u8*)tiles_game, 0, tiles_game_tile_count, COLOURS_8BPP);

    gamemap_generate();

    mapex_configure( 0, MAP_WIDTH, MAP_HEIGHT, SIZE_16, game_map_data_layer0 );
    mapex_configure( 1, MAP_WIDTH, MAP_HEIGHT, SIZE_16, game_map_data_layer1 );
    game_map_camera_position.x = 0;
    game_map_camera_position.y = 0;
    game_map_camera_position.z = 0;
}

void gamemap_setcamera(s16 X, s16 Y, s16 Z)
{
    gamemap_targetcamera( X, Y, Z, 1 );
}

void gamemap_setcamerav(vec3 Position)
{
    gamemap_setcamera(Position.x, Position.y, Position.z);
}
void gamemap_setcameravf(vec3f Position)
{
    gamemap_setcamera((s16)Position.x, (s16)Position.y, (s16)Position.z);
}

void gamemap_targetcamera(s16 X, s16 Y, s16 Z, u16 Time)
{
    game_map_camera_start.x = game_map_camera_position.x;
    game_map_camera_start.y = game_map_camera_position.y;
    game_map_camera_start.z = game_map_camera_position.z;
    game_map_camera_target.x = maths_clamp(X, 0, (MAP_WIDTH * 16) - 240);
    game_map_camera_target.y = maths_clamp(Y, 0, (MAP_HEIGHT * 16) - 160);
    game_map_camera_target.z = Z;

    game_map_camera_duration = Time; // (difx < Time && dify < Time ? maths_max(difx, dify) : Time);
    game_map_camera_current_time = 0;
}

void gamemap_targetcamerav(vec3 Position, u16 Time)
{
    gamemap_targetcamera(Position.x, Position.y, Position.z, Time);
}

void gamemap_targetcameravf(vec3f Position, u16 Time)
{
    gamemap_targetcamera((s16)Position.x, (s16)Position.y, (s16)Position.z, Time);
}

void gamemap_update()
{
    
    if( game_map_camera_current_time != game_map_camera_duration && game_map_camera_duration > 0 )
    {
        vec3 camerastep;
        float target;
        game_map_camera_current_time++;

        if( game_map_camera_current_time == game_map_camera_duration )
        {
            camerastep.x = game_map_camera_target.x;
            camerastep.y = game_map_camera_target.y;
            camerastep.z = game_map_camera_target.z;
            game_map_camera_duration = 0;
        }
        else
        {
            camerastep.x = easeoutcubic(
                                (float)game_map_camera_current_time, 
                                game_map_camera_start.x,
                                game_map_camera_target.x - game_map_camera_start.x,
                                (float)game_map_camera_duration
                                );
            camerastep.y = easeoutcubic(
                                (float)game_map_camera_current_time, 
                                game_map_camera_start.y,
                                game_map_camera_target.y - game_map_camera_start.y,
                                (float)game_map_camera_duration
                                );
            camerastep.z = easeoutcubic(
                                (float)game_map_camera_current_time, 
                                game_map_camera_start.z,
                                game_map_camera_target.z - game_map_camera_start.z,
                                (float)game_map_camera_duration
                                );
        }
        s32 deltax = camerastep.x - game_map_camera_position.x;
        s32 deltay = camerastep.y - game_map_camera_position.y;

        mapex_scrollviewport(0, deltax, deltay);
        mapex_scrollviewport(1, deltax, deltay);

        game_map_camera_position.x = camerastep.x;
        game_map_camera_position.y = camerastep.y;
        game_map_camera_position.z = camerastep.z;

    }
}
