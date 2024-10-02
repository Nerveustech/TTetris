#!/bin/sh

echo Inizio compilazione del TerminalTetris!
gcc Tengine_core.c -c -g
gcc Tengine_scoreboard.c -c -g
gcc TProject_main01.c Tengine_core.o Tengine_scoreboard.o -lcurses -lSDL2 -lSDL2_mixer -g -o TTetris
echo compilazione finita, buon divertimento :D
