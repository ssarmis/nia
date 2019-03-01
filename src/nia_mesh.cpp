#include "nia_mesh.h"

#include "nia_gl.h"
#include "nia_constants.h"

NIA_CALL niaMesh::niaMesh(){
}

NIA_CALL niaMesh::niaMesh(niaVertex* verts){
}

NIA_CALL niaMesh::~niaMesh(){
}

NIA_CALL niaMesh::niaMesh(niaVector3<r32>* coords, niaVector3<r32>* colors, niaVector3<r32>* normals, niaVector2<r32>* uvs, u16* indices, u32 vertexies, u32 indicesAmount){
    
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * vertexies, 0, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE);
    glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_NORMALS_STRIDE);
    glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_UV_STRIDE);
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    u32 index = 0;
    while(index <= vertexies - 1){
        // TODO get rid of the unneeded copy in the struct, unwrap it in subbuffers
        niaVertex v = {
            coords[index].x,
            coords[index].y,
            coords[index].z,

            colors[index].x,
            colors[index].y,
            colors[index].z,

            normals[index].x,
            normals[index].y,
            normals[index].z,

            uvs[index].x,
            uvs[index].y,

        };

        glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(niaVertex), sizeof(niaVertex), &v);
        
        index++;
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glGenBuffers(1, &veo);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, veo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * indicesAmount, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    glBindVertexArray(0);

    verts = indicesAmount;
}
