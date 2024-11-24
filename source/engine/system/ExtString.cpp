

 /*

    Program:- String.cpp

    The Extended CString class routines


	(c) Rw Software 1994 - 2007

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

char CString::zero_buffer[5];

void CString::LoadString( int strID )
{
  //lLogFmt("CString::LoadString with ID %d\n", strID);
  LoadText( *this, strID );
  //lLogFmt("= %s\n", m_string);
}

void CString::Format( const char * fmt, ... )
{
  lAssert( fmt != NULL );
//  OS_Printf("CString::Format with fmt %s\n", fmt);

  char buf[2048];
  va_list args;
  va_start( args, fmt );
  lPlatform_vsnprintf( buf, sizeof(buf)-1, fmt, args );
  buf[sizeof(buf)-1] = '\0';
  va_end( args );

  destroy();
  int sz = (int)strlen( buf );
  create( sz );
  strcpy( m_string, buf );
}


void CString::Format( int strID, ... )
{
  CString fmt;
  LoadText( fmt, strID );
//  OS_Printf("CString::Format(strID) with fmt %s\n", fmt);

  char buf[2048];
  va_list args;
  va_start( args, strID );
  lPlatform_vsnprintf( buf, sizeof(buf)-1, fmt, args );
  buf[sizeof(buf)-1] = '\0';
  va_end( args );

  destroy();
  int sz = (int)strlen( buf );
  create( sz );
  strcpy( m_string, buf );
}

/*------------------------------------------------------------------------------
	Method:   	CExtString::CExtString
	Access:    	public
	Parameter: 	CExtString* strText
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CExtString::CExtString(CExtString* strText)
	: CString(*strText)
{
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::CExtString
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CExtString::CExtString()
{
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::~CExtString
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CExtString::~CExtString()
{
}

/*------------------------------------------------------------------------------
	Method:   	CExtString::operator=
	Access:    	public
	Parameter: 	const CString& stringSrc
	Returns:   	const CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
const CString& CExtString::operator = (const CString& stringSrc)
{
	return CString::operator = (stringSrc);
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::operator=
	Access:    	public
	Parameter: 	const CExtString& stringSrc
	Returns:   	const CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
const CString& CExtString::operator = (const CExtString& stringSrc)
{
	return CString::operator = (stringSrc);
}


///*------------------------------------------------------------------------------
//  Method:     CExtString::operator=
//  Access:     public
//  Parameter:  const WCHAR* stringSrc
//  Returns:    const CString&
//  Qualifier:
//  Purpose:
//------------------------------------------------------------------------------*/
//const CString& CExtString::operator = (const WCHAR* stringSrc)
//{
//  return CString::operator = (stringSrc);
//}


