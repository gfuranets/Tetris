#include "Grid.h"

bool Grid::isValid(const Tetromino& figure)
{
    bool** view = figure.getView();
    for (int i = 0; i < figure.getSize(); ++i)
    {
        for (int j = 0; j < figure.getSize(); ++j)
        {
            if (view[i][j] && grid[figure.getY() + i][figure.getX() + j]) 
                return false;
        }
    }

    return true;
}

bool Grid::collision(const Tetromino& figure)
{
    bool** view = figure.getView();
    for (int i = figure.getSize(); i >= 0; --i)
    {
        for (int j = 0; j < figure.getSize(); ++j)
        {
            if (view[i][j] && grid[figure.getY() - i - 1][figure.getX() + j])
                return true;
        }
    }

    return false;
}
