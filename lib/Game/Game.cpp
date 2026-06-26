#include "Game.h"
#include "Config.h"
#include <cstdlib>

void Game::spawn()
{
    if (grid.checkCollision(current))
    {
        srand(2);
        char shape = shapes[rand() % 7];
        current = Tetromino(shape);
    }
}

void Game::handleInput()
{
    
}

void Game::mainLoop()
{
    for (int row = 0; row < 15; row++)
    {
        if (grid.isFull(row))
        {
            for (int next_row = row + 1; next_row < 16; next_row++)
            {
                grid.shiftRow(next_row, current);
            }
        }
    }
}
