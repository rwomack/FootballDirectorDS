#ifndef CSTRING_H
#define CSTRING_H

#include <fdmemory.h>
#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>

class CString
{
protected:
  char * m_string;
  // size stored at -sizeof(int) before m_string
  //int    m_size;

public:
  inline CString() : m_string( static_cast<char *>(0) )
  {
    create( 0 );
  }

  inline CString(const CString &s) : m_string( static_cast<char *>(0) )
  {
    if (&s != this)
    {
      if( s.m_string != NULL )
      {
        int ssize = s.Size();
        create( ssize );
        memcpy( m_string, s.m_string, ssize );
        m_string[ssize] = '\0';
      } else
      {
        create( 0 );
      }
    }
  }

  inline CString(const char *str) : m_string( static_cast<char *>(0) )
  {
    if ( str == NULL )
    {
      create( 0 );
    }
    else
    {
      int sz = (int)strlen( str );
      create( sz );
      memcpy( m_string, str, sz );
      m_string[sz] = '\0';
    }
  }

  inline CString(const char *str, int len) : m_string( static_cast<char *>(0) )
  {
    if ( str == NULL )
    {
      create( 0 );
    }
    else
    {
      create( len );
      memcpy( m_string, str, len );
      m_string[len] = '\0';
    }
  }

  inline CString(char c) : m_string( static_cast<char *>(0) )
  {
    create( 1 );
    m_string[0] = c;
    m_string[1] = '\0';
  }

  inline ~CString()
  {
    destroy();
  }

  inline void AllocateBlankString( int size )
  {
    destroy();
    create( size );
    memset( m_string, ' ', size );
    m_string[size] = '\0';
  }

  inline operator const char *()       { return m_string; }
  inline operator const char *() const { return m_string; }

  inline CString &operator=(const CString &s)
  {
    if (&s != this)
    {
      destroy();

      if( s.m_string != NULL )
      {
        int ssize = s.Size();
        create( ssize );
        memcpy( m_string, s.m_string, ssize );
        m_string[ssize] = '\0';
      } else
      {
        create(0);
      }
    }

    return (*this);
  }

  inline CString &operator=(const char *str)
  {
    destroy();
    if( str == NULL )
    {
      create( 0 );
    }
    else
    {
      int sz = (int)strlen( str );
      create( sz );
      memcpy( m_string, str, sz );
      m_string[sz] = '\0';
    }

    return (*this);
  }

  inline CString &operator=(char c)
  {
    destroy();
    create( 1 );
    m_string[0] = c;
    m_string[1] = '\0';

    return (*this);
  }

  inline CString operator+(const CString &s) const
  {
    CString result;
    result.destroy();

    if( m_string != NULL && s.m_string != NULL )
    {
      int size  = Size();
      int ssize = s.Size();

      int newSize = size + ssize;
      result.create( newSize );
      memcpy( result.m_string, m_string, size );
      memcpy( result.m_string+size, s.m_string, ssize );
      result.m_string[ size+ssize ] = '\0';
    } else
    if( m_string != NULL  )
    {
      result = *this;
    } else
    if( s.m_string != NULL  )
    {
      result = s;
    } else
    {
      result.create(0);
    }

    return result;
  }

  inline CString operator+( const char *str ) const
  {
    // dumb implementation
    CString toadd( str );
    return (*this + toadd );
  }


  inline CString &operator+=(const CString &s)
  {
    if((m_string != NULL) && (m_string != &zero_buffer[4]) && (s.m_string != NULL) && (s.m_string != &zero_buffer[4]) )
    {
      int size  = Size();
      int ssize = s.Size();

      int newSize = size + ssize;
      char *oldAllocStart = m_string - sizeof(int);
      char *newString = (char *)FD_STRING_REALLOC(newSize + 1 + sizeof(int), oldAllocStart);
      m_string = &newString[sizeof(int)];
      *((int *)newString) = newSize;

      memcpy( m_string+size, s.m_string, ssize );
      m_string[ size+ssize ] = '\0';
    } else
    if ((m_string != NULL) && (m_string != &zero_buffer[4]))
    {
      // stay as is
    } else
    if ((s.m_string != NULL) && (s.m_string != &zero_buffer[4]))
    {
      (*this) = s;
    }

    return (*this);
  }

  //
  inline bool operator%=(const CString &s) const
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

  inline bool CompareNoCase(const CString &s) const
  {
    return stricmp( m_string, s.m_string ) == 0;
  }

  inline bool operator==(const CString &s) const
  {
    return strcmp( m_string, s.m_string ) == 0;
  }

