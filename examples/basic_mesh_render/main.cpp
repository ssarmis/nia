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
    renderer.pushOrthographicView(0, 1024, 0, 1024, 0, 1000); // TODO make this also default if the call is not done

    niaVector3<float>* verts = (niaVector3<float>*)malloc(sizeof(niaVector3<float>) * 4);
    verts[0] = niaVector3<float>(0, 0, 0);
    verts[1] = niaVector3<float>(500, 500, 0);
    verts[2] = niaVector3<float>(0, 500, 0);
    verts[3] = niaVector3<float>(500, 0, 0);

    niaVector3<float>* colors = (niaVector3<float>*)malloc(sizeof(niaVector3<float>) * 4);
    colors[0] = niaVector3<float>(1, 1, 1);
    colors[1] = niaVector3<float>(1, 1, 1);
    colors[2] = niaVector3<float>(1, 1, 1);
    colors[3] = niaVector3<float>(1, 0, 1);

    niaVector3<float>* normals = (niaVector3<float>*)malloc(sizeof(niaVector3<float>) * 4);
    normals[0] = niaVector3<float>(1, 1, 1);
    normals[1] = niaVector3<float>(1, 1, 1);
    normals[2] = niaVector3<float>(1, 1, 1);
    normals[3] = niaVector3<float>(1, 1, 1);

    niaVector2<float>* uvs = (niaVector2<float>*)malloc(sizeof(niaVector2<float>) * 4);
    uvs[0] = niaVector2<float>(1, 1);
    uvs[1] = niaVector2<float>(1, 1);
    uvs[2] = niaVector2<float>(1, 1);
    uvs[3] = niaVector2<float>(1, 1);

    u16 indices[] = {
        0, 1, 2,
        0, 3, 1
    };

    niaMesh mesh(verts, colors, normals, uvs, indices, 4, 6);
    
    free(verts);
    free(colors);
    free(normals);
    free(uvs);

    niaEvent event;
    while(!window.isClosed()){
        window.handleEvents(event);

        glClearColor(1, 0, 1, 1);
        glClear(GL_COLOR_BUFFER_BIT);

        renderer.renderMesh(mesh);

        window.swapBuffers();
    }

    return 0;
}  
