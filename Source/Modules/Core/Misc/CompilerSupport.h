/*
    ==============================================================================

    This file is part of the Board Games Engine library.
    Copyright (c) 2014 - Joel R. Langlois

    Permission is granted to use this software under the terms of GPL v2 (or any later version).

    Details of these licenses can be found at: www.gnu.org/licenses

    This program is free software; you can redistribute it and/or
    modify it under the terms of the GNU General Public License
    as published by the Free Software Foundation; either version 2
    of the License, or (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
    See the GNU General Public License for more details.

    ------------------------------------------------------------------------------

    To release a closed-source product which uses the Board Games Engine,
    please contact Joel at joel.r.langlois@gmail.com for additional details.

    ==============================================================================
*/
#ifndef BGE_COMPILER_SUPPORT_H
#define BGE_COMPILER_SUPPORT_H

//==============================================================================
/** C++11 feature macro clean-ups */
#undef BGE_COMPILER_SUPPORTS_NULLPTR
#undef BGE_COMPILER_SUPPORTS_NOEXCEPT
#undef BGE_COMPILER_SUPPORTS_MOVE_SEMANTICS
#undef BGE_COMPILER_SUPPORTS_DELETED_FUNCTION
#undef BGE_COMPILER_SUPPORTS_LAMBDAS
#undef BGE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL
#undef BGE_COMPILER_SUPPORTS_ARC
#undef BGE_DELETED_FUNCTION

//==============================================================================
#if _MSC_VER
 #define BGE_MSVC                                   1
#elif __clang__
 #define BGE_CLANG                                  1
#elif __GNUC__
 #define BGE_GCC                                    1
#elif __INTEL_COMPILER
 #define BGE_INTEL                                  1
#else
 #error "Board Games Engine: unknown compiler!"
#endif

//==============================================================================
//Visual Studio
#if BGE_MSVC
 #if _MSC_VER >= 1600
  #define BGE_COMPILER_SUPPORTS_NULLPTR             1
  #define BGE_COMPILER_SUPPORTS_MOVE_SEMANTICS      1
 #endif

 #if _MSC_VER >= 1700
  #define BGE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL  1
  #define BGE_COMPILER_SUPPORTS_LAMBDAS             1
 #endif

 #if _MSC_VER >= 1900
  #define BGE_COMPILER_SUPPORTS_NOEXCEPT            1
  #define BGE_COMPILER_SUPPORTS_DELETED_FUNCTION    1
 #endif

 #pragma warning (default: 4191 4265 4355 4505 4738 4946)
#endif

//==============================================================================
// Clang
#if BGE_CLANG && defined (__has_feature)
 #if __has_feature (cxx_nullptr)
  #define BGE_COMPILER_SUPPORTS_NULLPTR             1
 #endif

 #if __has_feature (cxx_noexcept)
  #define BGE_COMPILER_SUPPORTS_NOEXCEPT            1
 #endif

 #if __has_feature (cxx_rvalue_references)
  #define BGE_COMPILER_SUPPORTS_MOVE_SEMANTICS      1
 #endif

 #if __has_feature (cxx_deleted_functions)
  #define BGE_COMPILER_SUPPORTS_DELETED_FUNCTION    1
 #endif

 #if __has_feature (cxx_lambdas) \
     && ((BGE_MAC && defined (MAC_OS_X_VERSION_10_8) && MAC_OS_X_VERSION_MIN_REQUIRED > MAC_OS_X_VERSION_10_8) \
         || (BGE_IOS && defined (__IPHONE_7_0) && __IPHONE_OS_VERSION_MIN_REQUIRED >= __IPHONE_7_0) \
         || ! (BGE_MAC || BGE_IOS))
  #define BGE_COMPILER_SUPPORTS_LAMBDAS             1
 #endif

  #define BGE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL  1
  #define BGE_COMPILER_SUPPORTS_ARC                 1
#endif

//==============================================================================
// GCC
#if BGE_GCC
    && (__cplusplus >= 201103L || defined (__GXX_EXPERIMENTAL_CXX0X__))
    && ((__GNUC__ * 100 + __GNUC_MINOR__) >= 405)
 #define BGE_COMPILER_SUPPORTS_NOEXCEPT             1
 #define BGE_COMPILER_SUPPORTS_NULLPTR              1
 #define BGE_COMPILER_SUPPORTS_MOVE_SEMANTICS       1

 #if (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && ! defined (BGE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL)
  #define BGE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL  1
 #endif

 #if (__GNUC__ * 100 + __GNUC_MINOR__) >= 407 && ! defined (BGE_DELETED_FUNCTION)
  #define BGE_COMPILER_SUPPORTS_DELETED_FUNCTION    1
 #endif

 #if (__GNUC__ * 100 + __GNUC_MINOR__) >= 406 && ! defined (BGE_COMPILER_SUPPORTS_LAMBDAS)
  #define BGE_COMPILER_SUPPORTS_LAMBDAS             1
 #endif
#endif

//==============================================================================
//Intel C++ Compiler (ICC)
#if BGE_INTEL
 #if __INTEL_COMPILER >= 1200
  #define BGE_COMPILER_SUPPORTS_DELETED_FUNCTION    1
  #define BGE_COMPILER_SUPPORTS_LAMBDAS             1
 #endif

 #if __INTEL_COMPILER >= 1210
  #define BGE_COMPILER_SUPPORTS_NULLPTR             1
 #endif

 #if __INTEL_COMPILER >= 1400
  #define BGE_COMPILER_SUPPORTS_MOVE_SEMANTICS      1
  #define BGE_COMPILER_SUPPORTS_NOEXCEPT            1
  #define BGE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL  1
 #endif
#endif

//==============================================================================
#if BGE_COMPILER_SUPPORTS_DELETED_FUNCTION
 /** This macro can be placed after a method declaration to allow the use of
     the C++11 feature "= delete" on all compilers.

     On newer compilers that support it, it does the C++11 "= delete";
     on older ones, it's just an empty definition.
 */
 #define BGE_DELETED_FUNCTION = delete
#else
 #define BGE_DELETED_FUNCTION
#endif

//==============================================================================
#if ! BGE_COMPILER_SUPPORTS_NOEXCEPT
 #undef noexcept
 #define noexcept throw()

 #if defined (_MSC_VER) && (_MSC_VER > 1600)
  #undef _ALLOW_KEYWORD_MACROS
  #define _ALLOW_KEYWORD_MACROS 1 //Stops some versions of Visual Studio from complaining
 #endif
#endif

//==============================================================================
#if ! BGE_COMPILER_SUPPORTS_NULLPTR
 #undef nullptr
 #define nullptr (0)
#endif

//==============================================================================
#if ! BGE_COMPILER_SUPPORTS_OVERRIDE_AND_FINAL
 #undef override
 #define override

 #undef final
 #define final
#endif

#endif //BGE_COMPILER_SUPPORT_H