#if 1
#include "nia.h"
#include "args.h"

#include "../benchmark/include/benchmark/benchmark.h"

niaWindow* window;

static void BM_RENDER_RECTANGLE(benchmark::State& state){
    niaRenderer renderer;

    glDrawBuffer(GL_FRONT);
    while(state.KeepRunning()){
        for(int i = 0 ; i < state.range(0); ++i){
            renderer.renderRectangle(0, 0, 100, 100);
        }
        // window->swapBuffers();
    }
}
BENCHMARK(BM_RENDER_RECTANGLE)BM_ARGS;

static void BM_RENDER_TEXTURED_RECTANGLE(benchmark::State& state){
    niaRenderer renderer;
    niaMesh mesh = niaMesh::quad(1.0);
    niaTexture texture("test.bmp");
    niaShader shader;
    while(state.KeepRunning()){
        for(int i = 0 ; i < state.range(0); ++i){
            shader.useShader();
            // renderer.renderMesh(mesh, texture);
            renderer.renderRectangleRaw(0, 0, 100, 100, texture);
            shader.unuseShader();
        }
        // window->swapBuffers();
    }
}
BENCHMARK(BM_RENDER_TEXTURED_RECTANGLE)BM_ARGS;

niaTexture* texture;
static void BM_RENDER_ANIMATED_TEXTURED_RECTANGLE(benchmark::State& state){
    niaRenderer renderer;
    niaMesh mesh = niaMesh::quad(1.0);
    while(state.KeepRunning()){
        for(int i = 0 ; i < state.range(0); ++i){
            renderer.renderMesh(mesh, *texture);
        }
        // window->swapBuffers();
    }
}
BENCHMARK(BM_RENDER_ANIMATED_TEXTURED_RECTANGLE)BM_ARGS;

// static void BM_RENDER_ANIMATED_TEXTURED_RECTANGLE(benchmark::State& state){
//     niaRenderer renderer;
//     niaTexture texture = niaTexture("test.gif", NIA_TEXTURE_ANIMATED);

//     while(state.range_x()){
//         renderer.renderMesh(niaMesh::quad(1.0), texture);
//     }
// }
// BENCHMARK(BM_RENDER_ANIMATED_TEXTURED_RECTANGLE)->Arg(1);

int main(int argc, char* argv[]){
    window->createWindow(640, 480, "Nia Benchmark");

    niaLoader::loadEverything();
    texture = new niaTexture("test.gif");
    // niaRenderer renderer;
    // renderer.pushPerspectiveView(60, 1.66, 0.01, 1000);
    // niaTransform transform;
    // transform.translate(niaVector3<float>(0, 0, -10));
    // niaEvent event;
    // while(!window->isClosed()){
    //     window->handleEvents(event);
    //     renderer.submitTransformation(transform);
    //     r32 colors[] = {0, 1, 0};
    //     // renderer.renderRectangle(0, 0, 100, 100, colors);
    //     renderer.renderMesh(niaMesh::quad(1.0));
    //     window->swapBuffers();
    // }

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();
    return 0;
}
#endif