//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lstring.h
// Description : String class
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LSTRING_H_INCLUDED
#define LSTRING_H_INCLUDED

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include "../platform/lcore.h"
#include "../io/lstream.h"

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lString - String class
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

class lString
{
  static const int LSTRING_INTLEN   = 32;
  static const int LSTRING_FLOATLEN = 64;

public:
  inline lString() : m_bFixed(false), m_string( static_cast<char *>(0) )
  {
    create( 0 );
    m_string[0] = '\0';
  }

  inline lString(const lString &s) : m_bFixed(false), m_string( static_cast<char *>(0) )
  {
    if (&s != this)
    {
      create( s.m_size );
      strcpy( m_string, s.m_string );
    }
  }

  inline lString(const char *str) : m_bFixed(false), m_string( static_cast<char *>(0) )
  {
    if ( str == NULL )
    {
      create( 0 );
      m_string[0] = '\0';
    }
    else
    {
      int sz = (int)strlen( str );
      create( sz );
      strcpy( m_string, str );
    }
  }

  inline lString(const char *str, int len) : m_bFixed(false), m_string( static_cast<char *>(0) )
  {
    if ( str == NULL )
    {
      create( 0 );
      m_string[0] = '\0';
    }
    else
    {
      create( len );
      memcpy( m_string, str, len );
      m_string[len] = '\0';
    }
  }

  inline lString(char c) : m_bFixed(false), m_string( static_cast<char *>(0) )
  {
    create( 1 );
    m_string[0] = c;
    m_string[1] = '\0';
  }

  inline ~lString()
  {
    if( !m_bFixed )
    {
      destroy();
    }
  }

  inline void AllocateBlankString( int size )
  {
    destroy();
    create( size );
    if( size )
    {
      memset( m_string, ' ', size );
      m_string[size] = '\0';
    }
  }

  //##
  // Archive to lArchive

  friend inline lStream& operator<< ( lStream &ar, lString &c )
  {
    int size = c.m_size;
    ar << size;
    if( ar.IsLoading() )
    {
      c.m_size = size;
      c.destroy();
      c.create(size);
    }

    ar.Serialize( c.m_string, c.m_size );
    c.m_string[c.m_size] = '\0';
    return ar;
  }

//  friend inline lResourceFixer & operator << ( lResourceFixer &fixer, lString &c )
//  {
//    c.m_bFixed = true;
//    fixer.FixString( c.m_string );
//    return fixer;
//  }

  inline operator const char *()       { return m_string; }
  inline operator const char *() const { return m_string; }

  inline lString &operator=(const lString &s)
  {
    if (&s != this)
    {
      destroy();
      create( s.m_size );
      strcpy( m_string, s.m_string );
    }

    return (*this);
  }

  inline lString &operator=(const char *str)
  {
    destroy();
    if( str == NULL )
    {
      create( 0 );
      m_string[0] = '\0';
    }
    else
    {
      int sz = (int)strlen( str );
      create( sz );
      strcpy( m_string, str );
    }

    return (*this);
  }

  inline lString &operator=(char c)
  {
    destroy();
    create( 1 );
    m_string[0] = c;
    m_string[1] = '\0';

    return (*this);
  }


  inline lString operator+(const lString &s) const
  {
    lString result;
    result.destroy();
    int newSize = m_size + s.m_size;
    result.create( newSize );
    strcpy( result.m_string, m_string );
    strcpy( result.m_string + m_size, s.m_string );

    return result;
  }

  inline lString operator+=(const lString &s)
  {
    if( m_bFixed )
    {
      // cannot create a fixed string
      lAssertAlways( !( "lString::operator+=: Cannot add a fixed string" ) );
      return (*this);
    }

    int newSize = m_size + s.m_size;
    char *newString = allocate<char>(__FILE__,__FUNCTION__,__LINE__,L_ARENA_PLATFORM,newSize + 1);
    strcpy( newString, m_string );
    strcpy( newString + m_size, s.m_string );
    destroy();
    m_string = newString;
    m_size = newSize;

    return (*this);
  }

  //
  inline bool operator%=(const lString &s) const
  {
    return strcmpi( m_string, s.m_string ) == 0;
  }

  inline bool operator%=(const char * s) const
  {
    lAssert( s != NULL );
    return strcmpi( m_string, s ) == 0;
  }
  //

