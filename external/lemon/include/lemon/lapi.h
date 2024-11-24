//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lapi.h
// Description : Lemon API configuration include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LAPI_H_INCLUDED
#define LAPI_H_INCLUDED

// TODO:
//
// Remove:
//   But need to sort out containers and other non-interface'd classes first.
//
// Check _SIGNED_CHAR on SNC does what we expect.
//
// Data Types
// ----------
//
//  Deal with declared data-types clean up. We have a problem using the data
//  types outside of the lemon namespace for C interfaces.
//
//    1. rename them to luint32_t or some such.
//    2. put them back into the lemon linkage namespace
//    3. remove the extern "C" from platform?
//    4. duplicate them as luint32_t in the global namespace?
//
// opted for option4 at the moment, means we have duplicate definitions.
//
// File Path Sizes
// ---------------
//
// What's the purpose of L_FILE_MAX_PATH in lcorefile.h. The maximum path
// can change from device to device? Should we be using _MAX_PATH where
// available (and PATH_MAX)?
// It's used by users to allocate enough space to hold a path. Should
// it be defined in the lapi.h platform section?
//
// Docs
// ----
//
// Document all defines, platform and chipsets and compilers supported.
//
// Configuration
// -------------
//
// Detect debug runtime?
// Multithread runtime?
//   - allow link against single threaded and remove eror case when not MT?
// May need common platform definition for windows.
// how to handle Xbox win32 subset?
// how to handle cygwin/mingw win32?
//
// MIPS
// ----
//
// For generic mips platforms check:
//   _mips
//   _MIPSEL
//   _MIPSEB
//   __mips_single_float
//   __mips_hard_float
//
// Mac/PPC
// -------
//
// Support Apple compiler, and power pc/ppc64
// Apple: __APPLE_CC__,  __APPLE__, __BIG_ENDIAN__,__APPLE__, __ppc__,__MACH__
// Mwerks: __MWERKS__, __powerpc__, _powerpc, __MC68K__, __INTEL__, __POWERPC__
//
// WinCE
// -----
//
// Detect eVC and detect the various wince chipsets.
// UNDER_CE, _SH3, __sh4__, _MIPS, ARM etc.
//
// Compilers
// ---------
//
// Intel compiler support.
//
// X86 SSE/SSE2/SSE3
// -----------------
//
// Sort out how we enable hard coded SSE instructions. We only enable SSE and SSE2
// on VC if the /arch flag is set to use SSE and SSE2.
//
// X64
// ---
//
// Can we enable X86 instructions on X64 (AMD X86-64) using L_CHIPSET_X86 ?
//
// Tests
// -----
//
// Write test suite that runs this through preprocessor for specific configurations and
// checks expected results.
// Eg:
// #include <lapi.h>
// ArchID: L_PLATFORM_ID,L_COMPILER_ID,L_CHIPSET_ID
// #ifdef L_NO_EXCEPTIONS
// Define: no-exceptions
// #endif
//
// cl /EP /MT /GX test.cpp
//
// Then parse output for Define: and ArchID: tags.
//
// ABI
// ---
//
// Support specific ABI? cdecl, stdcall, fastcall.
// VC and ilk: __cdecl, __stdcall, __fastcall.
// GNU: __attribute__((cdecl) .. etc.
//

// TEMP!
#ifdef _MSC_VER
#pragma warning(disable : 4251) /* needs to have dll-interface to be used by clients of class 'classname' */
#pragma warning(disable : 4275) /* non dll-interface class 'classname' used as base for dll-interface class 'classname' */

//SIMON: warnings disabled here for now
//#  pragma warning(disable: 4702) // unreachable code.
//#  pragma warning(disable: 4511) // copy constructor could not be generated.
//#  pragma warning(disable: 4512) // assignment operator could not be generated.
//#  pragma warning(disable: 4521) // alignment of a member was sensitive to packing.
//#  pragma warning(disable: 4121) // alignment of a member was sensitive to packing.
//#  pragma warning(disable: 4100) // unreferenced formal parameter.
//#  pragma warning(disable: 4701) // local variable may be used without having been initialized.
//#  pragma warning(disable: 4996) // ' ' was declared deprecated.
//#  pragma warning(disable: 4244) // conversion from 'long double' to 'double', possible loss of data
//// All long double from C double produce this message at level 4 (but not 3).

#  pragma warning(disable: 4127) // conditional expression is constant.
                                 //NOTE: Possible argument for keeping this enabled as it could catch errors, a lot of warnings
                                 // generated are from template optimisations like if( M == 3 && N == 3 ) for special case 3x3 matrix code for example

#endif

