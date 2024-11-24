

 /*

    File:- ExtString.h

    (c) Rw Software 1997 - 2007

    ---------------------------------------------------------------------------------------
*/

#ifndef __EXTCSTRING_H__
#define __EXTCSTRING_H__

#include <binaryresource.h>

 //------------------------------------------------------------------------------------------------
class CExtString : public CString
{
public:
//  CExtString(WCHAR* szStr);
	CExtString(CExtString* strText);
	CExtString();
	~CExtString();

	const CString& operator = (const CString& stringSrc);
    const CString& operator = (const CExtString& stringSrc);
  //  const CString& operator = (const WCHAR* stringSrc);

  void    Get(CDataFile* File, const ushort _nCharsToRead);
#ifdef WINDOWS
    void                    Get(CDataFile* File);
  void    Read(CDataFile* File);
  void    Write(CDataFile* File, bool _bWriteSize = true);
#endif
	void ReadFromResource(CBinaryReader& _Resource, const ushort _CharsToRead = 0);
	void					DoObfuscate();
    void                    StripTrailingNewLine();
	void					FormatLargeNumber(const int nValue);
	void					OutputDebug();
};
 /////////////////////////////////////////////////////////////////////////////////////////////////


 //------------------------------------------------------------------------------------------------
class CStringExtArray : public CStringArray
{
    public:
#ifdef WINDOWS
    void                    Read(CDataFile* File);
  void    Write(CDataFile* File);
#endif
};
#endif
