#ifndef _NIA_EXPORT_H_
#define _NIA_EXPORT_H_

#ifdef __cplusplus
extern "C"{
#endif
#if defined NIA_LIBRARY_BUILD
#   define NIA_EXPORT __declspec(dllexport)
#else
#   define NIA_EXPORT __declspec(dllimport)
#endif

#define NIA_CALL __stdcall NIA_EXPORT

#ifdef __cplusplus
};
#endif

#endif //_NIA_EXPORT_H_