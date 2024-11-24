//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lmd5.h
// Description : Message Digest 5 algorithm
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LMD5_H_INCLUDED
#define LMD5_H_INCLUDED

#include "lhashapi.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Checksum functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define L_MD5_DIGEST_SIZE 16

struct lMD5State
{
  uint32 count[2];     // message length (in bits)
  uint32 abcd[4];      // running digest
  uint8  buffer[64];   // input buffer
};

LHASH_API void lMD5Init(lMD5State &state);
LHASH_API void lMD5Data(lMD5State &state, const void * data, unsigned int len);
LHASH_API void lMD5Finish(lMD5State &state, uint8 digest[L_MD5_DIGEST_SIZE] );

LHASH_API void lMD5(const void *data, unsigned int len, uint8 digest[L_MD5_DIGEST_SIZE]);


END_L_NAMESPACE
#endif
