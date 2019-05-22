#ifndef _NIA_OBJ_PARSER_H_
#define _NIA_OBJ_PARSER_H_

#include "nia_array.h"
#include "nia_vector.h"
#include "nia_general.h"

NIA_CLASS niaObjParser {
private:
    niaArray<niaVector3<r32> > vertexies;
    niaArray<niaVector3<r32> > normals;
    niaArray<niaVector2<r32> > uvs;
    niaArray<niaVector3<u32> > faces;
    
    i32 fileSize;
    u8* source;

public:
    NIA_CALL niaObjParser(const char* filename);
    NIA_CALL ~niaObjParser();

    u32 NIA_CALL parse();

    niaArray<niaVector3<r32> >& NIA_CALL getVertexies();
    niaArray<niaVector3<r32> >& NIA_CALL getNormals();
    niaArray<niaVector2<r32> >& NIA_CALL getUVS();
    niaArray<niaVector3<u32> >& NIA_CALL getFaces();
};

#endif //_NIA_OBJ_PARSER_H_