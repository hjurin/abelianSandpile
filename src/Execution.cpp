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

	if (dispMode == 3) {
		pile.initSDL();
	}
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
		if (dispMode == 3) {
			// master thread manage SDL_Event
			if (omp_get_thread_num() == 0)
			{
				SDL_Event event;
				while (SDL_PollEvent(&event)) {
					if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
						pile.exitSDL();
						exit(0);
					} else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
						SDL_Delay(1000);
						while (1) {
							SDL_WaitEvent(&event);
							if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_p) {
								break;
							}
						}
					} else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
						pile.exitSDL();
						exit(0);
					}
				}
				pile.printSDL();
			}
		}
		pile.compute(batchSize);
    }
}
gettimeofday(&t2,NULL);
execTime = (double)((t2.tv_sec-t1.tv_sec)*1e3 + (t2.tv_usec-t1.tv_usec)/1e3);
  	if (dispMode > 0 && dispMode < 3) {
		fprintf(stdout, "\n\n");
		pile.print(stdout);
  	}
	if (dispMode == 3) {
		SDL_Event event;
        while (1) {
            SDL_WaitEvent(&event);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
                pile.exitSDL();
                break;
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
                pile.exitSDL();
                break;
            }
        }
	}

}

double Execution::executionTime()
{
	return execTime;
}
