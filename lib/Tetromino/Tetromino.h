#pragma once

extern bool threePieceShapes[5][3][3];

class Tetromino
{
public:
    Tetromino() = default;
    Tetromino(char shape);

    int getSize() const;
    int getX() const;
    int getY() const;
    bool at(int i, int j) const;

    void rotate();
    void move(char dir);

private:
    bool view[4][4] {}; // Stores tetromino orientation
    int x = 0, y = 15, size = 0; 
};
