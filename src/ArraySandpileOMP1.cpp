#include "ArraySandpileOMP1.hpp"

#include <stdio.h>
#include <cstring>
#include <cmath>
#include "omp.h"

ArraySandpileOMP1::ArraySandpileOMP1(int size, int globalHeight, int peakHeight, int nbThreads) : 
    ArraySandpile(size, globalHeight, peakHeight), 
    stable(false), isOnBorder(new bool[size*size]), nbThreads(nbThreads),
    start(new int[nbThreads]), end(new int[nbThreads]), subStable(new bool[nbThreads])
{
    int i, x, y;
    for (i = 0; i < size*size; i++) {
        x = i%size;
        y = i/size;
        isOnBorder[i] = (x != 0) && (x != size-1) && (y != 0) && (y != size-1);
    }
    float perThread = (float)(size*size)/nbThreads;
    for (i = 0; i < nbThreads; i++) {
        start[i] = (int)round(i * perThread);
        end[i] = (int)round((i+1) * perThread);
        subStable[i] = false;
    }
}

ArraySandpileOMP1::~ArraySandpileOMP1()
{
    delete[] isOnBorder;
    delete[] start;
    delete[] end;
    delete[] subStable;
}

void ArraySandpileOMP1::compute(int nbSteps)
{
    int tId = omp_get_thread_num();
    int i, x, y, val;
    subStable[tId] = false;
    int *g1 = grid1;
    int *g2 = grid2;
    int *tmp;
    while (0 < nbSteps) 
    {
        subStable[tId] = true;
        for (i = start[tId]; i < end[tId]; i++) {
            x = i%s;
            y = i/s;
            val = g1[i];
            // This conditions avoid the cells of the borders, which never topple
            if (isOnBorder[i] && (val >= 4)) {
                val -= 4;
                subStable[tId] = false;
            }
            if ((x != 0) && (x != s-1)) {
                val += (y-1 > 0) && (g1[i-s] >= 4);
                val += (y+1 < s-1) && (g1[i+s] >= 4);
            }
            if ((y != 0) && (y != s-1)) {
                val += (x-1 > 0) && (g1[i-1] >= 4);
                val += (x+1 < s-1) && (g1[i+1] >= 4);
            }
            g2[i] = val;
        }
        tmp = g1;
        g1 = g2;
        g2 = tmp;
        nbSteps--;
#pragma omp barrier
    }
}

bool ArraySandpileOMP1::isStable() {
    for (int i = 0; i < nbThreads; i++) {
        if (!subStable[i]) {
            return false;
        }
    }
    return true;
}

bool ArraySandpileOMP1::isOMPable() {
    return true;
}