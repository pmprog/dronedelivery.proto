#!/bin/bash

if [ -f dronedel.gba ]; then
    rm dronedel.gba
    rm dronedel.s*
fi

cd resources
./0construct.sh
cd ..

make clean && make -j4

if [ -f dronedel.gba ]; then
    # export LD_LIBRARY_PATH=/opt/Qt/5.15.2/gcc_64/lib
    mgba-qt dronedel.gba &
fi
