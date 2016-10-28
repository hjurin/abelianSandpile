#include "Execution.hpp"

#include <sys/time.h>

#define DISPLAY_FREQUENCY 20

const struct timespec Execution::UPDATE_PERIOD = {0, 1000000000/DISPLAY_FREQUENCY};

Execution::Execution(Sandpile& pile, int dispMode, int batchSize):
	pile(pile), dispMode(dispMode), batchSize(batchSize), execTime(0)
{
	if (this->batchSize % 2 == 1) {
		this->batchSize++;
	}
}

void Execution::start()
{
	struct timeval t1,t2;
	gettimeofday(&t1,NULL);
	
#pragma omp parallel if (pile.isOMPable())
{
	while (!pile.isStable())
	{
		#pragma omp barrier
		if (dispMode == 2) {
#pragma omp single
{
			fprintf(stdout, "\n\n");
			pile.print(stdout);
			nanosleep(&UPDATE_PERIOD, NULL);
}
		}
		pile.compute(batchSize);
    }
}
  	if (dispMode > 0) {
		fprintf(stdout, "\n\n");
		pile.print(stdout);
  	}

	gettimeofday(&t2,NULL);
	execTime = (double)((t2.tv_sec-t1.tv_sec)*1e3 + (t2.tv_usec-t1.tv_usec)/1e3);
}

double Execution::executionTime()
{
	return execTime;
}