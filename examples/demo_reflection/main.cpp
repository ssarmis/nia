#include "nia.h"

#include <malloc.h>
#include <GL/glext.h>

int main(){
    niaWindow window;
    window.createWindow(1280, 768, "no matter");

    niaLoader::loadEverything();

    window.enableVsync();

    niaRenderer renderer;

    renderer.pushPerspectiveView(60, 1.666, 0.01, 2000); // TODO make this also default if the call is not done

    niaCubeTexture* cubeTexture;
    niaTransform transform;
    niaTransform transform2;
    niaTransform transform3;
    niaMatrix4 camera;

    niaMesh mesh1 = niaMesh("sphere.obj");
    niaMesh mesh2 = niaMesh("sphere.obj");
    niaMesh mesh3 = niaMesh("sphere.obj");

    camera = niaMatrix4::lookAt(
        niaVector3<r32>(0, 0, 0),
        niaVector3<r32>(0, 0, 1),
        niaVector3<r32>(0, -1, 0));
        
    const char* textures[6] = {
        "mp_midnight/right.tga", "mp_midnight/left.tga",
        "mp_midnight/down.tga", "mp_midnight/up.tga",
        "mp_midnight/back.tga", "mp_midnight/front.tga",
    };

    cubeTexture = new niaCubeTexture(textures);

    camera = camera.mul(niaMatrix4::translate(-1, 5, -5));

    transform.translate(niaVector3<float>(5, -4, -10));
    transform2.translate(niaVector3<float>(0, -10, -20));
    transform3.translate(niaVector3<float>(-5, -5, -5));

    renderer.enableDepthTest();

    float time = 0;

    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);
        time += 0.1;

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        renderer.renderSkyBox(*cubeTexture);

        camera = camera.mul(niaMatrix4::translate(0, -2+4, -10));
        camera = camera.mul(niaMatrix4::rotate(-0.001, NIA_AXIS_Y));
        camera = camera.mul(niaMatrix4::translate(0, 2-4, 10));

        renderer.submitView(camera);

        renderer.submitTransformation(transform);
        renderer.renderReflectiveMesh(mesh1, *cubeTexture);

        transform2.translate(niaVector3<float>(0, sin(time) * 0.2, 0));
        renderer.submitTransformation(transform2);
        renderer.renderReflectiveMesh(mesh2, *cubeTexture);

        transform3.translate(niaVector3<float>(sin(time) * 0.08, 0, 0));
        renderer.submitTransformation(transform3);
        renderer.renderReflectiveMesh(mesh3, *cubeTexture);

        window.swapBuffers();
    }
    
    return 0;
}