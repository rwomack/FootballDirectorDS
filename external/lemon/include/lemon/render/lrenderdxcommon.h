//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lrenderdxcommon.h
// Description : DirectX common rendering interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LRENDERDXCOMMON_H_INCLUDED
#define LRENDERDXCOMMON_H_INCLUDED

#include "./lrshaderresourcedx.h"

// Pre-declared platform specifics
//
// These should have been defined by previously
// including "lrshaderresourcedx.h"
//
//class lrCompiledVS;     // Platform vertex shader resource
//class lrCompiledPS;     // Platform pixel shader resource

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRenderDXCommon - DirectX common static render interface
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LRENDER_API lRenderDXCommon : public lRender
{
public:

  // default pixel shader select

  static void DefaultPixelShaderSelect();

  // Vertex + Pixel Shader Constant optimisations

  static int  GetVertexShaderConstants();
  static void SetVertexShaderConstant( int startRegister, const float *constantData, int vector4fcount );
  static void SetVertexShaderConstant( int startRegister, const bool  *constantData, int boolcount );
  static void SetVertexShaderConstant( int startRegister, const int   *constantData, int vector4icount );
  static void SetPixelShaderConstant( int startRegister, const float *constantData, int vector4fcount );
  static void UpdateShaderConstants();
  static int  GetPixelShaderConstants();
};

END_L_NAMESPACE

#endif
