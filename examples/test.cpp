#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include <GL/gl.h>
#include <GL/glext.h>
#include "nia.h"
#include "nia_loader.h"

#include <stdio.h>
int main() {

    niaWindow window;
    window.createWindow(600, 400, "no matter");

    niaLoadEverything();

    window.enableVsync();
    
    niaBatchRenderer renderer;

    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    
        renderer.renderRectangle(0, 0, 1, 1);

        renderer.executeRender();
        window.swapBuffers();
    }

    return 0;
}  
