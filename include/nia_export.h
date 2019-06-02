/*
 * File: nia_export.h
 * Project: Nia
 * File created on October-07-2018
 * Author: Streanga Sarmis-Stefan (streangasarmis@gmail.com)
 * 
 * Copyright (C) 2019 Streanga Sarmis-Stefan, All Rights Reserved
 * Unauthorized copying of this file, via any medium is strictly prohibited
 * Proprietary and confidential
 */

#ifndef _NIA_EXPORT_H_
#define _NIA_EXPORT_H_

#ifdef __cplusplus
extern "C"{
#endif


#ifdef _WIN32
#   if defined _MSC_VER
#      if defined NIA_LIBRARY_BUILD
#           define NIA_EXPORT __declspec(dllexport)
#       else
#           define NIA_EXPORT __declspec(dllimport)
#       endif
#   elif __GNUC__ >= 4
#      if defined NIA_LIBRARY_BUILD
#           define NIA_EXPORT __attribute__((visibility("default")))
#       else
#           define NIA_EXPORT __attribute__((visibility("default")))
#       endif
#   endif
#else
#   if defined NIA_LIBRARY_BUILD
#       define NIA_EXPORT
#   else
#       define NIA_EXPORT
#   endif
#endif

#ifdef _WIN32
#   if defined _MSC_VER 
#      define NIA_CALL
#   elif __GNUC__ >= 4
#      define NIA_CALL __stdcall
#   endif
#elif defined __unix__
#   define NIA_CALL NIA_EXPORT
#endif

#ifdef __cplusplus
};

#endif
#endif //_NIA_EXPORT_H_