#ifdef __MWERKS__
#pragma warning off (10216) //   (10216) illegal empty declaration
#pragma warning off (10107)   // (10107) identifier expected
#pragma warning off (10317)   // (10317) implicit arithmetic conversion from 'int' to 'unsigned int'
#pragma warning off (10369)   // (10369) expression has no side effect
#pragma warning off (10178)   // (10178) function has no prototype

#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Preprocessor macros
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#if _MSC_VER
#  define L_PP_STRINGIZE(text) L_PP_STRINGIZE_A((text))
#  define L_PP_STRINGIZE_A(arg) L_PP_STRINGIZE_B ## (arg)
#  define L_PP_STRINGIZE_B(arg) L_PP_STRINGIZE_I ## arg
#elif __MWERKS__
#  define L_PP_STRINGIZE(text) L_PP_STRINGIZE_OO((text))
#  define L_PP_STRINGIZE_OO(par) L_PP_STRINGIZE_I ## par
#else
#  define L_PP_STRINGIZE(text) L_PP_STRINGIZE_I(text)
#endif
#define L_PP_STRINGIZE_I(text) #text

#if defined(__MWERKS__)
#   define L_PP_CAT(a, b) L_PP_CAT_OO((a, b))
#   define L_PP_CAT_OO(par) L_PP_CAT_I ## par
#else
#   define L_PP_CAT(a, b) L_PP_CAT_I(a, b)
#endif
#if defined(_MSC_VER)
#   define L_PP_CAT_I(a, b) L_PP_CAT_II(a ## b)
#   define L_PP_CAT_II(res) res
#else
#   define L_PP_CAT_I(a, b) a ## b
#endif

#define L_COMPILE_TIME_ASSERT(_name, _expr) typedef int Lemon_dummy_ ## _name[(_expr) ? 1 : -1 ]

#define L_UNUSED_PARAM(P) ((void)(P))

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Compiler selection and configuration
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// L_COMPILER_xxx             - Compiler identifier
// L_COMPILER_ID              - Identifier for compiler
// L_COMPILER_NAME            - String description for compiler
// L_COMPILER_VERSION_MAJOR   - Major compiler version number
// L_COMPILER_VERSION_MINOR   - Minor compiler version number
// L_COMPILER_VERSION_PATCH   - Patch compiler version number
//
// L_NO_INTRINSIC_WCHAR_T     - no wchar_t type defined intrinsically
// L_CHAR_IS_UNSIGNED         - char type is unsigned
// L_NO_EXCEPTIONS            - no exceptions enabled
// L_NO_RTTI                  - no rtti enabled
// L_HAS_MSVC_INT64           - has __int64 for 64 bit types
// L_HAS_LONG_LONG            - has 'long long' (see chipset L_LONG_IS_64BIT)
// L_HAS_NRVO                 - has named return value optimisation
// L_HAS_PRAGMA_PACK          - supports #pragma pack
//
// SNC:
//   Does SNC support NRVO?
//
// GCC:
//   L_NO_RTTI must be specified by hand as there is no mechanism to detect it.
//
// The GNU and MSVC selections must remain at the end of the list as
// some vendors (SN, Metrowerks etc.) can define __GNUC__ or _MSC_VER.
//
// Some compiler use the Edison Design Group C/C++ EDG front end (Intel, SN etc).
// If there are enough common options between these compilers EDG may warrant
// an additional common configuaration after this list.
//

#if defined(__SNC__)
// SN System C/C++

#  define L_COMPILER_SNC __SN_VER__

#  if defined(__VERSION__)
#    define L_COMPILER_NAME "SN System C/C++ v"  L_PP_STRINGIZE(__SN_VER__) " (" __VERSION__ ")"
#  else
#    define L_COMPILER_NAME "SN System C/C++ v"  L_PP_STRINGIZE(__SN_VER__)
#  endif
#  define L_COMPILER_ID "SNC"

#  define L_COMPILER_VERSION_MAJOR (__SN_VER__/100)
#  define L_COMPILER_VERSION_MINOR (__SN_VER__%100)
#  define L_COMPILER_VERSION_PATCH 0

#  if !defined(__WCHAR_T_IS_KEYWORD)
#    define L_NO_INTRINSIC_WCHAR_T 1
#  endif

#  if !defined(__SIGNED_CHARS)
#    define L_CHAR_IS_UNSIGNED 1
#  endif

#  if !defined(_EXCEPTIONS)
#    define L_NO_EXCEPTIONS 1
#  endif

#  if !defined(__RTTI)
#    define L_NO_RTTI 1
#  endif

#  if defined(_MSC_VER) && ( _MSC_VER >= 1200)
#    define L_HAS_MSVC_INT64 1
#  endif

