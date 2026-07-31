#include <iostream>

void rotate(bool view[4][4], int size)
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

void display(bool view[4][4], int size)
{
    for (int i = 0; i < size; ++i)
    {
        for (int j = 0; j < size; ++j)
        {
            std::cout << view[i][j] << " ";
        }
        std::cout << "\n";;
    }
}

int main()
{
    bool arr[4][4] = {{1, 1, 0}, 
                      {0, 1, 0}, 
                      {1, 0, 1}};

    display(arr, 3);
    rotate(arr, 3);
    std::cout << "Result: \n";
    display(arr, 3);
    return 0;
}