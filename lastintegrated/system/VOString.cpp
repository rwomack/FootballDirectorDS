//-------------------------------------------------------------------
// VOString implementation file
//-------------------------------------------------------------------
// 
// Copyright ©2000- 02 Virtual Office Systems Incorporated
// All Rights Reserved                      
//
// This code may be used in compiled form in any way you desire. This
// file may be redistributed unmodified by any means PROVIDING it is 
// not sold for profit without the authors written consent, and 
// providing that this notice and the authors name is included.
//
// This code can be compiled, modified and distributed freely, providing
// that this copyright information remains intact in the distribution.
//
// This code may be compiled in original or modified form in any private 
// or commercial application.
//
// This file is provided "as is" with no expressed or implied warranty.
// The author accepts no liability for any damage, in any form, caused
// by this code. Use it at your own risk.
//-------------------------------------------------------------------

#include "stdafx.h"
#include <atlbase.h>
#include <tchar.h>
#include "VOString.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVOString::CVOString(LPCTSTR pcszValue)
{
	m_pBuffer = NULL;
	m_dwBufferSize = 0;

	*this = pcszValue;
}

// Copy Constructor
CVOString::CVOString(const CVOString &rSrc)
{
	m_pBuffer = NULL;
	m_dwBufferSize = 0;

	*this = rSrc.m_pBuffer;
}

CVOString::~CVOString()
{
	if(m_pBuffer)
		delete m_pBuffer;
}

void CVOString::operator = (const CVOString& rSrc)
{
	*this = rSrc.m_pBuffer;
}

void CVOString::operator = (LPCTSTR pcszValue)
{
	if(pcszValue == NULL)
	{
		m_dwLength = 0;
		m_pBuffer = NULL;
	}
	else
	{
		m_dwLength = _tcslen(pcszValue);
		SetMinBufferSize(m_dwLength);
		_tcscpy(m_pBuffer, pcszValue);
	}
}

#ifndef _UNICODE
void CVOString::operator =(LPCWSTR pcwszValue)
{
	if(!pcwszValue)
	{
		m_dwLength = 0;
		m_pBuffer = NULL;
		return;
	}

	if(sizeof(TCHAR) == sizeof(WCHAR))
	{
		m_dwLength = wcslen(pcwszValue);
		SetMinBufferSize(m_dwLength);
		wcscpy((LPWSTR)m_pBuffer, pcwszValue);
	}
	else
	{
		int		nChars = wcslen(pcwszValue) + 1;
		LPTSTR	pszBuffer = new TCHAR[nChars];

		for(int n = 0; n < nChars; n++)
			pszBuffer[n] = (TCHAR) pcwszValue[n];

		m_dwLength = _tcslen(pszBuffer);
		SetMinBufferSize(m_dwLength);
		_tcscpy(m_pBuffer, pszBuffer);

		delete pszBuffer;
	}
}
#endif

BOOL CVOString::operator == (LPCTSTR pcszValue) const
{
	if(!m_pBuffer)
	{
		if(!pcszValue)
			return TRUE;

		return (_tcsicmp(pcszValue, TEXT("")) == 0);
	}

	return(_tcsicmp(pcszValue, m_pBuffer) == 0);
}

BOOL CVOString::operator == (const CVOString& rCompare) const
{
	if(!m_pBuffer)
	{
		if(rCompare.GetLength() == 0)
			return TRUE;
		else
			return FALSE;
	}

	if(!rCompare.m_pBuffer)
		return FALSE;

	return(_tcsicmp(rCompare.m_pBuffer, m_pBuffer) == 0);
}

LPCTSTR CVOString::operator += (LPCTSTR pcszAppend)
{
	if(m_pBuffer)
	{
		SetMinBufferSize(GetLength() + _tcslen(pcszAppend));
		_tcscat(m_pBuffer, pcszAppend);
		m_dwLength = _tcslen(m_pBuffer);
	}
	else
		*this = pcszAppend;

	return *this;
}

LPCTSTR CVOString::operator += (TCHAR chAppend)
{
	SetMinBufferSize(m_dwLength + 2);
	m_dwLength++;
	m_pBuffer[m_dwLength] = 0;
	m_pBuffer[m_dwLength - 1] = chAppend;

	return *this;
}

