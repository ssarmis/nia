#if 1
#include "nia.h"

#include "../benchmark/include/benchmark/benchmark.h"

niaWindow* window;

static void BM_RENDER_RECTANGLE(benchmark::State& state){
    niaRenderer renderer;

    while(state.KeepRunning()){
        renderer.renderRectangle(0, 0, 100, 100);
        window->swapBuffers();
    }
}
BENCHMARK(BM_RENDER_RECTANGLE);

static void BM_RENDER_TEXTURED_RECTANGLE(benchmark::State& state){
    niaRenderer renderer;
    niaTexture texture("test.bmp");

    while(state.KeepRunning()){
        renderer.renderMesh(niaMesh::quad(1.0), texture);
        window->swapBuffers();
    }
}
// BENCHMARK(BM_RENDER_TEXTURED_RECTANGLE);

// static void BM_RENDER_ANIMATED_TEXTURED_RECTANGLE(benchmark::State& state){
//     niaRenderer renderer;
//     niaTexture texture = niaTexture("test.gif", NIA_TEXTURE_ANIMATED);

//     while(state.range_x()){
//         renderer.renderMesh(niaMesh::quad(1.0), texture);
//     }
// }
// BENCHMARK(BM_RENDER_ANIMATED_TEXTURED_RECTANGLE)->Arg(1);

int main(int argc, char* argv[]){
    window->createWindow(640, 480, "Create Rectangle Benchmark");

    niaLoader::loadEverything();
   
    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}
#endif