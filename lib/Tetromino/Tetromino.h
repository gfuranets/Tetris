#pragma once

extern bool threePieceShapes[5][3][3];

class Tetromino
{
public:
    Tetromino() = default;
    Tetromino(char shape);
    void rotate();
    void move(char dir);

    int getSize() const;
    int getX() const;
    int getY() const;
    bool** getView() const;

private:
    bool view[4][4] {}; // Stores tetromino orientation
    int x = 0, y = 0, size = 0; 
};
