#include "ArraySandpile.hpp"

#include <cmath>

ArraySandpile::ArraySandpile(int size, int globalHeight, int peakHeight) : 
    s(size), 
    grid1(new int[size*size]),
    grid2(new int[size*size])
{
    for (int i = 0; i < s*s; i++) {
        grid1[i] = globalHeight;
    }
    grid1[s*s/2 + s/2] = peakHeight; // Central cell
}

ArraySandpile::~ArraySandpile()
{
    delete[] grid1;
    delete[] grid2;
}

int ArraySandpile::size()
{
    return s;
}

int ArraySandpile::height(int x, int y)
{
    return grid1[s*y + x];
}

void ArraySandpile::print(FILE *stream) 
{
    /* To make the display more readable, the height of each cell is printed
     * with the same width, which must thus be computed before hand.*/
    int max = 0;
    for (int i = 0; i < s*s; i++) {
        max = (max < grid1[i]) ? grid1[i] : max;
    }
    int width = ceil(log10(max));
    for (int i = 0; i < s*s; i++) {
        fprintf(stream, "%*d ", width, grid1[i]);
        if (i % s == s-1) {
          fprintf(stream, "\n");
        }
    }
}

void ArraySandpile::nextStep() {
    int * tmp = grid1;
    grid1 = grid2;
    grid2 = tmp;
}
