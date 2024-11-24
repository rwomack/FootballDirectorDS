//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcore.h
// Description : Common include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCORE_H_INCLUDED
#define LCORE_H_INCLUDED

#include <time.h>     // for time_t
#include <string.h>   // for string functions
#include <stdlib.h>   // for _MAX_PATH

// include the api linkage definitions

#include "../lapi.h"

// api linkage

#ifdef LPLATFORM_BUILD
#  define LPLATFORM_API L_EXPORT
#else
#  define LPLATFORM_API L_IMPORT
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Temp compatability API
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus
extern "C" {
#endif

#if defined(L_PLATFORM_SYMBIAN)
#define strcmpi strcasecmp
#endif

#if defined(L_PLATFORM_WIN32) || defined(L_PLATFORM_XBOX) || defined(L_PLATFORM_XBOX360)
#  define lPlatform_vsnprintf _vsnprintf
#  define lPlatform_snprintf  _snprintf
#  define lPlatform_itoa      itoa
#  define lPlatform_makepath  _makepath
#  define lPlatform_splitpath _splitpath

#elif defined(L_PLATFORM_NDS)
//#  define lPlatform_snprintf OS_SNPrintf
#  define lPlatform_vsnprintf OS_VSNPrintfEx
int    lPlatform_snprintf(char *, unsigned int size, const char *, ...);
//int    lPlatform_snprintf(char *dst, size_t len, const char *fmt, ...);
//int    lPlatform_vsnprintf(char *dst, size_t len, const char *fmt, va_list vlist);
char * lPlatform_itoa ( int val, char *buf, int radix );
void   lPlatform_splitpath ( const char *path, char *drive, char *dir, char *fname, char *ext );
void   lPlatform_makepath ( char *path, const char *drive, const char *dir, const  char *fname, const char *ext );

#else

//TODO: Change vsnprintf to be an actual function as well so we can perform error checking if needed in it
#  define lPlatform_vsnprintf(a,b,c,d) vsprintf(a,c,d)
char * lPlatform_itoa ( int val, char *buf, int radix );
void   lPlatform_splitpath ( const char *path, char *drive, char *dir, char *fname, char *ext );
void   lPlatform_makepath ( char *path, const char *drive, const char *dir, const  char *fname, const char *ext );
int    lPlatform_snprintf(char *, unsigned int size, const char *, ...);

#if defined(L_PLATFORM_SYMBIAN)
  #include "symbian_typeinfo.h"
#endif
#endif                                  // #if defined(L_PLATFORM_WIN32) || defined(L_PLATFORM_XBOX)

#if defined(L_PLATFORM_NDS) || defined(L_PLATFORM_WII) || defined(L_PLATFORM_SYMBIAN)
  #define __max(a,b)  (((a) > (b)) ? (a) : (b))
  #define __min(a,b)  (((a) < (b)) ? (a) : (b))

  #ifndef _MAX_PATH
  #define _MAX_PATH   260 /* max. length of full pathname */
  #endif
  #ifndef _MAX_DRIVE
  #define _MAX_DRIVE  3   /* max. length of drive component */
  #endif
  #ifndef _MAX_DIR
  #define _MAX_DIR    256 /* max. length of path component */
  #endif
  #ifndef _MAX_FNAME
  #define _MAX_FNAME  256 /* max. length of file name component */
  #endif
  #ifndef _MAX_EXT
  #define _MAX_EXT    256 /* max. length of extension component */
  #endif

  int jn( unsigned int n, float x );
  #define j1(x) jn(1,x)

#endif

#ifdef L_PLATFORM_NDS
  struct tm
  {
    int	tm_sec;
    int	tm_min;
    int	tm_hour;
    int	tm_mday;
    int	tm_mon;
    int	tm_year;
    int	tm_wday;
    int	tm_yday;
    int	tm_isdst;
  };

  tm* localtime( const time_t* );
  time_t time( time_t * );

  int lPlatform_stricmp ( const char * dst, const char * src );

  #define strcmpi lPlatform_stricmp
  #define stricmp lPlatform_stricmp
  //Unsupported file i/o functions
  #define fprintf
  #define fopen(a,b) NULL
  #define fdopen(a,b) NULL
#endif

#ifdef __cplusplus
}
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Platform API
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus
extern "C" {
#endif

//##
// Perform platform specific initialization
// Remarks:
//   Performs platform specific initialization. Should be called at the
//   start of the program.
// Win32:
//   This is a no-op on Win32 platforms.
// PS2:
//   Performs IOP image reboot, and basic IOP
//   module initialisation for the CD and file system.
//
// PSP:
//   Disables FPU exceptions.
LPLATFORM_API void lPlatform_Init();

//##
// Perform platform specific processing
// Remarks:
//   Performs platform specific processing to cooperate with the operating
//   system. See the platform specific notes for the use and affect of this
//   function.
//   lPlatform_Init should be called often enough to allow platform specific
//   processing to occur, usually once per frame. It does not perform any
//   per-frame or time based tracking so it is safe to call multiple times
//   per frame, or not call for several frames.
//
// Out:
//   Returns true if the application should continue,
//   or false if the application should exit.
// Win32:
//   On the win32 platform this functions manages the win32 message pump using
//   PeekMessage, TranslateMessage and DispatchMessage for the main thread.
//   If the WM_QUIT message is received this functions returns false, otherwise
//   it returns true.
//   <b>If you are providing your own message pump there is no need to call this
//   function.</b>
//   Not calling this function, or more precisely not providing a message pump,
//   means win32 messages will not be delivered causing the message queue to
//   overflow and the application will not respond to quit messages such as the
//   close button and ALT-F4.
//
// PS2:
//   This is a no-op on the PlayStation2 platform.
//
LPLATFORM_API bool lPlatform_Process();

//##
// Failed assertion function
// Remarks: Called by the lAssert and lAssertAlways macros when an assertion fails.
// In:
//   function - name of function
//   file     - name of file
//   line     - line in file
//   text     - assertion expression text
//
#if !defined(L_FINAL)
LPLATFORM_API void lPlatform_FailAssert( const char * function, const char * file, unsigned int line, const char * text );
#else
#define lPlatform_FailAssert( function, file, line, text ) ((void)0)
#endif

//##
// Debug stream output
// Remarks: Output a string to the platfrom debug stream
// In: str - string to output to the stream
//
LPLATFORM_API void lPlatform_DebugStreamPrint(const char *str);

// System info

//##
// Get the SDK build name
// Out: Returns the SDK build name
LPLATFORM_API const char * lPlatform_BuildName(void);

//##
// Get the platform name
// Out: Returns the friendly platform name
LPLATFORM_API const char * lPlatform_PlatformName(void);

//## Get the platform ID
// Out: Returns the platform ID suitable for simply identifying
// the platform in code. The format is [PLATFORM]/[VER1]/[VER2]/[INFO].
//<TABLE>
//  Part        Meaning
//  ----------  -----------------------------
//  PLATFORM    Platform tag
//  VER1        Major version
//  VER2        Minor version
//  INFO        Platform specific info
//</TABLE>
//
LPLATFORM_API const char * lPlatform_PlatformId(void);

//##
// Get the computer name
// Out: Returns the computer name
LPLATFORM_API const char * lPlatform_ComputerName(void);

//##
// Get the user name
// Out: Return the logged in user name running the application
LPLATFORM_API const char * lPlatform_UserName(void);

//##
// Get the CPU name
// Out: Returns the name of the CPU
LPLATFORM_API const char * lPlatform_ProcessorName(void);

//##
// Get the CPU feature string
// Out: Returns a space delimited list of CPU features.
// Todo: list the possible features and platforms.
LPLATFORM_API const char * lPlatform_ProcessorFeatures(void);

//##
// Get the CPU speed in Hz
// Out: Return the processor speed in Hz
LPLATFORM_API double       lPlatform_ProcessorSpeed(void);

//##
// Get the number of processors
// Out: Returns the number of processors
LPLATFORM_API unsigned int lPlatform_ProcessorNum(void);

//##
// Get the amount of physical memory
// Out: Returns the amount of physical memory
LPLATFORM_API unsigned int lPlatform_PhysicalMemory(void);

//##
// Get the page file size
// Out: Returns the page file size
LPLATFORM_API unsigned int lPlatform_PageFileMemory(void);

//##
// Get the amount of virtual memory
// Out: Returns the amount of virtual memory
LPLATFORM_API unsigned int lPlatform_VirtualMemory(void);

//##
// Check if the processor has a feature
// In: featureString - feature to check for
// Out: Returns true if the feature is available on the CPU, otherwise false
LPLATFORM_API bool         lPlatform_ProcessorHasFeature(const char *featureString);

#ifdef __cplusplus
}
#endif

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Macros
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Break

