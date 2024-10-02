#!/bin/sh

echo Controllo delle librerie necessarie
libraries=(
    "libncurses5-dev"
    "libncursesw5-dev"
    "libsdl2-2.0-0"
    "libsdl2-mixer-2.0-0"
    "libsdl2-dev"
    "libsdl2-mixer-dev"
)

is_installed() {
    dpkg -l | grep -qw "$1"
}

echo Aggiornamento della lista dei pacchetti...
sudo apt update
for lib in "${libraries[@]}"; do
    if is_installed "$lib"; then
        echo La libreria $lib è già installata.
    else
        echo La libreria $lib non è installata. Installazione in corso...
        sudo apt install -y "$lib"
    fi
done

echo Controllo e installazione delle librerie completati.
echo Inizio compilazione del TerminalTetris!
gcc Tengine_core.c -c -g
gcc Tengine_scoreboardtest.c -c -g
gcc TProject_main01.c Tengine_core.o Tengine_scoreboardtest.o -lcurses -lSDL2 -lSDL2_mixer -g -o TTetris
echo compilazione finita, buon divertimento :D
