//����������������������������������������������������������������������������
// File        : lcrc32.h
// Description : Mark Adler's Adler-32 algorithm
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LADLER32_H_INCLUDED
#define LADLER32_H_INCLUDED

#include "lhashapi.h"

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// Checksum functions
//����������������������������������������������������������������������������

LHASH_API unsigned int lAdler32(unsigned int adler, const void *data, unsigned int len);

END_L_NAMESPACE
#endif
