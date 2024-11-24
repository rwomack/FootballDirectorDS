
 /*

        Class:- DataFile


	(c) Sports Director Ltd 2008 - 

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CDataFile::CDataFile
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDataFile::CDataFile(void)
{
	m_pFile = NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CDataFile::~CDataFile
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDataFile::~CDataFile(void)
{
	EXT_ASSERT(m_pFile == NULL);
}


/*------------------------------------------------------------------------------
	Method:   	CDataFile::Close
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CDataFile::Close()
{
	EXT_ASSERT(m_pFile != NULL);
	fclose(m_pFile);
	m_pFile = NULL;
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CDataFile::OpenForRead
	Access:    	public 
	Parameter: 	CString & _Str
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CDataFile::OpenForRead(CString& _Str)
{
	EXT_ASSERT(m_pFile == NULL);
#ifdef UNICODE
	int nSize = _Str.GetLength();
	char* pAnsiString = new char[nSize + 1];
	wcstombs(pAnsiString, _Str, nSize + 1);
	m_pFile = fopen(pAnsiString, "rb");
	delete pAnsiString;
#else
	FILE* m_pFile = fopen(_Str, "rb");
#endif
	if (m_pFile == NULL)
	{
		Close();
		UserMessageBox(IDS_FILENOTFOUND, MB_OK + MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CDataFile::OpenForWrite
	Access:    	public 
	Parameter: 	CString & _Str
	Parameter: 	const bool _AskOverWrite
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CDataFile::OpenForWrite(CString& _Str, const bool _AskOverWrite /*= true*/)
{
	EXT_ASSERT(m_pFile == NULL);
#ifdef UNICODE
	int nSize = _Str.GetLength();
	char* pAnsiString = new char[nSize + 1];
	wcstombs(pAnsiString, _Str, nSize + 1);

	m_pFile = fopen(pAnsiString, "r");
	if (m_pFile != null)
	{
		if (_AskOverWrite == true && UserMessageBox(IDS_SUREOVERWRITE, MB_ICONEXCLAMATION + MB_YESNO) == IDNO)
		{
			Close();
			delete pAnsiString;
			return false;
		}
		Close();
	}

	m_pFile = fopen(pAnsiString, "wb");
	delete pAnsiString;
#else
	FILE* m_pFile = fopen(_Str, "wb");
#endif
	if (m_pFile == null)
	{
		UserMessageBox(IDS_FILENOTSAVED, MB_OK + MB_ICONEXCLAMATION);
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CDataFile::Read
	Access:    	public 
	Parameter: 	void * _Data
	Parameter: 	uint _Size
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDataFile::Read(void* _Data, uint _Size)
{
	EXT_ASSERT(m_pFile != NULL);
	fread(_Data, _Size, 1, m_pFile);
}


/*------------------------------------------------------------------------------
	Method:   	CDataFile::Write
	Access:    	public 
	Parameter: 	const void * _Data
	Parameter: 	uint _Size
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDataFile::Write(const void* _Data, uint _Size)
{
	EXT_ASSERT(m_pFile != NULL);
	fwrite(_Data, _Size, 1, m_pFile);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CFileID::CheckID
	Access:    	public 
	Parameter: 	CDataFile * _File
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CFileID::CheckID(CDataFile* _File)
{
	bool bRet = true;
#ifdef DEBUG
	byte Marker;
	_File->Read(&Marker, sizeof Marker);
	if (Marker != 199)
	{
		for (int LoopCount = 0; LoopCount < 20; LoopCount++)
		{
			_File->Read(&Marker, sizeof Marker);
			DEBUG_OUTPUT(("Next Value %d,  %d"), (LoopCount, Marker));
			g_DebugStr.OutputDebug();
		}
		bRet = false;
		EXT_ASSERT(false);
		UserMessageBox(IDS_FILENOTVALID, MB_ICONERROR);
		_File->Close();
	}
#endif
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFileID::Write
	Access:    	public 
	Parameter: 	CDataFile * _File
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFileID::Write(CDataFile* _File)
{
#ifdef DEBUG
	byte Marker = 199;
	_File->Write(&Marker, sizeof Marker);
#endif
}
