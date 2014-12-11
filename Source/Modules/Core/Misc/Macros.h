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
#ifndef BGE_CORE_MACROS_H
#define BGE_CORE_MACROS_H

#include "CompilerSupport.h"

//==============================================================================
#if defined (_WIN32) || defined (_WIN64)
  #define BGE_WINDOWS           1

  #include <Windows.h>
#elif defined (LINUX) || defined (__linux__)
  #define BGE_LINUX             1
#elif defined (__APPLE_CPP__) || defined (__APPLE_CC__)
  #define Point CarbonDummyPointName            //Workaround to avoid definition of "Point" by old Carbon headers
  #define Component CarbonDummyCompName

  #include <CoreFoundation/CoreFoundation.h>    //Needed to find out what platform we're using
  #undef Point
  #undef Component

  #if TARGET_OS_IPHONE || TARGET_IPHONE_SIMULATOR
   #define BGE_IPHONE           1
   #define BGE_IOS              1
  #else
   #define BGE_MAC              1
  #endif

  #include <sys/sysctl.h>
#else
 #error "Board Games Engine: unknown platform!"
#endif

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
#if BGE_IOS || BGE_LINUX || BGE_ANDROID || BGE_PPC
  /** This will try to break into the debugger if the app is currently being debugged.

      If called by an app that's not being debugged, the behaviour isn't defined.
      It may crash or not, depending on the platform.

      @see rassert()
  */
  #define reversi_breakDebugger     { ::kill (0, SIGTRAP); }
#elif BGE_USE_MSVC_INTRINSICS
  #ifndef __INTEL_COMPILER
    #pragma intrinsic (__debugbreak)
  #endif

  #define reversi_breakDebugger     { __debugbreak(); }
#elif BGE_GCC || BGE_MAC
   #define reversi_breakDebugger    { asm ("int $3"); }
#else
  #define reversi_breakDebugger     { __asm int 3 }
#endif

//==============================================================================
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

 #define rassertfalse           if (reversi_isRunningUnderDebugger()) reversi_breakDebugger;

 #define rassert(expression)    if (! (expression)) rassertfalse;
#else
 #define rassertfalse

 #define rassert(x)
#endif

//==============================================================================
#define BGE_DECLARE_NON_COPYABLE(className) \
    className (const className&);\
    className& operator= (const className&);

#endif //BGE_CORE_MACROS
