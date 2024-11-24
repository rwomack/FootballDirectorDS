//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lresource.h
// Description : Resource classes
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

/*
 Notes
 -----

 Dangers
 -------

 We need some sort of resource type checking!
 Either by name or by ID.
 Will need to go into FixPtr probably, or FixResource

 Notes

 1. Pointers should not be manipulated either before or after they are fixed.
 ----------------------------------------------------------------------------

 For example:

 struct Test
 {
   int * list;
   int   num;

   friend lResourceFixer & operator << (lResourceFixer &fixer, Test &obj )
   {
     for( int i=0;i<obj.num;i++ )
     {
       // ERROR: this will cause GPF on loading, as the pointer is currently an offset
       printf("int %d = %d\n", i, obj.list[i] );
     }

     fixer.FixPtr( obj.list, obj.num );

     for( int i=0;i<obj.num;i++ )
     {
       // ERROR: this will cause GPF on saving, as the pointer is now an offset
       printf("int %d = %d\n", i, obj.list[i] );
     }

     return fixer;
   }
 };

 2. Data members *can* be manipulated during resource fixing
 -----------------------------------------------------------

 struct Test
 {
   bool   fixed;
   int  * list;
   int    num;

   Model() : fixed(false) {}
     ~Model()
   {
     if( ! fixed ) delete list;
   }

   friend lResourceFixer & operator << (lResourceFixer &fixer, Test &obj )
   {
     obj.fixed = true;
     fixer.FixPtr( obj.list, obj.num );
     return fixer;
   }
 };

 The an object constructed with new() will have fixed == false, even after saving.
 An object loaded through a resource fixer will have fixed == true.
 Saving a resource will not affect the members of the original object.

*/

#include "../platform/lcore.h"

#ifndef LRESOURCE_H_INCLUDED
#define LRESOURCE_H_INCLUDED

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lResource - Base resource class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lResource derived classes should *not* have any virtual functions.
// The resource system works on the assumption that they don't.

class lResource
{
  unsigned int m_refCount;
  unsigned int m_flags;
  char       * m_name;
public:

  // note: these will be cleaned up
  //       the system structure will undergo some changes soon.
  unsigned int systemTableOffset;
  unsigned int systemTableSize;
  unsigned int contiguousTableOffset;
  unsigned int contiguousTableSize;
  unsigned int importTableOffset;
  unsigned int importTableSize;

  lResource() :
    m_refCount(0),
    m_flags(RESOURCE_ALLOCATED|RESOURCE_LOADED|RESOURCE_FIXED),
    m_name(0),
    systemTableOffset(0),
    systemTableSize(0),
    contiguousTableOffset(0),
    contiguousTableSize(0),
    importTableOffset(0),
    importTableSize(0)
  {
    // Note: if a resource is constructed, it is assumed to be allocated,
    // hence the RESOURCE_ALLOCATED flag.
    // It is also, by definition, loaded and fixed.
  }

  ~lResource()
  {
    // if we're being destructed, the ref count should be zero,
    // and the RESOURCE_ALLOCATED flag should be sest.

    lAssert( m_refCount == 0 );
    lAssert( (m_flags&RESOURCE_ALLOCATED) != 0 );

    // we can free our name as well

    if( m_name != static_cast<char*>(0) ) // realloc will return a valid pointer if m_name is NULL
    {
      m_name = reallocate<char>(__FILE__,__FUNCTION__,__LINE__,L_ARENA_RENDER,m_name,0);
    }
    lAssert( m_name == static_cast<char*>(0) );
  }

  enum
  {
    RESOURCE_ALLOCATED = 0x01,          // internal flag, should the resource delete it's own data?
    RESOURCE_LOADED    = 0x02,          // is the resource loaded?
    RESOURCE_FIXED     = 0x04,          // is the resource fixed?

  };

  inline void         AddReference()                               { m_refCount++;  }
  inline void         RemoveReference()
  {
    lAssert(m_refCount != 0);
    m_refCount--;
    lAssert(m_refCount != 0);

    // RcPtr is now expected to destroy the resource as it knows the 'T' type of the resource
    // and can call the correct destructor
    //if( (m_refCount == 0) && (m_flags&RESOURCE_ALLOCATED) )
    //{
    //  delete this;
    //  return;
    //}
  }

  inline int          GetSharedCount() const                       { return m_refCount; }
  inline void         ClearSharedCount()                           { m_refCount = 0; }

  inline unsigned int GetResourceFlags() const                     { return m_flags; }
  inline void         SetResourceFlags( unsigned int flags )       { m_flags = flags; }
  inline bool         GetResourceFlag( unsigned int flag ) const   { return ( m_flags & flag ) != 0; }
  inline void         SetResourceFlag( unsigned int flag )         { m_flags = m_flags | flag; }
  inline void         ClearResourceFlag( unsigned int flag )       { m_flags = m_flags & ~flag; }
  inline void         InvertResourceFlag( unsigned int flag )      { m_flags = m_flags ^ flag; }