#  if !defined(__NO_LONG_LONG)
#    define L_HAS_LONG_LONG 1
#  endif

// SNC supports NRVO
#  define L_HAS_NRVO 1

#elif defined __GCCE__
// GCCE compiler

#  define L_COMPILER_NAME "GCCE"
#  define L_COMPILER_ID "GCCE"
#  define L_COMPILER_GCCE 1

#  define L_COMPILER_VERSION 1
#  define L_COMPILER_VERSION_MAJOR 1
#  define L_COMPILER_VERSION_MINOR 0
#  define L_COMPILER_VERSION_PATCH 0

#    define L_NO_RTTI 1
#    define L_NO_EXCEPTIONS 1

#    pragma longlong on
#    define L_HAS_LONG_LONG 1

#elif defined __WINSCW__
// WINSCW compiler

#  define L_COMPILER_NAME "WINSCW"
#  define L_COMPILER_ID "WINSCW"
#  define L_COMPILER_WINSCW 1

#  define L_COMPILER_VERSION 1
#  define L_COMPILER_VERSION_MAJOR 1
#  define L_COMPILER_VERSION_MINOR 0
#  define L_COMPILER_VERSION_PATCH 0

#  define L_HAS_PRAGMA_PACK 1

#    define L_NO_RTTI 1
#    define L_NO_EXCEPTIONS 1

#    define L_HAS_LONG_LONG 1

#elif defined __MWERKS__
// Metrowerks CodeWarrior

#  define L_COMPILER_MWCW __MWERKS__
#  define L_COMPILER_NAME "Metrowerks CodeWarrior v"  L_PP_STRINGIZE(__MWERKS__)
#  define L_COMPILER_ID "MWCW"

#  define L_COMPILER_VERSION L_COMPILER_MWCW
#  if( __MWERKS__ == 1)
#    define L_COMPILER_VERSION_MAJOR 1
#    define L_COMPILER_VERSION_MINOR 0
#    define L_COMPILER_VERSION_PATCH 0
#  else
#    define L_COMPILER_VERSION_MAJOR ((__MWERKS__&0xF000)>>12)
#    define L_COMPILER_VERSION_MINOR ((__MWERKS__&0xF00)>>8)
#    define L_COMPILER_VERSION_PATCH (__MWERKS__&0xFF)
#  endif

#  if !__option(wchar_type)
#    define L_NO_INTRINSIC_WCHAR_T 1
#  endif

#  if __option(unsigned_char)
#    define L_CHAR_IS_UNSIGNED 1
#  endif

#  if !__option(exceptions)
#    define L_NO_EXCEPTIONS 1
#  endif

#  if !__option(RTTI)
#    define L_NO_RTTI 1
#  endif

#  if __option(longlong)
#    define L_HAS_LONG_LONG 1
#  endif

#  define L_HAS_PRAGMA_PACK 1

// We don't support MWCW prior to version 5.3
#  if (__MWERKS__ < 0x2301)
#    error "Metrowerks Code Warrior prior to version 5.3 is not supported"
#  endif

#elif defined __GNUC__
// Gnu C/C++

#  define L_COMPILER_GCC __GNUC__
#  define L_COMPILER_NAME "GNU C/C++ v" __VERSION__
#  define L_COMPILER_ID "GCC"

#  define L_COMPILER_VERSION_MAJOR __GNUC__
#  define L_COMPILER_VERSION_MINOR __GNUC_MINOR__
#  if defined(__GNUC_PATCHLEVEL__)
#    define L_COMPILER_VERSION_PATCH __GNUC_PATCHLEVEL__
#  else
#    define L_COMPILER_VERSION_PATCH 0
#  endif

// Single version number of easier testing
#define L_COMPILER_GCC_VERSION (L_COMPILER_VERSION_MAJOR * 10000 + L_COMPILER_VERSION_MINOR * 100 + __GNUC_PATCHLEVEL__)

#  if defined(__CHAR_UNSIGNED__)
#    define L_CHAR_IS_UNSIGNED 1
#  endif

// Note that we can't detect if RTTI is enabled and set L_NO_RTTI
// under GCC, it must be set by hand.

#  if !defined(__EXCEPTIONS)
#    define L_NO_EXCEPTIONS 1
#  endif

// Note temporarily commented out as long long on PS2 GCC compiler 3.2-ee-040921 is 64bit
//#  define L_HAS_LONG_LONG 1

// GCC 3.1 and up support NRVO
#  if __GNUC__ > 3 || ( __GNUC__ == 3 && __GNUC_MINOR__ >= 1 )
#    define L_HAS_NRVO 1
#  endif

