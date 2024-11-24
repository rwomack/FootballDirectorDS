//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : ltea.h
// Description : Tiny Encrpytion Algorithm
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LTEA_H_INCLUDED
#define LTEA_H_INCLUDED

#include "lcryptapi.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Crypt functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

enum { L_TEA_KEYSIZE    = 4 };  // 128 bit key
enum { L_TEA_BLOCKSIZE  = 2 };  // 64 bit blocks

// Encrypt and decrypt 64-bit blocks with a 128-bit key using TEA

LCRYPT_API void lTEAEncrypt64( const uint32 key[L_TEA_KEYSIZE], const uint32 inData[L_TEA_BLOCKSIZE], uint32 outData[L_TEA_BLOCKSIZE] );
LCRYPT_API void lTEADecrypt64( const uint32 key[L_TEA_KEYSIZE], const uint32 inData[L_TEA_BLOCKSIZE], uint32 outData[L_TEA_BLOCKSIZE] );

END_L_NAMESPACE
#endif