//##lBreak
// Perform a break command on this platform
#if defined( L_PLATFORM_WINDOWS )
#  define lBreak() { __debugbreak(); }
#elif defined( L_PLATFORM_PS2 )
#  define lBreak() { asm("break 1"); }
#elif defined( L_PLATFORM_NDS )
#  define lBreak() { asm("bkpt 0"); }
#else
#  define lBreak()
#endif


// Assertions / Template assertions and errors

//##lAssert
// Run-time time assertion
// In: _exp - expression to assert is true
// Remarks: Use this macro to perform a run-time assertion in debug mode (when L_DEBUG is defined).
// If L_DEBUG is not defined this macro will be compiled out to nothing.

#if defined(L_DEBUG)
#  define lAssert(_exp) (void)( (_exp) || (lPlatform_FailAssert(__FUNCTION__, __FILE__, __LINE__, #_exp), 0) )
#else
#  define lAssert(__ignore) ((void)0)
#endif

//##
// An assertion that is always checked
// In: _exp - expression to assert is true
// Remarks: Use this macro for an assertion that is always check in regardless
// of the compilation options (L_DEBUG). Removed only in L_FINAL builds.
//
#if !defined(L_FINAL)
#  define lAssertAlways(_exp) (void)( (_exp) || (lPlatform_FailAssert(__FUNCTION__, __FILE__, __LINE__, #_exp), 0) )
#else
#  define lAssertAlways(__ignore) ((void)0)
#endif