// We don't support GCC prior to version 2.9
#  if (__GNUC__ <= 2) && (__GNUC_MINOR__ < 9)
#    error "GCC prior to version 2.90 is not supported"
#  endif

#elif defined _MSC_VER
// Microsoft Visual C/C++

#  define L_COMPILER_MSVC _MSC_VER
#  define L_COMPILER_NAME "Microsoft Visual C/C++ v" L_PP_STRINGIZE(_MSC_VER)
#  define L_COMPILER_ID "MSVC"

#  if( (_MSC_FULL_VER / 10000) == _MSC_VER )
#    define L_COMPILER_MSVC_BUILD (_MSC_FULL_VER % 10000)
#  elif( (_MSC_FULL_VER / 100000) == _MSC_VER )
#    define L_COMPILER_MSVC_BUILD (_MSC_FULL_VER % 100000)
#  else
#    error Cannot work out MSVC build number from _MSC_FULL_VER
#  endif

#  define L_COMPILER_VERSION_MAJOR (_MSC_VER/100)
#  define L_COMPILER_VERSION_MINOR (_MSC_VER%100)
#  define L_COMPILER_VERSION_PATCH L_COMPILER_MSVC_BUILD

#  define L_COMPILER_DLLEXPORT __declspec(dllexport)
#  define L_COMPILER_DLLIMPORT __declspec(dllimport)

#  if !defined(_NATIVE_WCHAR_T_DEFINED)
#    define L_NO_INTRINSIC_WCHAR_T 1
#  endif

#  if defined(_CHAR_UNSIGNED)
#    define L_CHAR_IS_UNSIGNED 1
#  endif

#  if !defined(_CPPUNWIND)
#    define L_NO_EXCEPTIONS 1
#  endif

#  if !defined(_CPPRTTI)
#    define L_NO_RTTI 1
#  endif

#  if (_MSC_VER >= 1200)
#    define L_HAS_MSVC_INT64 1
#  endif

#  if (_MSC_VER >= 1310) && defined(_MSC_EXTENSIONS)
#    define L_HAS_LONG_LONG 1
#  endif

#  define L_HAS_PRAGMA_PACK 1

// We don't support MSVC prior to version 7.1
#  if _MSC_VER < 1310
#    error "Microsoft Visual C/C++ prior to version 7.1 is not supported"
#  endif

#else
// we don't recognise the compiler
#  error "Unknown compiler, please report this to dev@fourdoorlemon.com"

#endif

// Common configuration for GCC compatible compilers

#if defined(__GNUC__)
#  define L_GCC_PACKED(_n) __attribute__((packed,aligned(_n)))
#else
#  define L_GCC_PACKED(_n)
#endif

// Declare empty import/export if needed

#if !defined(L_COMPILER_DLLIMPORT)
#  define L_COMPILER_DLLIMPORT
#endif

#if !defined(L_COMPILER_DLLEXPORT)
#  define L_COMPILER_DLLIMPORT
#endif

// Check compiler identifiers

#if !defined(L_COMPILER_ID) || !defined(L_COMPILER_NAME)
#  error "Platform identifiers incomplete, please report this to dev@fourdoorlemon.com"
#endif


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Platform selection and configuration
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// L_PLATFORM_xxx             - Platform identifier
// L_PLATFORM_ID              - Identifier for platform
// L_PLATFORM_NAME            - String description for platform
//
// Windows:
//   L_PLATFORM_WINDOWS is declared on both Win32 and Win64 platforms.
//
// The WIN32 and WIN64 selections must remain at the end of the list as
// some platforms (Xbox) can define _WIN32
//

#if defined(__CYGWIN32__)
// Cygwin

#  define L_PLATFORM_CYGWIN         1
#  define L_PLATFORM_NAME           "Cygwin"
#  define L_PLATFORM_ID             "CYGWIN"

#elif defined( __SYMBIAN32__ )

// Symbian
#  define L_PLATFORM_SYMBIAN        1
#  define L_PLATFORM_NAME           "Symbian"
#  define L_PLATFORM_ID             "Symbian"


#elif defined(__MINGW32__)
// MinGW

#  define L_PLATFORM_MINGW          1
#  define L_PLATFORM_NAME           "MinGW"
#  define L_PLATFORM_ID             "MINGW"

#elif defined(__linux__)
// Linux

#  define L_PLATFORM_LINUX          1
#  define L_PLATFORM_NAME           "Linux"
#  define L_PLATFORM_ID             "LINUX"

#elif defined(__ee__) || defined(__iop__) || ( (__GNUC__ == 2) && (defined(_R5900) || defined(_R3000) ) )
// PlayStation 2
// Detect _R5900 and _R3000 on GCC 2.x as it doesn't define __ee__ or __iop__

