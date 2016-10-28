#ifndef ARRAY_SANDPILE_SEQ2_HPP
#define ARRAY_SANDPILE_SEQ2_HPP

#include "ArraySandpile.hpp"

class ArraySandpileSeq2: public ArraySandpile {        
    protected:
        bool stable;
        bool *isOnBorder;
    public:
        ArraySandpileSeq2(int size, int globalHeight, int peakHeight);
        ~ArraySandpileSeq2();
        void compute(int nbSteps);
        bool isStable();
        bool isOMPable();
};

#endif
