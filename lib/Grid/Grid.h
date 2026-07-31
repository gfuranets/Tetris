#pragma once

#include "Tetromino.h"
#include <cstdlib>

class Grid
{
public:
    Grid() = default;
    
    bool** getGrid() const;

    bool isValid(const Tetromino& piece);
    bool collision(const Tetromino& piece);

private:
    bool grid[16][8] {};
};