#ifdef __cplusplus
template <bool B> struct l_templateAssert {};
template <> struct l_templateAssert<true> { static void there_has_been_a_compile_time_assertion() {}; };

//##
// Compile time assertion
// In: _a   - expression
// Remarks:
//  If the expression <b>_a</b> evaluates to false at compile time, a compiler error will occur
//  with <b>there_has_been_a_compile_time_assertion</b> embedded in the error message.
#define lTAssert(_a)     { const bool _b = (_a) ? true:false; l_templateAssert<_b>::there_has_been_a_compile_time_assertion(); }

//##
// Compile time error
// In: _a   - expression
//     _msg - message to print
// Remarks:
//  If the expression <b>_a</b> evaluates to false at compile time, a compiler error will occur
//  with <b>_msg</b> embedded in it.
#define lTError(_a,_msg) { const bool _b = (_a) ? true:false; l_templateAssert<_b>::##_msg();}
#endif

//##
// Get offset of field within structure
//
#define L_FIELD_POS( _type , _field ) ((long)&(( _type * )NULL)->_field)

//##
// Treat a floating point number as a long without conversion
//
#define L_FLOAT_LONG( _f ) (*(long *)&_f)

//##
// Treat a long as a floating point number without conversion
//
#define L_LONG_FLOAT( _f ) (*(float *)&_f)

//##
// Isloate high word
//
#define L_LOWORD( _dd ) (short)(_dd)

