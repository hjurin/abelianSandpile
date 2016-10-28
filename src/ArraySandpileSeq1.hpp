#ifndef ARRAY_SANDPILE_SEQ1_HPP
#define ARRAY_SANDPILE_SEQ1_HPP

#include "ArraySandpile.hpp"

class ArraySandpileSeq1: public ArraySandpile {        
    protected:
        bool stable;
    public:
        ArraySandpileSeq1(int size, int globalHeight, int peakHeight);
        ~ArraySandpileSeq1();
        void compute(int nbSteps);
        bool isStable();
        bool isOMPable();
};

#endif
