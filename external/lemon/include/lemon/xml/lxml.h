//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lxml.h
// Description : XML system include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#include "./lxmlapi.h"

#ifndef LXML_H_INCLUDED
#define LXML_H_INCLUDED

#include "../platform/lcore.h"
#include "../containers/lstring.h"
#include "../containers/larray.h"
#include "../math/lmmatrix.h"

BEGIN_L_NAMESPACE

namespace TinyXML {
class TiXmlDocument;
class TiXmlElement;
}


// Pre-declared classes

class lIOStreamSeekable;
class lFileIOStream;

class lXMLNode;
class lXMLArchive;

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lXMLArchiveException class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LXML_API lXMLArchiveException : public lException
{
public:
  lXMLArchiveException( const char *str = NULL ) : m_str(str) {}
  ~lXMLArchiveException() {}

  static void Throw( const char * str, ... );

  // lException interface

  const char * GetErrorString(void);

private:
  const char * m_str;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lXMLArchive
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LXML_API lXMLArchive
{
public:

  // lXMLArchive implementation

  enum XMLArchiveMode { save, load };

  lXMLArchive( lIOStreamSeekable *stream, XMLArchiveMode mode );
  lXMLArchive( const char * fileName, XMLArchiveMode mode );
  virtual ~lXMLArchive();

  inline bool IsLoading(void) { return m_mode == load; }
  inline bool IsSaving(void)  { return m_mode == save; }

  enum NodeType
  {
    UniqueName,
    NonUniqueName,
  };

  lXMLNode Node( const char * name, enum NodeType type = UniqueName );
  lXMLNode FirstChild( const char *name = NULL );
  lXMLNode LastChild( const char *name = NULL );
  int      CountChildren( const char * name = NULL );
  void     Log();

private:
  lFileIOStream          * m_fileStream;
  lIOStreamSeekable      * m_stream;
  TinyXML::TiXmlDocument * m_doc;
  XMLArchiveMode           m_mode;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lXMLNode
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class LXML_API lXMLNode
{
  lXMLNode( lXMLArchive *ar, TinyXML::TiXmlElement * element );

public:

  lXMLNode( const lXMLNode &other );
  void operator=( const lXMLNode &other );

  inline bool IsLoading(void) { return m_ar->IsLoading(); }
  inline bool IsSaving(void)  { return m_ar->IsSaving(); }

  // lXMLNode interface

  enum NodeType
  {
    UniqueName,
    NonUniqueName,
  };

  const char * Name();
  bool         IsValid();

  // Node creation/lookup

  lXMLNode Node( const char * name, enum NodeType type = UniqueName );

  // Node iteration

  lXMLNode Next( const char *name = NULL );
  lXMLNode Prev( const char *name = NULL );
  lXMLNode FirstChild( const char *name = NULL );
  lXMLNode LastChild( const char *name = NULL );
  int      CountChildren( const char * name = NULL );

  // Attributes

  lXMLNode &Attribute( const char * name, bool &value, bool defaultValue = false );
  lXMLNode &Attribute( const char * name, int &value, int defaultValue = 0 );
  lXMLNode &Attribute( const char * name, unsigned int &value, unsigned int defaultValue = 0 );
  lXMLNode &Attribute( const char * name, float &value, float defaultValue = 0 );
  lXMLNode &Attribute( const char * name, double &value, double defaultValue = 0 );
  lXMLNode &Attribute( const char * name, char * value, int maxSize, const char * defaultValue = "" );
  lXMLNode &Attribute( const char * name, lString &str, const char * defaultValue = "" );

  lXMLNode &Attribute( const char * name, unsigned int &flag, unsigned int mask, bool defaultValue );

  lXMLNode &Check( const char * name, int );
  lXMLNode &Check( const char * name, float );
  lXMLNode &Check( const char * name, double );
  lXMLNode &Check( const char * name, const char * );

  // Body text

  lString GetText();
  void    SetText(const char *str);

  // Get an invalid node

  static lXMLNode InvalidNode( const lXMLNode& );
  static lXMLNode InvalidNode( lXMLArchive& );

private:
  friend class            lXMLArchive;
  lXMLArchive           * m_ar;
  TinyXML::TiXmlElement * m_element;
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Common XML archive operators
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Basic types

inline void operator << ( lXMLNode &ar, bool & c )    { ar.Attribute("value",c); }
inline void operator << ( lXMLNode &ar, int & c )     { ar.Attribute("value",c); }
inline void operator << ( lXMLNode &ar, float & c )   { ar.Attribute("value",c); }
inline void operator << ( lXMLNode &ar, double & c )  { ar.Attribute("value",c); }
inline void operator << ( lXMLNode &ar, lString & c ) { ar.Attribute("value",c); }

// Arrays

template<typename T>
inline void operator << ( lXMLNode& ar, lArray<T> &array )
{
  ar.Check("type","array");

  if( ar.IsLoading() )
  {
    int arraySize = ar.CountChildren("item");
    array.Add( arraySize );

    lXMLNode child = ar.FirstChild("item");
    int index = 0;
    while( child.IsValid() )
    {
      child << array.At(index);
      child = child.Next("item");
      index++;
    }
  }
  else
  {
    for( int i=0;i<array.Size();i++ )
    {
      ar.Node("item",lXMLNode::NonUniqueName) << array.At(i);
    }
  }
}

// Commons vectors, matrices and quaternions

inline void operator << ( lXMLNode &ar, lmVector3 &v )
{
  ar.Attribute( "x", v(0) );
  ar.Attribute( "y", v(1) );
  ar.Attribute( "z", v(2) );
}

inline void operator << ( lXMLNode &ar, lmVector2 &v )
{
  ar.Attribute( "x", v(0) );
  ar.Attribute( "y", v(1) );
}

inline void operator << ( lXMLNode &ar, lmMatrix3 &m )
{
  ar.Attribute( "m00", m(0,0) );
  ar.Attribute( "m01", m(0,1) );
  ar.Attribute( "m02", m(0,2) );

  ar.Attribute( "m10", m(1,0) );
  ar.Attribute( "m11", m(1,1) );
  ar.Attribute( "m12", m(1,2) );

  ar.Attribute( "m20", m(2,0) );
  ar.Attribute( "m21", m(2,1) );
  ar.Attribute( "m22", m(2,2) );
}

inline void operator << ( lXMLNode &ar, lmMatrix4 &m )
{
  ar.Attribute( "m00", m(0,0) );
  ar.Attribute( "m01", m(0,1) );
  ar.Attribute( "m02", m(0,2) );
  ar.Attribute( "m03", m(0,3) );

  ar.Attribute( "m10", m(1,0) );
  ar.Attribute( "m11", m(1,1) );
  ar.Attribute( "m12", m(1,2) );
  ar.Attribute( "m13", m(1,3) );

  ar.Attribute( "m20", m(2,0) );
  ar.Attribute( "m21", m(2,1) );
  ar.Attribute( "m22", m(2,2) );
  ar.Attribute( "m23", m(2,3) );

  ar.Attribute( "m20", m(3,0) );
  ar.Attribute( "m21", m(3,1) );
  ar.Attribute( "m22", m(3,2) );
  ar.Attribute( "m23", m(3,3) );
}

END_L_NAMESPACE

#endif
