#include "ArraySandpileSeq1.hpp"

#include <cstring>

ArraySandpileSeq1::ArraySandpileSeq1(int size, int globalHeight, int peakHeight) : 
    ArraySandpile(size, globalHeight, peakHeight),
    stable(false)
{
}

ArraySandpileSeq1::~ArraySandpileSeq1()
{
}

void ArraySandpileSeq1::compute(int nbSteps)
{
    int x, y, i;
    while (nbSteps > 0 && !stable) 
    {
        stable = true;
        memcpy(grid2, grid1, s*s * sizeof(int));
        // Note that cells of the borders never topple
        for (y = 1; y < s-1; y++) {
            for (x = 1; x < s-1; x++) 
            {
                i = s*y + x;
                if (grid1[i] >= 4) {
                    stable = 0;
                    grid2[i] -= 4;
                    grid2[i-s] += 1;
                    grid2[i+1] += 1;
                    grid2[i+s] += 1;
                    grid2[i-1] += 1;
                }
            }
        }
        nextStep();
        nbSteps--;
    }
}

bool ArraySandpileSeq1::isStable() {
    return stable;
}

bool ArraySandpileSeq1::isOMPable() {
    return false;
}