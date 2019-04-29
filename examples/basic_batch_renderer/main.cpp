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

typedef struct color{
    float r;
    float g;
    float b;
};

float noarmlize(u32 value){
    return (float)value / 255.0;
}

color pastels[5] = {
    {noarmlize(0xff),
     noarmlize(0xb3),
     noarmlize(0xba)},

    {noarmlize(0xff),
     noarmlize(0xdf),
     noarmlize(0xba)},

    {noarmlize(0xff),
     noarmlize(0xff),
     noarmlize(0xba)},

    {noarmlize(0xba),
     noarmlize(0xff),
     noarmlize(0xc9)},

    {noarmlize(0xba),
     noarmlize(0xe1),
     noarmlize(0xff)}
};

int main() {
#if 0
    niaVector2<int> v0;
    niaVector2<int> v1(12, 5);
    niaVector2<int> v2 = v0.add(v1).mul(2);

    printf("%d, %d\n", v2.x, v2.y);

    niaVector3<int> vv0;
    niaVector3<int> vv1(12, 5, 2);
    niaVector3<int> vv2 = vv0.add(vv1).mul(2);

    printf("%d, %d, %d\n", vv2.x, vv2.y, vv2.z);

    niaVector4<int> vvv0;
    niaVector4<int> vvv1(12, 5, 2, 6);
    niaVector4<int> vvv2 = vvv0.add(vvv1).mul(2);

    printf("%d, %d, %d, %d\n", vvv2.x, vvv2.y, vvv2.z, vvv2.w);
#endif

    niaWindow window;
    window.createWindow(1024, 1024, "no matter");

    niaLoader::loadEverything();

    window.enableVsync();

    niaBatchRenderer renderer;

    renderer.pushOrthographicView(0, 1024, 0, 1024, 0, 1000); // TODO make this also default if the call is not done

    srand(time(NULL));
    int i = 0;

    u32 index = 0;

    for (float y = 0; y < 100; y += 1){
        for (float x = 0; x < 100; x += 1){
            // rectangles[i].r = ((double) rand() / (RAND_MAX));
            // rectangles[i].g = ((double) rand() / (RAND_MAX));
            // rectangles[i].b = ((double) rand() / (RAND_MAX));

            rectangles[i].r = pastels[index].r;
            rectangles[i].g = pastels[index].g;
            rectangles[i].b = pastels[index].b;

            ++index;
            index %= 5;

            float speed = ((double) rand() / (RAND_MAX)) * 3;

            rectangles[i].vx = rand() % 2 ? speed : -speed;
            rectangles[i].vy = rand() % 2 ? speed : -speed;

            float xx = x * 12;
            float yy = y * 12;

            rectangles[i].x = xx;
            rectangles[i].y = yy;

            rectangles[i].w = 20;
            rectangles[i].h = 20;

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
        for (float y = 0; y < 100; y += 1){
            for (float x = 0; x < 100; x += 1){
                rect *r = &rectangles[i++];

                bool xr = r->x + r->w / 2 >= 1024;
                bool xl = r->x - r->w / 2 <= 0;

                if (xr || xl){
                   r->vx *= -1;
                }

                bool yd = r->y - r->h / 2 <= 0;
                bool yu = r->y + r->h / 2 >= 1024;

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
