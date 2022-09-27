
#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define tiles_game_len 30720
#define tiles_game_width 192
#define tiles_game_height 160
#define tiles_game_tile_width 8
#define tiles_game_tile_height 8
#define tiles_game_tile_count 480
extern const uint8_t tiles_game[];

#ifdef __cplusplus
}
#endif
