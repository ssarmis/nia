#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"
#include "nia_loader.h"

#include <GL/glext.h>

#include <stdio.h>
int main() {

    niaWindow window;
    window.createWindow(768, 768, "no matter");

    niaLoadEverything();

    window.enableVsync();
    
    niaBatchRenderer renderer;

    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);
    
        for (float y = -1.1; y < 1; y += 0.015){
            for (float x = -1.1; x < 1; x += 0.015){
                renderer.renderRectangle(x, y, 0.012, 0.012);
            }
        }

        renderer.executeRender();
        window.swapBuffers();
    }

    return 0;
}  
