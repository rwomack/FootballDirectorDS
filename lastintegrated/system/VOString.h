//-------------------------------------------------------------------
// VOString header file
//-------------------------------------------------------------------
// 
// Copyright ©2000-02 Virtual Office Systems Incorporated
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

#if !defined(AFX_VOSTRING_H__91406803_1D87_4DA9_A5A0_499A88AC4E86__INCLUDED_)
#define AFX_VOSTRING_H__91406803_1D87_4DA9_A5A0_499A88AC4E86__INCLUDED_


class CVOString  
{
protected:
	TCHAR* m_pBuffer;

public:
	int FindOneOf(LPCTSTR pcszDelims);
	int CompareNoCase(LPCTSTR pcszValue);
	void MakeUpper();
	CVOString Right(int nChars);
	const CVOString& TrimLeft();
	const CVOString& TrimRight();
	CVOString(const CVOString& rSrc);
	CVOString(LPCTSTR pcszValue = TEXT("") );
	virtual ~CVOString();

	DWORD ReleaseBuffer(int nChars = -1);
	int ReverseFind(LPCTSTR pcszSubstring) const;
	int Find(LPCTSTR pcszValue, int nStartingOffset = 0) const;
	int Find(TCHAR chValue, int nStartingOffset = 0) const;
	LPTSTR GetBuffer(DWORD dwMinSize);
	CVOString Mid(DWORD nOffset, int nLength = -1) const;
	CVOString Left(DWORD dwCount) const;
	TCHAR GetAt(DWORD dwOffset) const;
	operator LPCTSTR() const { return (LPCTSTR)m_pBuffer; }
	DWORD GetLength() const { return m_dwLength; }
	void operator =(LPCTSTR pcszValue);
#ifndef _UNICODE
	void operator =(LPCWSTR pcwszValue);
#endif
	void operator =(const CVOString& rSrc);
	BOOL operator == (LPCTSTR pcszValue) const;
	BOOL operator == (const CVOString& rValue) const;
	BOOL operator != (LPCTSTR pcszValue) const { return operator ==(pcszValue) == 0; }
	BOOL operator != (const CVOString& rValue) const { return operator ==(rValue) == 0; }
	LPCTSTR operator += (LPCTSTR pcszAppend);
	LPCTSTR operator += (TCHAR chAppend);
	CVOString operator + (LPCTSTR pcszAppend);
	DWORD Replace(LPCTSTR pcszSearchFor, LPCTSTR pcszReplaceWith);
	int _cdecl Format(LPCTSTR lpszFormat, ...);

	// Utility functions
public:
	static CVOString DecodeHex(LPCTSTR pcszValue);
	static CVOString EncodeHex(LPCTSTR pcszValue);
	BOOL ParseSystemTime(SYSTEMTIME& st, DWORD dwFlags = 0);
	BOOL ToSystemTime(SYSTEMTIME& st, DWORD dwFlags = 0);
	static CVOString DescribeErrorMessage(DWORD dwError);
	static CVOString AddQuotes(LPCTSTR pcszValue);
	static CVOString RemoveQuotes(LPCTSTR pcszValue);

protected:
	BOOL SetMinBufferSize(DWORD dwChars);
	DWORD m_dwLength;
	DWORD m_dwBufferSize;
};

#endif // !defined(AFX_VOSTRING_H__91406803_1D87_4DA9_A5A0_499A88AC4E86__INCLUDED_)
