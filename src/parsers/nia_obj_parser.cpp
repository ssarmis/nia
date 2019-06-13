/*
 * File: nia_obj_parser.cpp
 * Project: Nia
 * File created on April-29-2019
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#include "parsers/nia_obj_parser.h"

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

int advances = 0;
int wastes = 0;

#define MAXIMUM_MEMBERS_PER_VERT 3

#define ADVANCE(_x){\
    ++_x;\
    ++advances;\
}

#define WASTE(_x) {\
    --_x;\
    ++wastes;\
}

#define WASTE_UNTIL(_pointer, _size, _criteria){\
    while(*_pointer != _criteria && _size > 0){\
        ADVANCE(_pointer);\
        WASTE(_size);\
    }\
}


#define ATOI(_x) (_x - '0')

#define I32_UNTIL(_source, _store, _size, _sep0, _sep1, _sep2, _sep3){\
    _store = 0;\
    bool negative = false;\
    if(*_source == '-'){\
        negative = !negative;\
        ADVANCE(_source);\
        WASTE(_size);\
    }\
    while(*_source != _sep0 && *_source != _sep1 && *_source != _sep2 && *_source != _sep3){\
        _store *= 10;\
        _store += ATOI(*_source);\
        ADVANCE(_source);\
        WASTE(_size);\
    }\
    if(negative){\
        _store *= -1;\
    }\
}

// TODO improve r32_until, could be more minimal
#define R32_UNTIL(_source, _store, _size, _sep0, _sep1, _sep2){\
    bool _dot = false;\
    bool _negative = false;\
    u8 _power = 0;\
    i32 _integer = 0;\
    u32 _floating = 0;\
    while(*_source != _sep0 && *_source != _sep1 &&  *_source != _sep2){\
        if(*_source == '-'){\
            _negative = true;\
        } else {\
            if(*_source == '.'){\
                _dot = true;\
            } else {\
                if(_dot){\
                    ++_power;\
                    _floating *= 10;\
                    _floating += ATOI(*_source);\
                } else {\
                    _integer *= 10;\
                    _integer += ATOI(*_source);\
                }\
            }\
        }\
        ADVANCE(_source);\
        WASTE(_size);\
    }\
    if(!_floating){\
        _store = _integer;\
    } else {\
        _store = _integer + _floating / powf(10, _power);\
    }\
    if(_negative){\
        _store *= -1;\
    }\
}\

niaObjParser::niaObjParser(const char* filename){
    fileSize = 0;
    source = NULL;

    FILE* file = fopen(filename, "rb"); // TODO CONSIDER MMAP FOR LARGE FILES, THIS WILL SPEED UP THE COPYING QUITE A LOT

    if (!file){
        NIA_ERROR("Could not open file, returning.\n");
        return;
    }

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);
    fseek(file, 0, SEEK_SET);

    source = (u8*) malloc(fileSize * sizeof(u8));

    fread(source, sizeof(u8), fileSize, file);

    fclose(file);

    initializeChain3f(&vertexies);
    initializeChain3f(&normals);
    initializeChain2f(&uvs);
    initializeChain3i(&faces);
}

niaObjParser::~niaObjParser(){
    // vertexies.clean();
    // normals.clean();
    // uvs.clean();
    // faces.clean();
    
    // free(source); <-- this is busted because we moved the pointer, fix will come soon
}

u32 niaObjParser::parse(u32 objectIndex){
    u32 seenObjects = 0;

    if (fileSize <= 0){
        return 1;
    }

    bool forceStop = false;

    while(fileSize > 0 && !forceStop){
        switch (*source) {
            case '\r' : { // ignore
                    ADVANCE(source);
                    WASTE(fileSize);
                }
                break;

           case '\n' : { // ignore
                    ADVANCE(source);
                    WASTE(fileSize);
                }
                break;

            case '#' : { // comment
                    WASTE_UNTIL(source, fileSize, '\n');
                }
                break;

            case 'g': {
                    // for now the parser can only parse 1 mesh from the file,
                    // it will stop parsing if we encounter another one
                    // its because on how we decided to handle meshes
                    // we can extract any of the objects from the file
                    // but the usage of the parser only allows us to extract one.
                    // we can specify what object we want, by its number in the file
                    if(seenObjects == objectIndex){
                        WASTE_UNTIL(source, fileSize, '\n');
                    } else {
                        if(seenObjects > objectIndex){
                            forceStop = true;
                            break;
                        }
                        ++seenObjects;

                        ADVANCE(source);
                        WASTE(fileSize);

                        WASTE_UNTIL(source, fileSize, 'g');
                        WASTE_UNTIL(source, fileSize, '\n');
                    }
                }
                break;

            case 'o' : { // ignore for now
                    WASTE_UNTIL(source, fileSize, '\n');
                }
                break;

            case 'm' : { // ignore
                    WASTE_UNTIL(source, fileSize, '\n');
                }
                break;

            case 'u' : { // ignore
                    WASTE_UNTIL(source, fileSize, '\n');
                }
                break;

            case 'v' : { // geometrics or texture coordinates or normals
                    ADVANCE(source);
                    WASTE(fileSize);

                    switch (*source) {
                        case ' ': {// geometrics
                                // TODO add some error checking man
                                r32 values[3];
                                u8 iterations = 0;
                                
                                do {
                                    // jump over space
                                    ADVANCE(source);
                                    WASTE(fileSize);

                                    R32_UNTIL(source, values[iterations], fileSize, ' ', '\n', '\r');
                                    iterations++;
                                } while(*source != '\n' && *source != '\r');

                                // vertexies.add(niaVector3<r32>(values));
                                addNode3f(&vertexies, niaVector3<r32>(values));
                            }
                            break;

                        case 't': {// texture coord
                                // jump over character
                                ADVANCE(source);
                                WASTE(fileSize);

                                r32 values[3];
                                u8 iterations = 0;
                                
                                do {
                                    // jump over space
                                    ADVANCE(source);
                                    WASTE(fileSize);

                                    R32_UNTIL(source, values[iterations], fileSize, ' ', '\n', '\r');

                                    iterations++;
                                } while(*source != '\n' && *source != '\r');
                                // TODO BE CAREFUL SOMETIMES THERE CAN BE 3, ADD SOME TEST CASES LATER
                                // uvs.add(niaVector2<r32>(values));
                                addNode2f(&uvs, niaVector2<r32>(values));
                            }
                            break;

                        case 'n': {// normal
                                // jump over character
                                ADVANCE(source);
                                WASTE(fileSize);

                                r32 values[3];
                                u8 iterations = 0;
                                
                                do {
                                    // jump over space
                                    ADVANCE(source);
                                    WASTE(fileSize);

                                    R32_UNTIL(source, values[iterations], fileSize, ' ', '\n', '\r');

                                    iterations++;
                                } while(*source != '\n' && *source != '\r');
                                // normals.add(niaVector3<r32>(values));
                                addNode3f(&normals, niaVector3<r32>(values));
                            }
                            break;

                        case 'p': {// some space vertices ? ignore
                                WASTE_UNTIL(source, fileSize, '\n');
                            }
                            break;

                        default:
                            break;
                    }
                }
                break;

            case 'f' : { // faces
                    // TODO there are more cases, but these interest us for now
                    ADVANCE(source);
                    WASTE(fileSize);

                    u8 iterations = 0;

                    niaVector3<i32> vertex(1, 1, 1);

                    do {
                        ADVANCE(source);
                        WASTE(fileSize);

                        I32_UNTIL(source, vertex.data[iterations], fileSize, '/', '\r', '\n', ' ');

                        ++iterations;
                        iterations %= MAXIMUM_MEMBERS_PER_VERT;

                        if(*source == '\n' || *source == '\r' || *source == ' '){
                            iterations = 0;
                            // faces.add(vertex);
                            addNode3i(&faces, vertex);
                        }
                    } while(*source != '\n' && *source != '\r');
                   
                    WASTE_UNTIL(source, fileSize, '\n');
                }
                break;

            default:{
                    WASTE_UNTIL(source, fileSize, '\n');
                }
                break;
        }
    }

    return 0;
}

// niaArray<niaVector3<r32> >& niaObjParser::getVertexies() {
//     return vertexies;
// }

// niaArray<niaVector3<r32> >& niaObjParser::getNormals() {
//     return normals;
// }

// niaArray<niaVector2<r32> >& niaObjParser::getUVS() {
//     return uvs;
// }

// niaArray<niaVector3<i32> >& niaObjParser::getFaces() {
//     return faces;
// }
