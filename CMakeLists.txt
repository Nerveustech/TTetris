cmake_minimum_required(VERSION 3.10)

project(TTetris LANGUAGES C)

find_package(Curses REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

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

target_link_libraries(TTetris PRIVATE ${CURSES_LIBRARIES} ${SDL2_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
target_compile_options(TTetris PRIVATE -Wall -Wextra)

install(TARGETS TTetris DESTINATION bin)
install(FILES "Tetris.mp3" DESTINATION bin)