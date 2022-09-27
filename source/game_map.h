
#pragma once

#include "../engine/engine.h"

#define MAP_WIDTH       96
#define MAP_HEIGHT      64

extern u16 game_map_data_layer0[];
extern u16 game_map_data_layer1[];

extern vec3 game_map_camera_position;
extern vec3 game_map_camera_target;

extern u16 game_map_camera_duration;
extern u16 game_map_camera_current_time;


void gamemap_generate();
void gamemap_configuredisplay();
void gamemap_setcamera(s16 X, s16 Y, s16 Z);
void gamemap_setcamerav(vec3 Position);
void gamemap_setcameravf(vec3f Position);
void gamemap_targetcamera(s16 X, s16 Y, s16 Z, u16 Time);
void gamemap_targetcamerav(vec3 Position, u16 Time);
void gamemap_targetcameravf(vec3f Position, u16 Time);
void gamemap_update();