//##
// Isloate low word
//
#define L_HIWORD( _dd ) (short)(_dd>>16)

//##
// Isloate high byte
//
#define L_LOBYTE( _dw ) (char)(_dw)

//##
// Isloate low byte
//
#define L_HIBYTE( _dw ) (char)(_dw>>8)

//##
// Build a colour from RGB values
//
#define L_RGB( _r, _g, _b )      ((unsigned int)(((int)(_r)<<16) | ((int)(_g)<<8)  | (int)(_b)))

//##
// Build a colour from RGBA values
//
#define L_RGBA( _r, _g, _b, _a ) ((unsigned int)(((int)(_r)<<24) | ((int)(_g)<<16) | ((int)(_b)<<8) | (int)(_a) ))

//##
// Build a colour from ARGB values
//
#define L_ARGB( _a, _r, _g, _b ) ((unsigned int)(((int)(_a)<<24) | ((int)(_r)<<16) | ((int)(_g)<<8) | (int)(_b) ))

//##
// Build a multi-character constant
//
#define L_MAKE_ID( _a, _b, _c, _d ) ( (unsigned int)(unsigned char)(_a) | ( (unsigned int)(unsigned char)(_b) << 8 ) | ( (unsigned int)(unsigned char)(_c) << 16 ) | ( (unsigned int)(unsigned char)(_d) << 24 ) )

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Utility templates routines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus
BEGIN_L_NAMESPACE

// Endian conversion

struct lEndian
{
  static inline luint64_t Swap64( luint64_t x )
  {
    return ((x & 0x00000000000000ffLL) << 56) |
           ((x & 0x000000000000ff00LL) << 40) |
           ((x & 0x0000000000ff0000LL) << 24) |
           ((x & 0x00000000ff000000LL) << 8)  |
           ((x & 0x000000ff00000000LL) >> 8)  |
           ((x & 0x0000ff0000000000LL) >> 24) |
           ((x & 0x00ff000000000000LL) >> 40) |
           ((x & 0xff00000000000000LL) >> 56);
  }

  static inline luint32_t Swap32( luint32_t x )
  {
    return
      ( (x&0x000000ff) << 24) |
      ( (x&0x0000ff00) << 8)  |
      ( (x&0x00ff0000) >> 8)  |
      ( (x&0xff000000) >> 24);
  }

  static inline luint16_t Swap16( luint16_t x )
  {
    return(luint16_t)( ( (x&0x00ff) << 8)  | ( (x&0xff00) >> 8) );
  }

  #ifdef L_LITTLE_ENDIAN
  static inline luint64_t Big64( luint64_t x ) { return Swap64(x); }
  static inline luint64_t Little64( luint64_t x ) { return x; }

  static inline luint32_t Big32( luint32_t x ) { return Swap32(x); }
  static inline luint32_t Little32( luint32_t x ) { return x; }

  static inline luint16_t Big16( luint16_t x ) { return Swap16(x); }
  static inline luint16_t Little16( luint16_t x ) { return x; }
  #endif

  #ifdef L_BIG_ENDIAN
  static inline luint64_t Big64( luint64_t x ) { return x; }
  static inline luint64_t Little64( luint64_t x ) { return Swap64(x); }

  static inline luint32_t Big32( luint32_t x ) { return x; }
  static inline luint32_t Little32( luint32_t x ) { return Swap32(x); }

  static inline luint16_t Big16( luint16_t x ) { return x; }
  static inline luint16_t Little16( luint16_t x ) { return Swap16(x); }
  #endif
};

//##
// Unpack a 16 bit value into to two 8 bit values
// Remarks: Unpack the 16 bit value <b>v</b> into to 8 bit values <b>b0</b> and <b>b1</b>.
// <code>
// 16            8             0
// | b1 -------- | b0 -------- |
// </code>
// In: v - 16 bit value to unpack
//     b0 - value to receive lower 8 bits
//     b1 - value to receive upper 8 bits
// Out: Sets <b>b0</b> and <b>b1</b> to the lower and upper 8 bits of <b>v</b>
//
inline void lBitUnpack16( luint16_t v, luint8_t &b0, luint8_t &b1 )
{
  b0 = luint8_t(v);
  b1 = luint8_t(v >> 8);
}