/*------------------------------------------------------------------------------
	Method:   	CExtString::Get
	Access:    	public
  Parameter:  CDataFile* File
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::Get(CDataFile* File)
{
  lAssert(0);                           // function is broken?
//    FDASSERT(false);  //File != null);
//    LPTSTR junk = GetBuffer(2048);          // pointer to the CString
//    //@@fgets(junk, 2048, File);
//    ReleaseBuffer();
//    StripTrailingNewLine();
//    TrimRight();
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::Get
	Access:    	public
  Parameter:  CDataFile* File
	Parameter: 	const ushort nCharsToRead
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::Get(CDataFile* File, const ushort nCharsToRead)
{
  FDASSERT (File != null);
  //CString::Empty();
//TODO#ifdef UNICODE
//TODO  char* pAnsiString = FD_ENGINE_NEW char[nCharsToRead + 1];
//TODO  File->Read(pAnsiString, 1* nCharsToRead);    // Read until end of line char found
//TODO    LPTSTR junk = CString::GetBufferSetLength(nCharsToRead);  // pointer to the CString
//TODO  mbstowcs(junk, pAnsiString, nCharsToRead);
//TODO#else
   destroy();
   create(nCharsToRead);
    //LPTSTR junk = GetBufferSetLength(nCharsToRead);         // pointer to the CString
   File->Read(m_string, nCharsToRead);
   m_string[nCharsToRead] = '\0';

//TODO#endif
    //CString::ReleaseBuffer();
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::Read
	Access:    	public
  Parameter:  CDataFile* file
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::Read(CDataFile* File)
{
  EXT_ASSERT(File != null);//, "null file handle");
	byte nLength;
  File->Read(&nLength, sizeof(nLength));
	if (nLength > 0)
	{
    Get(File, nLength);
  }
	else
	{
		CString::Empty();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::ReadFromResource
	Access:    	public
	Parameter: 	CBinaryResource& aResource
	Parameter: 	const ushort _CharsToRead
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::ReadFromResource(CBinaryReader& _Resource, const ushort _CharsToRead /*= 0*/)
{
	byte nLength = _CharsToRead;
	if (_CharsToRead == 0)
	{
		nLength = _Resource.ReadByte();
	}
	CString::Empty();
	if (nLength > 0)
	{
//TODO#ifdef UNICODE
//TODO    char* pAnsiString = FD_ENGINE_NEW char[nLength + 1];
//TODO    _Resource.ReadBytes((byte*)pAnsiString, nLength);
//TODO    LPTSTR junk = CString::GetBufferSetLength(nLength);  // pointer to the CString
//TODO    mbstowcs(junk, pAnsiString, nLength);
//TODO    delete pAnsiString;
//TODO#else
   destroy();
   create(nLength);
//    LPTSTR junk = GetBufferSetLength(nLength);          // pointer to the CString
    _Resource.ReadBytes((byte*)m_string, nLength);
    m_string[nLength] = '\0';
//TODO#endif
//    CString::ReleaseBuffer();
  }
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::Write
	Access:    	public
  Parameter:  CDataFile* file
	Parameter: 	bool bWriteSize
	Returns:   	void
	Qualifier:
	Purpose:	Write a string value to an ascii file
------------------------------------------------------------------------------*/
void CExtString::Write(CDataFile* File, bool _bWriteSize /*= true*/)
{
  FDASSERT(File != null);
  byte nLength = CString::Size();
  FDASSERT( nLength < 256 );

  if (_bWriteSize == true)
  {
    File->Write(&nLength, sizeof(nLength));
	}

	if (nLength > 0 )
	{
//todo  #ifndef UNICODE
    //lLogFmt(" Writing %d: %s\n", nLength, m_string);
    File->Write(m_string, nLength);
//todo  #else
//todo    char* pAnsiString = FD_ENGINE_NEW char[nLength + 1];
//todo    wcstombs(pAnsiString, *this, nLength + 1);
//todo        File->Write(pAnsiString, nLength);
//todo    delete pAnsiString;
//todo  #endif
	}
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::DoObfuscate
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::DoObfuscate()
{
	bool bObfuscated = false;
	ushort nCurrChar;
  for (nCurrChar = CString::Size() - 1; nCurrChar > 0; nCurrChar--)
	{
		if (CString::Mid(nCurrChar, 1) == TEXT("a"))
		{
      m_string[nCurrChar] = 'o';
			bObfuscated = true;
			break;
		}
		else if (CString::Mid(nCurrChar, 1) == TEXT("e"))
		{
      m_string[nCurrChar] = 'i';
			bObfuscated = true;
			break;
		}
		else if (CString::Mid(nCurrChar, 1) == TEXT("i"))
		{
      m_string[nCurrChar] = 'e';
			bObfuscated = true;
			break;
		}
		else if (CString::Mid(nCurrChar, 1) == TEXT("o"))
		{
      m_string[nCurrChar] = 'a';
			bObfuscated = true;
			break;
		}
		else if (CString::Mid(nCurrChar, 1) == TEXT("u"))
		{
      m_string[nCurrChar] = 'a';
			bObfuscated = true;
			break;
		}
	}
	if (bObfuscated == false)
	{
    nCurrChar = CString::Size() - 1;
    m_string[nCurrChar] = 'i';
	}
//  CString::Delete(nCurrChar + 1, 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CStringExtArray::Read
	Access:    	public
  Parameter:  CDataFile* File
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CStringExtArray::Read(CDataFile* File)
{
  lAssert(0);
  // NOTE: This is messed up
  //FDASSERT (File != null);
  //CString strElement;
  //int HowMany;
  //File->Read(&HowMany, sizeof(HowMany));
  //FDASSERT (HowMany > -1);
  //
  //for (ushort nListPos = 0; nListPos < HowMany; nListPos ++)
  //{
  //  File->Read(&strElement, sizeof(strElement));
  //  SetAtGrow(nListPos, strElement);
  //}
}


/*------------------------------------------------------------------------------
	Method:   	CStringExtArray::Write
	Access:    	public
  Parameter:  CDataFile* File
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CStringExtArray::Write(CDataFile* File)
{
  lAssert(0);
  //NOTE: This is messed up
  //EXT_ASSERT(File != null);//, "null file handle");
  //int ArrCount;
  //CString ArrVar;
  //ArrCount = GetSize();
  //File->Write(&ArrCount, sizeof(ArrCount));
  //
  //for (ArrCount = 0; ArrCount < GetSize(); ArrCount ++)
  //{
  //  ArrVar = GetAt(ArrCount);
  //      File->Write(&ArrVar, sizeof(ArrVar));
  //}
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::StripTrailingNewLine
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::StripTrailingNewLine()
{
	// Get the index of the last character in the string
  int StringPos = Size() - 1;
  FDASSERT (StringPos > -1);

	// Move back towards string start until start or nonspace found
  while(StringPos > -1 && m_string[StringPos] == '\n')
	{
		StringPos--;
	}

   // Truncate string at last position plus one
  FDASSERT (StringPos > -2);    // Can't insert before start of string
  m_string[StringPos + 1] = '\0';
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::FormatLargeNumber
	Access:    	public
	Parameter: 	const int nValue
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::FormatLargeNumber(const int nValue)
{
  lLogFmt("%s\n", *this );
	CString::Format(_T("%d"), nValue);
	CString::MakeReverse();
	int nCharsSinceComma = 0;
  for (int Count = 0; Count < CString::Size(); Count ++)
	{
		if (nCharsSinceComma == 3)
		{
			nCharsSinceComma = 1;
      CString::Insert(Count);
      m_string[Count] = ',';
			Count++;
		}
		else
		{
			nCharsSinceComma++;
		}
	}
	CString::MakeReverse();
	lLogFmt("%s\n\n", *this );
}


///*------------------------------------------------------------------------------
//  Method:     CExtString::OutputDebug
//  Access:     public
//  Parameter:  _In_ _Printf_format_string_ PCXSTR pszFormat
//  Parameter:  ...
//  Returns:    void
//  Qualifier:
//  Purpose:
//------------------------------------------------------------------------------*/
//void CExtString::OutputDebug(_In_ _Printf_format_string_ PCXSTR pszFormat, ...)
//{
//  va_list ap;
//  va_start (ap, pszFormat);
//  CString::Format(pszFormat, ap);
//  va_end (ap);
//  OutputDebugString(GetBuffer());
//  OutputDebugString(_T("\n"));
//}


/*------------------------------------------------------------------------------
	Method:   	CExtString::OutputDebug
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::OutputDebug()
{
  lLogFmt("DEBUG: %s\n", m_string);
  //OutputDebugString(GetBuffer());
  //OutputDebugString(_T("\n"));
}
/////////////////////////////////////////////////////////////////////////////////////////////////