  inline bool operator!=(const CString &s) const
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

  inline bool operator<(const CString &s) const
  {
    return strcmp( m_string, s.m_string ) < 0;
  }

  inline bool operator<=(const CString &s) const
  {
    return strcmp( m_string, s.m_string ) <= 0;
  }

  inline bool operator>=(const CString &s) const
  {
    return strcmp( m_string, s.m_string ) >= 0;
  }

  inline bool operator>(const CString &s) const
  {
    return strcmp( m_string, s.m_string ) > 0;
  }

  inline char &operator()(int i)
  {
    if( m_string == NULL )
    {
      lAssert(0);
    }

    int size  = *(((int*)m_string)-1);
    lAssert( i>=0 && i<size );
    return m_string[i];
  }

  inline const char &operator()(int i) const
  {
    if( m_string == NULL )
    {
      lAssert(0);
    }

    int size  = *(((int*)m_string)-1);
    lAssert( i>=0 && i<size );
    return m_string[i];
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
    if( m_string == NULL )
    {
      return true;
    }

    return Size() == 0;
  }

  void Empty()
  {
    destroy();
  }

  inline int Size() const
  {
    if( m_string == NULL )
    {
      return 0;
    }

    int size  = *(((int*)m_string)-1);
    return size;
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
    if( m_string == NULL )
    {
      lAssert(0);
    }

    int size  = *(((int*)m_string)-1);
    return m_string[ size - 1 ];
  }

  inline char &Last()
  {
    if( m_string == NULL )
    {
      lAssert(0);
    }

    int size  = *(((int*)m_string)-1);
    return m_string[ size - 1 ];
  }


  inline void ToLower()
  {
    int size = Size();

    for (int i = 0; i < size; i++)
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
    int size = Size();

    for (int i = 0; i < size; i++)
    {
      char c = m_string[i];

      if ( c >= 'a'  &&  c <= 'z' )
      {
        m_string[i] = ( c - 'a' ) + 'A';
      }
    }
  }

  inline CString LowerCase() const
  {
    CString result = *this;
    result.ToLower();
    return result;
  }

  inline CString UpperCase() const
  {
    CString result = *this;
    result.ToUpper();
    return result;
  }

  void Format( const char * fmt, ... );

  //inline void VFormat( const char * fmt, va_list &args )
  //{
  //  lAssert( fmt != NULL );
  //  char buf[1024];
  //  lPlatform_vsnprintf( buf, sizeof(buf), fmt, args );
  //  buf[sizeof(buf)-1] = '\0';
  //
  //  destroy();
  //  int sz = (int)strlen( buf );
  //  create( sz );
  //  strcpy( m_string, buf );
  //}


  inline CString Left(int size) const
  {
    int ssize = Size();

    if( size > ssize ) size = ssize;
    return CString( &m_string[0], size );
  }

  inline CString Right(int size) const
  {
    int ssize = Size();

    if( size > ssize ) size = ssize;
    return CString( &m_string[ssize-size], size );
  }

  inline CString SubStr( int first ) const
  {
    return SubStr( first, Size() );
  }

  //##
  // Return an CString created from a substring of this string, the substring starts from 'first' and ends at 'last'
  // non-inclusive (the substring will not include the character indexed by 'last')
  inline CString SubStr( int first, int last ) const
  {
    if( last > Size() ) last = Size();
    if( first > Size() ) first = Size();
    if( last < 0 ) last = 0;
    if( first < 0 ) first = 0;
    if( last-first <= 0 ) return CString();
    return CString( &m_string[first], last-first );
  }

  inline CString Mid(int first, int num) const
  {
    return SubStr( first, first+num );
  }


  inline CString SpanExcluding( const CString &excludeList )
  {
    int ssize = Size();

    int last = 0;
    while(last < ssize)
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

    return CString( &m_string[0], last );
  }

  inline CString SpanIncluding( const CString &includeList )
  {
    int ssize = Size();

    int last = 0;
    while(last < ssize)
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

    return CString( &m_string[0], last );
  }

  //##
  // Finds first instance of character 'c' after character index 'idx' in this string (defaults to search from index 0)
  inline int FindFirst(char c, int idx = 0) const
  {
    int ssize = Size();

    if( idx < 0 ) idx = 0;
    for (int i = idx; i < ssize; i++)
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
    int ssize = Size();

    for (int i = ssize - 1; i >= 0; i--)
    {
      if ( m_string[i] == c )
      {
        return i;
      }
    }

    return -1;
  }

