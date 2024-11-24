

 /*

    Program:- String.cpp

    The Extended CString class routines


	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



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

#ifdef WIN32
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
#endif


/*------------------------------------------------------------------------------
	Method:   	CExtString::Get
	Access:    	public 
	Parameter: 	CDataFile * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::Get(CDataFile* _pFile)
{
    EXT_ASSERT(false);	//File != null);
    LPTSTR junk = GetBuffer(2048);					// pointer to the CString
    //@@fgets(junk, 2048, File);
    ReleaseBuffer();
    StripTrailingNewLine();
    TrimRight();
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::Get
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Parameter: 	const ushort nCharsToRead
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::Get(CDataFile* _pDataFile, const ushort nCharsToRead)
{
    EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CString::Empty();
#ifdef UNICODE
	char* pAnsiString = new char[nCharsToRead + 1];
	_pDataFile->Read(pAnsiString, nCharsToRead);		// Read until end of line char found
    LPTSTR junk = CString::GetBufferSetLength(nCharsToRead);  // pointer to the CString
	mbstowcs(junk, pAnsiString, nCharsToRead);
#else
    LPTSTR junk = GetBufferSetLength(nCharsToRead);					// pointer to the CString
    _pFile->Read(junk, nCharsToRead);
#endif
    CString::ReleaseBuffer();
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::Read
	Access:    	public 
	Parameter: 	CDataFile * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	byte nLength;
	_pDataFile->Read(&nLength, sizeof nLength);
	if (nLength > 0)
	{
		Get(_pDataFile, nLength);
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
#ifdef UNICODE
		char* pAnsiString = new char[nLength + 1];
		_Resource.ReadBytes((byte*)pAnsiString, nLength);
		LPTSTR junk = CString::GetBufferSetLength(nLength);  // pointer to the CString
		mbstowcs(junk, pAnsiString, nLength);
		delete pAnsiString;
#else
		LPTSTR junk = GetBufferSetLength(nLength);					// pointer to the CString
		_Resource.ReadBytes((byte*)junk, nLength);
#endif
		CString::ReleaseBuffer();
    }
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::Write
	Access:    	public 
	Parameter: 	FILE* file
	Parameter: 	bool bWriteSize
	Returns:   	void
	Qualifier: 	
	Purpose:	Write a string value to an ascii file
------------------------------------------------------------------------------*/
void CExtString::Write(CDataFile* _pDataFile, bool _bWriteSize /*= true*/)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    byte nLength = CString::GetLength();
	EXT_ASSERT( nLength < 100000 );
	if (_bWriteSize == true)
    {
		_pDataFile->Write(&nLength, sizeof nLength);
	}
	if (nLength > 0 )
	{
	#ifndef UNICODE
	    _pFile->Write(CString::GetBuffer(0), nLength);
	#else
		char* pAnsiString = new char[nLength + 1];
		wcstombs(pAnsiString, *this, nLength + 1);
        _pDataFile->Write(pAnsiString, nLength);
		delete pAnsiString;
	#endif
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
	for (nCurrChar = CString::GetLength() - 1; nCurrChar > 0; nCurrChar--)
	{
		if (CString::Mid(nCurrChar, 1) == TEXT("a"))
		{
			CString::Insert(nCurrChar, TEXT("o"));
			bObfuscated = true;
			break;
		}
		else if (CString::Mid(nCurrChar, 1) == TEXT("e"))
		{
			CString::Insert(nCurrChar, TEXT("i"));
			bObfuscated = true;
			break;
		}
		else if (CString::Mid(nCurrChar, 1) == TEXT("i"))
		{
			CString::Insert(nCurrChar, TEXT("e"));
			bObfuscated = true;
			break;
		}
		else if (CString::Mid(nCurrChar, 1) == TEXT("o"))
		{
			CString::Insert(nCurrChar, TEXT("a"));
			bObfuscated = true;
			break;
		}
		else if (CString::Mid(nCurrChar, 1) == TEXT("u"))
		{
			CString::Insert(nCurrChar, TEXT("a"));
			bObfuscated = true;
			break;
		}
	}
	if (bObfuscated == false)
	{
		nCurrChar = CString::GetLength() - 1;
		CString::Insert(nCurrChar, TEXT("i"));
	}
	CString::Delete(nCurrChar + 1, 1);
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
	int StringPos = GetLength() - 1;
	EXT_ASSERT(StringPos > -1);
	LPTSTR String = GetBuffer(0);

	// Move back towards string start until start or nonspace found
	while(StringPos > -1 && String[StringPos] == '\n')
	{
		StringPos--;
	}

	// Truncate string at last position plus one
	EXT_ASSERT(StringPos > -2);		// Can't insert before start of string
	String[StringPos + 1] = '\0';
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
	CString::Format(_T("%d"), nValue);
	CString::MakeReverse();
	int nCharsSinceComma = 0;
	for (int LoopCount = 0; LoopCount < CString::GetLength(); LoopCount ++)
	{
		if (nCharsSinceComma == 3)
		{
			nCharsSinceComma = 1;
			CString::Insert(LoopCount, TEXT(","));
			LoopCount++;
		}
		else
		{
			nCharsSinceComma++;
		}
	}
	CString::MakeReverse();
}


/*------------------------------------------------------------------------------
	Method:   	CExtString::OutputDebug
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CExtString::OutputDebug()
{
	OutputDebugString(GetBuffer());
	OutputDebugString(_T("\n"));
}
/////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CStringExtArray::Read
	Access:    	public 
	Parameter: 	CDataFile * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStringExtArray::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CString strElement;
	int HowMany;
    _pDataFile->Read(&HowMany, sizeof HowMany);
	EXT_ASSERT(HowMany > -1);
    for (int LoopCount = 0; LoopCount < HowMany; LoopCount ++)
	{
        _pDataFile->Read(&strElement, sizeof strElement);
		SetAtGrow(LoopCount, strElement);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CStringExtArray::Write
	Access:    	public 
	Parameter: 	CDataFile * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CStringExtArray::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	int ArrCount;
	CString ArrVar;
	ArrCount = GetSize();
    _pDataFile->Write(&ArrCount, sizeof ArrCount);
    for (ArrCount = 0; ArrCount < GetSize(); ArrCount ++)
	{
		ArrVar = GetAt(ArrCount);
        _pDataFile->Write(&ArrVar, sizeof ArrVar);
	}
}
