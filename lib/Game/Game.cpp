#include "Game.h"
#include <cstdlib>

Game::Game()
{
    srand(2);
    int piece = rand() % 7;
    char shape;

    switch (piece)
    {
        case 0: shape = 'O'; break;
        case 1: shape = 'T'; break;
        case 2: shape = 'L'; break;
        case 3: shape = 'J'; break;
        case 4: shape = 'S'; break;
        case 5: shape = 'Z'; break;
        case 6: shape = 'I'; break;
    }

    current = Tetromino(shape);
    test = Tetromino();
    grid = Grid();
}

Tetromino* Game::getCurrent() { return &current; }
Tetromino* Game::getTest() { return &test; }
Grid Game::getGrid() const { return grid; }

void Game::handleInput(char movement)
{
    Tetromino* test = getTest();
    Tetromino* current = getCurrent();

    switch (movement)
    {
        case 'L':
            *test = *current;
            test->move('L');

            if (grid.isValid(*test)) 
                *current = *test;

            break;

        case 'R':
            *test = *current;
            test->move('R');

            if (grid.isValid(*test)) 
                *current = *test;

            break;
        
        case 'C':
            *test = *current;
            test->rotate();

            if (grid.isValid(*test))
                *current = *test;
            
            break;  

        case 'D':
            *test = *current;

            while (!grid.isValid(*test))
                test->move('D');

            *current = *test;
            break;

        default:
            *test = *current;
            test->move('D');

            if (grid.isValid(*test))
                *current = *test;
    }
}

void Game::clear(uint8_t data[16]) const
{
    for (int i = 0; i < 16; ++i) data[i] = 0; 
}

void Game::display(uint8_t data[16]) const
{   
    // Draw static grid
    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            data[i] |= (grid.get(i, j) << j);
        }
    }

    // Draw current tetromino
    for (int i = 0; i < current.getSize(); ++i)
    {
        for (int j = 0; j < current.getSize(); ++j)
        {
            int row = current.getY() - i, col = current.getX() + j;

            if (current.at(i, j) && row >= 0 && row < 16 && col >= 0 && col < 8)
                data[row] |= (1 << col);
        }
    }
}