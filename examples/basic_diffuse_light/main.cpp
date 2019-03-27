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

    class myScene : public niaScene {
        niaTexture* texture;
        niaTransform transform;
        niaMatrix4 camera;
        niaMesh mesh;
        
    public:
        myScene(){
            texture = new niaTexture("mesh.bmp");
            mesh = niaMesh("mesh.obj");

            camera = niaMatrix4::lookAt(
                niaVector3<r32>(0, 0, 0),
                niaVector3<r32>(0, 0, 1),
                niaVector3<r32>(0, 1, 0));
                
            
            transform.translate(niaVector3<float>(-5, -5, -6));
            niaScene::setAttributeMat4(NIA_VIEW, camera);

            niaScene::setAttributeVec3(NIA_DIFFUSE_LIGHT_POSITION, niaVector3<r32>(200, 200, -200));
            niaScene::setAttributeVec3(NIA_DIFFUSE_LIGHT_COLOR, niaVector3<r32>(1, 1, 1));
        }

        void render(niaRenderer* r) {
            niaScene::bind(r);

            ///
            camera = camera.mul(niaMatrix4::translate(0, 0, 0.005));
            camera = camera.mul(niaMatrix4::rotate(0.0003, NIA_AXIS_Y));
            r->submitView(camera);

            r->submitTransformation(transform);

            r->renderMesh(mesh, *texture);
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