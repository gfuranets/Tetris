#include "Tetromino.h"

// Define shapes for figures with size = 3 - T, L, J, S, Z
bool threePieceShapes[5][3][3] = {{{ 0, 1, 0 }, 
                                   { 1, 1, 1 },
                                   { 0, 0, 0 }}, 

                                  {{ 0, 1, 0 },
                                   { 0, 1, 0 },
                                   { 0, 1, 1 }}, 

                                  {{ 0, 1, 0 },
                                   { 0, 1, 0 },
                                   { 1, 1, 0 }}, 

                                  {{ 0, 1, 1 },
                                   { 1, 1, 0 },
                                   { 0, 0, 0 }}, 

                                  {{ 1, 1, 0 },
                                   { 0, 1, 1 },
                                   { 0, 0, 0 }}};

Tetromino::Tetromino(char shape)
{
    switch (shape)
    {
        case 'O':
            size = 2, x = 3, y = 15; 
            view[0][0] = view[0][1] = view[1][0] = view[1][1] = 1;
            break;

        case 'I':
            size = 4, x = 2, y = 15;;
            for (int j = 0; j < 4; ++j) view[1][j] = 1;
            break;

        default:
            size = 3, x = 3, y = 15;
            int shapePos;
            switch (shape)
            {
                case 'T': shapePos = 0; break;
                case 'L': shapePos = 1; break;
                case 'J': shapePos = 2; break;
                case 'S': shapePos = 3; break;
                case 'Z': shapePos = 4; break;
                default: shapePos = 0;
            }

            for (int i = 0; i < 3; ++i)
            {
                for (int j = 0; j < 3; ++j)
                {
                    view[i][j] = threePieceShapes[shapePos][i][j];
                }
            }
            
    }
}

void Tetromino::rotate()
{
    for (int i = 0; i < size / 2; ++i)
    {
        for (int j = i; j < size - i - 1; ++j)
        {
            bool tmp;
            tmp = view[i][j];
            view[i][j] = view[size - j - 1][i];
            view[size - j - 1][i] = view[size - i - 1][size - j - 1];
            view[size - i - 1][size - j - 1] = view[j][size - i - 1];
            view[j][size - i - 1] = tmp;
        }
    }
}

void Tetromino::move(char dir)
{
    switch (dir)
    {
        case 'L': --x; break;
        case 'R': ++x; break;
        case 'D': --y; break;
        default: --y;
    }
}

int Tetromino::getSize() const { return size; }
int Tetromino::getX() const { return x; }
int Tetromino::getY() const { return y; }

bool** Tetromino::getView() const 
{
    bool** res;
    for (int i = 0; i < getSize(); ++i)
    {
        for (int j = 0; j < getSize(); ++j)
        {
            res[i][j] = view[i][j];
        }
    }

    return res;
}
