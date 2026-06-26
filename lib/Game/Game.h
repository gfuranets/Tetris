#pragma once

#include "Grid.h"
#include "Tetromino.h"

class Game
{
public:
    Grid grid;
    Tetromino current;

    Game();

    void spawn();
    void handleInput();
    void mainLoop();
    // 1. main game loop
    // 3. input managing (check with test tetromino + execute if possible)
    // 4. spawning pieces
    // 5. detecting game over
    // 6. timing + gravity
    // 7. render contents (managed by esp32)
};
