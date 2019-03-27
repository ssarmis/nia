#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"

#include <GL/glext.h>

int main(){
    niaWindow window;
    window.createWindow(1280, 768, "no matter");

    niaLoadEverything();

    window.enableVsync();

    niaRenderer renderer;

    renderer.pushPerspectiveView(10, 1.666, 0.01, 1000); // TODO make this also default if the call is not done

    niaTexture texture("mesh.bmp");
    niaMesh mesh("mesh.obj");
    niaTransform transform;
    niaMatrix4 camera;
    
    transform.translate(niaVector3<float>(-5, -10, 15));

    niaEvent event;
    renderer.enableDepthTest();
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        renderer.submitTransformation(transform);
        renderer.renderMesh(mesh, texture);

        window.swapBuffers();
    }
    return 0;
}