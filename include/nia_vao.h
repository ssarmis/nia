#ifndef _NIA_VAO_H_
#define _NIA_VAO_H_

#include "nia_general.h"

#include "nia_gl.h"

typedef struct niaVAO {
    GLuint id;
    GLuint vboId;
    GLuint veoId;
} niaVAO;

#endif // _NIA_VAO_H_