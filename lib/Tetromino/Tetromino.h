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
    bool** getView() const;

    void rotate();
    void move(char dir);

private:
    bool view[4][4] {}; // Stores tetromino orientation
    int x = 0, y = 0, size = 0; 
};
