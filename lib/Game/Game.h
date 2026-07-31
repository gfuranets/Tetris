#pragma once

#include <cstdint>
#include "Tetromino.h"
#include "Grid.h"

class Game
{
public:
    Game();

    Tetromino getCurrent() const;
    Tetromino getTest() const;
    Grid getGrid() const;

    void handleInput(char movement);
    void display(uint8_t data[16]) const;

private:
    Tetromino current, test;
    Grid grid;
};
