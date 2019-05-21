// #ifdef SFML_BENCHMARK
#include <SFML/Graphics.hpp>
#include "../benchmark/include/benchmark/benchmark.h"

sf::RenderWindow window;

static void BM_RENDER_RECTANGLE(benchmark::State& state){
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    while(state.KeepRunning()){
        window.draw(shape);
        // window.display();
    }
}
BENCHMARK(BM_RENDER_RECTANGLE);

static void BM_RENDER_TEXTURED_RECTANGLE(benchmark::State& state){
    sf::Texture texture;
    sf::Sprite sprite;
    texture.loadFromFile("test.bmp");
    sprite.setTexture(texture);
    while(state.KeepRunning()){
        window.draw(sprite);
        // window.display();
    }
}
BENCHMARK(BM_RENDER_TEXTURED_RECTANGLE);


int main(int argc, char* argv[]){
    window.create(sf::VideoMode(200, 200), "SFML benchmark");

    ::benchmark::Initialize(&argc, argv);
    ::benchmark::RunSpecifiedBenchmarks();

    return 0;
}
// #endif