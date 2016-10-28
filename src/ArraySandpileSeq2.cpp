#include "ArraySandpileSeq2.hpp"

#include <cstring>

ArraySandpileSeq2::ArraySandpileSeq2(int size, int globalHeight, int peakHeight) : 
    ArraySandpile(size, globalHeight, peakHeight), 
    stable(false), isOnBorder(new bool[size*size])
{
    int i, x, y;
    for (i = 0; i < s*s; i++) {
        x = i%s;
        y = i/s;
        isOnBorder[i] = (x != 0) && (x != s-1) && (y != 0) && (y != s-1);
    }
}

ArraySandpileSeq2::~ArraySandpileSeq2()
{
    delete[] isOnBorder;
}

void ArraySandpileSeq2::compute(int nbSteps)
{
    int i, x, y;
    while (0 < nbSteps && !stable) 
    {
        stable = true;
        for (i = 0; i < s*s; i++) {
            x = i%s;
            y = i/s;
            grid2[i] = grid1[i];
            // This conditions avoid the cells of the borders, which never topple
            if (isOnBorder[i] && (grid1[i] >= 4)) {
                grid2[i] -= 4;
                stable = false;
            }
            if ((x != 0) && (x != s-1)) {
                grid2[i] += (y-1 > 0) && (grid1[i-s] >= 4);
                grid2[i] += (y+1 < s-1) && (grid1[i+s] >= 4);
            }
            if ((y != 0) && (y != s-1)) {
                grid2[i] += (x-1 > 0) && (grid1[i-1] >= 4);
                grid2[i] += (x+1 < s-1) && (grid1[i+1] >= 4);
            }
        }
        nextStep();
        nbSteps--;
    }
}

bool ArraySandpileSeq2::isStable() {
    return stable;
}

bool ArraySandpileSeq2::isOMPable() {
    return false;
}