//##
// Pack two 8 bits values into a 16 bit value
// Remarks: Pack the two 8 bit values <b>b0</b> and <b>b1</b> into a 16 bit value and return it.
// <code>
// 16            8             0
// | b1 -------- | b0 -------- |
// </code>
// In: b0 - value for the lower 8 bits
//     b1 - value for upper 8 bits
// Out: Returns the 16 bit result.
//
inline luint16_t lBitPack16( luint8_t b0, luint8_t b1 )
{
  return (luint16_t)(luint16_t(b0) + (luint16_t(b1) << 8));
}

//##
// Unpack a 32 bit value into to four 8 bit values
// Remarks: Unpack the 32 bit value <b>v</b> into to four 8 bit values <b>b0</b>, <b>b1</b>, <b>b2</b> and <b>b3</b>.
// <code>
// 32            24            16            8             0
// | b3 -------- | b2 -------- | b1 -------- | b0 -------- |
// </code>
// In: v - 32 bit value to unpack
//     b0 - value to receive bits 8..0
//     b1 - value to receive bits 16..0
//     b2 - value to receive bits 24..16
//     b3 - value to receive bits 32..24
// Out: Unpacks <b>v</b> and into <b>b0</b>, <b>b1</b>, <b>b2</b> and <b>b3</b>.
//
inline void lBitUnpack32( luint32_t v, luint8_t &b0, luint8_t &b1, luint8_t &b2, luint8_t &b3 )
{
  b0 = luint8_t(v);
  b1 = luint8_t(v >> 8);
  b2 = luint8_t(v >> 16);
  b3 = luint8_t(v >> 24);
}

//##
// Pack four 8 bits values into a 32 bit value
// Remarks: Pack the four 8 bit values <b>b0</b>, <b>b1</b>, <b>b2</b> and <b>b4</b> into a 32 bit value and return it.
// <code>
// 32            24            16            8             0
// | b3 -------- | b2 -------- | b1 -------- | b0 -------- |
// </code>
// In: b0 - value for bits 8..0
//     b1 - value for bits 16..0
//     b2 - value for bits 24..16
//     b3 - value for bits 32..24
// Out: Returns the 32 bit result.
//
inline luint32_t lBitPack32( luint8_t b0, luint8_t b1, luint8_t b2, luint8_t b3 )
{
  return luint32_t(b0) + (luint32_t(b1) << 8) + (luint32_t(b2) << 16) + (luint32_t(b3) << 24);
}

//##
// Unpack a 64 bit value into to eight 8 bit values
// Remarks: Unpack the 64 bit value <b>v</b> into to eight 8 bit values <b>b0..7</b>.
// <code>
// 64       56       48       40       32       24       16       8        0
// | b7 --- | b6 --- | b5 --- | b4 --- | b3 --- | b2 --- | b1 --- | b0 --- |
// </code>
// In: v - 32 bit value to unpack
//     b0 - value to receive bits 8..0
//     b1 - value to receive bits 16..0
//     b2 - value to receive bits 24..16
//     b3 - value to receive bits 32..24
//     b4 - value to receive bits 40..32
//     b5 - value to receive bits 48..40
//     b6 - value to receive bits 56..48
//     b7 - value to receive bits 64..56
// Out: Unpacks <b>v</b> and into <b>b0</b>, <b>b1</b>, <b>b2</b>, <b>b3</b>, <b>b4</b>, <b>b5</b>, <b>b6</b> and <b>b7</b>.
//
#if defined(L_HAS_INTRINSIC_INT64)
inline void lBitUnpack64( uint64 v, luint8_t &b0, luint8_t &b1, luint8_t &b2, luint8_t &b3, luint8_t &b4, luint8_t &b5, luint8_t &b6, luint8_t &b7 )
{
  b0 = luint8_t(v);
  b1 = luint8_t(v >> 8);
  b2 = luint8_t(v >> 16);
  b3 = luint8_t(v >> 24);
  b4 = luint8_t(v >> 32);
  b5 = luint8_t(v >> 40);
  b6 = luint8_t(v >> 48);
  b7 = luint8_t(v >> 56);
}
#endif