CVOString CVOString::operator + (LPCTSTR pcszAppend)
{
	CVOString strReturn(*this);
	
	strReturn += pcszAppend;

	return strReturn;
}

BOOL CVOString::SetMinBufferSize(DWORD dwChars)
{
#ifdef _DEBUG
	if(dwChars > 65536)
		DebugBreak();	// This is just a precaution to protect against memory leaks.  Large strings are fine.
#endif
	if(m_dwBufferSize < dwChars + 1)
	{
		TCHAR*	pNewBuffer;
		DWORD	dwNewBufferSize = dwChars + 16;

		pNewBuffer = new TCHAR[dwNewBufferSize];

		memset(pNewBuffer, 0, sizeof(TCHAR) * dwNewBufferSize);

		if(m_pBuffer)
		{
			memmove(pNewBuffer, m_pBuffer, m_dwBufferSize * sizeof(TCHAR) );
			delete m_pBuffer;
		}

		m_pBuffer = pNewBuffer;
		m_dwBufferSize = dwNewBufferSize;
	}

	return TRUE;
}

TCHAR CVOString::GetAt(DWORD dwOffset) const
{
	if(dwOffset > m_dwLength - 1)
		return (TCHAR) 0;

	return m_pBuffer[dwOffset];
}

CVOString CVOString::Left(DWORD dwCount) const
{
	if(dwCount >= m_dwLength)
		return CVOString(*this);

	LPTSTR	pszTemp = new TCHAR[dwCount + 1];

	memset(pszTemp, 0, sizeof(TCHAR) * (dwCount + 1));
	memmove(pszTemp, m_pBuffer, sizeof(TCHAR) * dwCount);

	CVOString strValue(pszTemp);

	delete pszTemp;

	return strValue;
}

CVOString CVOString::Mid(DWORD dwOffset, int nLength) const
{
	if(dwOffset < 0 || dwOffset > m_dwLength)
		return CVOString(TEXT(""));

	if(nLength == -1)
		nLength = m_dwLength - dwOffset;
	else if(dwOffset + nLength > m_dwLength)
		nLength = m_dwLength - dwOffset;

	LPTSTR	pszTemp = new TCHAR[nLength + 1];

	memset(pszTemp, 0, sizeof(TCHAR) * (nLength + 1));
	memmove(pszTemp, m_pBuffer + dwOffset, sizeof(TCHAR) * nLength);

	CVOString strValue(pszTemp);

	delete pszTemp;

	return strValue;
}

LPTSTR CVOString::GetBuffer(DWORD dwMinSize)
{
	SetMinBufferSize(dwMinSize + 1);

	return m_pBuffer;
}

int CVOString::Format(LPCTSTR pcszFormat, ...)
{
    va_list vl;

    va_start( vl, pcszFormat);

	wvsprintf(GetBuffer(2048), pcszFormat, vl);
	ReleaseBuffer();

	va_end(vl);

	return GetLength();
}

int CVOString::Find(LPCTSTR pcszValue, int nStartingOffset) const
{
	LPTSTR pszSubstring;
	
	if(nStartingOffset < 0)
		nStartingOffset = 0;

	pszSubstring = _tcsstr(m_pBuffer + nStartingOffset, pcszValue);

	if(!pszSubstring)
		return -1;

	return (pszSubstring - m_pBuffer);
}

int CVOString::Find(TCHAR chValue, int nStartingOffset) const
{
	TCHAR pcszValue[2];

	pcszValue[0] = chValue;
	pcszValue[1] = 0;

	return Find(pcszValue, nStartingOffset);
}

int CVOString::ReverseFind(LPCTSTR pcszSubstring) const
{
	int nOffset = -1, nNextOffset = 0;

	while((nNextOffset = Find(pcszSubstring, nNextOffset)) != -1)
		nOffset = nNextOffset++;

	return nOffset;
}

DWORD CVOString::ReleaseBuffer(int nChars)
{
	if((m_pBuffer == NULL) || (m_dwBufferSize == 0))
		return 0;

	if(nChars == -1)
		m_dwLength = _tcslen(m_pBuffer);
	else
	{
		m_dwLength = (DWORD) nChars;

		if(m_dwLength > m_dwBufferSize)
			m_dwLength = m_dwBufferSize;

		m_pBuffer[m_dwLength] = 0;
	}

	return m_dwLength;
}

