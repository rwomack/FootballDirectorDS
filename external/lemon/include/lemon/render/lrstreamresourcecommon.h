//����������������������������������������������������������������������������
// File        : lrstreamresourcecommon.h
// Description : Common stream resource include
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lrenderapi.h"

#ifndef LRSTREAMRESOURCECOMMON_H_INCLUDED
#define LRSTREAMRESOURCECOMMON_H_INCLUDED

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// lrStreamResourceXXX
//����������������������������������������������������������������������������
//
// All platform specific stream resources should implement the following interface.
// This is not defined a common base class, as resources are accessible across
// platform. (ie, you can access PS2 and Xbox streams resources on PC).
//
//
//class LRENDER_API lrStreamResourceXXX : public lResource
//{
//};


END_L_NAMESPACE

#endif
