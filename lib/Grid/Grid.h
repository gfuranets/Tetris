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
    bool isEmpty(int row);
    bool isFull(int row);
    void shift();
    void clearLines();
    void lock(const Tetromino& figure);

private:
    bool grid[16][8] {};
};