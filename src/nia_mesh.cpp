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
niaMesh::niaMesh(const char* filename, u32 objectIndex){
    // Lets assume for now we only get .obj files, no checking, nothing
    // TODO add parsing for multiple objects inside the obj file...
    LARGE_INTEGER totalThen;
    LARGE_INTEGER totalNow;
    LARGE_INTEGER then;
    LARGE_INTEGER now;

    QueryPerformanceCounter(&totalThen);

    QueryPerformanceCounter(&then);

    niaObjParser parser(filename);

    if(parser.parse(objectIndex)){
        NIA_ERROR("Could not parse file, returning.\n");
        return;
    }

    QueryPerformanceCounter(&now);

    printf("Total time elapsed parsing file: %f\n", (double)(now.QuadPart - then.QuadPart));
    // return;
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

    vector3fChain vertexiesChain = parser.getVertexies();
    vector3fChain normalsChain = parser.getNormals();
    vector2fChain uvsChain = parser.getUVS();
    vector3iChain facesChain = parser.getFaces();

    u16* indices = new u16[facesChain.size];
    niaVertex* vertexies = new niaVertex[vertexiesChain.size];

    // 3   /   1   /   1
    // 
    // 0 -> 0.000000 2.100000 0.000000
    // 1 -> 1 1 1
    // 2 -> 0.306641 0.500000
    // 3 -> 0.000000 0.000000 0.000000


    if(!uvsChain.size){
        addNode2f(&uvsChain, niaVector2<r32>(0, 0));
    }

    if(!vertexiesChain.size){
        addNode3f(&vertexiesChain, niaVector3<r32>(0.0, 0.0, 0.0));
    }

    if(!normalsChain.size){
        addNode3f(&normalsChain, niaVector3<r32>(0, 0, 0));
    }

    i32 vert;
    i32 tex;
    i32 norm;
    // TODO if chain access is an issue try caching current index 
    //      in chain and using a doubly linked list to navigate faster

    QueryPerformanceCounter(&then);

    vector3iNode* faceNode;
    vector3fNode* normalNode;
    vector2fNode* uvsNode;
    vector3fNode* vertexNode;

    u32 previousVertexIndex = 0;
    u32 previousTexIndex = 0;
    u32 previousNormalIndex = 0;

    u32 currentVertIndex = 0;
    u32 currentTexIndex = 0;
    u32 currentNormIndex = 0;

    vector3fNode* previousVertexNode = NULL;
    vector2fNode* previousTexNode = NULL;
    vector3fNode* previousNormalNode = NULL;

    for(int index = 0; index < facesChain.size; ++index){
#if 1
        faceNode = parser.getNode3i(&facesChain, index);

        currentVertIndex = faceNode->data.data[0] - 1;
        currentTexIndex = faceNode->data.data[1] - 1;
        currentNormIndex = faceNode->data.data[2] - 1;

        if(currentVertIndex < 0){
            currentVertIndex = vertexiesChain.size - currentVertIndex;
        }

        if(currentTexIndex < 0){
            currentTexIndex = uvsChain.size - currentTexIndex;
        }

        if(currentNormIndex < 0){
            currentNormIndex = normalsChain.size - currentNormIndex;
        }

        indices[index] = currentVertIndex;

        if(currentNormIndex < previousNormalIndex){
            u32 deltaNormalIndex = previousNormalIndex - currentNormIndex;
            if(deltaNormalIndex < currentNormIndex){
                normalNode = parser.getNode3fBack(&normalsChain, previousNormalNode, deltaNormalIndex);
            } else {
                normalNode = parser.getNode3f(&normalsChain, currentNormIndex);
            }
        } else {
            normalNode = parser.getNode3f(&normalsChain, currentNormIndex);
        }

        if(currentTexIndex < previousTexIndex){
            u32 deltaTexIndex = previousTexIndex - currentTexIndex;
            if(deltaTexIndex < currentTexIndex){
                uvsNode = parser.getNode2fBack(&uvsChain, previousTexNode, deltaTexIndex);
            } else {
                uvsNode = parser.getNode2f(&uvsChain, currentTexIndex);
            }
        } else {
            uvsNode = parser.getNode2f(&uvsChain, currentTexIndex);
        }
        

        vertexies[currentVertIndex].nx = normalNode->data.x;
        vertexies[currentVertIndex].ny = normalNode->data.y;
        vertexies[currentVertIndex].nz = normalNode->data.z;
        vertexies[currentVertIndex].u = uvsNode->data.x;
        vertexies[currentVertIndex].v = uvsNode->data.y;

        if(index < vertexiesChain.size){
            if(previousVertexNode){
                vertexNode = previousVertexNode->next;
            } else {
                vertexNode = parser.getNode3f(&vertexiesChain, index);
            }

            vertexies[index].x = vertexNode->data.x;
            vertexies[index].y = vertexNode->data.y;
            vertexies[index].z = vertexNode->data.z;
            
            vertexies[index].r = colors[0];
            vertexies[index].g = colors[1];
            vertexies[index].b = colors[2];
        }

        previousNormalIndex = currentNormIndex;
        previousNormalNode = normalNode;

        previousTexIndex = currentTexIndex;
        previousTexNode = uvsNode;

        previousVertexNode = vertexNode;
    }


    QueryPerformanceCounter(&now);
    printf("Total time elapsed creating the mesh for opengl / iteration: %f\n", (double)(now.QuadPart - then.QuadPart)/(double)facesChain.size);

    QueryPerformanceCounter(&totalNow);
    printf("Total time elapsed %f\n",
     (double)(totalNow.QuadPart - totalThen.QuadPart));

    QueryPerformanceCounter(&then);

    NIA_GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(niaVertex) * vertexiesChain.size,
                             vertexies, GL_STATIC_DRAW));
    
    QueryPerformanceCounter(&now);
    
    printf("Total time elapsed buffering the mesh to opengl: %f\n", (double)(now.QuadPart - then.QuadPart));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glGenBuffers(1, &vao.veoId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.veoId));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * facesChain.size,
                             indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindVertexArray(0));

    verts = facesChain.size;
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

    r32 normals[] = {
        1, 0, 0,
        0, 0, 1,
        0, 1, 0,
        0, -1, 0,
        -1, 0, 0,
        0, 0, -1
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

            normals[offset + 0],
            normals[offset + 1],
            normals[offset + 2],

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

    NIA_GL_CALL(glGenBuffers(1, &result.vao.veoId));

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

    NIA_GL_CALL(glGenBuffers(1, &result.vao.veoId));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, result.vao.veoId));
    NIA_GL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(u16) * 6, indices, GL_STATIC_DRAW));

    NIA_GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));

    NIA_GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    NIA_GL_CALL(glBindVertexArray(0));

    result.verts = 6;
    return result;
}