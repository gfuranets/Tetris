#include "Tetromino.h"
#include "Config.h"

Tetromino::Tetromino() : x(0), y(0), size(0)
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            tetromino[i][j] = 0;
        }
    }
}

Tetromino::Tetromino(char shape) : x(3), y(0)
{
    switch(shape)
    {
        case 'O':
            size = 2;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    tetromino[i][j] = 1;
                }
            }
            break;

        case 'I':
            size = 4;
            for (int i = 0; i < size; i++)
            {
                for (int j = 0; j < size; j++)
                {
                    if (i == 1) tetromino[i][j] = 1;
                    else tetromino[i][j] = 0;
                }
            }
            break;
        
        default:
            size = 3;
            switch(shape)
            {
                int index;

                case 'T': index = 0; break;
                case 'J': index = 1; break;
                case 'L': index = 2; break;
                case 'S': index = 3; break;
                case 'Z': index = 4; break;
                default: index = 0;

                for (int i = 0; i < size; i++)
                {
                    for (int j = 0; j < size; j++)
                    {
                        tetromino[i][j] = shapes[index][i][j];
                    }
                }
                
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
