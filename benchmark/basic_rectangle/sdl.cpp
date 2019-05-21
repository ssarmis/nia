#ifdef SDL_BENCHMARK

#include <SDL2/SDL.h>
#include "../benchmark/include/benchmark/benchmark.h"

SDL_Renderer* renderer = NULL;
SDL_Window* window = NULL;

static void BM_RENDER_RECTANGLE(benchmark::State& state){
    renderer =  SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

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
BENCHMARK(BM_RENDER_RECTANGLE);

static void BM_RENDER_TEXTURED_RECTANGLE(benchmark::State& state){
    while(state.KeepRunning()){
    }
}
// BENCHMARK(BM_RENDER_TEXTURED_RECTANGLE);


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