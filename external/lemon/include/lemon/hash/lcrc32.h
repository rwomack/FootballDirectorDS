//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcrc32.h
// Description : Mark Adler's CRC-32 algorithm
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCRC32_H_INCLUDED
#define LCRC32_H_INCLUDED

#include "lhashapi.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Checksum functions
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

LHASH_API unsigned int lCRC32(unsigned int crc, const void *data, unsigned int len);

END_L_NAMESPACE
#endif
