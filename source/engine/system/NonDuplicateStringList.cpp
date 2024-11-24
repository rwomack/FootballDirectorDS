

 /*

	C++ Class - CNonDuplicateStringList


	(c) Rw Software 1994 - 2007

  	---------------------------------------------------------------
 */

#include "stdafx.h"


// -----------------------------------------------------------------------
int CNonDuplicateStringList::Add(const CString& _Str)
{
	return m_strList.Add(_Str);
}


 // -----------------------------------------------------------------------
CString& CNonDuplicateStringList::GetString(const ushort _ID)
{
  FDASSERT (_ID < m_strList.GetSize());
	return m_strList[_ID];
}


 // -----------------------------------------------------------------------
void CNonDuplicateStringList::Read(CDataFile* File, bool _bObfuscate /*= false*/)
{
  File->Read(&m_nOrginalListSize, sizeof(m_nOrginalListSize));

	CExtString strTemp;
	for (ushort ListPos = 0; ListPos < m_nOrginalListSize; ListPos ++)
	{
    strTemp.Read(File);

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
	for (ushort ListPos = 0; ListPos < m_nOrginalListSize; ListPos ++)
	{
		strTemp.ReadFromResource(_Resource);
    EXT_ASSERT(strTemp.IsEmpty() == false);//, "Empty string found");
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
	for (ushort ListPos = 0; ListPos < m_strList.GetSize(); ListPos ++)
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


// -----------------------------------------------------------------------
void CNonDuplicateStringList::ReadNew(CDataFile* File)
{
	CExtString strTemp;
	ushort  nHowMany;
  File->Read(&nHowMany, sizeof(nHowMany));
	for (ushort LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
    strTemp.Read(File);
		m_strList.Add(static_cast<CString>(strTemp));
	}
}


// -----------------------------------------------------------------------
void CNonDuplicateStringList::Write(CDataFile* File)
{
  //lLogFmt("Write: %d\n", m_strList.GetSize());
  DoWriteStringsToFile(File, 0, m_strList.GetSize());
}


// -----------------------------------------------------------------------
void CNonDuplicateStringList::WriteNew(CDataFile* File)
{
  //lLogFmt("WriteNew: %d - %d = %d\n", m_strList.GetSize(), m_nOrginalListSize, m_strList.GetSize() - m_nOrginalListSize);
  DoWriteStringsToFile(File, m_nOrginalListSize, m_strList.GetSize() - m_nOrginalListSize);
}


// -----------------------------------------------------------------------
void CNonDuplicateStringList::DoWriteStringsToFile(CDataFile* File, ushort _nFirstString, ushort _HowMany)
{
	CExtString strTemp;
  File->Write(&_HowMany, sizeof(_HowMany));
	for (ushort LoopCount = _nFirstString; LoopCount < _nFirstString + _HowMany; LoopCount++)
	{
		strTemp = GetString(LoopCount);
    strTemp.Write(File);
	}
}