const CVOString& CVOString::TrimRight()
{
	while(GetLength() && GetAt(GetLength() - 1) == TCHAR(' '))
		*this = Left(GetLength() - 1);

	return *this;
}

const CVOString& CVOString::TrimLeft()
{
	while(GetLength() && GetAt(0) == TCHAR(' '))
		*this = Mid(1);

	return *this;
}

CVOString CVOString::Right(int nChars)
{
	if((int)GetLength() < nChars)
		return *this;

	return Mid(GetLength() - nChars);
}

void CVOString::MakeUpper()
{
	for(int i = 0; i < (int)GetLength(); i++)
	{
		if(m_pBuffer[i] >= TCHAR('a') && m_pBuffer[i] <= TCHAR('z'))
			m_pBuffer[i] = m_pBuffer[i] - (TCHAR('a') - TCHAR('A'));
	}
}

int CVOString::CompareNoCase(LPCTSTR pcszValue)
{
	if(!m_pBuffer && !pcszValue)
		return 0;

	if(!pcszValue)
		return 1;

	if(!m_pBuffer)
		return -1;

	return _tcsicmp(m_pBuffer, pcszValue);
}

int CVOString::FindOneOf(LPCTSTR pcszDelims)
{
	if(m_pBuffer == NULL || m_dwBufferSize == 0)
		return -1;

	if(!pcszDelims)
		return -1;

	int nDelim = _tcscspn((LPCTSTR)m_pBuffer, pcszDelims);

	if(nDelim == (int)GetLength())
		return -1;

	return nDelim;
}

DWORD CVOString::Replace(LPCTSTR pcszSearchFor, LPCTSTR pcszReplaceWith)
{
	CVOString	strSearchFor(pcszSearchFor);
	CVOString	strReplaceWith(pcszReplaceWith);
	CVOString	strRemain;

	if(strReplaceWith.Find(strSearchFor) != -1)
		return 0;	// Would be infinite loop

	DWORD	dwCount = 0;
	int		nMatch = -1;

	while((nMatch = Find(strSearchFor, nMatch + 1)) != -1)
	{
		strRemain = Mid(nMatch + strSearchFor.GetLength());
		*this = Left(nMatch);
		*this += strReplaceWith;
		*this += strRemain;
		dwCount++;
	}

	return dwCount;
}

#define CH_QUOTE			TCHAR('\"')
#define SZ_QUOTE			TEXT("\"")
#define SZ_QUOTE_ENCODED	TEXT("%22")

CVOString CVOString::AddQuotes(LPCTSTR pcszValue)
{
	CVOString strValue(pcszValue);

	strValue.Replace(SZ_QUOTE, SZ_QUOTE_ENCODED);

	CVOString strNewValue;
	
	strNewValue += CH_QUOTE;
	strNewValue += strValue;
	strNewValue += CH_QUOTE;

	return strNewValue;
}

CVOString CVOString::RemoveQuotes(LPCTSTR pcszValue)
{
	CVOString strValue(pcszValue);

	if(strValue.GetLength() < 2)
		return strValue;

	if(	(strValue.GetAt(0) == CH_QUOTE) && 
		(strValue.GetAt(strValue.GetLength() - 1) == CH_QUOTE) )
		strValue = strValue.Mid(1, strValue.GetLength() - 2);

	strValue.Replace(SZ_QUOTE_ENCODED, SZ_QUOTE);

	return strValue;
}

CVOString CVOString::DescribeErrorMessage(DWORD dwError)
{
    LPVOID lpMsgBuf;
    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,    
                  NULL, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                  (LPTSTR) &lpMsgBuf, 0, NULL);
	CVOString strDescription = (LPCTSTR) lpMsgBuf;
    LocalFree(lpMsgBuf);

	return strDescription;
}