//##
// Pack eight 8 bits values into a 64 bit value
// Remarks: Pack the eight 8 bit values <b>b0..7</b> into a 64 bit value and return it.
// <code>
// 64       56       48       40       32       24       16       8        0
// | b7 --- | b6 --- | b5 --- | b4 --- | b3 --- | b2 --- | b1 --- | b0 --- |
// </code>
// In: v - 32 bit value to unpack
//     b0 - value for bits 8..0
//     b1 - value for bits 16..0
//     b2 - value for bits 24..16
//     b3 - value for bits 32..24
//     b4 - value for bits 40..32
//     b5 - value for bits 48..40
//     b6 - value for bits 56..48
//     b7 - value for bits 64..56
// Out: Returns the 64 bit result.
//
#if defined(L_HAS_INTRINSIC_INT64)
inline uint64 lBitPack64( luint8_t b0, luint8_t b1, luint8_t b2, luint8_t b3, luint8_t b4, luint8_t b5, luint8_t b6, luint8_t b7 )
{
  return uint64(b0) + (uint64(b1) << 8) + (uint64(b2) << 16) + (uint64(b3) << 24) +
         (uint64(b4) << 32) + (uint64(b5) << 40) + (uint64(b6) << 48) + (uint64(b7) << 56);
}
#endif

//##
// Check if a pointer is aligned on a 32 bit boundary
// In: ptr - pointer to check
// Out: Returns true if the pointer points to data aligned on a 32 bit boundary.
// Otherwise returns false.
//
inline bool lIsPtrAligned32( const void * ptr )
{
  return ((luint8_t*)ptr-((luint8_t*)0) & 0x3) == 0;
}

//##
// Check if a pointer is aligned on a 64 bit boundary
// In: ptr - pointer to check
// Out: Returns true if the pointer points to data aligned on a 64 bit boundary.
// Otherwise returns false.
//
inline bool lIsPtrAligned64( const void * ptr )
{
  return ((luint8_t*)ptr-((luint8_t*)0) & 0x7) == 0;
}

//##
// Check is a pointer is aligned on a 128 bit boundary
// In: ptr - pointer to check
// Out: Returns true if the pointer points to data aligned on a 128 bit boundary.
// Otherwise returns false.
//
inline bool lIsPtrAligned128( const void * ptr )
{
  return ((luint8_t*)ptr-((luint8_t*)0) & 0xf) == 0;
}

//##
//<COMBINE lAlign>
template<class T> T lBitAlign( const T x, unsigned int align );
template<class T> inline T lBitAlign( const T x, unsigned int align )
{
  return (T)(((unsigned int)x + align-1) & ~(align-1));
}

//##
// Align a value downwards, for power of 2 'align' values only
template<class T> inline T lAlignDownPow2( const T val, luint32_t align )
{
  return (T)((luint32_t)val & ~(align-1));
}

//##
// Align a value upwards, for power of 2 'align' values only
//TODO: ERROR: UNIT TEST ME < 64
template<class T> inline T lAlignUpPow2( luint32_t val, luint32_t align )
{
  return (T)(((luint32_t)val + (align-1)) & ~(align-1));
}

//##
// Return the number of 1 bits in x
inline int lBitCount( int x )
{
  x -= ( ( x >> 1 ) & 0x55555555 );
  x = ( ( ( x >> 2 ) & 0x33333333 ) + ( x & 0x33333333 ) );
  x = ( ( ( x >> 4 ) + x ) & 0x0f0f0f0f );
  x += ( x >> 8 );
  return ( ( x + ( x >> 16 ) ) & 0x0000003f );
}