#  define L_PLATFORM_PS2            1
#  define L_PLATFORM_NAME           "PlayStation 2"
#  define L_PLATFORM_ID             "PS2"

#elif defined(__psp__)
// PlayStation Portable

#  define L_PLATFORM_PSP            1
#  define L_PLATFORM_NAME           "PlayStation Portable"
#  define L_PLATFORM_ID             "PSP"

#elif ( (defined(RVL_SDK) && defined(RVL)) || defined(L_PLATFORM_WII) )
// Nintendo Wii
// No Wii specific define, so use RVL_SDK and RVL define needed by RVL SDK

#  define L_PLATFORM_WII            1
#  define L_PLATFORM_NAME           "Nintendo Wii"
#  define L_PLATFORM_ID             "WII"

#elif ( defined(SDK_ARM9) ) && ( defined(SDK_TEG) || defined(SDK_TS) || defined(L_PLATFORM_NDS) )
// Nintendo DS
// No NDS specific define, so use ARM 7/9 and TEG/TS defines needed by Nitro SDK to detect NDS

#  define L_PLATFORM_NDS            1
#  define L_PLATFORM_NAME           "Nintendo DS"
#  define L_PLATFORM_ID             "NDS"

#elif defined(_XBOX360)
// Xenon

#  define L_PLATFORM_XBOX360        1
#  define L_PLATFORM_NAME           "Xbox 360"
#  define L_PLATFORM_ID             "XBOX360"

#elif defined(_XBOX)
// Xbox

#  define L_PLATFORM_XBOX           1
#  define L_PLATFORM_NAME           "Xbox"
#  define L_PLATFORM_ID             "XBOX"



#elif defined(_WIN32_WCE)
// Windows CE

#  define L_PLATFORM_WINCE          1
#  define L_PLATFORM_NAME           "Windows CE"
#  define L_PLATFORM_ID             "WINCE"

#elif defined(_WIN32)
// Windows 32/64 bit

#  define L_PLATFORM_WINDOWS 1

#  if defined(_WIN64)
#    define L_PLATFORM_WIN64          1
#    define L_PLATFORM_NAME           "Windows 64"
#    define L_PLATFORM_ID             "WIN64"
#  else
#    define L_PLATFORM_WIN32          1
#    define L_PLATFORM_NAME           "Windows 32"
#    define L_PLATFORM_ID             "WIN32"
#  endif

#else
// we don't recognise the platform
#  error "Unknown platform, please report this to dev@fourdoorlemon.com"

#endif

// Check platform identifiers

#if !defined(L_PLATFORM_ID) || !defined(L_PLATFORM_NAME)
#  error "Platform identifiers incomplete, please report this to dev@fourdoorlemon.com"
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Chipset selection and configuration
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// L_CHIPSET_xxx           - Chipset identifier
// L_CHIPSET_ID            - Identifier for chipset
// L_CHIPSET_NAME          - String description for chipset
//
// L_LITTLE_ENDIAN         - Little endian
// L_BIG_ENDIAN            - Big endian
// L_POINTER_64            - Pointers are 64bit
// L_LONG_IS_64BIT         - Long data type is 64 bit,
// L_HAS_INTRINSIC_INT64   - Intrinsic 64 bit data types
// L_HAS_INTRINSIC_INT128  - Intrinsic 128 bit data types
// L_NO_FLOAT_DOUBLE       - No double floating point support
// L_NO_FLOAT              - No floating point support
//                          long long' will be 128 bit if available.
//

#if defined(__ee__) || ( (__GNUC__ == 2) && defined(_R5900) )
// PlayStation 2 Emotion Engine
// Detect _R5900 on GCC 2.x as it doesn't define __ee__

#  define L_CHIPSET_PS2_EE          1
#  define L_CHIPSET_NAME            "R5900 (EE)"
#  define L_CHIPSET_ID              "EE"

#  define L_LITTLE_ENDIAN           1
#  define L_LONG_IS_64BIT           1
#  define L_HAS_INTRINSIC_INT64     1
#  define L_HAS_INTRINSIC_INT128    1
#  define L_NO_FLOAT_DOUBLE         1

#elif defined(__iop__) || ( (__GNUC__ == 2) && defined(_R3000) )
// PlayStation 2 Emotion Engine (MIPS/Sony/Toshiba)
// Detect _R3000 on GCC 2.x as it doesn't define __ee__

#  define L_CHIPSET_PS2_IOP         1
#  define L_CHIPSET_NAME            "R3000 (IOP)"
#  define L_CHIPSET_ID              "IOP"

