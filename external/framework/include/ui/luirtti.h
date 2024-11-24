//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lurtti.h
// Description : lUIRTTI interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

/*
 Notes
 -----

 These macros are directly compatible with the lObject RTTI system, except
 using preprocessor macros instead of template inheritance.
 The only marked difference is that the class name is obtained by string-izing
 the macro parameter in L_IMPLEMENT_RTTI rather than using typeid() to keep
 class naming consistent across all platforms.

 Usage
 -----

 class MyClass : MySuperClass
 {
   L_RTTI( MyClass, MySuperClass );
 };

 // in implementation file.
 L_IMPLEMENT_RTTI( MyClass, lGUID(a,b,c,d), 0 );

 // if the class contains abstract virtual functions
 L_IMPLEMENT_ABSTRACT_RTTI( MyClass, lGUID(a,b,c,d), 0 );

*/


#ifndef LUIRTTI_H_INCLUDED
#define LUIRTTI_H_INCLUDED

#include <lemon/lapi.h>
#include <lemon/object/lobject.h>

BEGIN_L_NAMESPACE

// Pre-declared classes

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// LUI_RTTI
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Note:
// We provide a non-parameter version of operator new/delete
// rather then relying on default parameters so gcc 2.9
// recognizes it and doesn't issue a 'no suitable operator delete' error.

#define LUI_RTTI( TTHIS, TSUPER )                                                    \
public:                                                                              \
  typedef TTHIS  ThisClass;                                                          \
  typedef TSUPER SuperClass;                                                         \
                                                                                     \
  void* operator new( size_t size )                                                  \
  {                                                                                  \
    L_UNUSED_PARAM( size );                                                          \
    return lObject::Allocate( TTHIS::StaticClass(), NULL, NULL );                    \
  }                                                                                  \
                                                                                     \
  void operator delete( void *ptr )                                                  \
  {                                                                                  \
    return lObject::Free( ptr );                                                     \
  }                                                                                  \
                                                                                     \
  void* operator new( size_t size, lLibrary * pLibrary, const char * pName = NULL )  \
  {                                                                                  \
    L_UNUSED_PARAM( size );                                                          \
    return lObject::Allocate( TTHIS::StaticClass(), pLibrary, pName );               \
  }                                                                                  \
                                                                                     \
  void operator delete( void *ptr, lLibrary * pLibrary, const char * pName )         \
  {                                                                                  \
    L_UNUSED_PARAM( pLibrary );                                                      \
    L_UNUSED_PARAM( pName );                                                         \
    return lObject::Free( ptr );                                                     \
  }                                                                                  \
                                                                                     \
  static lClass * StaticClass(void)                                                  \
  {                                                                                  \
    return &m_class;                                                                 \
  }                                                                                  \
                                                                                     \
  virtual lClass * Class(void)                                                       \
  {                                                                                  \
    return StaticClass();                                                            \
  }                                                                                  \
                                                                                     \
  virtual const lClass * Class(void) const                                           \
  {                                                                                  \
    return StaticClass();                                                            \
  }                                                                                  \
private:                                                                             \
  static void classFactory(void *p);                                                 \
  static lClass m_class;


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// LUI_IMPLEMENT_RTTI
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define LUI_IMPLEMENT_RTTI( TTHIS, TGUID, TVERSION )                           \
void TTHIS::classFactory(void *p)                                              \
{                                                                              \
  lPlacementConstructor<TTHIS>::Construct(p);                                  \
}                                                                              \
lClass TTHIS::m_class( #TTHIS,                                                 \
                       sizeof(TTHIS), classFactory, TTHIS::ClassInitialiser,   \
                       TVERSION, TGUID, SuperClass::StaticClass() );


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// LUI_IMPLEMENT_ABSTRACT_RTTI
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#define LUI_IMPLEMENT_ABSTRACT_RTTI( TTHIS, TGUID, TVERSION )                  \
void TTHIS::classFactory(void *p) { }                                          \
lClass TTHIS::m_class( #TTHIS,                                                 \
                       sizeof(TTHIS), NULL, TTHIS::ClassInitialiser,           \
                       TVERSION, TGUID, SuperClass::StaticClass() );



END_L_NAMESPACE
#endif
