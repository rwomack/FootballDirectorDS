
 /*

	TrophyHistory.cpp: implementation of the CTrophyHistory class.

	Football Director


	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#include "stdafx.h"
#include "TrophyHistory.h"


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistory::CTrophyHistory
	Access:    	public 
	Parameter: 	CString Title
	Parameter: 	CString SuppInfo
	Parameter: 	CString Season
	Parameter: 	const byte Gfx
	Parameter: 	const byte Type
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTrophyHistory::CTrophyHistory(CString Title, CString SuppInfo, CString Season, const byte Gfx, const byte Type)
{
	EXT_ASSERT(Title.IsEmpty() == false);
	m_Title = Title;
	EXT_ASSERT(Season.IsEmpty() == false);
	m_Season = Season;
	EXT_ASSERT(SuppInfo.IsEmpty() == false);
	m_SuppInfo = SuppInfo;
	m_Gfx = Gfx;
	m_Type = Type;
}


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistory::~CTrophyHistory
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTrophyHistory::~CTrophyHistory()
{

}


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistory::IsDivision
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTrophyHistory::IsDivision()
{
	bool bRet;
	switch(GetType())
	{
		case DIVISIONWINNER:
        case DIVISIONRUNNERUP:
			bRet = true;
			break;

        case CUPWINNER:
        case CUPRUNNERUP:
			bRet = false;
			break;

		default :
			EXT_ASSERT(false);		// Unhandled type
	}
	return  bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistory::IsWinner
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTrophyHistory::IsWinner()
{
	bool bRet;
	switch(GetType())
	{
		case DIVISIONWINNER:
        case CUPWINNER:
			bRet = true;
			break;

        case DIVISIONRUNNERUP:
        case CUPRUNNERUP:
			bRet = false;
			break;

		default :
			EXT_ASSERT(false);		// Unhandled type
	}
	return  bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistory::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTrophyHistory::Read(CDataFile* _pDataFile)
{
	m_Title.Read(_pDataFile);
	m_Season.Read(_pDataFile);
	m_SuppInfo.Read(_pDataFile);
    _pDataFile->Read(&m_Gfx, sizeof m_Gfx);
    _pDataFile->Read(&m_Type, sizeof m_Type);
}


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistory::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTrophyHistory::Write(CDataFile* _pDataFile)
{
	m_Title.Write(_pDataFile);
	m_Season.Write(_pDataFile);
	m_SuppInfo.Write(_pDataFile);
    _pDataFile->Write(&m_Gfx, sizeof m_Gfx);
    _pDataFile->Write(&m_Type, sizeof m_Type);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



/*------------------------------------------------------------------------------
	Method:   	CTrophyHistoryList::~CTrophyHistoryList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTrophyHistoryList::~CTrophyHistoryList()
{
	RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistoryList::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTrophyHistoryList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	ushort HowMany;
	HowMany = GetSize();
    _pDataFile->Write(&HowMany, sizeof HowMany);
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        static_cast<CTrophyHistory*>(GetAt(ListPos))->Write(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistoryList::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTrophyHistoryList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	int LoopCount;
	ushort HowMany;
    _pDataFile->Read(&HowMany, sizeof HowMany);
	if (HowMany > 0)
	{
		// Ensure the list is the correct size
		if (HowMany > GetSize())
		{
			// Add items
			SetSize(HowMany);
			for (LoopCount = 0; LoopCount < HowMany; LoopCount++)
			{
				if (GetAt(LoopCount) == null)
				{
					SetAtGrow(LoopCount, new CTrophyHistory());
				}
			}

		}
		if (HowMany < GetSize())
		{
			// Remove items
			for (LoopCount = HowMany; LoopCount < GetSize(); LoopCount++)
			{
				delete  GetAt(LoopCount);
			}
			SetSize(HowMany);
			FreeExtra();
		}
		for (uint ListPos = 0; ListPos < HowMany; ListPos ++)
		{
			static_cast<CTrophyHistory*>(GetAt(ListPos))->Read(_pDataFile);
		}
	}
	else
	{
		RemoveList();
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistoryList::AddRunnersUp
	Access:    	public 
	Parameter: 	CString Title
	Parameter: 	CString SuppInfo
	Parameter: 	CString Season
	Parameter: 	const byte Gfx
	Parameter: 	bool IsCup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTrophyHistoryList::AddRunnersUp(CString Title, CString SuppInfo, CString Season, const byte Gfx, bool IsCup)
{
	byte Type = DIVISIONRUNNERUP;
	if (IsCup == true)
	{
		Type = CUPRUNNERUP;
	}
	Add(new CTrophyHistory(Title, SuppInfo, Season, Gfx, Type));
}


/*------------------------------------------------------------------------------
	Method:   	CTrophyHistoryList::AddChampions
	Access:    	public 
	Parameter: 	CString Title
	Parameter: 	CString SuppInfo
	Parameter: 	CString Season
	Parameter: 	const byte Gfx
	Parameter: 	bool IsCup
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTrophyHistoryList::AddChampions(CString Title, CString SuppInfo, CString Season, const byte Gfx, bool IsCup)
{
	byte Type = DIVISIONWINNER;
	if (IsCup == true)
	{
		Type = CUPWINNER;
	}
	Add(new CTrophyHistory(Title, SuppInfo, Season, Gfx, Type));
}
