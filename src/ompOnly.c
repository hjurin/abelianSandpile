#define _XOPEN_SOURCE 600

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include <sys/time.h>
#include <SDL2/SDL.h>
#include "omp.h"


#define TIME_DIFF(t1, t2) \
  ((double)((t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec)))

#define DEFAULT_SIZE 128
#define DEFAULT_DISPLAY 1
#define DEFAULT_INIT_CONFIG 0
#define DEFAULT_VERSION 1

#define HOMO_INIT_HEIGHT 5
#define PEAK_INIT_HEIGHT 100000

#define DISPLAY_FREQUENCY 20

SDL_Event event;
SDL_Renderer *renderer;
SDL_Window *window;

/* Initializes the specified sandpile to initial configuration 0
   (homogeneous height HOMO_INIT_HEIGHT). */
void homoInit(unsigned * sandpile, unsigned size)
{
    for (int i = 0; i < size*size; i++) {
        sandpile[i] = HOMO_INIT_HEIGHT;
    }
}

/* Initializes the specified sandpile to initial configuration 1
   (homogeneous height 0 except PEAK_INIT_HEIGHT in the middle). */
void peakInit(unsigned * sandpile, unsigned size)
{
    for (int i = 0; i < size*size; i++) {
        sandpile[i] = HOMO_INIT_HEIGHT;
    }
    sandpile[size*(size/2) + size/2] = PEAK_INIT_HEIGHT;
}

typedef void (*initFunc)(unsigned *, unsigned);
initFunc initFuncs[2] = { homoInit, peakInit };

void initSDL(unsigned s)
{
    if (s > 400) {
        SDL_CreateWindowAndRenderer(s-2, s-2, 0, &window, &renderer);
    } else {
        int ratio = 900 / s;
        SDL_CreateWindowAndRenderer((s - 2) * ratio, (s - 2) * ratio, 0, &window, &renderer);
    }
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
}

void printSDL(unsigned * array, unsigned s)
{
    for (int i = 0; i < s*s; i++) {
        if (array[i] > 7) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, array[i]*20, 0, 0, 255);

        }
        if (s > 400) {
            SDL_RenderDrawPoint(renderer, i%s-1, i/s-1);
        } else {
            SDL_Rect rect;
            rect.w = 900 / s;
            rect.h = rect.w;
            rect.x = (i%s - 1) * rect.w;
            rect.y = (i/s - 1) * rect.w;
            SDL_RenderFillRect(renderer, &rect);
        }
    }
    SDL_RenderPresent(renderer);
}

void exitSDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void print (unsigned * array, unsigned s)
{
    int max = 0;
    int val;
    for (int y = 0; y < s; y++) {
        for (int x = 0; x < s; x++) {
            val = array[s*y + x];
            max = (val > max) ? val : max;
        }
    }
    int width = ceil(log10(max));
    for (int i = 0; i < s; i++) {
        fprintf(stderr, "\n");
    }
    for (int y = 0; y < s; y++) {
        for (int x = 0; x < s; x++) {
            fprintf(stderr, "%*d ", width, array[s*y+x]);
        }
        fprintf(stderr, "\n");
    }
}


int nextStep(unsigned ** arrays, unsigned start, unsigned end, unsigned step, unsigned size)
{
    int val, i, x, y;
    int keepOn = 0;
    for (i = start; i < end; i++)
    {
        x = i % size;
        y = i / size;
        val = arrays[step%2][i];
        // This conditions avoid the cells of the borders, which never topple
        if ((x != 0) && (x != size-1) && (y != 0) && (y != size-1) && (val >= 4)) {
            val -= 4;
            keepOn = 1;
        }
        if ((x != 0) && (x != size-1)) {
            val += (y-1 > 0) && (arrays[step%2][i-size] >= 4);
            val += (y+1 < size-1) && (arrays[step%2][i+size] >= 4);
        }
        if ((y != 0) && (y != size-1)) {
            val += (x-1 > 0) && (arrays[step%2][i-1] >= 4);
            val += (x+1 < size-1) && (arrays[step%2][i+1] >= 4);
        }
        arrays[(step+1)%2][i] = val;
    }
    return keepOn;
}

