#ifndef _NIA_MESH_H_
#define _NIA_MESH_H_

#include "nia_general.h"

#include "nia_vao.h"
#include "nia_vector.h"
#include "nia_vertex.h"

NIA_CLASS niaMesh {
public:
    niaVAO vao;
    
    u16 verts;

    NIA_CALL niaMesh();
    NIA_CALL niaMesh(niaVertex* verts);
    NIA_CALL niaMesh(const char* filename);
    // x, y, z, r, g, b, nx, ny, nz, u, v
    NIA_CALL niaMesh(niaVector3<r32>* coords, niaVector3<r32>* colors, niaVector3<r32>* normals, niaVector2<r32>* uvs, u16* indices, u32 vertexies, u32 indicesAmount);
    NIA_CALL ~niaMesh();
};

#endif // _NIA_MESH_H_