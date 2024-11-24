//����������������������������������������������������������������������������
// File        : lrenderdxcommon.h
// Description : DirectX common rendering interface
// Notes       :
//
//����������������������������������������������������������������������������
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

//����������������������������������������������������������������������������
// lRenderDXCommon - DirectX common static render interface
//����������������������������������������������������������������������������

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
