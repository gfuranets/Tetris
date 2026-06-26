#include "Grid.h"

bool Grid::isFull(int row)
{
    for (int j = 0; j < 8; j++)
    {
        if (!grid[row][j]) return false;
    }

    return true;
}

void Grid::clear(int row)
{
    for (int j = 0; j < 8; j++)
    {
        grid[row][j] = 0;
    }
}

void Grid::shiftRow(int row, Tetromino &current)
{
    for (int j = 1; j < 8; j++)
    {
        grid[current.y + row + 1][j] = grid[current.y + row][j];
    }
}

bool Grid::canPlace(const Tetromino &current)
{
    for (int i = 0; i < current.size; i++)
    {
        for (int j = 0; j < current.size; j++)
        {
            if (current.tetromino[i][j] == 1)
            {
                if (grid[current.y + i][current.x + j] == 1 || current.y + i > 16 || current.x + j < 1 || current.x + j > 9) return false;
            }
        }
    }

    return true;
}

bool Grid::checkCollision(const Tetromino &current)
{
    for (int i = current.size - 1; i >= 0; i--)
    {
        for (int j = 0; j < current.size; j++)
        {
            // 1. Current tetromino contains a block at said position
            // 2.1. There is a block on the grid below this block
            // 2.2. This block lays on the floor (y + i = height - 1)
            if (current.tetromino[i][j] && 
                (grid[current.y + i + 1][current.x + j] || current.y + i == 15))
                return true;
        }
    }

    return false;
}

void Grid::lock(const Tetromino &current)
{
    for (int i = 0; i < current.size; i++)
    {
        for (int j = 0; j < current.size; j++)
        {
            grid[current.y + i][current.x + j] = current.tetromino[i][j];
        }
    }
}
