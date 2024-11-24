//����������������������������������������������������������������������������
// File        : fdtitle.h
// Description : FDTitle interface
// Notes       :
//
//����������������������������������������������������������������������������
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDTITLE_H_INCLUDED
#define FDTITLE_H_INCLUDED

BEGIN_L_NAMESPACE

//����������������������������������������������������������������������������
// FDTitle
//����������������������������������������������������������������������������

class FDTitle
{
public:
  static void Init();
  static void Deinit();
  static void Tick(float);
  static bool IsOpen();
  static void ForceClose();
  static void Render();
};

END_L_NAMESPACE

#endif