  inline int FindFirstFrom(const CString &charList) const
  {
    int ssize = Size();

    for (int i = 0; i < ssize; i++)
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

  inline int FindLastFrom(const CString &charList) const
  {
    int ssize = Size();

    for (int i = ssize - 1; i >= 0; i--)
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
  int FindFirst(const CString &s, int idx=0) const
  {
    int ssize = Size();

    int ssize2 = s.Size();

    if( idx < 0 ) idx = 0;
    int checkSize = ssize - ssize2;
    for (int i = idx; i <= checkSize; i++)
    {
      if ( m_string[i] == s.m_string[0] )
      {
        bool bFound=true;
        for(int j = 1; j < ssize2; j++)
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

  //int FindLast(const CString &s) const
  //{
  //  int checkFirst = m_size - s.m_size;
  //  for (int i = checkFirst; i >= 0; i--)
  //  {
  //    if ( m_string[i] == s.m_string[0] )
  //    {
  //      bool bFound=true;
  //      for (int j = 1; j < s.m_size; j++)
  //      {
  //        if ( m_string[ i + j ] != s.m_string[j] )
  //        {
  //          bFound=false;
  //          break;
  //        }
  //      }
  //      if( bFound ) return i;
  //    }
  //  }
  //
  //  return -1;
  //}
  //
  //void FindAndReplace(char c, char with)
  //{
  //  for (int i = 0; i < m_size; i++)
  //  {
  //    if ( m_string[i] == c )
  //    {
  //      m_string[i] = with;
  //    }
  //  }
  //}

  void Insert(int position, int n = 1)
  {
    int size = Size();
    CString oldStr = CString(*this);

    destroy();

    create( size + n );

    // copy everything up to @position
    int i;
    for (i = 0; i < position; i++)
    {
      m_string[i] = oldStr(i);
    }

    // put @n x space (0x20)
    for (; i < position + n; i++)
    {
      m_string[i] = 0x20;
    }

    // copy everything after @position + @n
    for (; i < size + n; i++)
    {
      m_string[i] = oldStr( i - n );
    }

    m_string[size+n] = '\0';

  }

  //void Remove(int position, int n = 1)
  //{
  //  // todo: doesn't resize string
  //  // todo: doesn't assert logic.
  //
  //  for (int i = position; i <= m_size - n; i++)
  //  {
  //    m_string[i] = m_string[ i + n ];
  //  }
  //
  //  m_size -= n;
  //}


//  void TrimLeft()
//  {
//    int i = 0;
//    while((m_string[i] == ' ' || m_string[i] == '\n' || m_string[i] == '\r' || m_string[i] == '\t') && i < m_size)
//    {
//      i++;
//    }
//
//    if(i == 0)
//    {
//      return;
//    }
//
//    Remove( 0, i );
//  }
//
//  void TrimRight()
//  {
//    int i = m_size - 1;
//    while((m_string[i] == ' ' || m_string[i] == '\n' || m_string[i] == '\r' || m_string[i] == '\t') && i >= 0)
//    {
//      i--;
//    }
//
//    if(i == m_size - 1)
//    {
//      return;
//    }
//
//    Remove( i + 1, m_size - i - 1 );
//  }
//

  void TrimStart()
  {
	  int iTrimCount = 0;
	  for (int i = 0; i < GetLength(); i++)
	  {
		  if (!::isspace((*this)[i]))
			  break;
		  iTrimCount++;
	  }

	  if (iTrimCount > 0)
	  {
		  *this = SubStr(iTrimCount);
	  }
  }

  void TrimEnd()
  {
	  int iTrimCount = 0;
	  for (int i = GetLength() - 1; i >= 0; i--)
	  {
		  if (!::isspace((*this)[i]))
			  break;
		  iTrimCount++;
	  }

	  if (iTrimCount > 0)
	  {
		  *this = Left(GetLength() - iTrimCount);
	  }
  }

  void Trim()
  {
	  TrimStart();
	  TrimEnd();
  }

  static CString BoolToString(bool b)
  {
    if ( b )
    {
      return CString( "true" );
    }
    else
    {
      return CString( "false" );
    }
  }

  //BAROG
  // Stuff for FD

  CString &MakeUpper()
  {
    ToUpper();
    return( *this );
  }

	// Convert the string to lowercase
  CString &MakeLower()
  {
    ToLower();
    return( *this );
  }

	// Reverse the string
  CString &MakeReverse()
  {
    int size = Size();

    int l = size - 1;
    if(l == 1) return( *this );

    for(int x=0;x < l;x++,l--)
    {
		L_NAMESPACE_NAME::lSwap( m_string[l], m_string[x] );
    }

    return( *this );
  }

  int GetLength() const
  {
    return Size();
  }

  void LoadString( int strID );
  void Format( int strID, ... );

  int Find( const char *str )
  {
    return FindFirst( CString( str ) );
  }

  //char GetAt( int idx )
  //{
  //  lAssert( m_string != NULL );
  //  return m_string[idx];
  //}

  CString GetAt( int idx ) const
  {
    lAssert( m_string != NULL );
	lAssert( idx < Size() );
    return CString(m_string[idx]);
  }


protected:
  static char zero_buffer[5];
  inline char * GetZeroBuffer()
  {
#ifdef L_PLATFORM_PS2
    //NOTE: Temporary workaround for compiler warning on PS2, should really be moved into lib to prevent multiple copies
    *((int *)zero_buffer) = 1;
    zero_buffer[4] = '\0';
#else
    *((int *)zero_buffer) = 1;
    zero_buffer[4] = '\0';
#endif
    return zero_buffer;
  }

  inline void create(int size)
  {
    lAssert( m_string == static_cast<char *>(0) );

    if( size == 0 )
    {
      m_string = GetZeroBuffer();
    }
    else
    {
      m_string = FD_STRING_NEW char[size + 1 + sizeof(int)];
    }

    // offset around size
    m_string += sizeof(int);
    *((int *)m_string-1) = size;
  }

  inline void destroy()
  {
    if (( m_string != NULL ) && (m_string != &zero_buffer[4]))
    {
      delete[] (m_string-sizeof(int));
    }

    m_string = static_cast<char *>(0);
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
};


//TODO: Original class used pointers to CString
class CStringArray
{
 public:

// Construction
  CStringArray()
  {
  }

  ~CStringArray()
  {
  }

// Attributes
  int GetSize() const
  {
    return m_array.Size();
  }
//  int GetCount() const;
  bool  IsEmpty() const
  {
    return m_array.Size() == 0;
  }

  int GetUpperBound() const
  {
    return m_array.Size() - 1;
  }
//  void SetSize(int nNewSize, int nGrowBy = -1);

// Operations
	// Clean up
//  void FreeExtra();
//  void RemoveAll();

	// Accessing elements
  CString &GetAt(int nIndex)
  {
//    lLogFmt("%x: GetAt[] %d = %s\n", this, nIndex, m_array(nIndex));
    return m_array(nIndex);
  }
//  void SetAt(int nIndex, LPCTSTR newElement);
//  void SetAt(int nIndex, const CString& newElement);

//  CString& ElementAt(int nIndex);

	// Direct Access to the element data (may return NULL)
//  const CString* GetData() const;
//  CString* GetData();

	// Potentially growing the array
//  void SetAtGrow(int nIndex, LPCTSTR newElement);
  void SetAtGrow(int nIndex, const CString& newElement)
  {
//    lLogFmt("%x: SetAtGrow %d %s\n", this, nIndex, newElement);
    if( nIndex >= m_array.Size() )
    {
      m_array.SetSize( nIndex+1 );
    }

    //m_array(nIndex) = FD_ENGINE_NEW CString;
    m_array(nIndex) = newElement;
  }

//  int Add(LPCTSTR newElement);
  int Add(const CString& newElement)
  {
//    lLogFmt("%x: Add %s\n", this, newElement);
    m_array.Push( newElement );
    //m_array.Last() = newElement;
    return m_array.Size()-1;
  }

//  int Append(const CStringArray& src);
//  void Copy(const CStringArray& src);

	// overloaded operator helpers
  const CString &operator[](int nIndex) const
  {
//    lLogFmt("%x: opreator[]const %d = %s\n", this, nIndex, m_array(nIndex));
    return m_array(nIndex);
  }

  CString &operator[](int nIndex)
  {
//    lLogFmt("%x: opreator[] %d = %s\n", this, nIndex, m_array(nIndex));
    return m_array(nIndex);
  }

  void FreeAll()
  {
    m_array.Empty();
  }


	// Operations that move elements around
//  void InsertAt(int nIndex, LPCTSTR newElement, int nCount = 1);
//
//  void InsertAt(int nIndex, const CString& newElement, int nCount = 1);
//
//  void RemoveAt(int nIndex, int nCount = 1);
//  void InsertAt(int nStartIndex, CStringArray* pNewArray);

// Implementation
protected:
	L_NAMESPACE_NAME::lArray<CString> m_array;
};

#endif // #ifndef CSTRING_H
