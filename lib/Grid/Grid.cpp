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

void Grid::shift(int row)
{
    for (int j = 0; j < 8; ++j)
    {
        grid[row + 1][j] = grid[row][j];
        grid[row][j] = 0;
    }
}

void Grid::clearLines()
{

}

void Grid::lock() 
{

}
