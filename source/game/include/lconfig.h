//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lconfig.h
// Description : Base configuration system
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

/*
 Modification History
 --------------------

 26/11/2005 - First version                                       - TimJ

 Notes
 -----

*/

#include <lemon/io/lfileiostream.h>
#include <lemon/containers/larray.h>
#include <lemon/containers/lstring.h>

#ifndef LCONFIG_H_INCLUDED
#define LCONFIG_H_INCLUDED

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Defines
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lConfigBase - Base configuration functionality
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Config Base Class
//
class lConfigBase
{
  public:
    virtual ~lConfigBase() {};

    virtual bool         GetBool( const char * section, const char * token, bool _default = false ) = 0;
    virtual int          GetInteger( const char * section, const char * token, int _default = 0 ) = 0;
    virtual float        GetFloat( const char * section, const char * token, float _default = 0 ) = 0;
    virtual const char * GetString( const char * section, const char * token, const char * _default = "" ) = 0;

    virtual void         SetBool( const char * section, const char * token, bool value ) = 0;
    virtual void         SetInteger( const char * section, const char * token, int value ) = 0;
    virtual void         SetFloat( const char * section, const char * token, float value ) = 0;
    virtual void         SetString( const char * section, const char * token,  const char * value ) = 0;

//    // integer token accessors
//
//    virtual bool         GetBool( const char * section, int token, bool _default = false )        { char str[25]; itoa(token,str,25); return GetBool(section,str,_default);    }
//    virtual int          GetInteger( const char * section, int token, int _default = 0 )          { char str[25]; itoa(token,str,25); return GetInteger(section,str,_default); }
//    virtual float        GetFloat( const char * section, int token, float _default = 0 )          { char str[25]; itoa(token,str,25); return GetFloat(section,str,_default);   }
//    virtual const char * GetString( const char * section, int token, const char * _default = "" ) { char str[25]; itoa(token,str,25); return GetString(section,str,_default);  }
//
//    virtual void         SetBool( const char * section, int token, bool value )                   { char str[25]; itoa(token,str,25); SetBool(section,str,value);    }
//    virtual void         SetInteger( const char * section, int token, int value )                 { char str[25]; itoa(token,str,25); SetInteger(section,str,value); }
//    virtual void         SetFloat( const char * section, int token, float value )                 { char str[25]; itoa(token,str,25); SetFloat(section,str,value);   }
//    virtual void         SetString( const char * section, int token, const char * value )         { char str[25]; itoa(token,str,25); SetString(section,str,value);  }

    // iterators

    virtual const char * GetNextSection( const char * ) { return NULL; }
    virtual const char * GetNextToken( const char * section, const char * token ) { return NULL; }
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lConfigNull - Null configuration functionality
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// Config Null Class
//
class lConfigNull : public lConfigBase
{
  public:
    lConfigNull() {}
    virtual ~lConfigNull() {};

    virtual bool         GetBool( const char * section, const char * token, bool _default = 0 )        { return _default; }
    virtual int          GetInteger( const char * section, const char * token, int _default = 0 )      { return _default; }
    virtual float        GetFloat( const char * section, const char * token, float _default =0 )       { return _default; }
    virtual const char * GetString( const char * section, const char * token, const char * _default =0){ return _default; }

    virtual void         SetBool( const char * section, const char * token, bool value )               { }
    virtual void         SetInteger( const char * section, const char * token, int value )             { }
    virtual void         SetFloat( const char * section, const char * token, float value )             { }
    virtual void         SetString( const char * section, const char * token,  const char * value )    { }

    virtual void         GetSectionContents(const char *section, lArray<lString> &names,
                                            lArray<lString> &values)                                 { }
};


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lConfigFile - File configuration functionality
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

// TimJ:
// Bit of a hack here, I want a default section.
// So tokens that are not found are checked for in the default section first
// before returning the default value.
//
// The interface could probably do with reworking.
//
// Also made the GetBool() return true/false if the token is a string value
// or true or false.

// Config File Class
//
class lConfigFile : public lConfigBase
{
  public:
    lConfigFile();
    lConfigFile( lIOStreamSeekable * );
    lConfigFile( const char * );
    virtual ~lConfigFile();

    void Reset();

    virtual bool Read( lIOStreamSeekable * stream );
    virtual bool Write( lIOStreamSeekable * stream );