//##
// Return the bit reverse of x
inline int lBitReverse( int x )
{
  x = ( ( ( x >> 1 ) & 0x55555555 ) | ( ( x & 0x55555555 ) << 1 ) );
  x = ( ( ( x >> 2 ) & 0x33333333 ) | ( ( x & 0x33333333 ) << 2 ) );
  x = ( ( ( x >> 4 ) & 0x0f0f0f0f ) | ( ( x & 0x0f0f0f0f ) << 4 ) );
  x = ( ( ( x >> 8 ) & 0x00ff00ff ) | ( ( x & 0x00ff00ff ) << 8 ) );
  return ( ( x >> 16 ) | ( x << 16 ) );
}

//##
// Align on arbitrary boundry
// Remarks: Align <b>x</b> value to nearest <b>align</b> boundary.
// For lBitAlign <b>align</b> must be a power-of-2 otherwise the results are undefined.
// In: x - value to align
//     align - alignment
// Out: Returns <b>x</b> aligned to the nearest <b>align</b> boundary
//
//
template<class T> T lAlign( const T x, unsigned int align );
template<class T> inline T lAlign( const T x, unsigned int align )
{
  if( x == 0 ) return x;
  T top = (T)x+align;
  T a   = (T)(top/align) * align;
  if( a == top ) return x;         // evenly divides, return original value
  else return a;                   // odd divide, round to next boundary
}

//##
// Swap two values
// Remarks: Swap the two values.
// Use lSwapL for typecasted swap using xor method.
// In: a - first value
//     b - second value
//
template<class T> void lSwap( T& a, T& b );
template<class T> inline void lSwap( T& a, T& b )
{
  if( &a != &b )
  {
    const T t = a;
    a = b;
    b = t;
  }
}

//##
//<COMBINE lSwap>
template<class T> void lSwapL( T &a, T &b );
template<class T> inline void lSwapL( T &a, T &b )
{
  // note: if both a and b are the same, this function fails.
  //       with this extra if() is it actually worth doing the xor method.
  if( a == b ) return;

  (*(unsigned long *)&(a)) ^= (*(unsigned long *)&(b));
  (*(unsigned long *)&(b)) ^= (*(unsigned long *)&(a));
  (*(unsigned long *)&(a)) ^= (*(unsigned long *)&(b));
}

//##
// Get the minimum or maximum of two values
// In: a - first value
//     b - second value
// Out: Minumum or maximum of a and b
//
template<class T> const T lMin(const T& a,const T& b);
template<class T> inline const T lMin(const T& a,const T& b) { return (a<b) ? a : b; }

//##
//<COMBINE lMin>
template<class T> const T lMax(const T& a,const T& b);
template<class T> inline const T lMax(const T& a,const T& b) { return (a>b) ? a : b; }

//##
// Get the minimum or maximum of two values
// In: a - first value
//     b - second value
//     c - third value
// Out: Minumum or maximum of a, b and
//
template<class T> const T lMin3(const T& a,const T& b,const T& c);
template<class T> inline const T lMin3(const T& a,const T& b,const T& c) { return ((c < ((a < b) ? a : b)) ? c : ((a < b) ? a : b)); }

//##
//<COMBINE lMin3>
template<class T> const T lMax3(const T& a,const T& b,const T& c);
template<class T> inline const T lMax3(const T& a,const T& b,const T& c) { return ((c > ((a > b) ? a : b)) ? c : ((a > b) ? a : b)); }

END_L_NAMESPACE
#endif /*__cplusplus*/

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// include core systems
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#include "lfatalerror.h"
#include "lcorelog.h"
#include "lcoretimer.h"
#include "lcorefile.h"
#include "lcorememory.h"
#include "lcoreexception.h"

#endif

