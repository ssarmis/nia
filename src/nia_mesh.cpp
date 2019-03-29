#include "nia_mesh.h"

#include "nia_gl.h"
#include "nia_obj_parser.h"

#include "nia_constants.h"

NIA_CALL niaMesh::niaMesh(){
}

// TODO Make parser a base class and obj parser a derived one...
NIA_CALL niaMesh::niaMesh(const char* filename){
    // Lets assume for now we only get .obj files, no checking, nothing
    niaObjParser parser(filename);

    if(parser.parse()){
        NIA_ERROR("Could not parse file, returning.\n");
        return;
    }

    NIA_GL_CALL(glGenVertexArrays(1, &vao.id));
    NIA_GL_CALL(glBindVertexArray(vao.id));

    NIA_GL_CALL(glGenBuffers(1, &vao.vboId));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vao.vboId));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * parser.getVertexies().getSize(), 0, GL_STATIC_DRAW));
    
    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_NORMALS_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_UV_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));
    NIA_GL_CALL(glEnableVertexAttribArray(2));
    NIA_GL_CALL(glEnableVertexAttribArray(3));

    r32 colors[3] = {1, 1, 1}; 

    niaArray<u16> indices;

    int vertexOffset = 0;

    niaVertex* vertexies = new niaVertex[parser.getVertexies().getSize()];

    // 3   /   1   /   1
    // 
    // 0 -> 0.000000 2.100000 0.000000
    // 1 -> 1 1 1
    // 2 -> 0.306641 0.500000
    // 3 -> 0.000000 0.000000 0.000000

    if(!parser.getUVS().getSize()){
        parser.getUVS().add(niaVector2<r32>(0, 0));
    }

    if(!parser.getVertexies().getSize()){
        parser.getVertexies().add(niaVector3<r32>(0.0, 0.0, 0.0));
    }

    if(!parser.getFaces().getSize()){
        parser.getFaces().add(niaVector3<u32>(0, 0, 0));
    }

    for(int index = 0; index < parser.getFaces().getSize(); ++index){
        u32 vert = parser.getFaces()[index].data[0];
        u32 tex = parser.getFaces()[index].data[1];
        u32 norm = parser.getFaces()[index].data[2];

        indices.add((u16)(vert - 1));// TODO fix f a//b

        vertexies[vert - 1].nx = parser.getNormals()[norm - 1].x;
        vertexies[vert - 1].ny = parser.getNormals()[norm - 1].y;
        vertexies[vert - 1].nz = parser.getNormals()[norm - 1].z;

        vertexies[vert - 1].u = parser.getUVS()[tex - 1].x;
        vertexies[vert - 1].v = parser.getUVS()[tex - 1].y;
    }

    for(int index = 0; index < parser.getVertexies().getSize(); ++index){
        vertexies[index].x = parser.getVertexies()[index].x;
        vertexies[index].y = parser.getVertexies()[index].y;
        vertexies[index].z = parser.getVertexies()[index].z;
        
        vertexies[index].r = colors[0];
        vertexies[index].g = colors[1];
        vertexies[index].b = colors[2];

        NIA_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(niaVertex), sizeof(niaVertex), &vertexies[index]));
    }


    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glGenBuffers(1, &vao.veoId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.veoId));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * indices.getSize(), indices.getData(), GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindVertexArray(0));

    verts = indices.getSize();
    delete[] vertexies;
}

NIA_CALL niaMesh::niaMesh(niaVertex* verts){
}

NIA_CALL niaMesh::~niaMesh(){
}

NIA_CALL niaMesh::niaMesh(niaVector3<r32>* coords, niaVector3<r32>* colors, niaVector3<r32>* normals, niaVector2<r32>* uvs, u16* indices, u32 vertexies, u32 indicesAmount){
    // TODO clean code and make it more reusable
    NIA_GL_CALL(glGenVertexArrays(1, &vao.id));
    NIA_GL_CALL(glBindVertexArray(vao.id));

    NIA_GL_CALL(glGenBuffers(1, &vao.vboId));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vao.vboId));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * vertexies, 0, GL_STATIC_DRAW));
    
    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_NORMALS_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_UV_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));
    NIA_GL_CALL(glEnableVertexAttribArray(2));
    NIA_GL_CALL(glEnableVertexAttribArray(3));

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

        NIA_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(niaVertex), sizeof(niaVertex), &v));
        
        index++;
    }

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glGenBuffers(1, &vao.veoId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.veoId));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * indicesAmount, indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindVertexArray(0));

    verts = indicesAmount;
}

NIA_CALL niaMesh niaMesh::cube(u32 size){
    niaMesh result;

    r32 vertexies[] = {
        1.000000 * size, -1.000000 * size, -1.000000 * size,
        1.000000 * size, -1.000000 * size, 1.000000 * size,
        -1.000000 * size, -1.000000 * size, 1.000000 * size,
        -1.000000 * size, -1.000000 * size, -1.000000 * size,
        1.000000 * size, 1.000000 * size, -0.999999 * size,
        0.999999 * size, 1.000000 * size, 1.000001 * size,
        -1.000000 * size, 1.000000 * size, 1.000000 * size,
        -1.000000 * size, 1.000000 * size, -1.000000 * size,
    };

    NIA_GL_CALL(glGenVertexArrays(1, &result.vao.id));
    NIA_GL_CALL(glBindVertexArray(result.vao.id));

    NIA_GL_CALL(glGenBuffers(1, &result.vao.vboId));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, result.vao.vboId));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * 36, 0, GL_STATIC_DRAW));
    
    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_NORMALS_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_UV_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));
    NIA_GL_CALL(glEnableVertexAttribArray(2));
    NIA_GL_CALL(glEnableVertexAttribArray(3));

    u32 index = 0;
    u32 offset = 0;
    while(index <= 36 - 1){
        // TODO get rid of the unneeded copy in the struct, unwrap it in subbuffers
        niaVertex v = {
            vertexies[offset + 0],
            vertexies[offset + 1],
            vertexies[offset + 2],

            1,
            1,
            1,

            1,
            1,
            1,

            1,
            1,
        };

        NIA_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(niaVertex), sizeof(niaVertex), &v));
        
        index++;
        offset += 3;
    }

    u16 indices[] = {
        2-1, 4-1, 1-1,
        8-1, 6-1, 5-1,
        5-1, 2-1, 1-1,
        6-1, 3-1, 2-1,
        3-1, 8-1, 4-1,
        1-1, 8-1, 5-1,
        2-1, 3-1, 4-1,
        8-1, 7-1, 6-1,
        5-1, 6-1, 2-1,
        6-1, 7-1, 3-1,
        3-1, 7-1, 8-1,
        1-1, 4-1, 8-1
    };

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.vao.veoId));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * 36, indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    NIA_GL_CALL(glBindVertexArray(0));

    result.verts = 36;

    return result;
}