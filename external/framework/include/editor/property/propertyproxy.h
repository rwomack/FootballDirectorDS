//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : propertyproxy.h
// Description : Property proxy interfaces
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef PROPERTYPROXY_H_INCLUDED
#define PROPERTYPROXY_H_INCLUDED

#include <editor/leditor.h>

BEGIN_L_NAMESPACE
class lProperty;
END_L_NAMESPACE

namespace lEditor {

class PropertyProxy;
class PropertyEdit;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// PropertyProxyManager
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class PropertyProxyManager
{
public:

  static UITool        * PropertyCreateRollout( Editor *, lObject *, const lProperty * );
  static PropertyEdit  * PropertyCreateEdit( lObject *, const lProperty * );

  static void            PropertyProxyAdd( PropertyProxy *, lClass * );
  static void            PropertyProxyRemove( PropertyProxy * );

  static PropertyProxy * GetPropertyProxy( const lProperty * );

private:

  struct PropertyMap
  {
    PropertyProxy * proxy;
    lClass        * objectClass;
  };

  static lArray<PropertyMap> m_propertyMap;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// PropertyProxy
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class PropertyProxy
{
public:
  virtual ~PropertyProxy() {}

  virtual void              PropertyProxyRender( lGameLeaf *, const lProperty * )                                    { }
  virtual bool              PropertyGetProxyAABox( lGameLeaf *, const lProperty *, lmAABox3 & )                      { return false; }
  virtual bool              PropertyProxyTraceRay( lGameLeaf *, const lProperty *, const lmRay3 &ray, float &tnear ) { return false; }
  virtual MouseCreator    * PropertyBeginMouseCreate( lGameLeaf *, const lProperty * )                               { return NULL; }
  virtual void              PropertyEndMouseCreate( MouseCreator * )                                                 { }

  virtual UITool          * PropertyCreateRollout( Editor *, lObject *, const lProperty * )                          { return NULL; }
  virtual PropertyEdit    * PropertyCreateEdit( lObject *, const lProperty * )                                       { return NULL; }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// PropertyEdit
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class PropertyEdit
{
public:
  virtual HWND GetHWND() = 0;
  virtual void Init(HWND parent) = 0;
  virtual void Deinit() = 0;
  virtual void Refresh() = 0;

  lSignal0<> SignalEditChange;
};

}
#endif

