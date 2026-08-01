#pragma once

#include "Tetromino.h"
#include <cstdlib>

class Grid
{
public:
    Grid() = default;
    
    bool get(int i, int j) const;
    void set(int i, int j, int val);

    bool isValid(const Tetromino& piece);
    void shift(int row);
    void clearLines();
    void lock();

private:
    bool grid[16][8] {};
};