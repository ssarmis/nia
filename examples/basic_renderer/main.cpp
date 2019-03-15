#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"

#include <GL/glext.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    niaWindow window;
    window.createWindow(1024, 1024, "no matter");

    niaLoadEverything();

    window.enableVsync();

    niaRenderer renderer;

    niaTransform transform;


    transform.translate(niaVector3<r32>(400, 400, 0));
    transform.scale(2);
    transform.rotate(50, NIA_AXIS_Z);


    niaMatrix4::printMat4(transform.transform);

#if 0
    renderer.pushPerspectiveView(60, 1, 0, 1000);
#endif
    renderer.pushOrthographicView(0, 1024, 0, 1024, 0, 1000);

    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);
        glClearColor(0, 0, 0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // TODO think about reseting all transformations at the end of the rendering maybe...

        r32 colors[] = {0.0, 0.5, 0.2}; // green
        renderer.submitTransformation(transform.transform);


        renderer.renderRectangle(0, 0, 100, 150, colors);
#if 0
        renderer.renderRectangle(0, 0, 60, 100, 150, colors);
#endif


        window.swapBuffers();
    }

    return 0;
}  


