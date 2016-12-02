#include "ArraySandpile.hpp"

#include <cmath>

ArraySandpile::ArraySandpile(int size, int globalHeight, int peakHeight) :
    s(size),
    grid1(new int[size*size]),
    grid2(new int[size*size])
{
    for (int i = 0; i < s*s; i++) {
        grid1[i] = globalHeight;
    }
    grid1[s*s/2 + (1 - s%2)*s/2] = peakHeight; // Central cell
}

ArraySandpile::~ArraySandpile()
{
    delete[] grid1;
    delete[] grid2;
}

int ArraySandpile::size()
{
    return s;
}

int ArraySandpile::height(int x, int y)
{
    return grid1[s*y + x];
}

void ArraySandpile::print(FILE *stream)
{
    /* To make the display more readable, the height of each cell is printed
     * with the same width, which must thus be computed before hand.*/
    int max = 0;
    for (int i = 0; i < s*s; i++) {
        max = (max < grid1[i]) ? grid1[i] : max;
    }
    int width = ceil(log10(max));
    for (int i = 0; i < s*s; i++) {
        fprintf(stream, "%*d ", width, grid1[i]);
        if (i % s == s-1) {
          fprintf(stream, "\n");
        }
    }
}

void ArraySandpile::initSDL()
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

void ArraySandpile::printSDL()
{
    for (int i = 0; i < s*s; i++) {
        if (grid1[i] > 7) {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        }
        else {
            SDL_SetRenderDrawColor(renderer, grid1[i]*20, 0, 0, 255);

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

void ArraySandpile::exitSDL()
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void ArraySandpile::nextStep() {
    int * tmp = grid1;
    grid1 = grid2;
    grid2 = tmp;
}
