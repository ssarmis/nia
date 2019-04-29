#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"

#include <GL/glext.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    float width = 1280.0, height = 768.0;

    niaWindow window;
    window.createWindow((int)width, (int)height, "no matter");

    niaLoader::loadEverything();

    window.enableVsync();

    niaRenderer renderer;

    niaTransform transform;

#if 0
    renderer.pushOrthographicView(0, 1024, 0, 1024, 0, 1000);
#endif
    renderer.pushPerspectiveView(90.0, width/height, 0.0, 1000.0);

    float rotation = 0;

    transform.translate(niaVector3<r32>(0, 0, 60));

    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        r32 colors[] = {0.0, 0.5, 0.2};

        transform.translate(niaVector3<r32>(50, 75, 60));
        transform.rotate(0.01, NIA_AXIS_Y);
        transform.translate(niaVector3<r32>(-50, -75, -60));

        renderer.submitTransformation(transform);
        renderer.renderRectangle(0, 0, 60, 100, 150, colors);

        window.swapBuffers();
    }

    return 0;
}  


