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

// maybe when we will have more model parsers, I will move thse somewhere more general

NIA_STRUCT vector3fNode {
    niaVector3<r32> data;
    struct vector3fNode* next;
    struct vector3fNode* prev;
} vector3fNode;

NIA_STRUCT vector2fNode {
    niaVector2<r32> data;
    struct vector2fNode* next;
    struct vector2fNode* prev;
} vector2fNode;

NIA_STRUCT vector3iNode {
    niaVector3<i32> data;
    struct vector3iNode* next;
    struct vector3iNode* prev;
} vector3iNode;

NIA_STRUCT vector3fChain {
    u32 size;
    vector3fNode* head;
    vector3fNode* tail;
} vector3fChain;

NIA_STRUCT vector2fChain {
    u32 size;
    vector2fNode* head;
    vector2fNode* tail;
} vector2fChain;

NIA_STRUCT vector3iChain {
    u32 size;
    vector3iNode* head;
    vector3iNode* tail;
} vector3iChain;

NIA_STATIC NIA_INLINE void initializeChain3f(vector3fChain* chain){
    chain->size = 0;
    chain->head = NULL;
    chain->tail = NULL;
}

NIA_STATIC NIA_INLINE void initializeChain2f(vector2fChain* chain){
    chain->size = 0;
    chain->head = NULL;
    chain->tail = NULL;
}

NIA_STATIC NIA_INLINE void initializeChain3i(vector3iChain* chain){
    chain->size = 0;
    chain->head = NULL;
    chain->tail = NULL;
}

NIA_STATIC NIA_INLINE void addNode3f(vector3fChain* chain, const niaVector3<r32>& vector){
    vector3fNode* node = new vector3fNode;
    node->data = vector;
    node->next = NULL;
    node->prev = NULL;
    
    if(chain->head){
        chain->tail->next = node;
        chain->tail->next->prev = chain->tail;
        chain->tail = node;
    } else {
        chain->head = chain->tail = node;
    }

    ++chain->size;
}


NIA_STATIC NIA_INLINE void addNode2f(vector2fChain* chain, const niaVector2<r32>& vector){
    vector2fNode* node = new vector2fNode;
    node->data = vector;
    node->next = NULL;
    node->prev = NULL;

    if(chain->head){
        chain->tail->next = node;
        chain->tail->next->prev = chain->tail;
        chain->tail = node;
    } else {
        chain->head = chain->tail = node;
    }
    
    ++chain->size;
}

NIA_STATIC NIA_INLINE void addNode3i(vector3iChain* chain, const niaVector3<i32>& vector){
    vector3iNode* node = new vector3iNode;
    node->data = vector;
    node->next = NULL;
    node->prev = NULL;

    if(chain->head){
        chain->tail->next = node;
        chain->tail->next->prev = chain->tail;
        chain->tail = node;
    } else {
        chain->head = chain->tail = node;
    }
    
    ++chain->size;
}


// TODO add cleaning for these bad boys

//

NIA_CLASS niaObjParser {
private:
    vector3fChain vertexies;
    vector3fChain normals;
    vector2fChain uvs;
    vector3iChain faces;
    // niaArray<niaVector3<r32> > vertexies;
    // niaArray<niaVector3<r32> > normals;
    // niaArray<niaVector2<r32> > uvs;
    // niaArray<niaVector3<i32> > faces;
    
    i32 fileSize;
    u8* source;

public:
    NIA_CALL niaObjParser(const char* filename);
    NIA_CALL ~niaObjParser();

    u32 NIA_CALL parse(u32 objectIndex = 0);

    NIA_INLINE vector3fChain NIA_CALL getVertexies() {
        return vertexies;
    }

    NIA_INLINE vector3fChain NIA_CALL getNormals() {
        return normals;
    }

    NIA_INLINE vector2fChain NIA_CALL getUVS() {
        return uvs;
    }

    NIA_INLINE vector3iChain NIA_CALL getFaces() {
        return faces;
    }


    NIA_STATIC NIA_INLINE vector3fNode* getNode3f(vector3fChain* chain, u32 index){
        if(chain->size <= index){
            return NULL;
        }

        vector3fNode* iterator = chain->head;
        while(index--){
            iterator = iterator->next;
        }

        return iterator;
    }

    NIA_STATIC NIA_INLINE vector3fNode* getNode3fNext(vector3fChain* chain, vector3fNode* currentNode){
        return currentNode->next;
    }

    NIA_STATIC NIA_INLINE vector2fNode* getNode2f(vector2fChain* chain, u32 index){
        if(chain->size <= index){
            return NULL;
        }

        vector2fNode* iterator = chain->head;
        while(index--){
            iterator = iterator->next;
        }

        return iterator;
    }

    NIA_STATIC NIA_INLINE vector3iNode* getNode3i(vector3iChain* chain, u32 index){
        if(chain->size <= index){
            return NULL;
        }

        vector3iNode* iterator = chain->head;
        while(index--){
            iterator = iterator->next;
        }

        return iterator;
    }

    NIA_STATIC NIA_INLINE vector3fNode* getNode3fBack(vector3fChain* chain, vector3fNode* currentNode, u32 backwardsDistance){
        if(chain->size - backwardsDistance < 0){
            return NULL;
        }

        vector3fNode* iterator = currentNode;
        while(backwardsDistance--){
            iterator = iterator->prev;
        }

        return iterator;
    }

    NIA_STATIC NIA_INLINE vector2fNode* getNode2fBack(vector2fChain* chain, vector2fNode* currentNode, u32 backwardsDistance){
        if(chain->size - backwardsDistance < 0){
            return NULL;
        }

        vector2fNode* iterator = currentNode;
        while(backwardsDistance--){
            iterator = iterator->prev;
        }

        return iterator;
    }

    NIA_STATIC NIA_INLINE vector3iNode* getNode3iBack(vector3iChain* chain, vector3iNode* currentNode, u32 backwardsDistance){
        if(chain->size - backwardsDistance < 0){
            return NULL;
        }

        vector3iNode* iterator = currentNode;
        while(backwardsDistance--){
            iterator = iterator->prev;
        }

        return iterator;
    }


    // niaArray<niaVector3<r32> >& NIA_CALL getVertexies();
    // niaArray<niaVector3<r32> >& NIA_CALL getNormals();
    // niaArray<niaVector2<r32> >& NIA_CALL getUVS();
    // niaArray<niaVector3<i32> >& NIA_CALL getFaces();
};

#endif //_NIA_OBJ_PARSER_H_