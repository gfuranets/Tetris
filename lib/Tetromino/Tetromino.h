#pragma once

extern const bool shapes[7][4][4];

class Tetromino
{
private:
    int x, y;
    bool tetromino[4][4];

public:
    // 1. piece data

    Tetromino();
    Tetromino(char shape);

    void move(char dir);
    void rotate();
    
    // 2. movement
    // 3. rotation (cw)
    // 4. dropping (movement sub-type)
};
