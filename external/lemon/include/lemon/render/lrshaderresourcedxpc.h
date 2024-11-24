//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrshaderresourcedxpc.h
// Description : Win32 PC DirectX shader resource
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LRSHADERRESOURCEDXPC_H_INCLUDED
#define LRSHADERRESOURCEDXPC_H_INCLUDED

#include "./lrshaderresourcedxcommon.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrCompiledVSPC - Compiled Vertex Shader for PC
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrCompiledVSPC : public lResource
{
public:
  lrCompiledVSPC() : vertexShaderId(0) {}

  unsigned int   vertexShaderId; // quick hack to make this structure different from lrCompiledPSPC
  lArray<uint32> tokenList;

  //friend lResourceFixer & operator << (lResourceFixer &fixer, lrCompiledVSPC &vs )
  //{
  //  fixer << vs.tokenList;
  //  return fixer;
  //}
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lrCompiledPSPC - Compiled Pixel Shader for PC
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lrCompiledPSPC : public lResource
{
public:
  lArray<uint32> tokenList;

  //friend lResourceFixer & operator << (lResourceFixer &fixer, lrCompiledPSPC &ps )
  //{
  //  fixer << ps.tokenList;
  //  return fixer;
  //}
};

END_L_NAMESPACE

#endif
