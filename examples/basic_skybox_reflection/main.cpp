#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"
#include "nia_constants.h"
#include "nia_tga_parser.h"
#include "nia_shader_cubemap.h"

#include <malloc.h>
#include <GL/glext.h>

int main(){
    niaWindow window;
    window.createWindow(1280, 768, "no matter");

    niaLoadEverything();

    window.enableVsync();

    niaRenderer renderer;

    renderer.pushPerspectiveView(60, 1.666, 0.01, 2000); // TODO make this also default if the call is not done

    niaMatrix4 camera;
    niaMesh mesh;

    class myScene : public niaScene {
        niaCubeTexture* cubeTexture;

        niaTransform transform0;
        niaTransform transform1;
        niaTransform transform2;
        niaTransform transform3;

        niaMatrix4 camera;

        niaMesh mesh0;
        niaMesh mesh1;
        niaMesh mesh2;
        niaMesh mesh3;
        
    public:
        myScene(){
            mesh1 = niaMesh("shpere.obj");

            camera = niaMatrix4::lookAt(
                niaVector3<r32>(0, 0, 0),
                niaVector3<r32>(0, 0, 1),
                niaVector3<r32>(0, -1, 0));
                
            const char* textures[6] = {
                "ame_purple/right.tga", "ame_purple/left.tga",
                "ame_purple/down.tga", "ame_purple/up.tga",
                "ame_purple/back.tga", "ame_purple/front.tga",
            };

            cubeTexture = new niaCubeTexture(textures);

            camera = camera.mul(niaMatrix4::translate(-1, 2, 0));

            // niaScene::setSkyBoxTextures(textures);

            transform1.translate(niaVector3<float>(4, -2, -10));

            niaScene::setAttributeMat4(NIA_VIEW, camera);

            niaScene::setAttributeVec3(NIA_DIFFUSE_LIGHT_POSITION, niaVector3<r32>(200, 200, -200));
            niaScene::setAttributeVec3(NIA_DIFFUSE_LIGHT_COLOR, niaVector3<r32>(1, 1, 1));
        }

        void render(niaRenderer* r) {
            niaScene::bind(r);

            r->renderSkyBox(*cubeTexture);

            ///
            camera = camera.mul(niaMatrix4::translate(4+1, -2+4, -10));
            camera = camera.mul(niaMatrix4::rotate(-0.003, NIA_AXIS_Y));
            camera = camera.mul(niaMatrix4::translate(-(4+1), 2-4, 10));

            r->submitView(camera);


            r->submitTransformation(transform1);
            r->renderReflectiveMesh(mesh1, *cubeTexture);

            ///

            niaScene::unbind(r);
        }
    };
    myScene scene;

    niaEvent event;

    renderer.enableDepthTest();
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        scene.render(&renderer);

        window.swapBuffers();
    }
    
    return 0;
}