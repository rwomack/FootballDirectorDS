//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lzss.h
// Description : LZSS encode/decode routines
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef __LZSS_H
#define __LZSS_H

#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>

BEGIN_L_NAMESPACE

typedef void (*LZSSEncodeProgress)(luint32_t bytesProcessed, luint32_t bytesWritten);

void      LZSSDecode(void * outputBuffer, luint32_t outputSize, const void * inputBuffer, luint32_t inputSize );
luint32_t LZSSEncode( lArray<char> &output, const void *inputBuffer, int inputSize, LZSSEncodeProgress progress );

END_L_NAMESPACE
#endif

