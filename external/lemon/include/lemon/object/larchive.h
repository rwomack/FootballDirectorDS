//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : larchive.h
// Description : lArchive class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LARCHIVE_H_INCLUDED
#define LARCHIVE_H_INCLUDED

#include "lobjectapi.h"
#include "../io/liostream.h"
#include "../io/lstream.h"
#include "../io/liff.h"

BEGIN_L_NAMESPACE

// IFF Id's

#define L_IFF_LARC L_IFFID('L','A','R','C')
#define L_IFF_LOBJ L_IFFID('L','O','B','J')
#define L_IFF_LPRP L_IFFID('L','P','R','P')
#define L_IFF_LCOL L_IFFID('L','C','O','L')

class LOBJECT_API lArchive;
class LOBJECT_API lArchiveStream;

template<typename T> class lArchiveComplexCollection;
template<typename T,unsigned int ID> class lArchiveSimpleCollection;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Type Traits
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T> struct lTypeTraitBase
{
};

template <class T> struct lTypeTrait : public lTypeTraitBase<T>
{
  typedef lArchiveComplexCollection<T> CollectionType;
  static void Archive( lArchive &ar, const char * name, T &value );
};

template<typename T, unsigned int ID> struct lTypeTraitSimple : public lTypeTraitBase<T>
{
  typedef lArchiveSimpleCollection<T,ID> CollectionType;
  static void Archive( lArchive &ar, const char * name, T &value );
};

template<> struct lTypeTrait<bool> : public lTypeTraitSimple<bool, L_IFFID('B','O','O','L')> { };

template<> struct lTypeTrait<unsigned int> : public lTypeTraitSimple<unsigned int, L_IFFID('U','I','3','2')> { };
template<> struct lTypeTrait<int> : public lTypeTraitSimple<int, L_IFFID('S','I','3','2')> { };

template<> struct lTypeTrait<signed char> : public lTypeTraitSimple<unsigned int, L_IFFID('U','I','8',' ')> { };
template<> struct lTypeTrait<unsigned char> : public lTypeTraitSimple<int, L_IFFID('S','I','8',' ')> { };

template<> struct lTypeTrait<signed short> : public lTypeTraitSimple<unsigned int, L_IFFID('U','I','1','6')> { };
template<> struct lTypeTrait<unsigned short> : public lTypeTraitSimple<int, L_IFFID('S','I','1','6')> { };

template<> struct lTypeTrait<int64> : public lTypeTraitSimple<unsigned int, L_IFFID('U','I','6','4')> { };
template<> struct lTypeTrait<uint64> : public lTypeTraitSimple<int, L_IFFID('S','I','6','4')> { };

template<> struct lTypeTrait<float> : public lTypeTraitSimple<float, L_IFFID('F','L','3','2')> { };
template<> struct lTypeTrait<double> : public lTypeTraitSimple<double, L_IFFID('F','L','6','4')> { };

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lArchiveStream
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// TODO: need to ref-count this class, do same as lIFFData.
//       stream implementations just implement the appropriate members.
//       should then remove m_bClose


class LOBJECT_API lArchiveStream : public lStream
{
  bool       m_bClose;
  lArchive * m_parent;

public:

  lArchiveStream();
  lArchiveStream(lArchive *, bool bClose = true);
  ~lArchiveStream();

  // lStream interface

