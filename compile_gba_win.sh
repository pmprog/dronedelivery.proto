#!/bin/bash
cd resources
./0construct.cmd
cd ..

make clean && make -j4
rm dronedel.s*

if [ -f dronedel.gba ]; then
    /j/Emulators/Gameboy/mGBA/mGBA.exe dronedel.gba &
fi