  inline bool WildCardCompare( const char * pattern ) const
  {
    if(m_string == NULL || pattern == NULL) return false;
    return patimat(pattern, m_string);
  }

  inline bool WildCardCompareNoCase( const char * pattern ) const
  {
    if(m_string == NULL || pattern == NULL) return false;
    return patimatNoCase(pattern, m_string);
  }


  inline bool operator==(const lString &s) const
  {
    return strcmp( m_string, s.m_string ) == 0;
  }

  inline bool operator!=(const lString &s) const
  {
    return strcmp( m_string, s.m_string ) != 0;
  }

  inline bool operator==(const char *s) const
  {
    lAssert( s != NULL );
    return strcmp( m_string, s ) == 0;
  }

  inline bool operator!=(const char *s) const
  {
    lAssert( s != NULL );
    return strcmp( m_string, s ) != 0;
  }

  inline bool operator<(const lString &s) const
  {
    return strcmp( m_string, s.m_string ) < 0;
  }

  inline bool operator<=(const lString &s) const
  {
    return strcmp( m_string, s.m_string ) <= 0;
  }

  inline bool operator>=(const lString &s) const
  {
    return strcmp( m_string, s.m_string ) >= 0;
  }

  inline bool operator>(const lString &s) const
  {
    return strcmp( m_string, s.m_string ) > 0;
  }

  inline char &operator()(int i)
  {
    lAssert( i>=0 && i<m_size );
    return m_string[i];
  }

  inline const char &operator()(int i) const
  {
    lAssert( i>=0 && i<m_size );
    return m_string[i];
  }

  inline bool IsFixed() const
  {
    return m_bFixed;
  }

  inline char *CStr()
  {
    return m_string;
  }

  inline const char *CStr() const
  {
    return m_string;
  }

  bool IsEmpty() const
  {
    return m_size == 0;
  }

  inline int Size() const
  {
    return m_size;
  }

  inline char First() const
  {
    return m_string[0];
  }

  inline char &First()
  {
    return m_string[0];
  }

  inline char Last() const
  {
    return m_string[ m_size - 1 ];
  }

  inline char &Last()
  {
    return m_string[ m_size - 1 ];
  }


  inline void ToLower()
  {
    for (int i = 0; i < m_size; i++)
    {
      char c = m_string[i];

      if ( c >= 'A'  &&  c <= 'Z' )
      {
        m_string[i] = ( c - 'A' ) + 'a';
      }
    }
  }

  inline void ToUpper()
  {
    for (int i = 0; i < m_size; i++)
    {
      char c = m_string[i];

      if ( c >= 'a'  &&  c <= 'z' )
      {
        m_string[i] = ( c - 'a' ) + 'A';
      }
    }
  }

  inline lString LowerCase() const
  {
    lString result = *this;
    result.ToLower();
    return result;
  }

  inline lString UpperCase() const
  {
    lString result = *this;
    result.ToUpper();
    return result;
  }

  inline void Format( const char * fmt, ... )
  {
    lAssert( fmt != NULL );
    char buf[1024];
    va_list args;
    va_start( args, fmt );
    lPlatform_vsnprintf( buf, sizeof(buf), fmt, args );
    buf[sizeof(buf)-1] = '\0';
    va_end( args );

    destroy();
    int sz = (int)strlen( buf );
    create( sz );
    strcpy( m_string, buf );
  }

  inline void VFormat( const char * fmt, va_list &args )
  {
    lAssert( fmt != NULL );
    char buf[1024];
    lPlatform_vsnprintf( buf, sizeof(buf), fmt, args );
    buf[sizeof(buf)-1] = '\0';

    destroy();
    int sz = (int)strlen( buf );
    create( sz );
    strcpy( m_string, buf );
  }


  inline lString Left(int size) const
  {
    if( size > m_size ) size = m_size;
    return lString( &m_string[0], size );
  }

  inline lString Right(int size) const
  {
    if( size > m_size ) size = m_size;
    return lString( &m_string[m_size-size], size );
  }

  inline lString SubStr( int first ) const
  {
    return SubStr( first, Size() );
  }

