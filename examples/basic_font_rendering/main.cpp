#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"
#include "nia_loader.h"

#include "nia_font_renderer_gl.h"

#include <GL/glext.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

#include "parsers/nia_ttf_parser.h"


int main() {

    niaWindow window;
    window.createWindow(1024, 1024, "no matter");

    niaLoader::loadEverything();

    niaTexture texture("test.bmp");
    window.enableVsync();

    // niaFontRenderer renderer("xandu.ttf");
    niaRenderer renderer;
    renderer.pushOrthographicView(0, 1024, 0, 1024, 0, 1000); // TODO make this also default if the call Renderernot done

    niaEvent event;


    // niaTextureStreaming::updateStreams();

    niaShaderQuad quad;
#if 1
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(0.5, 0, 0.5, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        quad.useShader();
        renderer.renderMeshRaw(niaMesh::quad(1.0), texture);
        quad.unuseShader();

        // renderer.renderString(64, 128, 20, "Sarm,is string 0123456789     #*/");
        // renderer.renderString(64, 200, 20, "Font used: xandu.tff");

        window.swapBuffers();
    }
#endif
    return 0;
}  
