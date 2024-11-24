

 /*
 
    File:- ExtString.h

    (c) Rw Software 1997 - 2008

    ---------------------------------------------------------------------------------------
*/

#ifndef __EXTCSTRING_H__
#define __EXTCSTRING_H__


class CExtString : public CString
{
public:
	CExtString(CExtString* strText);
	CExtString();
	~CExtString();

	const CString&			operator = (const CString& stringSrc);
    const CString&			operator = (const CExtString& stringSrc);

	void					DoObfuscate();
	void					FormatLargeNumber(const int nValue);
	void					Get(CDataFile* _pFile, const ushort _nCharsToRead);
    void                    Get(CDataFile* _pFile);
	void					OutputDebug();
	void					Read(CDataFile* _pDataFile);
	void					ReadFromResource(CBinaryReader& _Resource, const ushort _CharsToRead = 0);
	void                    StripTrailingNewLine();
	void					Write(CDataFile* _pFile, bool _bWriteSize = true);
};
 /////////////////////////////////////////////////////////////////////////////////////////////////


class CStringExtArray : public CStringArray
{
public:
	void Read(CDataFile* _pDataFile);
	void Write(CDataFile* _pDataFile);
};
#endif