  //##
  // Return an lString created from a substring of this string, the substring starts from 'first' and ends at 'last'
  // non-inclusive (the substring will not include the character indexed by 'last')
  inline lString SubStr( int first, int last ) const
  {
    if( last > Size() ) last = Size();
    if( first > Size() ) first = Size();
    if( last < 0 ) last = 0;
    if( first < 0 ) first = 0;
    if( last-first <= 0 ) return lString();
    return lString( &m_string[first], last-first );
  }

  inline lString SpanExcluding( lString excludeList )
  {
    int last = 0;
    while(last < m_size)
    {
      int i;
      for(i = 0; i < excludeList.Size(); i++)
      {
        if(m_string[last] == excludeList(i))
        {
          break;
        }
      }

      if(i != excludeList.Size())
      {
        // we got a match in exclude list
        break;
      }

      last++;
    }

    return lString( &m_string[0], last );
  }

  inline lString SpanIncluding( lString includeList )
  {
    int last = 0;
    while(last < m_size)
    {
      int i;
      for(i = 0; i < includeList.Size(); i++)
      {
        if(m_string[last] != includeList(i))
        {
          break;
        }
      }

      if(i != includeList.Size())
      {
        // we got a mismatch in include list
        break;
      }

      last++;
    }

    return lString( &m_string[0], last );
  }

  //##
  // Finds first instance of character 'c' after character index 'idx' in this string (defaults to search from index 0)
  inline int FindFirst(char c, int idx = 0) const
  {
    if( idx < 0 ) idx = 0;
    for (int i = idx; i < m_size; i++)
    {
      if ( m_string[i] == c )
      {
        return i;
      }
    }

    return -1;
  }

  //##
  // Finds last instance of 'c'
  inline int FindLast(char c) const
  {
    for (int i = m_size - 1; i >= 0; i--)
    {
      if ( m_string[i] == c )
      {
        return i;
      }
    }

    return -1;
  }

  inline int FindFirstFrom(const lString &charList) const
  {
    for (int i = 0; i < m_size; i++)
    {
      for (int j = 0; j < charList.Size(); j++)
      {
        if ( m_string[i] == charList(j) )
        {
          return i;
        }
      }
    }

    return -1;
  }

  inline int FindLastFrom(const lString &charList) const
  {
    for (int i = m_size - 1; i >= 0; i--)
    {
      for (int j = 0; j < charList.Size(); j++)
      {
        if ( m_string[i] == charList(j) )
        {
          return i;
        }
      }
    }

    return -1;
  }

  //##
  // Finds first instance of string 's' after character index 'idx' in this string (defaults to search from index 0)
  int FindFirst(const lString &s, int idx=0) const
  {
    if( idx < 0 ) idx = 0;
    int checkSize = m_size - s.m_size;
    for (int i = idx; i <= checkSize; i++)
    {
      if ( m_string[i] == s.m_string[0] )
      {
        bool bFound=true;
        for(int j = 1; j < s.m_size; j++)
	      {
          if( m_string[i+j] != s.m_string[j] )
		      {
            bFound=false;
		        break;
          }
	      }

        if( bFound ) return i;
      }
    }

    return -1;
  }

  int FindLast(const lString &s) const
  {
    int checkFirst = m_size - s.m_size;
    for (int i = checkFirst; i >= 0; i--)
    {
      if ( m_string[i] == s.m_string[0] )
      {
        bool bFound=true;
        for (int j = 1; j < s.m_size; j++)
	      {
	        if ( m_string[ i + j ] != s.m_string[j] )
		      {
            bFound=false;
		        break;
		      }
	      }
        if( bFound ) return i;
      }
    }

    return -1;
  }

  void FindAndReplace(char c, char with)
  {
    for (int i = 0; i < m_size; i++)
    {
      if ( m_string[i] == c )
      {
        m_string[i] = with;
      }
    }
  }

  void Insert(int position, int n = 1)
  {
    char *oldStr = m_string;

    create( m_size + n );

    int i;
    // copy everything up to @position
    for (i = 0; i < position; i++)
    {
      m_string[i] = oldStr[i];
    }

    // put @n x space (0x20)
    for (; i < n; i++)
    {
      m_string[i] = 0x20;
    }

    // copy everything after @position + @n
    for (; i <= m_size; i++)
    {
      m_string[i] = oldStr[ i - n ];
    }

    delete[] oldStr;
  }

