
 /*

    File :- CupTiedPlayer.cpp

    The cup tied player class routines


	 Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "CupTiedPlayer.h"



/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayer::CCupTiedPlayer
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerID::CClubAndPlayerID(const ushort iPlayerID, const ushort iClubID)
{
	m_PlayerID = iPlayerID;
	m_ClubID = iClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayer::CCupTiedPlayer
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerID::CClubAndPlayerID()
	: m_PlayerID(wNOTFOUND)
	, m_ClubID(wNOTFOUND)
{
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayer::~CCupTiedPlayer
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerID::~CClubAndPlayerID()
{

}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayer::GetPlayerID
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubAndPlayerID::GetPlayerID() const
{
	return m_PlayerID;
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayer::SetPlayerID
	Access:    	public 
	Parameter: 	const ushort x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerID::SetPlayerID(const ushort x)
{
	m_PlayerID = x;
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayer::GetClubID
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CClubAndPlayerID::GetClubID() const
{
	return m_ClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayer::SetClubID
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerID::SetClubID(const ushort _ClubID)
{
	m_ClubID = _ClubID;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::Add
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Add(const ushort _PlayerID, const ushort _ClubID)
{
	// Only add if not already there
	if (HasPlayed(_PlayerID) == false)
	{
		CClubAndPlayerID* pCupTiedPlayer = new CClubAndPlayerID(_PlayerID, _ClubID);
		Add(pCupTiedPlayer);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::Add
	Access:    	public 
	Parameter: 	CCupTiedPlayer* pNeg
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Add(CClubAndPlayerID* _Neg)
{
	EXT_ASSERT_POINTER(_Neg, CClubAndPlayerID);
	m_List.Add(_Neg);
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::HasPlayed
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubAndPlayerIDList::HasPlayed(const ushort _PlayerID)
{
	bool bRet = false;
    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
        if (operator[](LoopCount).GetPlayerID() == _PlayerID)
		{
            bRet = true;
			break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::DoFindListPosition
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Returns:   	const ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
const ushort CClubAndPlayerIDList::DoFindListPosition(const ushort iPlayerID)
{
	ushort nRet = wNOTFOUND;
    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
        if (operator[](LoopCount).GetPlayerID() == iPlayerID)
		{
            nRet = LoopCount;
			break;
		}
	}
	EXT_ASSERT(nRet != wNOTFOUND);
	return nRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::HasPlayedThisClub
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubAndPlayerIDList::HasPlayedThisClub(const ushort iPlayerID, const ushort iClubID)
{
	bool bRet = false;
    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
        if (operator[](LoopCount).GetPlayerID() == iPlayerID)
		{
			if (operator[](LoopCount).GetClubID() == iClubID)
			{
				bRet = true;
			}
			break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::Delete
	Access:    	public 
	Parameter: 	CCupTiedPlayer* pCupTiedPlayer
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Delete(CClubAndPlayerID* pCupTiedPlayer)
{
    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
        if (m_List.GetAt(LoopCount) == pCupTiedPlayer)
		{
			m_List.RemoveAt(LoopCount);
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::HasPlayedDifferentClub
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubAndPlayerIDList::HasPlayedDifferentClub(const ushort iPlayerID, const ushort iClubID)
{
	bool bRet = false;
	EXT_ASSERT(false);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::Remove
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Remove(const ushort iPlayerID, const ushort iClubID)
{
    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
        if (operator[](LoopCount).GetPlayerID() == iPlayerID && operator[](LoopCount).GetClubID() == iClubID)
		{
			m_List.RemoveAt(LoopCount);
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Read(CDataFile* _pDataFile)
{
	ushort count;
	_pDataFile->Read(&count, sizeof count);
    CClubAndPlayerID* p_CupTiedPlayer;
    RemoveList();

	for (uint i = 0; i < count; i++)
	{
        p_CupTiedPlayer = new CClubAndPlayerID();
        _pDataFile->Read(p_CupTiedPlayer, sizeof CClubAndPlayerID);
        Add(p_CupTiedPlayer);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Write(CDataFile* _pDataFile)
{
	ushort count;
	count = m_List.GetSize();
	_pDataFile->Write(&count, sizeof count);

    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
        _pDataFile->Write(&operator[](LoopCount), sizeof CClubAndPlayerID);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::DeleteAll
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::DeleteAll()
{
	ushort HowMany = m_List.GetSize();
	if (HowMany == 0)
	{
		return;		// None to delete
	}
    for (uint ListPos = 0; ListPos < HowMany; ListPos ++)
	{
		delete m_List.GetAt(ListPos);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::RemoveList
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::RemoveList()
{
	DeleteAll();
    m_List.RemoveAll();
 }


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::operator[]
	Access:    	public 
	Parameter: 	const ushort x
	Returns:   	CCupTiedPlayer&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerID& CClubAndPlayerIDList::operator [](const ushort x)
{
	return *(static_cast<CClubAndPlayerID*>(m_List.GetAt(x)));
}


/*------------------------------------------------------------------------------
	Method:   	CCupTiedPlayerList::GetSize
	Access:    	public 
	Parameter: 	void
	Returns:   	const ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
const ushort CClubAndPlayerIDList::GetSize()
{
	return m_List.GetSize();
}

