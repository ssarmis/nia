#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"

#include <malloc.h>
#include <GL/glext.h>

int main(){
    niaWindow window;
    window.createWindow(1280, 768, "no matter");

    niaLoadEverything();

    window.enableVsync();

    niaRenderer renderer;

    renderer.pushPerspectiveView(60, 1.666, 0.01, 2000); // TODO make this also default if the call is not done

    niaTexture texture("mesh.bmp");
    niaMesh mesh("mesh.obj");
    niaTransform transform;
    
    niaEvent event;

    transform.translate(niaVector3<float>(-5, -8, 20));

    renderer.enableDepthTest();
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        transform.rotate(0.01, NIA_AXIS_Y);

        renderer.submitTransformation(transform);
        renderer.renderMesh(mesh, texture);

        window.swapBuffers();
    }

}
