#include "nia_event.h"
#include "nia_window.h"
#include "nia_renderer.h"

#include <GL/gl.h>
#include <GL/glcorearb.h>

int main() {

    niaWindow window;
    window.createWindow(600, 400, "no matter");

    niaRenderer renderer;

    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.swapBuffers(window);
    }

    return 0;
}  
