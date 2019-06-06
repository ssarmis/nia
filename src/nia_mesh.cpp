/*
 * File: nia_mesh.cpp
 * Project: Nia
 * File created on March-01-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "nia_mesh.h"

#include "nia_gl.h"

#include "parsers/nia_obj_parser.h"

#include "nia_constants.h"

#include <windows.h> //remove

niaMesh::niaMesh(){
}

// TODO Make parser a base class and obj parser a derived one...
niaMesh::niaMesh(const char* filename){
    // Lets assume for now we only get .obj files, no checking, nothing
    // TODO add parsing for multiple objects inside the obj file...
    LARGE_INTEGER then;
    LARGE_INTEGER now;

    QueryPerformanceCounter(&then);

    niaObjParser parser(filename);

    if(parser.parse()){
        NIA_ERROR("Could not parse file, returning.\n");
        return;
    }

    QueryPerformanceCounter(&now);

    printf("Total time elapsed parsing file: %f\n", (double)(now.QuadPart - then.QuadPart));

    NIA_GL_CALL(glGenVertexArrays(1, &vao.id));
    NIA_GL_CALL(glBindVertexArray(vao.id));

    NIA_GL_CALL(glGenBuffers(1, &vao.vboId));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vao.vboId));

    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_NORMALS_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_UV_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));
    NIA_GL_CALL(glEnableVertexAttribArray(2));
    NIA_GL_CALL(glEnableVertexAttribArray(3));

    r32 colors[3] = {1, 1, 1}; 

    int vertexOffset = 0;

    u16* indices = new u16[parser.getFaces().getSize()];
    niaVertex* vertexies = new niaVertex[parser.getVertexies().getSize()];

    // 3   /   1   /   1
    // 
    // 0 -> 0.000000 2.100000 0.000000
    // 1 -> 1 1 1
    // 2 -> 0.306641 0.500000
    // 3 -> 0.000000 0.000000 0.000000

    QueryPerformanceCounter(&then);

    if(!parser.getUVS().getSize()){
        parser.getUVS().add(niaVector2<r32>(0, 0));
    }

    if(!parser.getVertexies().getSize()){
        parser.getVertexies().add(niaVector3<r32>(0.0, 0.0, 0.0));
    }

    if(!parser.getNormals().getSize()){
        parser.getNormals().add(niaVector3<r32>(0, 0, 0));
    }

    i32 vert;
    i32 tex;
    i32 norm;

    for(int index = 0; index < parser.getFaces().getSize(); ++index){
        vert = parser.getFaces()[index].data[0] - 1;
        tex = parser.getFaces()[index].data[1] - 1;
        norm = parser.getFaces()[index].data[2] - 1;

        if(vert < 0){
            vert = parser.getVertexies().getSize() - vert;
        }

        if(tex < 0){
            tex = parser.getUVS().getSize() - tex;
        }

        if(norm < 0){
            norm = parser.getNormals().getSize() - norm;
        }

        indices[index] = vert;

        vertexies[vert].nx = parser.getNormals()[norm].x;
        vertexies[vert].ny = parser.getNormals()[norm].y;
        vertexies[vert].nz = parser.getNormals()[norm].z;
        vertexies[vert].u = parser.getUVS()[tex].x;
        vertexies[vert].v = parser.getUVS()[tex].y;

        if(index < parser.getVertexies().getSize()){
            vertexies[index].x = parser.getVertexies()[index].x;
            vertexies[index].y = parser.getVertexies()[index].y;
            vertexies[index].z = parser.getVertexies()[index].z;
            
            vertexies[index].r = colors[0];
            vertexies[index].g = colors[1];
            vertexies[index].b = colors[2];
        }
    }

    QueryPerformanceCounter(&now);

    printf("Total time elapsed creating the mesh for opengl / iteration: %f\n", (double)(now.QuadPart - then.QuadPart)/(double)parser.getFaces().getSize());

    QueryPerformanceCounter(&then);

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * parser.getVertexies().getSize(),
                             vertexies, GL_STATIC_DRAW));
    
    QueryPerformanceCounter(&now);
    
    printf("Total time elapsed buffering the mesh to opengl: %f\n", (double)(now.QuadPart - then.QuadPart));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glGenBuffers(1, &vao.veoId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.veoId));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * parser.getFaces().getSize(),
                             indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindVertexArray(0));

    verts = parser.getFaces().getSize();
    delete[] vertexies;
}

niaMesh::niaMesh(niaVertex* verts){
}

niaMesh::~niaMesh(){
}

niaMesh::niaMesh(niaVector3<r32>* coords, niaVector3<r32>* colors, niaVector3<r32>* normals, niaVector2<r32>* uvs, u16* indices, u32 vertexies, u32 indicesAmount){
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

niaMesh niaMesh::cube(r32 size){
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

            0,
            0
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

niaMesh niaMesh::quad(r32 size){
    niaMesh result;
    
    r32 vertexies[] = {
        -size, size, 0,
        size, -size, 0,
        -size, -size, 0,
        size, size, 0
    };

    NIA_GL_CALL(glGenVertexArrays(1, &result.vao.id));
    NIA_GL_CALL(glBindVertexArray(result.vao.id));

    NIA_GL_CALL(glGenBuffers(1, &result.vao.vboId));
    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, result.vao.vboId));

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * 4, 0, GL_STATIC_DRAW));
    
    NIA_GL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_VERTEX_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(1, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_COLOR_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(2, 3, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_NORMALS_STRIDE));
    NIA_GL_CALL(glVertexAttribPointer(3, 2, GL_FLOAT, false, sizeof(niaVertex), (GLvoid*)NIA_UV_STRIDE));
    
    NIA_GL_CALL(glEnableVertexAttribArray(0));
    NIA_GL_CALL(glEnableVertexAttribArray(1));
    NIA_GL_CALL(glEnableVertexAttribArray(2));
    NIA_GL_CALL(glEnableVertexAttribArray(3));

    u32 index = 0;
    u32 vertOffset = 0; // use vectors...
    u32 texOffset = 0;

    r32 uvs[8] = {
        0, 0,
        size, -size,
        0, -size,
        size, 0
    };

    while(index <= 4 - 1){
        // TODO get rid of the unneeded copy in the struct, unwrap it in subbuffers
        niaVertex v = {
            vertexies[vertOffset + 0],
            vertexies[vertOffset + 1],
            vertexies[vertOffset + 2],

            1,
            1,
            1,

            1,
            1,
            1,

            uvs[texOffset + 0],
            uvs[texOffset + 1],
        };

        NIA_GL_CALL(glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(niaVertex), sizeof(niaVertex), &v));
        
        index++;
        vertOffset += 3;
        texOffset += 2;
    }

    u16 indices[] = {
        0, 1, 2,
        0, 3, 1
    };

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.vao.veoId));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * 6, indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    NIA_GL_CALL(glBindVertexArray(0));

    result.verts = 6;
    return result;
}