#  define L_LITTLE_ENDIAN           1
#  define L_NO_FLOAT                1

#elif defined(__psp__) && ( defined(_R4000) || defined(__psp_gcc__) )
// Playstation Portable R4000 based
// Check for __psp_gcc__ as SCE GCC PSP defines _R3000 instead of R4000.

#  define L_CHIPSET_PSP_R4000       1
#  define L_CHIPSET_NAME            "R4000 (PSP)"
#  define L_CHIPSET_ID              "PSP_R4000"

#  define L_LITTLE_ENDIAN           1
#  define L_HAS_INTRINSIC_INT64     1
#  define L_HAS_INTRINSIC_INT128    1
#  define L_NO_FLOAT_DOUBLE         1

#elif ( (defined(__EPOC32__) && defined(__MARM__)) || defined(L_PLATFORM_SYMBIAN) )
// Symbian

#  define L_CHIPSET_SYMBIAN_ARMV5        1
#  define L_CHIPSET_NAME            "ARM v5"
#  define L_CHIPSET_ID              "ARM_V5"

#  define L_LITTLE_ENDIAN           1
#  define L_HAS_INTRINSIC_INT64     1

#elif ( (defined(RVL_SDK) && defined(RVL)) || defined(L_PLATFORM_WII) )
// Nintendo Wii broadway
// No Wii specific define, so use RVL_SDK and RVL define needed by RVL SDK

#  define L_CHIPSET_NDS_ARM9        1
#  define L_CHIPSET_NAME            "Broadway (Wii)"
#  define L_CHIPSET_ID              "WII_BROADWAY"

#  define L_BIG_ENDIAN              1
#  define L_HAS_INTRINSIC_INT64     1

#elif ( defined(SDK_ARM9) ) && ( defined(SDK_TEG) || defined(SDK_TS) || defined(L_PLATFORM_NDS) )
// Nintendo DS ARM 964e-s
// No NDS specific define, so use ARM 9 and TEG/TS defines needed by Nitro SDK

#  define L_CHIPSET_NDS_ARM9        1
#  define L_CHIPSET_NAME            "ARM 964e-s (NDS)"
#  define L_CHIPSET_ID              "NDS_ARM9"

#  define L_LITTLE_ENDIAN           1
#  define L_HAS_INTRINSIC_INT64     1
#  define L_NO_FLOAT                1

#elif defined(SDK_ARM7)  && ( defined(SDK_TEG) || defined(SDK_TS) )
// Nintendo DS ARM 7TDMI
// No NDS specific define, so use ARM 7 and TEG/TS defines needed by Nitro SDK

#  define L_CHIPSET_NDS_ARM9        1
#  define L_CHIPSET_NAME            "ARM 7TDMI (NDS)"
#  define L_CHIPSET_ID              "NDS_ARM7"

#  define L_LITTLE_ENDIAN           1
#  define L_NO_FLOAT                1

#elif defined(_XBOX360)
// Xbox 360 Xenon chipset

#  define L_CHIPSET_XENON           1
#  define L_CHIPSET_NAME            "XENON"
#  define L_CHIPSET_ID              "XENON"

#  define L_BIG_ENDIAN              1
#  define L_HAS_INTRINSIC_INT64     1

#elif defined(_M_IX86) || defined(_X86) || defined(__i386__)
// X86 (SSE/SSE2)

#  define L_CHIPSET_X86             1

#  if defined(_M_IX86_FP) && (_M_IX86_FP == 1)
#     define L_CHIPSET_NAME         "X86 (SSE)"
#     define L_CHIPSET_X86_SSE      1
#     define L_CHIPSET_ID           "X86SSE"
#  elif defined(_M_IX86_FP) && (_M_IX86_FP == 2)
#     define L_CHIPSET_NAME         "X86 (SSE2)"
#     define L_CHIPSET_X86_SSE2     1
#     define L_CHIPSET_ID           "X86SSE2"
#  else
#    define L_CHIPSET_NAME          "X86"
#    define L_CHIPSET_ID            "X86"
#  endif

#  define L_LITTLE_ENDIAN           1
#  define L_HAS_INTRINSIC_INT64     1

#elif defined(_M_X64) || defined(__x86_64__)
// X64

#  define L_CHIPSET_X64             1
#  define L_CHIPSET_NAME            "X64"
#  define L_CHIPSET_ID              "X64"

#  define L_LITTLE_ENDIAN           1
#  define L_POINTER_64              1
#  define L_HAS_INTRINSIC_INT64     1

#elif defined(_M_IA64) || defined(__ia64__)
// IA64

