/*  
    base/compiler.h: File that defines compiler specific definitions

    Current supported compilers include:
      * COMPILER_MSVC
      * COMPILER_CLANG
      * COMPILER_GCC (not actually supported!!!)
    
    Definitions include:
      * COMPILER_RTTI = 'bool'
      * COMPILER_EXCEPTIONS = 'bool'
      * COMPILER_UNWIND = 'bool'
*/

#ifndef COMPILER_H
#define COMPILER_H

#ifndef PLATFORM_H
#include "platform.h"
#endif

#define COMPILER_RTTI 0
#define COMPILER_EXCEPTIONS 0
#define COMPILER_UNWIND 0

// Clang (LLVM)
#if defined(__clang__) || defined(COMPILER_CLANG)
    #include <x86intrin.h>
    #if !defined(COMPILER_CLANG)
    #define COMPILER_CLANG 1
    #endif
    #define COMPILER_VERSION (__clang_major__ + __clang_minor__)
    #define COMPILER_NAME "Clang (LLVM)"

    // todo(jax): Get COMPILER_RTTI and COMPILER_EXCEPTIONS
    // Look at clang common compiler flags?
// MSVC (Microsoft VC++)
#elif defined(_MSC_VER) || defined(COMPILER_MSVC)
    #include <intrin.h>
    #if !defined(COMPILER_MSVC)
    #define COMPILER_MSVC 1
    #endif
    #define COMPILER_VERSION _MSC_VER
    #define COMPILER_NAME "Microsoft Visual C++"
    
    // The following is based on: https://docs.microsoft.com/en-us/cpp/preprocessor/predefined-macros?view=vs-2019
    #if defined(_CPPRTTI)
    #undef COMPILER_RTTI
    #define COMPILER_RTTI 1
    #endif

    #if defined(_CPPUNWIND)
    #undef COMPILER_EXCEPTIONS
    #undef COMPILER_UNWIND
    #define COMPILER_EXCEPTIONS 1
    #define COMPILER_UNWIND 1
    #endif

// GCC (GNU Compiler)
#elif defined(__GNUC__)
    #error GCC is not supported yet!!!!
    #define COMPILER_GCC
    #define COMPILER_VERSION (__GNUC__ * 1000 + __GNUC_MINOR__)
    #define COMPILER_NAME "GCC"

    #if defined(__GXX_RTTI)
    #undef COMPILER_RTTI
    #define COMPILER_RTTI 1
    #endif

    #if defined(__EXCEPTIONS)
    #undef COMPILER_EXCEPTIONS
    #define COMPILER_EXCEPTIONS 1
    #endif
#endif
#endif