  void Remove(int position, int n = 1)
  {
    // todo: doesn't resize string
    // todo: doesn't assert logic.

    for (int i = position; i <= m_size - n; i++)
    {
      m_string[i] = m_string[ i + n ];
    }

    m_size -= n;
  }


  void TrimLeft()
  {
    int i = 0;
    while((m_string[i] == ' ' || m_string[i] == '\n' || m_string[i] == '\r' || m_string[i] == '\t') && i < m_size)
    {
      i++;
    }

    if(i == 0)
    {
      return;
    }

    Remove( 0, i );
  }

  void TrimRight()
  {
    int i = m_size - 1;
    while((m_string[i] == ' ' || m_string[i] == '\n' || m_string[i] == '\r' || m_string[i] == '\t') && i >= 0)
    {
      i--;
    }

    if(i == m_size - 1)
    {
      return;
    }

    Remove( i + 1, m_size - i - 1 );
  }

  static lString IntToString(int i)
  {
    char buffer[LSTRING_INTLEN];
    sprintf( buffer, "%d", i );
    return lString( buffer );
  }

  static lString FloatToString(float f)
  {
    char buffer[LSTRING_FLOATLEN];
    sprintf( buffer, "%f", f );
    return lString( buffer );
  }

  static lString DoubleToString(double d)
  {
    char buffer[LSTRING_FLOATLEN];
    sprintf( buffer, "%f", d );
    return lString( buffer );
  }

  static lString BoolToString(bool b)
  {
    if ( b )
    {
      return lString( "true" );
    }
    else
    {
      return lString( "false" );
    }
  }


private:
  inline char * GetZeroBuffer()
  {
#ifdef L_PLATFORM_PS2
    //NOTE: Temporary workaround for compiler warning on PS2, should really be moved into lib to prevent multiple copies
    static char zero_buffer[1];
    zero_buffer[0] = '\0';
#else
    static char zero_buffer[1] = {'\0'};
#endif
    return zero_buffer;
  }

  inline void create(int size)
  {
    if( m_bFixed )
    {
      // cannot create a fixed string
      lAssertAlways( !( "lString::create: Cannot create a fixed string" ) );
      return;
    }

    lAssert( m_string == static_cast<char *>(0) );

    if( size == 0 )
    {
      m_string = GetZeroBuffer();
    }
    else
    {
      m_string = allocate<char>(__FILE__,__FUNCTION__,__LINE__,L_ARENA_PLATFORM,size + 1);
    }

    m_size = size;
  }

  inline void destroy()
  {
    if( m_bFixed )
    {
      // cannot destroy a fixed string
      lAssertAlways( !( "lString::destroy: Cannot destroy a fixed string" ) );
      return;
    }

    if( m_string != GetZeroBuffer() )
    {
      deallocate(__FILE__,__FUNCTION__,__LINE__,m_string);
    }

    m_string = static_cast<char *>(0);
    m_size = 0;
  }

  inline bool patimat(const char *pat, const char *str) const
  {
    switch (*pat)
    {
      case '\0': return !*str;
      case '*' : return patimat(pat+1, str) || *str && patimat(pat, str+1);
      case '?' : return *str && patimat(pat+1, str+1);
      //case '\\': return (*str == '\\' || *str == '/') && patimat(pat+1, str+1);
      //case '/' : return (*str == '\\' || *str == '/') && patimat(pat+1, str+1);
      default  : return ((*pat) == (*str)) && patimat(pat+1, str+1);

    }
  }

  inline bool patimatNoCase(const char *pat, const char *str) const
  {
    switch (*pat)
    {
      case '\0': return !*str;
      case '*' : return patimatNoCase(pat+1, str) || *str && patimatNoCase(pat, str+1);
      case '?' : return *str && patimatNoCase(pat+1, str+1);
      //case '\\': return (*str == '\\' || *str == '/') && patimatNoCase(pat+1, str+1);
      //case '/' : return (*str == '\\' || *str == '/') && patimatNoCase(pat+1, str+1);
      default  : return (toupper(*pat) == toupper(*str)) && patimatNoCase(pat+1, str+1);
    }
  }


private:
  char * m_string;
  uint32 m_size:31;
  uint32 m_bFixed:1;
};

END_L_NAMESPACE
#endif





