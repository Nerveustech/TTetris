cmake_minimum_required(VERSION 3.10)

project(TTetris LANGUAGES C)

find_package(Curses REQUIRED)
if(NOT CURSES_FOUND)
    message(FATAL_ERROR "Libreria Curses non trovata! Installa il pacchetto di sviluppo Curses. Sui sistemi basati su Debian, puoi usare: sudo apt-get install libncurses5-dev libncursesw5-dev")
endif()

find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
    message(FATAL_ERROR "Libreria SDL2 non trovata! Installa il pacchetto di sviluppo SDL2. Sui sistemi basati su Debian, puoi usare: sudo apt-get install libsdl2-dev libsdl2-2.0-0")
endif()

find_package(SDL2_mixer REQUIRED)
if(NOT SDL2_MIXER_FOUND)
    message(FATAL_ERROR "Libreria SDL2_mixer non trovata! Installa il pacchetto di sviluppo SDL2_mixer. Sui sistemi basati su Debian, puoi usare: sudo apt-get install libsdl2-mixer-dev libsdl2-mixer-2.0-0")
endif()

set(SOURCES
    src/core/Tengine_core.c
    src/core/Tengine_scoreboard.c
    src/core/TProject_main01.c
)

add_executable(TTetris ${SOURCES})

target_include_directories(TTetris PRIVATE include/core)
target_include_directories(TTetris PRIVATE ${CURSES_INCLUDE_DIR})
target_include_directories(TTetris PRIVATE ${SDL2_INCLUDE_DIRS})
target_include_directories(TTetris PRIVATE ${SDL2_MIXER_INCLUDE_DIRS})