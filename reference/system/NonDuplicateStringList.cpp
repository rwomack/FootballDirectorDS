

 /*
 
	C++ Class - CNonDuplicateStringList


	(c) Rw Software 1994 - 2008

  	---------------------------------------------------------------
 */

#include "stdafx.h"



// -----------------------------------------------------------------------
int CNonDuplicateStringList::Add(const CString& _Str)
{
	return m_strList.Add(_Str);
}


/*------------------------------------------------------------------------------
	Method:   	CNonDuplicateStringList::GetString
	Access:    	public 
	Parameter: 	const ushort _ID
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CNonDuplicateStringList::GetString(const ushort _ID)
{
	EXT_ASSERT(_ID < m_strList.GetSize());
	return m_strList[_ID];
}


/*------------------------------------------------------------------------------
	Method:   	CNonDuplicateStringList::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Parameter: 	bool _bObfuscate
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNonDuplicateStringList::Read(CDataFile* _pDataFile, bool _bObfuscate /*= false*/)
{
	_pDataFile->Read(&m_nOrginalListSize, sizeof m_nOrginalListSize);
	CExtString strTemp;
	for (int ListPos = 0; ListPos < m_nOrginalListSize; ListPos ++)
	{
		strTemp.Read(_pDataFile);
		if (_bObfuscate == true)
		{
#ifdef _DEBUG
//			afxDump << strTemp << " ";
#endif
			strTemp.DoObfuscate();
#ifdef _DEBUG
//			afxDump << strTemp << "\n";
#endif
		}
		if (ListPos < m_strList.GetSize())
		{
			m_strList[ListPos] = strTemp;
		}
		else
		{
			m_strList.Add(strTemp);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CNonDuplicateStringList::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader & _Resource
	Parameter: 	bool _Obfuscate
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNonDuplicateStringList::DoInitialiseFromResource(CBinaryReader& _Resource, bool _Obfuscate /*= false*/)
{
	m_nOrginalListSize = _Resource.ReadUInt16();
	CExtString strTemp;
	for (int ListPos = 0; ListPos < m_nOrginalListSize; ListPos ++)
	{
		strTemp.ReadFromResource(_Resource);
		EXT_ASSERT(strTemp.IsEmpty() == false);
		if (_Obfuscate == true)
		{
#ifdef _DEBUG
//			afxDump << strTemp << " ";
#endif
			strTemp.DoObfuscate();
#ifdef _DEBUG
//			afxDump << strTemp << "\n";
#endif
		}
		if (ListPos < m_strList.GetSize())
		{
			m_strList[ListPos] = strTemp;
		}
		else
		{
			m_strList.Add(strTemp);
		}
	}
}


 // Find the ID of the string, if not found then adds the string to the list
 // -----------------------------------------------------------------------
ushort CNonDuplicateStringList::DoFindStringID(const CString& _NewName)
{
	ushort iFound = wNOTFOUND;
	for (int ListPos = 0; ListPos < m_strList.GetSize(); ListPos ++)
	{
		if (m_strList[ListPos] == _NewName)
		{
			iFound = ListPos;
			break;
		}
	}
	if (iFound == wNOTFOUND)
	{
		m_strList.Add(_NewName);
		iFound = m_strList.GetUpperBound();
	}
	return iFound;
}


/*------------------------------------------------------------------------------
	Method:   	CNonDuplicateStringList::ReadNew
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNonDuplicateStringList::ReadNew(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CExtString strTemp;
	ushort  nHowMany;
	_pDataFile->Read(&nHowMany, sizeof nHowMany);
	for (int LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
		strTemp.Read(_pDataFile);
		m_strList.Add(static_cast<CString>(strTemp));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CNonDuplicateStringList::Write
	Access:    	public 
	Parameter: 	CDataFile * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNonDuplicateStringList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	DoWriteStringsToFile(_pDataFile, 0, m_strList.GetSize());
}


/*------------------------------------------------------------------------------
	Method:   	CNonDuplicateStringList::WriteNew
	Access:    	public 
	Parameter: 	CDataFile * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNonDuplicateStringList::WriteNew(CDataFile* _pFile)
{
	DoWriteStringsToFile(_pFile, m_nOrginalListSize, m_strList.GetSize() - m_nOrginalListSize);
}


/*------------------------------------------------------------------------------
	Method:   	CNonDuplicateStringList::DoWriteStringsToFile
	Access:    	public 
	Parameter: 	CDataFile * _pFile
	Parameter: 	ushort _nFirstString
	Parameter: 	ushort _HowMany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNonDuplicateStringList::DoWriteStringsToFile(CDataFile* _pDataFile, ushort _nFirstString, ushort _HowMany)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CExtString strTemp;
	_pDataFile->Write(&_HowMany, sizeof _HowMany);
	for (int LoopCount = _nFirstString; LoopCount < _nFirstString + _HowMany; LoopCount++)
	{
		strTemp = GetString(LoopCount);
		strTemp.Write(_pDataFile);
	}
}
