
#pragma once

#include "../engine/engine.h"

typedef struct world_entity
{
    u8      type;
    u8      state;
    u8      state_time;
    vec3f   position;
    vec3f   velocity;
    u8      attached_to;
} world_entity;

void game_start();
void game_finish();
void game_update();

static const StageRecord game_stage = { game_start, game_update, game_finish };



