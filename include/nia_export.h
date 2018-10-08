#ifndef _NIA_EXPORT_H_
#define _NIA_EXPORT_H_

#ifdef __cplusplus
extern "C"{
#endif

#ifdef _WIN32_
#   if defined NIA_LIBRARY_BUILD
#       define NIA_EXPORT __declspec(dllexport)
#   else
#       define NIA_EXPORT __declspec(dllimport)
#   endif
#else
#   if defined NIA_LIBRARY_BUILD
#       define NIA_EXPORT
#   else
#       define NIA_EXPORT
#   endif
#endif

#ifdef _WIN32_
#define NIA_CALL __stdcall NIA_EXPORT
#elif defined __unix__
#define NIA_CALL NIA_EXPORT
#endif

#ifdef __cplusplus
};
#endif

#endif //_NIA_EXPORT_H_