    virtual bool Read( const char * );
    virtual bool ReadLocal( const char * );
    virtual bool Write( const char * );

    virtual bool         GetBool( const char * section, const char * token, bool _default = 0 )            { Token * t; if( (t=FindToken(section,token)) || (m_defaultSectionName.Size() > 0 && (t=FindToken(m_defaultSectionName,token))) ) { if( strcmpi(t->value.Data(),"true") == 0 ) return true; if( strcmpi(t->value.Data(),"false") == 0 ) return false; return (atoi(t->value.Data()) != 0); } else { return _default; } }
    virtual int          GetInteger( const char * section, const char * token, int _default = 0 )          { Token * t; if( (t=FindToken(section,token)) || (m_defaultSectionName.Size() > 0 && (t=FindToken(m_defaultSectionName,token))) ) { return atoi(t->value.Data());        } else { return _default; } }
    virtual float        GetFloat( const char * section, const char * token, float _default = 0 )          { Token * t; if( (t=FindToken(section,token)) || (m_defaultSectionName.Size() > 0 && (t=FindToken(m_defaultSectionName,token))) ) { if( strcmpi(t->value.Data(),"inf") == 0 ) return LM_FLOAT_MAX; return atof(t->value.Data()); } else { return _default; } }
    virtual const char * GetString( const char * section, const char * token, const char * _default = 0 )  { Token * t; if( (t=FindToken(section,token)) || (m_defaultSectionName.Size() > 0 && (t=FindToken(m_defaultSectionName,token))) ) { return t->value.Data();              } else { return _default; } }


    virtual void         SetBool( const char * section, const char * token, bool value )               { char dest[64]; lPlatform_snprintf( dest, sizeof(dest)-1, "%d", value ); SetToken( section, token, dest ); }
    virtual void         SetInteger( const char * section, const char * token, int value )             { char dest[64]; lPlatform_snprintf( dest, sizeof(dest)-1, "%d", value ); SetToken( section, token, dest ); }
    virtual void         SetFloat( const char * section, const char * token, float value )             { char dest[64]; lPlatform_snprintf( dest, sizeof(dest)-1, "%f", value ); SetToken( section, token, dest ); }
    virtual void         SetString( const char * section, const char * token, const char * value )     { SetToken( section, token, value ); }


    virtual void         GetSectionContents(const char *section, lArray<lString> &names,
                                            lArray<lString> &values);

    virtual const char * GetSectionMatch( const char *section, const unsigned int skip, int &matchChars );
    virtual bool         GetSectionTokenValid( const char * section, const char * token );
    virtual const char * GetBestSectionForToken( const char *section, const char * token );

    const char * GetNextSection( const char * section );
    const char * GetNextToken( const char * section, const char * token );

    //TimJ: default section stuff
    void         SetDefaultSectionName( const char * sectionName ) { m_defaultSectionName = sectionName; }
    const char * GetDefaultSectionName()                           { return m_defaultSectionName; }

  private:

    lString m_defaultSectionName;

    struct Token
    {
      lArray<char> name;
      lArray<char> value;
    };

    struct Section
    {
      lArray<char> name;
      lArray<Token> tokenList;
      bool Find( const char *str, Token **tok );
    };

    lArray<Section> _sectionList;

    void    WriteString( lIOStreamSeekable * stream, const char *str );
    bool    FindSection( const char *str, Section **sec );
    Token * FindToken( const char * section, const char * token );
    void    SetToken( const char * section, const char * token, const char * value );

