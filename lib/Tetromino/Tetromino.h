#pragma once

class Tetromino
{
public:
    int x, y, size;
    bool tetromino[4][4];
    Tetromino();
    Tetromino(char shape);

    void move(char dir);
    void rotate();
};