  inline const char * GetResourceName() const                      { return m_name; }
  inline void         SetResourceName( const char * name )
  {
    if( m_flags&RESOURCE_ALLOCATED )
    {
      // okay to set name, allocate space and set it

      if( name != static_cast<char*>(0) )
      {
        int length = (int)strlen(name)+1;
        m_name = reallocate<char>(__FILE__,__FUNCTION__,__LINE__,L_ARENA_RENDER,m_name,length);
        memcpy( m_name, name, length );
      }
      else
      {
        m_name = reallocate<char>(__FILE__,__FUNCTION__,__LINE__,L_ARENA_RENDER,m_name,0);
        lAssert( m_name == static_cast<char*>(0) );
      }
    }
    else
    {
      // not okay to set name, as we shouldn't allocate space
      lAssertAlways( !( "lResource::SetResourceName: Cannot set name, resource is not allocated" ) );
    }
  }

  // this should only be called when manipulating the resource during save/load.
  // in which case, the allocated flag shouldn't be set.

  inline void SetResourceNameInternal( char * name )
  {
    lAssert( (m_flags&RESOURCE_ALLOCATED) == 0 );
    m_name = name;
  }

};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lResourceFixer
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lResourceFixer
{
public:

  enum DataType
  {
    ANY,      // any alignment
    VERTEX,   // vertex data alignment
    INDEX,    // index data alignment
    TEXTURE,  // texture data alignment
  };

  // Pointer to a FixPtrThunk
  typedef void (lResourceFixer::*TFixPtrFunc)( lResource *& voidData, unsigned int count, lResourceFixer::DataType type );

  // FixPtr
  //   Fix or Unfix a pointer-to-T object,
  //   Uses the specified count and data type
  //
  template<typename T> inline void FixPtr( T *& data, unsigned int count, DataType type=ANY )
  {
    // make sure a pointer-to-T is the default size (4 bytes)

    lAssert( sizeof(T*) == sizeof(void*) );

    // actually do the fix/unfix

    T * dataPtr = (T*)FixPtrInternal( (void *&)data, sizeof(T) * count, type );

    // recurse into sub objects, using the new copy

    for( int i=0;i<count;i++ )
    {
      *this << dataPtr[i];
    }
  }

  // FixData
  //   Fix or Unfix a pointer-to-T data,
  //   Uses the specified count and data type
  //
  template<typename T> inline void FixData( T *& data, unsigned int byteSize, DataType type=ANY )
  {
    // make sure a pointer-to-T is the default size (4 bytes)

    lAssert( sizeof(T*) == sizeof(void*) );

    // actually do the fix/unfix

    //T * dataPtr = (T*)FixPtrInternal( (void *&)data, byteSize, type );
    FixPtrInternal( (void *&)data, byteSize, type );
  }

  // FixString
  //   Fix or Unfix a string,
  //
  inline void FixString( char *& data )
  {
    // actually do the fix/unfix
    FixStringInternal( data );
  }

  // FixPtrThunk
  //   Thunk interface to fix or unfix a pointer-to-T, accepting any 'lResource *'
  //
  template<typename T> void FixPtrThunk( lResource *& voidData, unsigned int count, DataType type )
  {
    FixPtr( (T*&)voidData, count, type );
  }

  // FixResource
  //  Fix or Unfix a pointer to a resource, used for resolving references to other
  //  resources by RcPtr<>
  //
  template<typename T> void FixResource( T *& resource )
  {
    FixResourceInternal( (lResource *&)resource, &lResourceFixer::FixPtrThunk<T> );
  }

  virtual bool IsLoading() = 0;
  virtual bool IsSaving() = 0;

protected:

  virtual void * FixPtrInternal( void *& data, unsigned int dataSize, DataType type ) = 0;
  virtual void   FixStringInternal( char *& data ) = 0;
  virtual void   FixResourceInternal( lResource *& resource, TFixPtrFunc ) = 0;
};

// basic type implementation for fixing

inline lResourceFixer & operator << (lResourceFixer &fixer, unsigned char ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, char ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, bool ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, unsigned short ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, short ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, unsigned long ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, long ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, unsigned int ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, int ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, float ) { return fixer; }
inline lResourceFixer & operator << (lResourceFixer &fixer, double ) { return fixer; }

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lResourceResolver
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lResourceResolver
{
public:
  // GetFixedResource
  //  Returns a pointer to a fixed resource, or NULL if not found.
  //  If the resource is loaded but not fixed the function should use
  //  the TFixPtrFunc to fix the resource in memory.
  //
  virtual lResource * GetResourceImport( lResource * resource, unsigned int importIndex, lResourceFixer::TFixPtrFunc pFixFunc ) = 0;
};


END_L_NAMESPACE
#endif
