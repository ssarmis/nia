#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"
#include "nia_loader.h"

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

    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        r32 colors[] = {0.25, 0.5, 1};
        renderer.renderRectangle(-0.5, -0.5, 1, 1, colors);

        window.swapBuffers();
    }

    return 0;
}  
