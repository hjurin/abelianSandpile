#ifndef ARRAY_SANDPILE_OMP2_HPP
#define ARRAY_SANDPILE_OMP2_HPP

#include "ArraySandpile.hpp"

class ArraySandpileOMP2: public ArraySandpile {

    typedef struct Neighbours {
        int nb;
        int index[4];
    } Neighbours;


    protected:
        bool stable;
        Neighbours *givers;
        int nbThreads;
        int *start;
        int *end;
        bool *subStable;
    public:
        ArraySandpileOMP2(int size, int globalHeight, int peakHeight, int nbThreads);
        ~ArraySandpileOMP2();
        void compute(int nbSteps);
        bool isStable();
        bool isOMPable();
};

#endif
