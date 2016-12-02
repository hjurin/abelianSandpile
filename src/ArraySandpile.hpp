#ifndef ARRAY_SANDPILE_HPP
#define ARRAY_SANDPILE_HPP

#include "Sandpile.hpp"

class ArraySandpile: public Sandpile  {
    protected:
        int s;
        int *grid1;
        int *grid2;
        SDL_Event event;
        SDL_Renderer *renderer;
        SDL_Window *window;

        ArraySandpile(int size, int globalHeight, int peakHeight);
        ~ArraySandpile();
        int size();
        int height(int x, int y);
        void print(FILE *stream);
        void initSDL();
        void printSDL();
        void exitSDL();
        void nextStep();
};

#endif
