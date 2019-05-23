#ifdef SDL_BENCHMARK

#include <SDL2/SDL.h>
#include "args.h"
#include "../benchmark/include/benchmark/benchmark.h"

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;

static void BM_RENDER_RECTANGLE(benchmark::State& state){
    if(!renderer){
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    }

    SDL_Rect r;
    r.x = 50;
    r.y = 50;
    r.w = 50;
    r.h = 50;

    while(state.KeepRunning()){
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &r);
        // SDL_RenderPresent(renderer);
    }
}
BENCHMARK(BM_RENDER_RECTANGLE)BM_ARGS;

static void BM_RENDER_TEXTURED_RECTANGLE(benchmark::State& state){
    if(!renderer){
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);
    }
    SDL_Surface* surf = SDL_LoadBMP("test.bmp");
    SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, surf);
    SDL_FreeSurface(surf);
    SDL_Rect SrcR;
    SDL_Rect DestR;

    SrcR.x = 0;
    SrcR.y = 0;
    SrcR.w = 10;
    SrcR.h = 10;

    DestR.x = 0;
    DestR.y = 0;
    DestR.w = 10;
    DestR.h = 10;

    while(state.KeepRunning()){
        SDL_RenderCopy(renderer, tex, NULL, NULL);
        SDL_RenderCopy(renderer, tex, &SrcR, &DestR);
    }
}
BENCHMARK(BM_RENDER_TEXTURED_RECTANGLE)BM_ARGS;


int main(int argc, char* argv[]){
    SDL_Init(SDL_INIT_EVERYTHING);


    window = SDL_CreateWindow(
        "SDL Benchmark", SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        640,
        480,
        SDL_WINDOW_OPENGL
    );

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
#endif