//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lobject.h
// Description : Object Class RTTI/MetaData and object factory system
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 Notes
 -----

 This object system provides a factory, class RTTI, and class meta-data.

 lClass
 -------

 The lClass object provides information about a class that can be used
 to identifty the class and implement a factory for it. It also provides
 a meta-data mechanism for storing arbitrary data with a class.

 lObject
 --------

 Objects should derive from lObject should use the lRTTI<> template
 desribed below.

 Class() is a virtual fuction that returns the static lClass for the
 "instance" and can be used to get the derived lClass definition for an
 object, no matter what it is type cast to.

 StaticClass() is a static function that returns the static lClass for the
 "class". This can be used to get the lClass definition for the current type
 cast, or to reference a lClass definition by name (MyClass::StaticClass()).

 Usage
 -----

 Derive from lObject and use the lRTTI<this,super,classId,version)
 template to implement some standard functions for the system to work.
 Derive from lAbstractRTTI<> for an abstract class, so a class factory
 won't be generated.

 To register all class globally and call their ClassInitialiser() static
 methods, use lClassRegistrar::RegisterClasses() at initialisation time.
 This will also check for clashing CLSID's and write a message to the log.

 lObjectCast<> can be used to cast from one object to another. If the
 cast is invlid it returns NULL.

 You can use the command line lguid.exe to generate unique class-id's.
 The classID defaults to lCLSID<0,0,0,0> and the version defaults to zero.

   class Test : public lRTTI<Test,lObject>
   {
   };

 lClassMetaData
 ---------------

 The lClass object provides FindMetaData() and CreateMetaData() which are
 used to manage lClassMetaData objects. Only one instance of a meta-data object
 can exist in a lClass. FindMetaData() will find an existing meta data object
 and CreateMetaData() will find it first, then create it if it doesn't exist.

 lClassMetaData objects derive from lObject and as such can have metadata
 themselves and be identified by a lClass.

 To create a new class meta-data object derive from lClassMetaData and call
 the lClass CreateMetaData() method.

   class MyMeta : public lRTTI< MyMeta, lClassMetaData >
   {
   public:
   };

   lClass * pClass = MyObject::StaticClass();

   MyMeta * metaData = pClass->CreateMetaData( MyMeta::StaticClass() );

 Template versions of these mehods are available as well:

   MyMeta * pMeta = pClass->FindMetaData<MyMeta>();
   MyMeta * pMeta = pClass->CreateMetaData<MyMeta>();

*/

#ifndef LOBJECT_H_INCLUDED
#define LOBJECT_H_INCLUDED

#ifndef L_PLATFORM_SYMBIAN
#include <typeinfo>
#endif

#include "lobjectapi.h"
#include "../io/lioapi.h"
#include "../containers/larray.h"

BEGIN_L_NAMESPACE

// object array trait
typedef TArrayTrait<3,8,16,L_ARENA_OBJECT> TObjectArrayTrait;  // 3/8 + 16

class LOBJECT_API lLibrary;
class LOBJECT_API lObject;
class LOBJECT_API lClass;
class LOBJECT_API lClassMetaData;
class LOBJECT_API lClassRegistrar;

class LIO_API lStream;
class LIO_API lIOStream;
class LIO_API lIOStreamSeekable; //for lLibrary
class LIO_API lOldArchive;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// GUID
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

struct LOBJECT_API lGUID
{
  uint32         data1;
  uint16         data2;
  uint16         data3;
  uint8          data4[8];

  static lGUID Null;

  lGUID();
  lGUID(uint32 D1,uint16 D2, uint16 D3, uint64 D4);
  bool operator == ( const lGUID & other ) const;
  bool operator != ( const lGUID & other ) const;
  void Log( const char *postStr = "" );
};

