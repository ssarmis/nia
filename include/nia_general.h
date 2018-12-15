#ifndef _NIA_GENERAL_H_
#define _NIA_GENERAL_H_

#include <stdint.h>
#include <stdio.h>

#include "nia_export.h"

#ifdef NIA_DEBUG_BUILD
#   define NIA_TRACE(...) fprintf(stdout, __VA_ARGS__)
#else
#   define NIA_TRACE(...) 
#endif

#define NIA_ERROR(...) fprintf(stderr, __VA_ARGS__)

#define NIA_CLASS class NIA_EXPORT
#define NIA_STRUCT struct NIA_EXPORT
#define NIA_INLINE inline

#define NIA_EXTERN extern
#define NIA_INTERNAL
#define NIA_STATIC static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef float r32;
typedef double r64;

NIA_INLINE void nia_memset(u8* dest, u32 value, u32 amount){
    while(amount){
        dest[amount - 1] = value;
        amount--;
    }
}

NIA_INLINE u32 nia_strlen(const char* str){
    u32 result;
    while(*str++){
        ++result;
    }
    return result;
}

#endif //_NIA_GENERAL_H_