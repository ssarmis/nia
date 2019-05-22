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

    niaMesh();
    niaMesh(niaVertex* verts);
    niaMesh(const char* filename);
    // x, y, z, r, g, b, nx, ny, nz, u, v
    niaMesh(niaVector3<r32>* coords, niaVector3<r32>* colors, niaVector3<r32>* normals, niaVector2<r32>* uvs, u16* indices, u32 vertexies, u32 indicesAmount);
    ~niaMesh();

    static niaMesh cube(r32 size);
    static niaMesh quad(r32 size);
};

#endif // _NIA_MESH_H_