lStream & operator << ( lStream& ar, lGUID &guid );

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// ClassID template
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<uint32 D1,uint16 D2, uint16 D3, uint64 D4> class lCLSID
{
public:
  static lGUID MakeGUID()
  {
    return lGUID(D1,D2,D3,D4);
  }

  operator lGUID()
  {
    return MakeGUID();
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lObject - Base object
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// GUID{0xF369CB7A,0x882E,0x4268,0x94BC8217F86F475B}

class LOBJECT_API lObject
{
private:
  lLibrary   * m_library;
  const char * m_name;
  static lArray<lObject *,TObjectArrayTrait> s_objectList;

protected:
  static void ClassInitialiser();
  LOBJECT_API friend lStream& operator << ( lStream& ar, lObject *& obj );

public:
  lObject();
  virtual ~lObject();

  virtual void           Serialize( lOldArchive & ar );

  static  lClass       * StaticClass(void);
  virtual lClass       * Class(void) = 0;
  virtual const lClass * Class(void) const = 0;

  bool                   IsA( const lClass * ) const;
  bool                   IsA( const lClass * );

  static lObject       * Create( lClass * );

  static lObject       * Load( lClass *, const char * fileName );
  static lObject       * Load( lClass *, lIOStream * );

  static void            Save( lObject *, const char * fileName );
  static void            Save( lObject *, lIOStream * );

  //

  static lObject       * Create( lClass *, lLibrary *, const char * pName );

  template<typename T> static T * Create( lLibrary *pLibrary, const char * pName )
  {
    return static_cast<T*>(Create( T::StaticClass(), pLibrary, pName ));
  }

  static void          * Allocate( lClass *, lLibrary *, const char * pName );
  static void            Free( void * pData );
  static lObject *       FindObject( lClass * pClass, lLibrary * pLibrary, const char * pName );
  const char *           GetName();
  lLibrary             * GetLibrary();
  virtual void           Serialize( lStream &stream );

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// RTTI template
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Base RTTI

template< class TTHIS, class TSUPER = lObject, typename TCLSID = lCLSID<0,0,0,0>,
          unsigned int TVERSION = 0 >
class lBaseRTTI : public TSUPER
{
public:

  typedef TTHIS  ThisClass;
  typedef TSUPER SuperClass;

//  lStream& operator << ( lStream& ar )
//  {
//    return ar << *reinterpret_cast<lObject**>(this);
//  }

  // Note:
  // We provide a non-parameter version of operator new/delete
  // rather then relying on default parameters so gcc 2.9
  // recognizes it and doesn't issue a 'no suitable operator delete' error.

  void* operator new( size_t size )
  {
    L_UNUSED_PARAM( size );
    return lObject::Allocate( TTHIS::StaticClass(), NULL, NULL );
  }

  void operator delete( void *ptr )
  {
    return lObject::Free( ptr );
  }

  void* operator new( size_t size, lLibrary * pLibrary, const char * pName = NULL )
  {
    L_UNUSED_PARAM( size );
    return lObject::Allocate( TTHIS::StaticClass(), pLibrary, pName );
  }

  void operator delete( void *ptr, lLibrary * pLibrary, const char * pName )
  {
    L_UNUSED_PARAM( pLibrary );
    L_UNUSED_PARAM( pName );
    return lObject::Free( ptr );
  }

private:
};

// Abstract class

template< class TTHIS, class TSUPER = lObject, typename TCLSID = lCLSID<0,0,0,0>,
          unsigned int TVERSION = 0 >
class lAbstractRTTI : public lBaseRTTI<TTHIS,TSUPER,TCLSID,TVERSION>
{
public:

  static lClass * StaticClass(void)
  {
    return &m_class;
  }

  virtual lClass * Class(void)
  {
    return StaticClass();
  }

  virtual const lClass * Class(void) const
  {
    return StaticClass();
  }

private:

  static lClass m_class;
};

template<class TTHIS, class TSUPER, typename TCLSID, unsigned int TVERSION>
lClass lAbstractRTTI<TTHIS,TSUPER,TCLSID,TVERSION>::m_class( typeid(TTHIS).name(),
    sizeof(TTHIS), NULL, TTHIS::ClassInitialiser,
    TVERSION, TCLSID::MakeGUID(), TSUPER::StaticClass() );

// Concrete class

template< class TTHIS, class TSUPER = lObject, typename TCLSID = lCLSID<0,0,0,0>,
          unsigned int TVERSION = 0 >
class lRTTI : public lBaseRTTI<TTHIS,TSUPER,TCLSID,TVERSION>
{
public:

  static lClass * StaticClass(void)
  {
    return &m_class;
  }

  virtual lClass * Class(void)
  {
    return StaticClass();
  }

  virtual const lClass * Class(void) const
  {
    return StaticClass();
  }

private:

  static void classFactory(void *p)
  {
    lPlacementConstructor<TTHIS>::Construct(p);
  }

  static lClass m_class;
};

template<class TTHIS, class TSUPER, typename TCLSID, unsigned int TVERSION>
lClass lRTTI<TTHIS,TSUPER,TCLSID,TVERSION>::m_class( typeid(TTHIS).name(),
    sizeof(TTHIS), classFactory, TTHIS::ClassInitialiser,
    TVERSION, TCLSID::MakeGUID(), TSUPER::StaticClass() );


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lClass - Class type object
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// GUID{0x56DADCBF,0x7661,0x4FFE,0x9D531DD8D1581E13}

class LOBJECT_API lClass : public lObject
{
protected:
  static void ClassInitialiser();
  LOBJECT_API friend lStream& operator << ( lStream& ar, lClass *& obj );

public:

  lClass( const char *className, unsigned int classSize,
           void (*classConstructor)(void *), void (*classInitialiser)(), unsigned int classVersion,
           const lGUID &guid, lClass * superClass );

  ~lClass();

  static lClass        * StaticClass(void);
  virtual lClass       * Class(void);
  virtual const lClass * Class(void) const;

  void               ClassConstructor(void *);
  unsigned int       ClassSize() const;
  const char *       ClassName(void) const;
  unsigned int       ClassVersion(void) const;
  const lGUID      & ClassId(void) const;
  lClass           * SuperClass(void) const;

  bool               IsDerivedFrom( const lClass * ) const;

  lClassMetaData   * FirstMetaData() const;
  lClassMetaData   * FindMetaData(lClass *) const;
  lClassMetaData   * CreateMetaData(lClass *);

  template<typename T> T * FindMetaData()
  {
    lClassMetaData * metaData = FindMetaData(T::StaticClass());
    return static_cast<T*>(metaData);
  }

  template<typename T> T * CreateMetaData()
  {
    lClassMetaData * metaData = CreateMetaData(T::StaticClass());
    return static_cast<T*>(metaData);
  }

  static lClass    * FindClass( const char * name );
  static lClass    * FindClass( const lGUID & guid );

  static lClass    * FirstClass();
  lClass           * NextClass() const;

private:

  lClass();
  friend class lPlacementConstructor<lClass>;

  lGUID              m_guid;
  const char       * m_className;
  unsigned int       m_classSize;
  void              (*m_classConstructor)(void *);
  void              (*m_classInitialiser)();
  unsigned int       m_classVersion;
  lClass           * m_superClass;
  lClassMetaData   * m_metaData;
  lClass           * m_nextClass;
  int                m_classObjectNameId;

  friend class lObject;
  friend class lClassRegistrar;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lClassMetaData
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LOBJECT_API lClassMetaData : public lRTTI<lClassMetaData, lObject, lCLSID<0x586EB10C,0x7E67,0x49C8,0xBA5C1AEB4A2AD7E8ULL> >
{
public:

  lClassMetaData();
  ~lClassMetaData();

  lClassMetaData * NextMetaData() const;

private:
  friend class lClass;
  lClassMetaData * m_next;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lClassRegistrar - lClass registrar class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LOBJECT_API lClassRegistrar
{
public:
  static void RegisterClasses();
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lLibrary
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LOBJECT_API lLibrary : public lRTTI< lLibrary, lObject, lCLSID<0xB66232F6,0xABFE,0x4868,0xAE4E9E73B2442C92ULL> >
{
protected:
  friend class lPlacementConstructor<lLibrary>;
  lLibrary();
  friend class lObject;
public:

  virtual ~lLibrary();

  int        GetNumObjects();
  lObject *  GetObject( int );

  // static interface

  static int              GetNumLibraries();
  static lLibrary       * GetLibrary(int);
  static lLibrary       * CreateLibrary( const char * name );
  static lLibrary       * FindLibrary( const char * name );
  static lLibrary       * GetTransientLibrary();
  static void             ReleaseTransientLibrary();

  static lLibrary       * Load( const char * fileName, const char * libraryName );
  static lLibrary       * Load( lIOStreamSeekable *, const char * libraryName );
  void                    Save( const char * fileName );
  void                    Save( lIOStreamSeekable * );

private:
  static lArray<lLibrary *,TObjectArrayTrait> s_libraryList;
  static lLibrary         * s_transientLibrary;
  lArray<lObject *,TObjectArrayTrait>         m_objectList;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lObjectCast
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
//
// Cast an object to another object.
// If the cast is valid, a pointer to the new type is returned.
// If the cast in invalid, NULL is returned.
//
// This will handle casting from multiply inherited objects if the type of
// U is already cast along the inheritance branch from lObject.

template<typename T,typename U> T* lObjectCast( U *obj )
{
  if( obj != NULL && obj->IsA(T::StaticClass()) ) return static_cast<T*>(obj);
  else return NULL;
}

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lClassId
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<class T> lGUID lClassId()
{
  return T::StaticClass()->ClassId();
}

template<class T> lGUID lClassId(T * pClass)
{
  if( pClass == NULL ) return lGUID::Null;
  return pClass->Class()->ClassId();
}

END_L_NAMESPACE



#endif
