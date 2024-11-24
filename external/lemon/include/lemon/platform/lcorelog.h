//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcorelog.h
// Description : Log include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCORELOG_H_INCLUDED
#define LCORELOG_H_INCLUDED

#include "lcore.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Logging API
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus
extern "C" {
#endif

//##
// Log hook function
// In: str - string to print
// See Also: lLogCHook, lLogCUnhook
typedef void (*lCLogHookFunction_t)(const char *str);

#if !defined(L_FINAL)

LPLATFORM_API void   lLogCHook( lCLogHookFunction_t logPrinter );
LPLATFORM_API void   lLogCUnhook( lCLogHookFunction_t logPrinter );

// Log messages

LPLATFORM_API void   lLogReset();
LPLATFORM_API void   lLogFmt( const char *fmt, ... );
LPLATFORM_API void   lLogSys( const char *sys, const char *fmt, ... );
LPLATFORM_API void   lLogWarning( int level, const char *fmt, ... );
LPLATFORM_API void   lLogPrint( const char *fmt, ... );

// Log formating

LPLATFORM_API void   lLogBreak( void );
LPLATFORM_API int    lLogIndent( int value );
LPLATFORM_API int    lLogUnindent( int value );
LPLATFORM_API void   lLogSetIndent( int value );
LPLATFORM_API int    lLogGetIndent( void );

#else

#define              lLogCHook( logPrinter )            ((void)0)
#define              lLogCUnhook( logPrinter )          ((void)0)

// Log messages

#define              lLogReset()                        ((void)0)
#define              lLogFmt( ... )                     ((void)0)
#define              lLogSys( ... )                     ((void)0)
#define              lLogWarning( ... )                 ((void)0)
#define              lLogPrint( ... )                   ((void)0)

// Log formating

#define              lLogBreak()                        ((void)0)
#define              lLogIndent( value )                ((int)0)
#define              lLogUnindent( value )              ((int)0)
#define              lLogSetIndent( value )             ((void)0)
#define              lLogGetIndent()                    ((int)0)

#endif

#ifdef __cplusplus
}
#endif


#endif

