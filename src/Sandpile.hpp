#ifndef SANDPILE_HPP
#define SANDPILE_HPP

#include <cstdio>
#include <SDL2/SDL.h>

class Sandpile {
    public:

        virtual ~Sandpile() {}
        virtual int size() = 0;
        virtual int height(int x, int y) = 0;
        virtual void print(FILE *stream) = 0;
        virtual void initSDL() = 0;
        virtual void printSDL() = 0;
        virtual void exitSDL() = 0;
        virtual void compute(int nbSteps) = 0;
        virtual bool isStable() = 0;
        virtual bool isOMPable() = 0;

};

#endif
