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
    niaObjParser(const char* filename);
    ~niaObjParser();

    u32 parse();

    niaArray<niaVector3<r32> >& getVertexies();
    niaArray<niaVector3<r32> >& getNormals();
    niaArray<niaVector2<r32> >& getUVS();
    niaArray<niaVector3<u32> >& getFaces();
};

#endif //_NIA_OBJ_PARSER_H_