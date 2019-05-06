#include "nia.h"
#include <list>
#include <vector>
#include <stdlib.h>

int globalId = 0;
int maxTime = 240;

class Particle {
private:
    int lifetime;
    niaVec2f position;
    niaVec3f color;
    niaVec3f velocity;

public:
    int id;
    bool alive;

    Particle();
    Particle(const niaVec2f& position);
    Particle(const Particle& other){
        alive = other.alive;
        lifetime = other.lifetime;
        id = other.id;
        color = other.color;
        position = other.position;
        velocity = other.velocity;
    }

    ~Particle();

    void update();
    void render(niaBatchRenderer* renderer);

    bool operator==(const Particle& other){
        return other.id == id;
    }
};

Particle::Particle(){}

Particle::Particle(const niaVec2f& p):
alive(true), lifetime((rand() % maxTime) + maxTime / 2), position(p), color({1, 1, 1}){
    id = globalId++;
    velocity.x = (float)((rand() % 255) / 255.0) * 2;
    velocity.y = -(float)((rand() % 255) / 255.0) * 1.1;
    if(rand() % 2 == 0){
        velocity.x *= -1;
    }
}

Particle::~Particle(){
}


void Particle::update(){
    --lifetime;
    if(!lifetime){
        alive = false;
        return;
    }
    color = {(float)lifetime / (float)maxTime, (float)lifetime / (float)maxTime, (float)lifetime / (float)maxTime};


    position.x += velocity.x;
    position.y += velocity.y;
    velocity.y += 0.01;
    velocity.x *= 0.9999999;
}

void Particle::render(niaBatchRenderer* renderer){
    renderer->renderRectangle(position.x, position.y, 10, 10, color);
}

void int2chars(char* dest, int number){
    int tmp = number;
    number = 0;

    while(tmp){
        number *= 10;
        number += tmp % 10;
        tmp /= 10;
    }

    int i = 0;
    while(number){
        dest[i++] = (number % 10) + '0';
        number /= 10;
    }
}

int main(){
    niaWindow window;

    window.createWindow(640, 480, "Particle system demo");

    niaLoader::loadEverything(); // maybe put this inside the window...

    niaBatchRenderer renderer;
    niaFontRenderer fontRenderer("xandu.ttf");
    
    renderer.pushOrthographicView(0, 640, 0, 480, 0, 1);
    
    fontRenderer.pushOrthographicView(0, 640, 0, 480, 0, 1);

    window.enableVsync();

    std::list<Particle> particles;

    char particleAmount[10] = {0};


    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);

        for(int i = 0 ; i < 20; ++i){
            particles.push_back(Particle({320, 100}));
        }

        renderer.clear();

        std::list<Particle>::iterator it = particles.begin();
        while (it != particles.end()) {
            it->update();
            if(!it->alive){
                particles.erase(it++);
                continue;
            } else {
                (it++)->render(&renderer);
            }
        }

        renderer.executeRender();

        fontRenderer.renderString(32, 32, 50, "Number of particles: ");
        int2chars(particleAmount, particles.size());
        fontRenderer.renderString(256, 32, 50, particleAmount);

        window.swapBuffers();
    }

    return 0;
}