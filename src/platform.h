/*  
    base/platform.h: File that defines platform specific definitions

    Current supported platforms include:
      * PLATFORM_WINDOWS
      * PLATFORM_LINUX
      * PLATFORM_OSX
      * PLATFORM_MAC
      * PLATFORM_UNIX (this is a group -- can be defined along with other platforms)
      * PLATFORM_APPLE (this is a group -- can be defined along with other platforms)
    
    Definitions include:
      * PLATFORM_NAME = 'string'
      * PLATFORM_NAME_DETAIL = 'string'
      * CPU_XXX = 'x64' or 'x86'
*/

#ifndef PLATFORM_H
#define PLATFORM_H

// Windows
// _WIN32, _WIN64 defined by VC++ and GCC
// _M_IX86, _M_AMD64 defined by VC++
#if defined(ENGINE_WIN32) || (defined(_WIN32) || defined(_WIN64))
    #define PLATFORM_WINDOWS
    #define PLATFORM_NAME "Windows"
    #ifdef _WIN64
        #define PLATFORM_WIN64
    #elif _WIN32
        #define PLATFORM_WIN32
    #endif
    #if defined(_M_AMD64) || defined(__x86_64__)
        #define CPU_X64
        #define PLATFORM_NAME_DETAIL "Windows x64"
    #elif defined(_M_IX86)
        #define CPU_X86
        #define PLATFORM_NAME_DETAIL "Windows x86"
    #else
        #error Unknown CPU
    #endif

// Linux
// __linux, __linux__ are defined by GCC
// __i386__, __intel__ are defined by GCC
#elif defined(ENGINE_LINUX) || (defined(__linux) || defined(__linux__))
    #define PLATFORM_LINUX
    #define PLATFORM_UNIX
    #define PLATFORM_NAME "Linux" // todo(jax): Get distribution if possible
    #if defined(__x86_64__)
        #define CPU_X64
        #define PLATFORM_NAME_DETAIL "Linux x64"
    #elif defined(__i386__) || defined(__intel__)
        #define CPU_X86
        #define PLATFORM_NAME_DETAIL "Linux x86"
    #else
        #error Unknown CPU
    #endif 

// Apple family targetting
#elif defined(__APPLE__) && __APPLE__
    #define PLATFORM_APPLE

    // todo(jax): I think Apple provides a header that defines TARGET_OS_XXXX, research.
    // todo(jax): Do we want to look for additional apple targets?
    // We aren't shipping mobile.

    // MacOS
    // __i386__, __intel__ are defined by GCC
    #if defined(TARGET_OS_MAC) && (defined(ENGINE_OSX) || defined(ENGINE_MAC))
        #define PLATFORM_OSX
        #define PLATFORM_MAC
        #define PLATFORM_UNIX
        #define PLATFORM_NAME "MacOS"
        #if defined(__i386__) || defined(__intel__)
            #define CPU_X64
            #define PLATFORM_NAME_DETAIL "MacOS x64"    
        #elif defined(__x86_64) || defined(__x86_64__)
            #define CPU_X86
            #define PLATFORM_NAME_DETAIL "MacOS x86"            
        #else
            #error Unknown CPU
        #endif
    #else
        #error Unknown Apple target
    #endif
#else
    #error Unknown platform
    #error Unknown CPU
#endif

#endif