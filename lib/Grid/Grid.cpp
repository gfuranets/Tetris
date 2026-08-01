#include "Grid.h"

bool Grid::get(int i, int j) const { return grid[i][j]; }
void Grid::set(int i, int j, int val) { grid[i][j] = val; }

bool Grid::isValid(const Tetromino& figure)
{
    for (int i = 0; i < figure.getSize(); ++i)
    {
        for (int j = 0; j < figure.getSize(); ++j)
        {
            if (!figure.at(i, j)) continue;

            int row = figure.getY() - i, col = figure.getX() + j;

            // Check out of bounds
            if (row < 0 || col < 0 || col > 7) return false; 
            // Check overlaying
            if (row < 16 && grid[row][col]) return false;
        }
    }

    return true;
}

bool Grid::isFull(int row)
{
    for (int j = 0; j < 8; ++j)
    {
        if (!grid[row][j]) return false;
    }

    return true;
}

bool Grid::isEmpty(int row)
{
    for (int j = 0; j < 8; ++j)
    {
        if (grid[row][j]) return false;
    }

    return true;
}

void Grid::shift()
{
    int w = 0;

    for (int i = 0; i < 16; ++i)
    {
        if (isEmpty(i)) continue;

        if (w != i)
        {
            for (int j = 0; j < 8; ++j)
            {
                grid[w][j] = grid[i][j];
                grid[i][j] = 0;
            }
        }

        ++w;
    }
}

void Grid::clearLines()
{
    for (int i = 0; i < 16; ++i)
    {
        if (isFull(i))
        {
            for (int j = 0; j < 8; ++j)
            {
                grid[i][j] = 0;
            }
        }
    }
}

void Grid::lock(const Tetromino& figure) 
{
    for (int i = 0; i < figure.getSize(); ++i)
    {
        for (int j = 0; j < figure.getSize(); ++j)
        {
            int row = figure.getY() - i, col = figure.getX() + j;
            if (figure.at(i, j) && row >= 0 && row < 16 && col >= 0 && col < 8)
                grid[row][col] = 1;
        }
    }
}
