//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lfatalerror.h
// Description : Platform fatal error
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LFATALERROR_H_INCLUDED
#define LFATALERROR_H_INCLUDED

#include "lcore.h"

#ifdef __cplusplus
extern "C" {
#endif

//##
// Fatal error function
// Remarks: Called by the lFatalError macro.
// In:
//   function - name of function
//   file     - name of file
//   line     - line in file
//   text     - fatal error detail
//
LPLATFORM_API void lPlatform_FatalError( const char * function, const char * file, unsigned int line, const char * text );

// Assertions / Template assertions and errors

//##lFatalError
// Run-time fatal error
// In: _text - string to detail the fatal error
// Remarks: Use this macro to signal a run-time fatal error.
//
#define lFatalError(_text) (void)( (lPlatform_FatalError(__FUNCTION__, __FILE__, __LINE__, _text), 0) )

#ifdef __cplusplus
}
#endif

#endif