BOOL CVOString::ToSystemTime(SYSTEMTIME &st, DWORD dwFlags)
{
	USES_CONVERSION;

	DATE	dt;
	SCODE	sc;

	if (FAILED(sc = VarDateFromStr((LPOLESTR)T2COLE(m_pBuffer), LANG_USER_DEFAULT, dwFlags, &dt)))
	{
		if (sc == DISP_E_TYPEMISMATCH)
		{
			OutputDebugString(TEXT("CVOString::ToSystemTime() Failed - Type Mismatch\n"));
			
			return FALSE;
		}
		else if (sc == DISP_E_OVERFLOW)
		{
			OutputDebugString(TEXT("CVOString::ToSystemTime() Failed - Overflow\n"));

			return FALSE;
		}
		else
		{
			OutputDebugString(TEXT("CVOString::ToSystemTime() Failed\n"));

			return FALSE;
		}
	}

	VariantTimeToSystemTime(dt, &st);

	return TRUE;
}

BOOL CVOString::ParseSystemTime(SYSTEMTIME &st, DWORD dwFlags)
{
	USES_CONVERSION;

	DATE dt;

	SystemTimeToVariantTime(&st, &dt);

	CComBSTR bstrDate;

	if(VarBstrFromDate(dt, LANG_USER_DEFAULT, dwFlags, &bstrDate) != S_OK)
		return FALSE;

	*this = bstrDate;

	return TRUE;
}

CVOString CVOString::EncodeHex(LPCTSTR pcszValue)
{
	if(!pcszValue)
		return CVOString(TEXT(""));

	static TCHAR hexChar[] = {	TCHAR('0'), TCHAR('1'), TCHAR('2'), TCHAR('3'),
								TCHAR('4'), TCHAR('5'), TCHAR('6'), TCHAR('7'),
								TCHAR('8'), TCHAR('9'), TCHAR('A'), TCHAR('B'),
								TCHAR('C'), TCHAR('D'), TCHAR('E'), TCHAR('F') };
	CVOString strValue;

	LPCTSTR pcszCur = pcszValue;

	unsigned char chCur;

	while(*pcszCur)
	{
		chCur = (unsigned char)(*pcszCur);
		strValue += hexChar[chCur >> 4];
		strValue += hexChar[chCur & 0xF];
		pcszCur++;
	}

	return strValue;
}

CVOString CVOString::DecodeHex(LPCTSTR pcszValue)
{
	CVOString strValue(pcszValue);
	CVOString strDecoded;

	strValue.MakeUpper();

	if(strValue.GetLength() % 2)	// Uneven hex pairs
		return CVOString(TEXT(""));

	unsigned char	ch;
	TCHAR			nibHigh, nibLow;

	for(int nChar = 0; nChar < (int)strValue.GetLength() / 2; nChar++)
	{
		ch = 0;

		nibHigh = strValue.GetAt(nChar * 2);
		nibLow = strValue.GetAt(nChar * 2 + 1);

		switch(nibHigh)
		{
		case TCHAR('0'):
		case TCHAR('1'):
		case TCHAR('2'):
		case TCHAR('3'):
		case TCHAR('4'):
		case TCHAR('5'):
		case TCHAR('6'):
		case TCHAR('7'):
		case TCHAR('8'):
		case TCHAR('9'):
			ch += (unsigned char)(nibHigh - TCHAR('0'));
			break;

		case TCHAR('A'):
		case TCHAR('B'):
		case TCHAR('C'):
		case TCHAR('D'):
		case TCHAR('E'):
		case TCHAR('F'):
			ch += (unsigned char)(nibHigh - TCHAR('A') + 10);
			break;
		}

		ch = ch << 4;

		switch(nibLow)
		{
		case TCHAR('0'):
		case TCHAR('1'):
		case TCHAR('2'):
		case TCHAR('3'):
		case TCHAR('4'):
		case TCHAR('5'):
		case TCHAR('6'):
		case TCHAR('7'):
		case TCHAR('8'):
		case TCHAR('9'):
			ch += (unsigned char)(nibLow - TCHAR('0'));
			break;

		case TCHAR('A'):
		case TCHAR('B'):
		case TCHAR('C'):
		case TCHAR('D'):
		case TCHAR('E'):
		case TCHAR('F'):
			ch += (unsigned char)(nibLow - TCHAR('A') + 10);
			break;
		}

		strDecoded += (TCHAR)ch;
	}

	return strDecoded;
}
