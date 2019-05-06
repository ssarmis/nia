#ifndef _NIA_CONSTANTS_H_
#define _NIA_CONSTANTS_H_

#define NIA_VERSION             "a1.0"

#ifdef __unix__
#define NIA_GLSL_VERSION    "#version 300 es"
#define NIA_GLSL_PRECISION  "highp"
#elif defined _WIN32
#define NIA_GLSL_VERSION    "#version 440 core"
#define NIA_GLSL_PRECISION  "highp"
#endif

#define NIA_UNIFORM_PROJECTION              "mP"
#define NIA_UNIFORM_VIEW                    "mV"
#define NIA_UNIFORM_TRANSFORM               "mT"

#define NIA_UNIFORM_DIFFUSE_LIGHT_POSITION  "dlP"
#define NIA_UNIFORM_DIFFUSE_LIGHT_COLOR     "dlC"

#define NIA_UNIFORM_SPECULAR_LIGHT_POSITION  "slP"
#define NIA_UNIFORM_SPECULAR_LIGHT_COLOR     "slC"


#define NIA_PI                  3.14159

#define NIA_VERTEX_STRIDE       0
#define NIA_COLOR_STRIDE        (3 * sizeof(r32))
#define NIA_NORMALS_STRIDE      (6 * sizeof(r32))
#define NIA_UV_STRIDE           (9 * sizeof(r32))

#endif //_NIA_CONSTANTS_H_