/*
    ==============================================================================

    This file is part of the Board Games Engine library.
    Copyright (c) 2014 to present - Joel R. Langlois

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
#ifndef BGE_CORE_MACROS_H
#define BGE_CORE_MACROS_H

#include "CompilerSupport.h"

//==============================================================================
} //Escape bge namespace since OS headers must be in global scope.

#if defined (_WIN32) || defined (_WIN64)
  #define BGE_WINDOWS               1

   #ifdef _WIN64
    #define BGE_64BIT               1
    #define BGE_USE_INTRINSICS      1
   #endif

  #include <Windows.h>
#elif defined (LINUX) || defined (__linux__)
  #define BGE_LINUX                 1

  #if defined (__LP64__) || defined (_LP64) || defined (__arm64__)
   #define BGE_64BIT                1
  #endif
#elif defined (__APPLE_CPP__) || defined (__APPLE_CC__)
  #define Point CarbonDummyPointName            //Workaround to avoid definition of "Point" by old Carbon headers
  #define Component CarbonDummyCompName

  #include <CoreFoundation/CoreFoundation.h>    //Needed to find out what platform we're using
  #undef Point
  #undef Component

  #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
   #define BGE_IPHONE   1
   #define BGE_IOS      1
  #else
   #define BGE_MAC      1
  #endif

  #ifdef __LP64__
   #define BGE_64BIT    1
  #endif

  #include <sys/sysctl.h>
#else
 #error "Board Games Engine: unknown platform!"
#endif

#undef Rectangle

namespace bge //Back into bge namespace
{

//==============================================================================
#if defined (DEBUG) || defined (_DEBUG) || ! (defined (NDEBUG) || defined (_NDEBUG))
 #define BGE_DEBUG 1
#endif

//==============================================================================
/** This macro defines the C calling convention used as the standard for calls. */
#if BGE_MSVC
 #define BGE_CALLTYPE   __stdcall
 #define BGE_CDECL      __cdecl
#else
 #define BGE_CALLTYPE
 #define BGE_CDECL
#endif

//==============================================================================
#if BGE_USE_INTRINSICS
  #ifndef BGE_INTEL
   #pragma intrinsic (__debugbreak)
  #endif

  /** This will try to break into the debugger if the application is currently being debugged.

      If called by an application that's not being debugged, the behaviour isn't defined.
      It may crash or not, depending on the platform.

      @see rassert()
  */
  #define reversi_breakDebugger     { __debugbreak(); }
#elif BGE_IOS || BGE_LINUX || BGE_ANDROID || BGE_PPC
  #define reversi_breakDebugger     { ::kill (0, SIGTRAP); }
#elif BGE_GCC || BGE_MAC
   #define reversi_breakDebugger    { asm ("int $3"); }
#else
  #define reversi_breakDebugger     { __asm int 3 }
#endif

//==============================================================================
#if BGE_MSVC
    #define BGE_MACRO_WITH_FORCED_SEMICOLON(x) \
        __pragma (warning (push)) \
        __pragma (warning (disable: 4127)) \
        do { x } while (false) \
        __pragma (warning (pop))
#else
    /** This is the good old C++ trick for creating a macro that forces the user to put
        a semicolon after it when they use it.
    */
    #define BGE_MACRO_WITH_FORCED_SEMICOLON(x) do { x } while (false)
#endif

#if BGE_DEBUG
    static bool BGE_CALLTYPE reversi_isRunningUnderDebugger()
    {
       #if BGE_WINDOWS
        return IsDebuggerPresent() != FALSE;
       #elif BGE_MAC
        static char testResult = 0;

        if (testResult == 0)
        {
            struct kinfo_proc info;
            int m[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
            size_t sz = sizeof (info);
            sysctl (m, 4, &info, &sz, 0, 0);
            testResult = ((info.kp_proc.p_flag & P_TRACED) != 0) ? 1 : -1;
        }

        return testResult > 0;
       #else
        return false; //If the platform is supported -> TODO!
       #endif
    }

 #define rassertfalse           BGE_MACRO_WITH_FORCED_SEMICOLON (if (reversi_isRunningUnderDebugger()) reversi_breakDebugger;)

 #define rassert(expression)    BGE_MACRO_WITH_FORCED_SEMICOLON (if (! (expression)) rassertfalse;)
#else
 #define rassertfalse

 #define rassert(x)
#endif

//==============================================================================
/** This is a shorthand macro for declaring stubs for a class' copy constructor and operator=. */
#define BGE_DECLARE_NON_COPYABLE(ClassName) \
    ClassName (const ClassName&) BGE_DELETED_FUNCTION; \
    ClassName& operator= (const ClassName&) BGE_DELETED_FUNCTION;

/** This macro can be added to class definitions to disable the use of new/delete,
    thus preventing allocating of the said object on the heap.
*/
#define BGE_PREVENT_HEAP_ALLOCATION \
    static void* operator new (size_t) BGE_DELETED_FUNCTION; \
    static void operator delete (void*) BGE_DELETED_FUNCTION;

/** This macro can be added to class/structs to design an object whose
    sole purpose is to contain static functions.
*/
#define BGE_DECLARE_UTILITY_CLASS(ClassName) \
    ClassName() BGE_DELETED_FUNCTION; \
    BGE_DECLARE_NON_COPYABLE; \
    BGE_PREVENT_HEAP_ALLOCATION;

#endif //BGE_CORE_MACROS
