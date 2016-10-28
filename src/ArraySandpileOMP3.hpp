#ifndef ARRAY_SANDPILE_OMP3_HPP
#define ARRAY_SANDPILE_OMP3_HPP

#include "ArraySandpile.hpp"

class ArraySandpileOMP3: public ArraySandpile {

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
        ArraySandpileOMP3(int size, int globalHeight, int peakHeight, int nbThreads);
        ~ArraySandpileOMP3();
        void compute(int nbSteps);
        bool isStable();
        bool isOMPable();
};

#endif