#  define L_CHIPSET_IA64            1
#  define L_CHIPSET_NAME            "IA64"
#  define L_CHIPSET_ID              "IA64"

#  define L_LITTLE_ENDIAN           1
#  define L_POINTER_64              1
#  define L_HAS_INTRINSIC_INT64     1

#else
// we don't recognise the chipset
#  error "Unknown chipset, please report this to dev@fourdoorlemon.com"

#endif

// Check chipset identifiers

#if !defined(L_CHIPSET_ID) || !defined(L_CHIPSET_NAME)
#  error "Platform identifiers incomplete, please report this to dev@fourdoorlemon.com"
#endif

// Endian

#if !defined(L_LITTLE_ENDIAN) && !defined(L_BIG_ENDIAN)
#  error "L_LITTLE_ENDIAN or L_BIG_ENDIAN was not defined, please report this to dev@fourdoorlemon.com"
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Linkage and Namespace
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Default to library build if L_DLL is not defined

#if !defined(L_LIB) && !defined(L_DLL)
#  define L_LIB 1
#endif

// Configure namespace and exports for lib

#if defined(L_LIB)
#  if defined(L_DLL)
#  error L_LIB must not be mixed with L_DLL
#  endif
#  define L_NAMESPACE_NAME Lemon_lib
#  define L_EXPORT
#  define L_IMPORT
#endif

// Configure namespace and exports for dll

#if defined(L_DLL)
#  if defined(L_LIB)
#  error L_DLL must not be mixed with L_LIB
#  endif
#  define L_NAMESPACE_NAME Lemon_dll
#  define L_EXPORT L_COMPILER_DLLEXPORT
#  define L_IMPORT L_COMPILER_DLLIMPORT
#endif

// Set up namespace

#if defined(__cplusplus)
#  define BEGIN_L_NAMESPACE namespace L_NAMESPACE_NAME {
#  define END_L_NAMESPACE }
BEGIN_L_NAMESPACE
END_L_NAMESPACE
namespace Lemon {
  using namespace L_NAMESPACE_NAME;
}
#else
#  define BEGIN_L_NAMESPACE
#  define END_L_NAMESPACE
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Data types
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// Only declare Lemon data types when compiling C++ as they are declared in
// the Lemon linkage namespace only.
// Ambiguities are resolved using Koenig (argument dependent) lookup.

#if defined(__cplusplus)
BEGIN_L_NAMESPACE

#  if defined(L_CHAR_IS_UNSIGNED)
typedef signed char int8;
#  else
typedef char int8;
#  endif

typedef unsigned char uint8;

typedef short   int16;
typedef int     int32;
typedef unsigned short uint16;
typedef unsigned int   uint32;

// Intrinsic 64 bit data type

#  if defined(L_HAS_INTRINSIC_INT64)
#    if defined(L_HAS_MSVC_INT64)
typedef __int64   int64;
typedef unsigned __int64 uint64;
#    elif defined(L_LONG_IS_64BIT)
typedef long int64;
typedef unsigned long uint64;
#    elif defined(L_HAS_LONG_LONG) && !defined(L_LONG_IS_64BIT)
typedef long long int64;
typedef unsigned long long uint64;
#    elif defined(__GNUC__)
typedef int   int64 __attribute__(( mode(DI) ));
typedef unsigned int uint64 __attribute__(( mode(DI) ));
#    else
#      error "Don't know how to declare 64 bit intrinsic type for platform and compiler"
#    endif
#endif

// Intrinsic 128 bit data type

#  if defined(L_HAS_INTRINSIC_INT128)
#    if defined(L_HAS_LONG_LONG) && defined(L_LONG_IS_64BIT)
typedef long long int128;
typedef unsigned long long uint128;
#    elif defined(__GNUC__)
typedef int   int128 __attribute__(( mode(TI) ));
typedef unsigned int uint128 __attribute__(( mode(TI) ));
#    else
#      error "Don't know how to declare 128 bit intrinsic type for platform and compiler"
#    endif
#  endif

// Check type sizes

L_COMPILE_TIME_ASSERT( int8,    sizeof(int8)   == 1 );
L_COMPILE_TIME_ASSERT( uint8,   sizeof(uint8)  == 1 );
L_COMPILE_TIME_ASSERT( int16,   sizeof(int16)  == 2 );
L_COMPILE_TIME_ASSERT( uint16,  sizeof(uint16) == 2 );
L_COMPILE_TIME_ASSERT( int32,   sizeof(int32)  == 4 );
L_COMPILE_TIME_ASSERT( uint32,  sizeof(uint32) == 4 );

