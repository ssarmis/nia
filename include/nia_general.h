#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif

#ifndef _NIA_GENERAL_H_
#define _NIA_GENERAL_H_

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "nia_export.h"

#ifdef _MSC_VER
#define NIA_ALIGN(_x) __declspec(align(_x))
#elif __GNUC__
#define NIA_ALIGN(_x) __attribute__((aligned(_x)))
#endif


#ifdef NIA_DEBUG_BUILD
#define NIA_TRACE(...) fprintf(stdout, __VA_ARGS__)
#else
#define NIA_TRACE(...) 
#endif


#ifdef NIA_DEBUG_BUILD
#define NIA_ASSERT(_exp) assert(_exp)
#else
#define NIA_ASSERT(_exp) 
#endif


#define NIA_ERROR(...) fprintf(stderr, __VA_ARGS__)


#ifdef NIA_DEBUG_BUILD
static char* GL_ERRORS[] = {
    /* 1280 */ "GL_INVALID_ENUM -> Set when an enumeration parameter is not legal.",
    /* 1281 */ "GL_INVALID_VALUE -> Set when a value parameter is not legal.",
    /* 1282 */ "GL_INVALID_OPERATION -> Set when the state for a command is not legal for its given parameters.",
    /* 1283 */ "GL_STACK_OVERFLOW -> Set when a stack pushing operation causes a stack overflow.",
    /* 1284 */ "GL_STACK_UNDERFLOW -> Set when a stack popping operation occurs while the stack is at its lowest point.",
    /* 1285 */ "GL_OUT_OF_MEMORY -> Set when a memory allocation operation cannot allocate (enough) memory.",
    /* 1286 */ "GL_INVALID_FRAMEBUFFER_OPERATION -> Set when reading or writing to a framebuffer that is not complete."
};

#define NIA_GL_CALL(_call) {\
    _call;\
    GLenum retCode = glGetError();\
    NIA_PARSE_GL_ERROR(retCode);\
}

#define NIA_PARSE_GL_ERROR(_code){\
    if (_code) {\
        NIA_TRACE("%s:%d %s \n", __FILE__, __LINE__, GL_ERRORS[_code & 9]);\
    }\
}

#else
#define NIA_GL_CALL(_call) _call
#define NIA_PARSE_GL_ERROR(_code) 
#endif

#define NIA_CLASS class NIA_EXPORT
#define NIA_STRUCT typedef struct NIA_EXPORT
#define NIA_INLINE inline

#define NIA_EXTERN extern

#ifdef _MSC_VER
#define NIA_INTERNAL
#elif __GNUC__
#define NIA_INTERNAL __attribute__((visibility("internal")))
#endif

#define NIA_STATIC static

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef float r32;
typedef double r64;

NIA_INLINE r64 nia_cot(r64 angle){
  return (r64)(1.0 / tanf(angle));
}

NIA_INLINE void nia_memset(u8* dest, u32 value, u32 amount){
    while(amount){
        dest[amount - 1] = value;
        amount--;
    }
}

NIA_INLINE u32 nia_strlen(const char* str){
    u32 result = 0;
    while(*str++){
        ++result;
    }
    return result;
}

NIA_INLINE i32 nia_sign(float number){
    if(number < 0){
        return -1;
    } else if(number > 0){
        return 1;
    }

    return 0;
}

#endif //_NIA_GENERAL_H_