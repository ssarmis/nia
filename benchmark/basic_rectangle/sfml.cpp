// #ifdef SFML_BENCHMARK
#include <SFML/Graphics.hpp>
#include "args.h"
#include "../benchmark/include/benchmark/benchmark.h"

sf::RenderWindow window;

static void BM_RENDER_RECTANGLE(benchmark::State& state){
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while(state.KeepRunning()){
        for(int i = 0 ; i < state.range(0); ++i){
            window.draw(shape);
        }
    }
}
BENCHMARK(BM_RENDER_RECTANGLE)BM_ARGS;

static void BM_RENDER_TEXTURED_RECTANGLE(benchmark::State& state){
    sf::Texture texture;
    sf::Sprite sprite;
    texture.loadFromFile("test.bmp");
    sprite.setTexture(texture);
    while(state.KeepRunning()){
        for(int i = 0 ; i < state.range(0); ++i){
            window.draw(sprite);
        }
    }
}
BENCHMARK(BM_RENDER_TEXTURED_RECTANGLE)BM_ARGS;


int main(int argc, char* argv[]){
    window.create(sf::VideoMode(200, 200), "SFML benchmark");

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();

    return 0;
}
// #endif