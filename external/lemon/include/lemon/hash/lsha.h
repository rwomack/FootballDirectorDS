//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lsha.h
// Description : Secure Hash Algorithm
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSHA_H_INCLUDED
#define LSHA_H_INCLUDED

#include "lhashapi.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Checksum functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define L_SHA1_DIGEST_SIZE 20
#define L_SHA224_DIGEST_SIZE 28
#define L_SHA256_DIGEST_SIZE 32
#define L_SHA384_DIGEST_SIZE 48
#define L_SHA512_DIGEST_SIZE 64

struct lSHA1State
{
  uint32 count[2];     // message length (in bits)
  uint32 h[5];         // running digest
  uint8  buffer[64];   // input buffer
};

struct lSHA256State
{
  uint32 count[2];     // message length (in bits)
  uint32 h[8];         // running digest
  uint8  buffer[64];   // input buffer
};

struct lSHA512State
{
  uint32 count[4];     // message length (in bits)
  uint64 h[8];         // running digest
  uint8  buffer[128];  // input buffer
};

typedef lSHA256State lSHA224State; // SHA-224 state is identical to SHA-256 state
typedef lSHA512State lSHA384State; // SHA-384 state is identical to SHA-512 state

LHASH_API void lSHA1Init(lSHA1State &state);
LHASH_API void lSHA1Data(lSHA1State &state, const void * data, unsigned int len);
LHASH_API void lSHA1Finish(lSHA1State &state, uint8 digest[L_SHA1_DIGEST_SIZE] );

LHASH_API void lSHA224Init(lSHA224State &state);
LHASH_API void lSHA224Data(lSHA224State &state, const void * data, unsigned int len);
LHASH_API void lSHA224Finish(lSHA224State &state, uint8 digest[L_SHA224_DIGEST_SIZE] );

LHASH_API void lSHA256Init(lSHA256State &state);
LHASH_API void lSHA256Data(lSHA256State &state, const void * data, unsigned int len);
LHASH_API void lSHA256Finish(lSHA256State &state, uint8 digest[L_SHA256_DIGEST_SIZE] );

LHASH_API void lSHA384Init(lSHA384State &state);
LHASH_API void lSHA384Data(lSHA384State &state, const void * data, unsigned int len);
LHASH_API void lSHA384Finish(lSHA384State &state, uint8 digest[L_SHA384_DIGEST_SIZE] );

LHASH_API void lSHA512Init(lSHA512State &state);
LHASH_API void lSHA512Data(lSHA512State &state, const void * data, unsigned int len);
LHASH_API void lSHA512Finish(lSHA512State &state, uint8 digest[L_SHA512_DIGEST_SIZE] );

LHASH_API void lSHA1(const void *data, unsigned int len, uint8 digest[L_SHA1_DIGEST_SIZE]);
LHASH_API void lSHA224(const void *data, unsigned int len, uint8 digest[L_SHA224_DIGEST_SIZE]);
LHASH_API void lSHA256(const void *data, unsigned int len, uint8 digest[L_SHA256_DIGEST_SIZE]);
LHASH_API void lSHA384(const void *data, unsigned int len, uint8 digest[L_SHA384_DIGEST_SIZE]);
LHASH_API void lSHA512(const void *data, unsigned int len, uint8 digest[L_SHA512_DIGEST_SIZE]);

END_L_NAMESPACE
#endif
