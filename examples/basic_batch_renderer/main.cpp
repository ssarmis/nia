#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"
#include "nia_loader.h"

#include <GL/glext.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
    float vx;
    float vy;
    float x;
    float y;
    float w;
    float h;
    float r;
    float g;
    float b;
} rect;

rect rectangles[1024 * 1024];

int main() {

    niaWindow window;
    window.createWindow(1024, 1024, "no matter");

    niaLoadEverything();

    window.enableVsync();
    

    niaBatchRenderer renderer;

    srand(time(NULL));
    int i = 0;
    for (float y = -5; y < 5; y += 0.03){
        for (float x = -5; x < 5; x += 0.03){
            rectangles[i].r = ((double) rand() / (RAND_MAX));
            rectangles[i].g = ((double) rand() / (RAND_MAX));
            rectangles[i].b = ((double) rand() / (RAND_MAX));

            float speed = ((double) rand() / (RAND_MAX)) * 0.005;

            rectangles[i].vx = rand() % 2 ? speed : -speed;
            rectangles[i].vy = rand() % 2 ? speed : -speed;

            float xx = -1 + 2 * ((float)rand()) / RAND_MAX;
            float yy = -1 + 2 * ((float)rand()) / RAND_MAX;

            rectangles[i].x = xx;
            rectangles[i].y = yy;

            rectangles[i].w = 0.027;
            rectangles[i].h = 0.027;

            ++i;
        }
    }
    
    printf("Quads count: %d\n", i);
    
    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        int i = 0;
        for (float y = -5; y < 5; y += 0.03){
            for (float x = -5; x < 5; x += 0.03){
                rect *r = &rectangles[i++];

                bool xr = r->x + r->w / 2 >= 1;
                bool xl = r->x - r->w / 2 <= -1;

                if (xr || xl){
                   r->vx *= -1;
                }

                bool yd = r->y - r->h / 2 <= -1;
                bool yu = r->y + r->h / 2 >= 1;

                if (yd || yu){
                   r->vy *= -1;
                }

                float colors[] = {r->r, r->g, r->b};

                r->x += r->vx;
                r->y += r->vy;
                renderer.renderRectangle(r->x, r->y, 
                                        r->w, r->h, colors);
            }
        }

        renderer.executeRender();
        window.swapBuffers();
    }

    return 0;
}  
