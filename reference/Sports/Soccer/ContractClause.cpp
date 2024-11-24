/*

	ContractClause.cpp: implementation of the CContractClause class.


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*------------------------------------------------------------------------------
	Method:   	CContractClause::CContractClause
	Access:    	public 
	Parameter: 	const ushort PlayerID
	Parameter: 	byte SellOnClausePercentage
	Parameter: 	const ushort ClubToID
	Parameter: 	const ushort ClubFromID
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractClause::CContractClause(const ushort _PlayerID, byte _SellOnClausePercentage, const ushort _ClubToID, const ushort _ClubFromID)
{
	DoSetVariables(_PlayerID, _SellOnClausePercentage, _ClubToID, _ClubFromID);
}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::~CContractClause
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractClause::~CContractClause()
{

}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::GetPlayerID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CContractClause::GetPlayerID() const					
{ 
	return m_PlayerID; 
}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::GetClubTo
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CContractClause::GetClubTo()					
{ 
	return WorldData().GetClub(m_ClubToID); 
}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::GetClubFrom
	Access:    	public 
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CContractClause::GetClubFrom()					
{ 
	return WorldData().GetClub(m_ClubFromID); 
}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::GetSellOnClausePercentage
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CContractClause::GetSellOnClausePercentage() const 
{ 
	return m_SellOnClausePercentage; 
}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::IsEmpty
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CContractClause::IsEmpty()						
{ 
	return false; 
}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::DoSetVariables
	Access:    	public 
	Parameter: 	const ushort PlayerID
	Parameter: 	byte SellOnClausePercentage
	Parameter: 	const ushort ClubToID
	Parameter: 	const ushort ClubFromID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClause::DoSetVariables(const ushort PlayerID, byte _SellOnClausePercentage, const ushort _ClubToID, const ushort _ClubFromID)
{
	EXT_ASSERT(PlayerID != NOPLAYER && _ClubToID != NOCLUB);
	m_PlayerID = PlayerID;
	m_SellOnClausePercentage = _SellOnClausePercentage;
	m_ClubFromID = _ClubFromID;
	m_ClubToID = _ClubToID;
}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::DoResetVariables
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClause::DoResetVariables()
{
	m_PlayerID = NOPLAYER;
	m_SellOnClausePercentage = 0;
	m_ClubFromID = NOCLUB;
	m_ClubToID = NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CContractClause::CheckForSwapClubs
	Access:    	public 
	Parameter: 	ushort ClubA
	Parameter: 	ushort ClubB
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClause::DoCheckForSwapClubs(ushort _Club1ID, ushort _Club2ID)
{
	if (m_ClubToID == _Club1ID)
	{
		m_ClubToID = _Club2ID;
	}
	else if (m_ClubToID == _Club2ID)
	{
		m_ClubToID = _Club1ID;
	}
	if (m_ClubFromID == _Club1ID)
	{
		m_ClubFromID = _Club2ID;
	}
	else if (m_ClubFromID == _Club2ID)
	{
		m_ClubFromID = _Club1ID;
	}
}
//////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CContractClauseList::~CContractClauseList
	Access:    	public 
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractClauseList::~CContractClauseList()
{
    RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CContractClauseList::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClauseList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    CContractClause* pContractClause;
	RemoveList();
	ushort count, Idx;
    _pDataFile->Read(&count, sizeof count);
	SetSize(count);
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        _pDataFile->Read(&Idx, sizeof Idx);
		if (Idx != wNOTFOUND)
		{
			pContractClause = new CContractClause();
			EXT_ASSERT(pContractClause != null);
			_pDataFile->Read(pContractClause, sizeof CContractClause);
		}
		else
		{
			pContractClause = null;
		}
        SetAtGrow(LoopCount, pContractClause);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CContractClauseList::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClauseList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    CContractClause* pContractClause;
	ushort nHowMany = GetSize();
    _pDataFile->Write(&nHowMany, sizeof nHowMany);
	ushort nNull = wNOTFOUND;
    for (uint LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
        pContractClause = static_cast<CContractClause*>(GetAt(LoopCount));
		if (pContractClause != null)
		{
            _pDataFile->Write(&nHowMany, sizeof nHowMany);
			_pDataFile->Write(pContractClause, sizeof CContractClause);
		}
		else
		{
            _pDataFile->Write(&nNull, sizeof nNull);
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CContractClauseList::DoAddOne
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Parameter: 	byte _SellOnClausePercentage
	Parameter: 	const ushort _ClubToID
	Parameter: 	const ushort _ClubFromID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClauseList::DoAddOne(const ushort _PlayerID, byte _SellOnClausePercentage, const ushort _ClubToID, const ushort _ClubFromID)
{
    CContractClause* pContractClause;
	ushort Id = wNOTFOUND;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        pContractClause = static_cast<CContractClause*>(GetAt(ListPos));
		if (pContractClause == null)
		{
			SetAt(ListPos, new CContractClause(_PlayerID, _SellOnClausePercentage, _ClubToID, _ClubFromID));
			Id = ListPos;
			break;
		}
        if (pContractClause->IsEmpty() == true)
		{
			pContractClause->DoSetVariables(_PlayerID, _SellOnClausePercentage, _ClubToID, _ClubFromID);
			Id = ListPos;
			break;
		}
	}
	if (Id == wNOTFOUND)
	{
		Add(new CContractClause(_PlayerID, _SellOnClausePercentage, _ClubToID, _ClubFromID));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CContractClauseList::RemoveAllForPlayer
	Access:    	public 
	Parameter: 	const ushort PlayerID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClauseList::RemoveAllForPlayer(const ushort _PlayerID)
{
	CContractClause* pContractClause;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		pContractClause = static_cast<CContractClause*>(GetAt(ListPos));
		if (pContractClause != null && pContractClause->GetPlayerID() == _PlayerID)
		{
			pContractClause->DoResetVariables();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CContractClauseList::RemoveOne
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Parameter: 	CClub * _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClauseList::RemoveOne(const ushort _PlayerID, CClub* _Club)
{
	if (GetSize() > 0)
	{
		CContractClause* pContractClause = DoFindPlayerClubClause(_PlayerID, _Club);
		if (pContractClause != null)
		{
			pContractClause->DoResetVariables();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CContractClauseList::CheckForSwapClubs
	Access:    	public 
	Parameter: 	ushort ClubAID
	Parameter: 	ushort ClubBID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContractClauseList::DoCheckForSwapClubs(ushort _Club1ID, ushort _Club2ID)
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
         static_cast<CContractClause*>(GetAt(ListPos))->DoCheckForSwapClubs(_Club1ID, _Club2ID);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CContractClauseList::DoFindPlayerClubClause
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Parameter: 	CClub * _Club
	Returns:   	CContractClause*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContractClause* CContractClauseList::DoFindPlayerClubClause(const ushort _PlayerID, CClub* _Club)
{
	CContractClause* pContractClause = null;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (static_cast<CContractClause*>(GetAt(ListPos))->GetPlayerID() == _PlayerID && &(static_cast<CContractClause*>(GetAt(ListPos))->GetClubFrom()) == _Club)
		{
			pContractClause = static_cast<CContractClause*>(GetAt(ListPos));
			break;
		}
    }
	return pContractClause;
}

