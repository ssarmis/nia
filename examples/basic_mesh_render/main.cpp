#define NIA_RENDERER_GL

#define GL_GLEXT_PROTOTYPES

#include "nia.h"

#include <GL/glext.h>

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

int main() {

    niaWindow window;
    window.createWindow(1280, 768, "no matter");

    niaLoader::loadEverything();

    window.enableVsync();

    niaRenderer renderer;

    renderer.pushPerspectiveView(10, 1.666, 0.01, 1000); // TODO make this also default if the call is not done


    // simple custim mesh, this one is a quad
    niaVector3<float>* verts = (niaVector3<float>*)malloc(sizeof(niaVector3<float>) * 4);
    verts[0] = niaVector3<float>(0, 0, 0);
    verts[1] = niaVector3<float>(5, 5, 0);
    verts[2] = niaVector3<float>(0, 5, 0);
    verts[3] = niaVector3<float>(5, 0, 0);

    niaVector3<float>* colors = (niaVector3<float>*)malloc(sizeof(niaVector3<float>) * 4);
    colors[0] = niaVector3<float>(1, 1, 1);
    colors[1] = niaVector3<float>(1, 1, 1);
    colors[2] = niaVector3<float>(1, 1, 1);
    colors[3] = niaVector3<float>(1, 1, 1);

    niaVector3<float>* normals = (niaVector3<float>*)malloc(sizeof(niaVector3<float>) * 4);
    normals[0] = niaVector3<float>(1, 1, 1);
    normals[1] = niaVector3<float>(1, 1, 1);
    normals[2] = niaVector3<float>(1, 1, 1);
    normals[3] = niaVector3<float>(1, 1, 1);

    niaVector2<float>* uvs = (niaVector2<float>*)malloc(sizeof(niaVector2<float>) * 4);
    uvs[0] = niaVector2<float>(0, 0);
    uvs[1] = niaVector2<float>(5, 5);
    uvs[2] = niaVector2<float>(0, 5);
    uvs[3] = niaVector2<float>(5, 0);

    u16 indices[] = {
        0, 1, 2,
        0, 3, 1
    };

    // initialize mesh with custom verticies
    niaMesh mesh(verts, colors, normals, uvs, indices, 4, 6);
    
    // we don't need them anymore
    free(verts);
    free(colors);
    free(normals);
    free(uvs);

    niaTransform meshTransform;
    niaEvent event;

    meshTransform.translate(niaVector3<float>(-2.5, -2.5, 10));

    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(0.0, 0.0, 0.0, 1);
        glClear(GL_COLOR_BUFFER_BIT);
        
        meshTransform.translate(niaVector3<float>(0, 0, 0.08));

        renderer.submitTransformation(meshTransform);

        renderer.renderMesh(mesh);

        window.swapBuffers();
    }

    return 0;
}  
