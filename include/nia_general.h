#ifndef _NIA_GENERAL_H_
#define _NIA_GENERAL_H_

#include <stdint.h>
#include <stdio.h>

#include "nia_export.h"

#ifdef NIA_DEBUG_BUILD
#   define NIA_TRACE(...) fprintf(stdout, __VA_ARGS__)
#   define NIA_ERROR(...) fprintf(stderr, __VA_ARGS__)
#else
#   define NIA_TRACE(...) 
#   define NIA_ERROR(...) 
#endif

#define NIA_CLASS class NIA_EXPORT

#define NIA_INTERNAL static

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;

typedef float r32;
typedef double r64;

#endif //_NIA_GENERAL_H_