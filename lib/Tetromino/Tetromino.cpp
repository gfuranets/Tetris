#include "Tetromino.h"

const bool shapes[7][4][4] = {{{0, 0, 0, 0},
                               {0, 1, 1, 0},
                               {0, 1, 1, 0},
                               {0, 0, 0, 0}},

                              {{0, 1, 0, 0},
                               {0, 1, 1, 0},
                               {0, 0, 1, 0},
                               {0, 0, 0, 0}},

                              {{0, 0, 1, 0},
                               {0, 1, 1, 0},
                               {0, 1, 0, 0},
                               {0, 0, 0, 0}},

                              {{0, 0, 1, 0},
                               {0, 0, 1, 0},
                               {0, 1, 1, 0},
                               {0, 0, 0, 0}},
                            
                              {{0, 1, 0, 0},
                               {0, 1, 0, 0},
                               {0, 1, 1, 0},
                               {0, 0, 0, 0}},
                            
                              {{0, 1, 0, 0},
                               {0, 1, 1, 0},
                               {0, 1, 0, 0},
                               {0, 0, 0, 0}},
                            
                              {{0, 1, 0, 0},
                               {0, 1, 0, 0},
                               {0, 1, 0, 0},
                               {0, 1, 0, 0}}};

Tetromino::Tetromino() : x(2), y(15)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tetromino[i][j] = 0;
        }
    }
}

Tetromino::Tetromino(char shape) : x(2), y(15)
{
    int index;
    switch(shape)
    {
        case 'O': index = 0; break;
        case 'S': index = 1; break;
        case 'Z': index = 2; break;
        case 'L': index = 3; break;
        case 'J': index = 4; break;
        case 'T': index = 5; break;
        case 'I': index = 6; break;
        default: index = 0;
    }

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tetromino[i][j] = shapes[index][i][j];
        }
    }
}

void Tetromino::move(char dir)
{
    // Change position of top-left pixel
    switch(dir)
    {
        case 'l': x--; break;
        case 'r': x++; break;
        case 'd': y++; break;
    }
}

void Tetromino::rotate()
{
    for (int i = 0; i < 2; i++)
    {
        for (int j = i; j <= 2 - i; j++)
        {
            // Swap elements inside of a ring clockwise
            bool t = this->tetromino[i][j];
            this->tetromino[i][j] = this->tetromino[3 - j][i];
            this->tetromino[3 - j][i] = this->tetromino[3 - i][3 - j];
            this->tetromino[3 - i][3 - j] = this->tetromino[j][3 - i];
            this->tetromino[j][3 - i] = t;
        }
    }
}
