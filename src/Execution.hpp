#ifndef EXECUTION_HPP
#define EXECUTION_HPP

#include "Sandpile.hpp"
#include <time.h>
#include "omp.h"

class Execution {
    public:
    	Execution(Sandpile& pile, int dispMode, int batchSize);
    	void start();
    	double executionTime();

    private:
    	Sandpile& pile;
    	int dispMode;
    	int batchSize;
    	double execTime;
    	static const struct timespec UPDATE_PERIOD;
};

#endif