int main(int argc, char **argv) {
    struct timeval t1,t2;
    gettimeofday(&t1,NULL);

    int displayMode = DEFAULT_DISPLAY;
    int size = DEFAULT_SIZE;
    int initConfig = DEFAULT_INIT_CONFIG;

    int opt;
    while ((opt = getopt(argc, argv, "d:s:c:")) != -1) {
        switch (opt) {
            /* display mode
            0 : none
            1 : final state
            2 : all states
            3 : SDL */
            case 'd':
            displayMode = atoi(optarg);
            if (displayMode < 0 || displayMode > 3) {
                fprintf(stderr, "Wrong argument for option -d (display mode). Please use :\n");
                fprintf(stderr, "0 for no display, \n");
                fprintf(stderr, "1 for final state only, \n");
                fprintf(stderr, "2 for all states. \n");
                fprintf(stderr, "3 for SDL. \n");
                abort();
            }
            break;

            /* Size of the Sandpile*/
            case 's':
            size = atoi(optarg);
            if (size < 0) {
                fprintf(stderr, "Wrong argument for option -s (Sandpile size). Please use positive integers.\n");
                abort();
            }
            break;

            /* Initial configuration of the SandPile*/
            case 'c':
            initConfig = atoi(optarg);
            if (initConfig < 0 || initConfig > 1) {
                fprintf(stderr, "Wrong argument for option -c (initial configuration). Please use :\n");
                fprintf(stderr, "0 for an homogenous configuration,\n");
                fprintf(stderr, "1 for a peak configuration.\n");
                abort();
            }
            break;
            case '?':
            if (optopt == 'd' || optopt == 'v' || optopt == 's' || optopt == 'c') {
                fprintf(stderr, "Option -%c requires an argument.\n", optopt);
            }
            else if (isprint (optopt)) {
                fprintf(stderr, "Unknown option `-%c'.\n", optopt);
            }
            else {
                fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
            }
            return 1;
            default:
            abort();
        }
    }


    unsigned * current = (unsigned*)malloc(size*size*sizeof(unsigned));
    initFuncs[initConfig](current, size);
    unsigned * next = (unsigned*)malloc(size*size*sizeof(unsigned));
    unsigned * arrays[2] = {current, next};

    int nbThreads = omp_get_max_threads();
    float perThread = (float)(size*size)/nbThreads;
    int start[nbThreads];
    int end[nbThreads];
    for (int i = 0; i < nbThreads; i++) {
        start[i] = (int)round(i * perThread);
        end[i] = (int)round((i+1) * perThread);
    }

    int keepOn = 1;
    int keepOns[nbThreads];

    if (displayMode == 3) {
        initSDL(size);
    }

    #pragma omp parallel
    {
        int tId = omp_get_thread_num();
        int step = 0;
        while(keepOn) {
            keepOns[tId] = nextStep(arrays, start[tId], end[tId], step, size);
            step++;
            #pragma omp barrier
            // print if needed
            if (displayMode == 2) {
                #pragma omp single
                print(arrays[step%2], size);
            }
            if (displayMode == 3) {
                // master thread manage SDL_Event
                if (tId == 0)
                {
                    SDL_Event event;
                    while (SDL_PollEvent(&event)) {
                        if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
                            exitSDL();
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
                            exitSDL();
                            exit(0);
                        }
                    }
                    printSDL(arrays[step%2], size);
                }
            }
            // Checks for stability every once in a while
            if (step % 100 == 0) {
                #pragma omp single
                {
                    keepOn = 0;
                    for (int i = 0; i < nbThreads; i++) {
                        keepOn = keepOn || keepOns[i];
                    }
                }
            }
        }

        #pragma omp single
        {
            gettimeofday(&t2,NULL);
            if (displayMode > 0 && displayMode < 3) {
                print(arrays[step%2], size);
            }
            printf("\n%g\n", TIME_DIFF(t1,t2) / 1000);
        }
    }

    if (displayMode == 3) {
        SDL_Event event;
        while (1) {
            SDL_WaitEvent(&event);
            if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_q) {
                exitSDL();
                break;
            } else if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {
                exitSDL();
                break;
            }
        }
    }

    free(current);
    free(next);
}
