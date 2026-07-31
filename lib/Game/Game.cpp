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

Tetromino Game::getCurrent() const { return current; }
Tetromino Game::getTest() const { return test; }
Grid Game::getGrid() const { return grid; }

void Game::handleInput(char movement)
{
    Tetromino test = getTest();
    Tetromino current = getCurrent();

    switch (movement)
    {
        case 'L':
            test = current;
            test.move('L');

            if (grid.isValid(test)) 
                current = test;

            break;

        case 'R':
            test = current;
            test.move('R');

            if (grid.isValid(test)) 
                current = test;

            break;

        case 'D':
            test = current;

            while (!grid.isValid(test) && !grid.collision(test))
                test.move('D');

            current = test;
            break;

        default:
            test = current;
            test.move('D');

            if (grid.isValid(test))
                current = test;
    }
}

void Game::display(uint8_t data[16]) const
{   
    bool** g = grid.getGrid();

    for (int i = 0; i < 16; ++i)
    {
        for (int j = 0; j < 8; ++j)
        {
            data[i] |= (g[i][j] << j);
        }
    }
}