#pragma once

#include "Tetromino.h"

class Grid
{   
public:
    // 1. Store grid # DONE
    // 2. Manage borders
    // 3. Check collisions
    // 4. Lock tetrominos (sync current - grid) # DONE
    // 5. Clear lines
    // 6. check whether movement / rotation is possible (Tetromino test = current)
    bool grid[17][10];

    bool isFull(int row);
    void clear(int row);
    void shiftRow(int row, Tetromino &current);

    bool canPlace(const Tetromino &current);
    bool checkCollision(const Tetromino &current);
    void lock(const Tetromino &current);
};
