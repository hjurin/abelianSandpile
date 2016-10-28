#ifndef ARRAY_SANDPILE_OMP1_HPP
#define ARRAY_SANDPILE_OMP1_HPP

#include "ArraySandpile.hpp"

class ArraySandpileOMP1: public ArraySandpile {        
    protected:
        bool stable;
        bool *isOnBorder;
        int nbThreads;
        int *start;
        int *end;
        bool *subStable;
    public:
        ArraySandpileOMP1(int size, int globalHeight, int peakHeight, int nbThreads);
        ~ArraySandpileOMP1();
        void compute(int nbSteps);
        bool isStable();
        bool isOMPable();
};

#endif
