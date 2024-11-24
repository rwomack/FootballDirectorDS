//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lproperty.h
// Description : lProperty
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LPROPERTY_H_INCLUDED
#define LPROPERTY_H_INCLUDED

#include <lemon/lapi.h>
#include "./lpropertyset.h"
#include <lemon/object/lobject.h>
#include <lemon/xml/lxml.h>

BEGIN_L_NAMESPACE

class lProperty;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyObjectMap
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyObjectMap
{
public:
  virtual int       MapObjectToIndex( lObject * ) = 0;
  virtual lObject * MapIndexToObject( int ) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyObjectMapNull
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyObjectMapNull : public lPropertyObjectMap
{
public:
  virtual int       MapObjectToIndex( lObject * ) { return 0; }
  virtual lObject * MapIndexToObject( int ) { return 0; }
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyAccessorBase - Base class for all property accessors
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyAccessorBase
{
public:
  virtual ~lPropertyAccessorBase() {}
  // no-op
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyValueAccessor - Base value Get/Set interface for a property
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T> class lPropertyValueAccessor : public lPropertyAccessorBase
{
public:
  virtual T    Get(lObject *c) = 0;
  virtual void Set(lObject *c, T v) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyValueAccessorT - Template lPropertyValueAccessor for a class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename CLASS,typename T> class lPropertyValueAccessorT : public lPropertyValueAccessor<T>
{
public:

  lPropertyValueAccessorT( T CLASS::*ptr )
    : m_ptr(ptr),
      m_get(NULL),
      m_set(NULL)
  {
  }

  lPropertyValueAccessorT( T (CLASS::*inget)(), void (CLASS::*inset)(T) )
    :  m_ptr(NULL),
       m_get(inget),
       m_set(inset)
  {
  }

  virtual T Get(lObject *c)
  {
    lAssert(c->IsA(CLASS::StaticClass()));

    if( m_get )
    {
      return (((CLASS*)(c))->*m_get)();
    }
    else
    {
      return ((CLASS*)(c))->*m_ptr;
    }
  }

  virtual void Set(lObject *c, T v)
  {
    lAssert(c->IsA(CLASS::StaticClass()));

    if( m_set )
    {
      (((CLASS*)(c))->*m_set)(v);
    }
    else
    {
      ((CLASS*)(c))->*m_ptr = v;
    }
  }

private:
  T CLASS::*m_ptr;
  T (CLASS::*m_get)();
  void (CLASS::*m_set)(T);
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyRefAccessor - Base value GetRef interface for a property
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T> class lPropertyRefAccessor : public lPropertyAccessorBase
{
public:
  virtual T& GetRef(lObject *c) = 0;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyRefAccessorT - Template lPropertyRefAccessor for a class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename CLASS,typename T> class lPropertyRefAccessorT : public lPropertyRefAccessor<T>
{
public:

  lPropertyRefAccessorT( T CLASS::*ptr )
    : m_ptr(ptr),
      m_get(NULL)
  {
  }

  lPropertyRefAccessorT( T& (CLASS::*get)() )
    : m_ptr(NULL),
      m_get(get)
  {
  }

  virtual T & GetRef(lObject *c)
  {
    lAssert(c->IsA(CLASS::StaticClass()));
    if( m_get )
    {
      return (((CLASS*)(c))->*m_get)();
    }
    else
    {
      return ((CLASS*)(c))->*m_ptr;
    }
  }

private:
  T CLASS::*m_ptr;
  T& (CLASS::*m_get)();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyFlagAccessorT - bool as masked unsigned int accessor
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename CLASS> class lPropertyFlagAccessorT : public lPropertyValueAccessor<bool>
{
public:

  lPropertyFlagAccessorT( unsigned int mask, unsigned int CLASS::*ptr )
    : m_mask(mask),
      m_ptr(ptr),
      m_get(NULL),
      m_set(NULL)
  {
  }

  lPropertyFlagAccessorT( unsigned int mask, unsigned int (CLASS::*inget)(), void (CLASS::*inset)(unsigned int) )
    : m_mask(mask),
      m_ptr(NULL),
      m_get(inget),
      m_set(inset)
  {
  }

  virtual bool Get(lObject *c)
  {
    lAssert(c->IsA(CLASS::StaticClass()));

    if( m_get )
    {
      return (((((CLASS*)(c))->*m_get)()) & m_mask) == m_mask;
    }
    else
    {
      return ((((CLASS*)(c))->*m_ptr) & m_mask) == m_mask;
    }
  }

  virtual void Set(lObject *c, bool v)
  {
    lAssert(c->IsA(CLASS::StaticClass()));

    unsigned int oldValue = Get(c);
    unsigned int newValue = oldValue;

    if( v ) newValue |= m_mask;
    else newValue &= (~m_mask);

    if( oldValue != newValue )
    {
      if( m_set )
      {
        (((CLASS*)(c))->*m_set)(newValue);
      }
      else
      {
        ((CLASS*)(c))->*m_ptr = newValue;
      }
    }
  }

private:
  unsigned int m_mask;
  unsigned int CLASS::*m_ptr;
  unsigned int (CLASS::*m_get)();
  void (CLASS::*m_set)(unsigned int);
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lProperty - Base object property class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lProperty : public lAbstractRTTI< lProperty, lObject >
{
public:

  // lProperty interface

  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar ) = 0;

  virtual const char * GetName() const = 0;
  virtual int GetFlags() const = 0;

  bool GetFlag( int flag ) const { return ((GetFlags()&flag) == flag); }

  enum
  {
    EDIT       = 0x1,
    PERSISTENT = 0x2,
  };

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lValuePropertyRTTI - Value property RTTI template
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template< class TTHIS, typename T >
class lValuePropertyRTTI : public lRTTI<TTHIS,lProperty>
{
protected:
  lPropertyValueAccessor<T> * m_propAccess;
  lString                     m_name;
  unsigned int                m_propFlags;

  lValuePropertyRTTI() : m_propAccess(NULL)
  {
  }

public:

  virtual const char * GetName() const        { return m_name.CStr(); }
  virtual int          GetFlags() const       { return m_propFlags; }
  virtual T            Get(lObject *c)       { return m_propAccess->Get(c); }
  virtual void         Set(lObject *c, T v)  { m_propAccess->Set(c,v); }

  template<typename TCLASS> static TTHIS * Create( lClass * objClass, unsigned int propFlags, char *name, T TCLASS::*ptr )
  {
    TTHIS * prop = new TTHIS();
    prop->m_name       = name;
    prop->m_propFlags  = propFlags;
    prop->m_propAccess = new lPropertyValueAccessorT<TCLASS,T>(ptr);
    if( objClass ) lPropertySet::GetClassPropertySet( objClass ).AddProperty( prop );
    return prop;
  }

  template<typename TCLASS> static TTHIS * Create( lClass * objClass, unsigned int propFlags, char *name, T (TCLASS::*inget)(), void (TCLASS::*inset)(T) )
  {
    TTHIS * prop = new TTHIS();
    prop->m_name       = name;
    prop->m_propFlags  = propFlags;
    prop->m_propAccess = new lPropertyValueAccessorT<TCLASS,T>(inget,inset);
    if( objClass ) lPropertySet::GetClassPropertySet( objClass ).AddProperty( prop );
    return prop;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lRefPropertyRTTI - Reference property RTTI template
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template< class TTHIS, typename T >
class lRefPropertyRTTI : public lRTTI<TTHIS,lProperty>
{
protected:
  lPropertyRefAccessor<T> * m_propAccess;
  lString                   m_name;
  unsigned int              m_propFlags;

  lRefPropertyRTTI() : m_propAccess(NULL)
  {
  }

public:

  virtual const char * GetName() const    { return m_name.CStr(); }
  virtual int          GetFlags() const   { return m_propFlags; }

  virtual T   & GetRef(lObject *c)   { return m_propAccess->GetRef(c); }

  template<typename TCLASS> static TTHIS * Create( lClass * objClass, unsigned int propFlags, char *name, T TCLASS::*ptr )
  {
    TTHIS * prop = new TTHIS();
    prop->m_name       = name;
    prop->m_propFlags  = propFlags;
    prop->m_propAccess = new lPropertyRefAccessorT<TCLASS,T>(ptr);
    if( objClass ) lPropertySet::GetClassPropertySet( objClass ).AddProperty( prop );
    return prop;
  }

  template<typename TCLASS> static TTHIS * Create( lClass * objClass, unsigned int propFlags, char *name, T& (TCLASS::*inget)() )
  {
    TTHIS * prop = new TTHIS();
    prop->m_name       = name;
    prop->m_propFlags  = propFlags;
    prop->m_propAccess = new lPropertyRefAccessorT<TCLASS,T>(inget);
    if( objClass ) lPropertySet::GetClassPropertySet( objClass ).AddProperty( prop );
    return prop;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyObject
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyObject : public lValuePropertyRTTI< lPropertyObject, lObject * >
{
public:
  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyBool
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyBool : public lValuePropertyRTTI< lPropertyBool, bool >
{
public:
  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyInt
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyInt : public lValuePropertyRTTI< lPropertyInt, int >
{
public:
  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyFloat
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyFloat : public lValuePropertyRTTI< lPropertyFloat, float >
{
public:
  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar );
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyFlag
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyFlag : public lRTTI<lPropertyFlag,lProperty>
{
protected:
  lPropertyValueAccessor<bool> * m_propAccess;
  lString                        m_name;
  unsigned int                   m_propFlags;

public:
  lPropertyFlag() : m_propAccess(NULL)
  {
  }

  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar );

  virtual const char * GetName() const          { return m_name.CStr(); }
  virtual int          GetFlags() const         { return m_propFlags; }
  virtual bool         Get(lObject *c)         { return m_propAccess->Get(c); }
  virtual void         Set(lObject *c, bool v) { m_propAccess->Set(c,v); }

  template<typename TCLASS> static lPropertyFlag * Create( lClass * objClass, unsigned int propFlags, char *name, unsigned int TCLASS::*ptr, unsigned int mask )
  {
    lPropertyFlag * prop = new lPropertyFlag();
    prop->m_name       = name;
    prop->m_propFlags  = propFlags;
    prop->m_propAccess = new lPropertyFlagAccessorT<TCLASS>(mask,ptr);
    if( objClass ) lPropertySet::GetClassPropertySet( objClass ).AddProperty( prop );
    return prop;
  }

  template<typename TCLASS> static lPropertyFlag * Create( lClass * objClass, unsigned int propFlags, char *name, bool (TCLASS::*inget)(), void (TCLASS::*inset)(bool), unsigned int mask )
  {
    lPropertyFlag * prop = new lPropertyFlag();
    prop->m_name       = name;
    prop->m_propFlags  = propFlags;
    prop->m_propAccess = new lPropertyFlagAccessorT<TCLASS>(mask,inget,inset);
    if( objClass ) lPropertySet::GetClassPropertySet( objClass ).AddProperty( prop );
    return prop;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lPropertyEnum
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lPropertyEnum : public lValuePropertyRTTI< lPropertyEnum, int >
{
public:

  virtual void XMLSerialize( lObject *o, lPropertyObjectMap &mapper, lXMLNode &ar );

  // lPropertyEnum interface

  struct Entry
  {
    lString name;
    int     value;
  };

  lPropertyEnum  * Add( const char * name, int value );
  lArray<Entry>  & GetEnumList();
  int              Lookup( const char * name );
  const char *     Lookup( int value );

private:
  lArray<Entry> m_enumList;
};

END_L_NAMESPACE
#endif