    bool    _patimatNoCase(const char *pat, const char *str, int &matchChars) const;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lConfigFile implementation
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

inline void lConfigFile::Reset()
{
  _sectionList.Empty();
  m_defaultSectionName = "";
}

inline lConfigFile::lConfigFile()
{
}


inline lConfigFile::lConfigFile(lIOStreamSeekable * stream)
{
  Read( stream );
}

inline lConfigFile::lConfigFile( const char * name )
{
  Read( name );
}

inline lConfigFile::~lConfigFile()
{
}

inline bool lConfigFile::Read( lIOStreamSeekable * stream )
{
  if( stream == NULL ) return false;

  char * baseStr = allocate<char>( __FILE__, __FUNCTION__, __LINE__, L_ARENA_GLOBAL, stream->Length() + 1 );
  char * str = baseStr;
  int len = stream->Read( str, stream->Length() );
  str[len] = '\0';

  int done=0;
  char * section = NULL;

  while( !done )
  {
    while( *str == '\r' || *str == '\n' ) str++;
    char *value = str;
    while( *str != '\r' && *str != '\n' && *str != '\0' ) str++;

    if( *str == '\0' ) done=1;

    if( value[0] == '#' ) continue;

    str--;
    while( *str == ' ' || *str == '\t' ) str--;
    str++;
    *str = '\0';

    if( value[0] == '[' &&  str[-1] == ']' )
    {
      str[-1] = '\0';
      value++;

      while( *value == ' ' || *value == '\t' ) value++;

      char * end = str-2;
      while( *end == ' ' || *end == '\t' ) end--;
      end[1] = '\0';

      section = value;
    }
    else
    {
      while( *value == ' ' || *value == '\t' ) value++;

      char *scan = value;
      while( *scan != '\0' && *scan != '=' ) scan++;
      if( *scan == '=' )
      {
        char *right = scan+1;
        while( *right == ' ' || *right == '\t' ) right++;

        scan--;
        while( *scan == ' ' || *scan == '\t' ) scan--;
        scan[1] = '\0';

        if( right[0] == '"' && str[-1] == '"' )
        {
          right++;
          str[-1] = '\0';
        }

        if( section )
        {
          SetString( section, value, right );
        }
      }
    }

    str++;
  }

  deallocate<char>( __FILE__, __FUNCTION__, __LINE__, baseStr );
  return true;
}

inline bool lConfigFile::Write( lIOStreamSeekable * stream )
{
  if( stream == NULL ) return false;

  for(int i=0;i<_sectionList.Size();i++)
  {
    WriteString(stream,"[");
    WriteString(stream,_sectionList(i).name.Data() );
    WriteString(stream,"]\r\n");

    for(int j=0;j<_sectionList(i).tokenList.Size();j++)
    {
      WriteString(stream,_sectionList(i).tokenList(j).name.Data());
      WriteString(stream,"=\"");
      WriteString(stream,_sectionList(i).tokenList(j).value.Data());
      WriteString(stream,"\"\r\n");
    }

  }

  return true;
}

inline bool lConfigFile::Read( const char * name )
{
  return ReadLocal(name);
}

inline bool lConfigFile::ReadLocal( const char * name )
{
  lFileIOStream stream( name, L_IO_ACCESS_READ );
  if( !stream.IsValid() ) return false;
  bool result = Read( &stream );
  stream.Close();
  return result;
}


inline bool lConfigFile::Write( const char * name )
{
  lFileIOStream stream( name, L_IO_ACCESS_WRITE );
  if( !stream.IsValid() ) return false;
  bool result = Write( &stream );
  stream.Close();
  return result;
}

inline bool lConfigFile::_patimatNoCase(const char *pat, const char *str, int &matchChars) const
{
  switch (*pat)
  {
    case '\0': return !*str;
    case '*' : return _patimatNoCase(pat+1, str, matchChars) || *str && _patimatNoCase(pat, str+1, matchChars);
    case '?' : return *str && _patimatNoCase(pat+1, str+1, matchChars);
    default  :
    {
      // pattern is not a wildcard, if it matches carry on, if not fail
      if((toupper(*pat) == toupper(*str)))
      {
        matchChars++;
        return _patimatNoCase(pat+1, str+1, matchChars);
      }

      return false;
    }
  }
}

inline const char * lConfigFile::GetSectionMatch( const char *section, const unsigned int skip, int &matchChars )
{
  unsigned int skipCount = skip;

  for(int i=0;i<_sectionList.Size();i++)
  {
    matchChars = 0;

    if( _patimatNoCase(_sectionList(i).name.Data(), section, matchChars) )
    {
      // if we are supposed to be skipping, reduce skip count and continue, otherwise return this section name
      if(skipCount)
      {
        skipCount--;
        continue;
      }

      return _sectionList(i).name.Data();
    }
  }

  return NULL;
}

inline bool  lConfigFile::GetSectionTokenValid( const char * section, const char * token )
{
  Section *s;

  if ( FindSection( section, &s ) )
  {
    Token *t;
    if( !s->Find(token, &t) ) return false;
    return true;
  }

  return false;
}

inline const char * lConfigFile::GetBestSectionForToken( const char *section, const char * token )
{
  const char *curSection;
  unsigned int curSkip = 0;
  int matchChars, bestMatch = -1, bestSkip = -1;

  // go through all section matches
  while(curSection = GetSectionMatch( section, curSkip, matchChars ))
  {
    // check if this section contains this token
    if(GetSectionTokenValid( curSection, token ))
    {
      // check if it is best match
      if(matchChars >= bestMatch)       // section at the bottom of file will always win
      {
        bestMatch = matchChars;
        bestSkip = curSkip;
      }
    }

    curSkip++;
  }

  // return best match
  if(bestSkip != -1)
  {
    return GetSectionMatch( section, bestSkip, matchChars );
  }

  return NULL;
}

inline void lConfigFile::GetSectionContents(const char *section,
                                             lArray<lString> &names,
                                             lArray<lString> &values)
{
  Section *sec;

  if ( FindSection( section, &sec ) )
  {
    for (int i = 0; i < sec->tokenList.Size(); i++)
    {
      Token &token = sec->tokenList(i);

      names.Push( token.name.Data() );
      values.Push( token.value.Data() );
    }
  }
}


inline const char * lConfigFile::GetNextSection( const char * section )
{
  if( section == NULL )
  {
    if( _sectionList.Size() > 0 ) return _sectionList(0).name.Data();
    else return NULL;
  }

  for(int i=0;i<_sectionList.Size();i++)
  {
    if( strcmpi( _sectionList(i).name.Data(), section ) == 0 )
    {
      if( i<_sectionList.Size()-1 ) return _sectionList(i+1).name.Data();
      return NULL;
    }
  }
  return NULL;
}

inline const char * lConfigFile::GetNextToken( const char * section, const char * token )
{
  if( section == NULL ) return NULL;
  Section * s;
  if( !FindSection(section,&s) ) return NULL;
  if( token == NULL )
  {
    if( s->tokenList.Size() > 0 ) return s->tokenList(0).name.Data();
    else return NULL;;
  }

  for(int i=0;i<s->tokenList.Size();i++ )
  {
    if( strcmpi( s->tokenList(i).name.Data(), token ) == 0 )
    {
      if( i<s->tokenList.Size()-1 ) return s->tokenList(i+1).name.Data();
      return NULL;
    }
  }

  return NULL;
}


inline bool lConfigFile::Section::Find( const char *str, Token **tok )
{
  for(int i=0;i<tokenList.Size();i++)
  {
    if( strcmpi( tokenList(i).name.Data(), str ) == 0 )
    {
      (*tok) = &tokenList(i);
      return true;
    }
  }

  return false;
}


inline void lConfigFile::WriteString( lIOStreamSeekable * stream, const char *str )
{
  if( stream == NULL ) return;
  stream->Write( (void *)str, strlen(str) );
}

inline bool lConfigFile::FindSection( const char *str, Section **sec )
{
  for(int i=0;i<_sectionList.Size();i++)
  {
    if( strcmpi( _sectionList(i).name.Data(), str ) == 0 )
    {
      (*sec) = &_sectionList(i);
      return true;
    }
  }

  return false;
}

inline lConfigFile::Token * lConfigFile::FindToken( const char * section, const char * token )
{
  Section *s;
  if( !FindSection( section, &s ) ) return NULL;

  Token *t;
  if( !s->Find(token, &t) ) return NULL;

  return t;
}

inline void lConfigFile::SetToken( const char * section, const char * token, const char * value )
{
  Section *s;

  if( !FindSection( section, &s ) )
  {
    int index = _sectionList.Add();
    s = &_sectionList(index);
    memset(s,0,sizeof(Section));

    s->tokenList.Empty();
    s->name.Empty();
    s->name.Add( strlen(section) + 1 );
    strcpy( s->name.Data(), section );
  }

  Token *t;

  if( !s->Find(token, &t) )
  {
    int index = s->tokenList.Add();
    t = & s->tokenList(index);
    memset(t,0,sizeof(Token));

    t->name.Empty();
    t->name.Add(strlen(token)+1);
    strcpy( t->name.Data(), token );
  }

  t->value.Empty();
  t->value.Add(strlen(value)+1);
  strcpy( t->value.Data(), value );
}

END_L_NAMESPACE

#endif
