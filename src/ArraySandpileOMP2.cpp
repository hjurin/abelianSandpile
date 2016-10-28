#include "ArraySandpileOMP2.hpp"

#include <stdio.h>
#include <cstring>
#include <cmath>
#include "omp.h"

ArraySandpileOMP2::ArraySandpileOMP2(int size, int globalHeight, int peakHeight, int nbThreads) : 
    ArraySandpile(size, globalHeight, peakHeight), 
    stable(false), givers(new Neighbours[size*size]), nbThreads(nbThreads),
    start(new int[nbThreads]), end(new int[nbThreads]), subStable(new bool[nbThreads])
{
    int i, x, y;
    for (i = 0; i < size*size; i++) {
        x = i%size;
        y = i/size;
        givers[i].nb = 0;
        if (0 < x && x < s-1 && y >= 2) {
        givers[i].index[givers[i].nb++] = i - s;
        }
        if (0 < x && x < s-1 && y <= s-3) {
        givers[i].index[givers[i].nb++] = i + s;
        }
        if (0 < y && y < s-1 && x >= 2) {
        givers[i].index[givers[i].nb++] = i - 1;
        }
        if (0 < y && y < s-1 && x <= s-3) {
        givers[i].index[givers[i].nb++] = i + 1;
        }
    }
  
    float perThread = (float)(size*size)/nbThreads;
    for (i = 0; i < nbThreads; i++) {
        start[i] = (int)round(i * perThread);
        end[i] = (int)round((i+1) * perThread);
        subStable[i] = false;
    }
}

ArraySandpileOMP2::~ArraySandpileOMP2()
{
    delete[] givers;
    delete[] start;
    delete[] end;
    delete[] subStable;
}

void ArraySandpileOMP2::compute(int nbSteps)
{
    int tId = omp_get_thread_num();
    int i, j, val;
    subStable[tId] = false;
    int *g1 = grid1;
    int *g2 = grid2;
    int *tmp;
    while (0 < nbSteps) 
    {
        subStable[tId] = true;
        for (i = start[tId]; i < end[tId]; i++) {
            val = g1[i];
            // This conditions avoid the cells of the borders, which never topple
            if (givers[i].nb > 1 && (val >= 4)) {
                val -= 4;
                subStable[tId] = false;
            }
            for (j = 0; j < givers[i].nb; j++) {
                if (g1[givers[i].index[j]] >= 4) {
                    val += 1;
                }
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

bool ArraySandpileOMP2::isStable() {
    for (int i = 0; i < nbThreads; i++) {
        if (!subStable[i]) {
            return false;
        }
    }
    return true;
}

bool ArraySandpileOMP2::isOMPable() {
    return true;
}