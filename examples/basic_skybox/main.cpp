#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"
#include "nia_constants.h"
#include "nia_tga_parser.h"
#include "shaders/nia_shader_cubemap.h"

#include <malloc.h>
#include <GL/glext.h>

int main(){
    niaWindow window;
    window.createWindow(1280, 768, "no matter");

    niaLoader::loadEverything();

    window.enableVsync();

    niaRenderer renderer;

    renderer.pushPerspectiveView(60, 1.666, 0.01, 2000); // TODO make this also default if the call is not done

    niaMatrix4 camera;
    niaMesh mesh;

    const char* textures[6] = {
        "ame_purple/right.tga", "ame_purple/left.tga",
        "ame_purple/down.tga", "ame_purple/up.tga",
        "ame_purple/back.tga", "ame_purple/front.tga",
    };

    niaCubeTexture skyBox(textures);

    niaEvent event;

    camera = niaMatrix4::lookAt(niaVector3<float>(0, 0, 0),
                                niaVector3<float>(0, 0, 1),
                                niaVector3<float>(0, -1, 0)
                            );

    renderer.enableDepthTest();
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        camera = camera.mul(camera.rotate(0.01, NIA_AXIS_Y));

        renderer.submitView(camera);
        renderer.renderSkyBox(skyBox);

        window.swapBuffers();
    }
    
    return 0;
}