  virtual void      Serialize( void *data, unsigned int size );
  virtual lObject * SerializeObject( lObject * );
  virtual bool      IsLoading(void);
  virtual bool      IsSaving(void);
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lArchive
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LOBJECT_API lArchive
{
public:

  enum lArchiveMode
  {
    save,
    load
  };

  lArchive( lIOStreamSeekable *iostream, lArchiveMode mode );
  ~lArchive();

  bool IsLoading(void);
  bool IsSaving(void);

  void Close();

  lIFFContext * Top();

  bool FindChunk( lIFFId id, const char * name );
  bool FindComplexChunk( lIFFContext * context, lIFFId id, const char * name );
  lArchiveStream Stream(const char *name);

  template<typename T> void ComplexAttr( const char * name, lIFFId id, T & value )
  {
    if( IsSaving() )
    {
      lIFFContext * oldTop = Top();
      lIFFContext context;

      int err = lIFFWrite::BeginGroup( oldTop, &context, L_IFF_FORM, -1, id );
      if( err != lIFFERR_OK ) throw lIFFException(err);

      // Write name chunk

      if( name != NULL )
      {
        err = lIFFWrite::OpenChunk( &context, L_IFFID('N','A','M','E'), -1 );
        if( err != lIFFERR_OK ) throw lIFFException(err);

        int size = strlen(name);

        err = lIFFWrite::WriteChunkBytes(&context,&size,sizeof(size));
        if( err != lIFFERR_OK ) throw lIFFException(err);

        err = lIFFWrite::WriteChunkBytes(&context,name,size);
        if( err != lIFFERR_OK ) throw lIFFException(err);

        err = lIFFWrite::CloseChunk(&context,L_IFFID('N','A','M','E'));
        if( err != lIFFERR_OK ) throw lIFFException(err);
      }

      // Archive the object

      m_top = &context;

      (*this) << value;

      m_top = oldTop;

      err = lIFFWrite::EndGroup( oldTop, &context );
      if( err != lIFFERR_OK ) throw lIFFException(err);
    }
    else
    {
      lIFFContext context;

      if( FindComplexChunk( &context, id, name ) )
      {
        lIFFContext * oldTop = Top();
        m_top = &context;

        (*this) << value;

        m_top = oldTop;

        int err = lIFFRead::CloseGroup( oldTop, &context );
        if( err != lIFFERR_OK ) throw lIFFException(err);
      }
    }
  }

  template<typename T> void SimpleAttr( const char * name, lIFFId id, T & value )
  {
    if( IsSaving() )
    {
      int err = lIFFWrite::OpenChunk( Top(), id, -1 );
      if( err != lIFFERR_OK ) throw lIFFException(err);

      int size = strlen(name);

      err = lIFFWrite::WriteChunkBytes(Top(),&size,sizeof(size));
      if( err != lIFFERR_OK ) throw lIFFException(err);

      err = lIFFWrite::WriteChunkBytes(Top(),name,size);
      if( err != lIFFERR_OK ) throw lIFFException(err);

      lArchiveStream(this) << value;
    }
    else
    {
      if( FindChunk( id, name ) )
      {
        lArchiveStream(this) << value;
      }
    }
  }

  template<typename T> lArchive &Attr( const char * name, T & value )
  {
    lTypeTrait<T>::Archive( *this, name, value );
    return *this;
  }

private:
  lArchiveMode m_mode;
  lIFFContext  * m_top;
  lIFFContext  m_arc;
  lIFFContext  m_streamContext;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Type Traits function
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template <class T>
  void lTypeTrait<T>::Archive( lArchive &ar, const char * name, T &value )
  {
    ar.ComplexAttr(name,L_IFF_LPRP,value);
  }

template<typename T, unsigned int ID>
  void lTypeTraitSimple<T,ID>::Archive( lArchive &ar, const char * name, T &value )
  {
    ar.SimpleAttr(name,ID,value);
  }

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lArchiveSimpleCollection
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T,unsigned int ID>
class lArchiveSimpleCollection
{
  lArchive *  m_parent;
  unsigned int m_size;

public:
  lArchiveSimpleCollection( lArchive * parent ) : m_parent(parent)
  {
    // TODO: make sure we're inside an LCOL group.

    if( m_parent->IsLoading() )
    {
      int err = lIFFRead::NextChunk(m_parent->Top());
      if( err != lIFFERR_OK ) throw lIFFException(err);

      if( m_parent->Top()->chunkHdr.chunkId == L_IFFID('S','I','Z','E') )
      {
        lIFFRead::ReadChunkBytes( m_parent->Top(), &m_size, sizeof(m_size ) );
      }
      else
      {
        m_size = 0;
      }

      err = lIFFRead::NextChunk(m_parent->Top());
      if( err != lIFFERR_OK ) throw lIFFException(err);

      // TODO: chunk id is 'ID'

    }
    else
    {
      // Write place holder size

      int err = lIFFWrite::OpenChunk( m_parent->Top(), L_IFFID('S','I','Z','E'), 4 );
      if( err != lIFFERR_OK ) throw lIFFException(err);

      unsigned int size = 0;
      m_size = m_parent->Top()->stream->Tell();
      lIFFWrite::WriteChunkBytes( m_parent->Top(), &size, sizeof(size) );

      err = lIFFWrite::CloseChunk( m_parent->Top(), L_IFFID('S','I','Z','E') );
      if( err != lIFFERR_OK ) throw lIFFException(err);

      // Open the element chunk

      err = lIFFWrite::OpenChunk( m_parent->Top(), ID, -1 );
      if( err != lIFFERR_OK ) throw lIFFException(err);

    }
  }

  ~lArchiveSimpleCollection()
  {
    if( m_parent->IsSaving() )
    {
      int err = lIFFWrite::CloseChunk(m_parent->Top(),ID);
      if( err != lIFFERR_OK ) throw lIFFException(err);
    }
  }

  bool IsLoading() { return m_parent->IsLoading(); }
  bool IsSaving() { return m_parent->IsSaving(); }

  void NumElements( unsigned int & size )
  {
    if( IsSaving() )
    {
      // run back and fill in the size

      unsigned int pos = m_parent->Top()->stream->Tell();
      m_parent->Top()->stream->Seek( m_size, L_IO_SEEK_SET );
      m_parent->Top()->stream->Write( &size, sizeof(size) );
      m_parent->Top()->stream->Seek( pos, L_IO_SEEK_SET );
    }
    else
    {
      size = m_size;
    }
  }

  void NextElement( T&value )
  {
    lArchiveStream(m_parent,false) << value;
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lArchiveComplexCollection
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T>
class lArchiveComplexCollection
{
  lArchive * m_parent;
  unsigned int m_size;
public:
  lArchiveComplexCollection( lArchive * parent ) : m_parent(parent)
  {
    // TODO: make sure we're inside an LCOL group.

    if( m_parent->IsLoading() )
    {
      int err = lIFFRead::NextChunk(m_parent->Top());
      if( err != lIFFERR_OK ) throw lIFFException(err);

      if( m_parent->Top()->chunkHdr.chunkId == L_IFFID('S','I','Z','E') )
      {
        lIFFRead::ReadChunkBytes( m_parent->Top(), &m_size, sizeof(m_size ) );
      }
      else
      {
        m_size = 0;
      }
    }
    else
    {
      // Write place holder size

      int err = lIFFWrite::OpenChunk( m_parent->Top(), L_IFFID('S','I','Z','E'), 4 );
      if( err != lIFFERR_OK ) throw lIFFException(err);

      unsigned int size = 0;
      m_size = m_parent->Top()->stream->Tell();
      lIFFWrite::WriteChunkBytes( m_parent->Top(), &size, sizeof(size) );

      err = lIFFWrite::CloseChunk( m_parent->Top(), L_IFFID('S','I','Z','E') );
      if( err != lIFFERR_OK ) throw lIFFException(err);
    }
  }

  void NumElements( unsigned int & size )
  {
    if( IsSaving() )
    {
      // run back and fill in the size

      unsigned int pos = m_parent->Top()->stream->Tell();
      m_parent->Top()->stream->Seek( m_size, L_IO_SEEK_SET );
      m_parent->Top()->stream->Write( &size, sizeof(size) );
      m_parent->Top()->stream->Seek( pos, L_IO_SEEK_SET );
    }
    else
    {
      size = m_size;
    }
  }

  bool IsLoading() { return m_parent->IsLoading(); }
  bool IsSaving() { return m_parent->IsSaving(); }

  void NextElement( T&value )
  {
    m_parent->Attr( NULL, value );
  }
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lArchiveCollection
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

template<typename T>
class lArchiveCollection : public lTypeTrait<T>::CollectionType
{
public:
  lArchiveCollection( lArchive * parent ) : lTypeTrait<T>::CollectionType(parent) {}
  ~lArchiveCollection() {}
};


END_L_NAMESPACE
#endif