#if defined(L_HAS_INTRINSIC_INT64)
L_COMPILE_TIME_ASSERT( int64,   sizeof(int64)  == 8 );
L_COMPILE_TIME_ASSERT( uint64,  sizeof(uint64) == 8 );
#endif

#if defined(L_HAS_INTRINSIC_INT128)
L_COMPILE_TIME_ASSERT( int128,  sizeof(int128)  == 16 );
L_COMPILE_TIME_ASSERT( uint128, sizeof(uint128) == 16 );
#endif

END_L_NAMESPACE
#endif

// Add to global namespace as well. See notes at the top of this file regarding
// the data type issue.

#  if defined(L_CHAR_IS_UNSIGNED)
typedef signed char lint8_t;
#  else
typedef char lint8_t;
#  endif

typedef unsigned char luint8_t;

typedef short   lint16_t;
typedef int     lint32_t;
typedef unsigned short luint16_t;
typedef unsigned int   luint32_t;

// Intrinsic 64 bit data type

#  if defined(L_HAS_INTRINSIC_INT64)
#    if defined(L_HAS_MSVC_INT64)
typedef __int64   lint64_t;
typedef unsigned __int64 luint64_t;
#    elif defined(L_LONG_IS_64BIT)
typedef long lint64_t;
typedef unsigned long luint64_t;
#    elif defined(L_HAS_LONG_LONG) && !defined(L_LONG_IS_64BIT)
typedef long long lint64_t;
typedef unsigned long long luint64_t;
#    elif defined(__GNUC__)
typedef int   lint64_t __attribute__(( mode(DI) ));
typedef unsigned int luint64_t __attribute__(( mode(DI) ));
#    else
#      error "Don't know how to declare 64 bit intrinsic type for platform and compiler"
#    endif
#endif

// Intrinsic 128 bit data type

#  if defined(L_HAS_INTRINSIC_INT128)
#    if defined(L_HAS_LONG_LONG) && defined(L_LONG_IS_64BIT)
typedef long long lint128_t;
typedef unsigned long long luint128_t;
#    elif defined(__GNUC__)
typedef int   lint128_t __attribute__(( mode(TI) ));
typedef unsigned int luint128_t __attribute__(( mode(TI) ));
#    else
#      error "Don't know how to declare 128 bit intrinsic type for platform and compiler"
#    endif
#  endif

// Check type sizes

L_COMPILE_TIME_ASSERT( lint8_t,    sizeof(lint8_t)   == 1 );
L_COMPILE_TIME_ASSERT( luint8_t,   sizeof(luint8_t)  == 1 );
L_COMPILE_TIME_ASSERT( lint16_t,   sizeof(lint16_t)  == 2 );
L_COMPILE_TIME_ASSERT( luint16_t,  sizeof(luint16_t) == 2 );
L_COMPILE_TIME_ASSERT( lint32_t,   sizeof(lint32_t)  == 4 );
L_COMPILE_TIME_ASSERT( luint32_t,  sizeof(luint32_t) == 4 );

#if defined(L_HAS_INTRINSIC_INT64)
L_COMPILE_TIME_ASSERT( lint64_t,   sizeof(lint64_t)  == 8 );
L_COMPILE_TIME_ASSERT( luint64_t,  sizeof(luint64_t) == 8 );
#endif

#if defined(L_HAS_INTRINSIC_INT128)
L_COMPILE_TIME_ASSERT( lint128_t,  sizeof(lint128_t)  == 16 );
L_COMPILE_TIME_ASSERT( luint128_t, sizeof(luint128_t) == 16 );
#endif


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Run-time
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#if defined(_MSC_VER) && !defined( _VC_NODEFAULTLIB )

// Check for multithreaded runtime
#  if !defined(_MT)
#    error "Compiler option: _MT not defined. Must link against a multithreaded MSVC runtime (/MD[d] /MT[d])."
#  endif

// Check dll/static runtime

#  if defined(L_DLL)
#    if !defined(_DLL)
#      error "Compiler option: L_DLL defined. Must use multithreaded DLL MSVC runtime (/MD[d])."
#    endif
#  else
#    if defined(_DLL)
#      error "Compiler option: L_DLL not defined. Must use multithreaded static MSVC runtime (/MT[d])."
#    endif
#  endif

#endif

// TIMJ: TEMP HACK!
#if defined(L_PLATFORM_NDS)
#pragma showmessagenumber on
//#pragma warning off (10317) // type conversion
#pragma warning off (10182) // unused parameters
//#pragma warning off (10182) // unused parameters
#pragma warning off (10107) // triling commas on declaration lists
#pragma warning off (10216) // empty declarations
#pragma warning off (10178) // function has no prototype
#endif

#endif // LAPI_H_INCLUDED


