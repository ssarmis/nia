/*
 * File: nia_obj_parser.h
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

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
    niaArray<niaVector3<i32> > faces;
    
    i32 fileSize;
    u8* source;

public:
    NIA_CALL niaObjParser(const char* filename);
    NIA_CALL ~niaObjParser();

    u32 NIA_CALL parse(u32 objectIndex = 0);

    niaArray<niaVector3<r32> >& NIA_CALL getVertexies();
    niaArray<niaVector3<r32> >& NIA_CALL getNormals();
    niaArray<niaVector2<r32> >& NIA_CALL getUVS();
    niaArray<niaVector3<i32> >& NIA_CALL getFaces();
};

#endif //_NIA_OBJ_PARSER_H_