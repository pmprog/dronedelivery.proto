@echo off

echo Game Palette File
embedconv2.exe -in aap-64.gpl -outc jamfox_gba.c -outh jamfox_gba.h -name jamfox_gba -struct -structdef "../engine/typedefs.h"

echo Logo Sprite
embedconv2.exe -in sprites_pmprog.png -outc sprites_pmprog.c -outh sprites_pmprog.h -palette aap-64.gpl -tile 8 8 -8bpp -structdef "../engine/typedefs.h"

echo Game Tiles
embedconv2.exe -in tiles_game.png -outc tiles_game.c -outh tiles_game.h -palette aap-64.gpl -tile 8 8 -8bpp -structdef "../engine/typedefs.h"

echo Processing HUD Map
embedconv2.exe -in dronehud.tmx -outc dronehud.c -outh dronehud.h
