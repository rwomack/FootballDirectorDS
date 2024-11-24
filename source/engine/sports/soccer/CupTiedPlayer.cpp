
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
	Method:   	CClubAndPlayerID::CClubAndPlayerID
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerID::CClubAndPlayerID(const ushort iPlayerID, const ClubID cClubID)
{
	m_PlayerID = iPlayerID;
	m_ClubID = cClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::CClubAndPlayerID
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerID::CClubAndPlayerID()
{
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::~CClubAndPlayerID
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerID::~CClubAndPlayerID()
{

}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerID::DoResetVariables()
{
	m_PlayerID = NOPLAYER;
	m_ClubID.id = NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::GetPlayerID
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
	Method:   	CClubAndPlayerID::GetPlayer
	Access:    	public 
	Returns:   	CPlayer&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer& CClubAndPlayerID::GetPlayer()
{
	return *WorldData().GetPlayer(GetPlayerID());
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::SetPlayerID
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerID::SetPlayerID(const ushort _PlayerID)
{
	m_PlayerID = _PlayerID;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::GetClubID
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CClubAndPlayerID::GetClubID() const
{
	return m_ClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::SetClubID
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerID::SetClubID(const ClubID _ClubID)
{
	m_ClubID = _ClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::SetClubID
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerID::SetClubID(const ushort _ClubID)
{
	m_ClubID.id = _ClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerID::Read(CDataFile* _pDataFile)
{
	_pDataFile->Read(&m_PlayerID, sizeof(m_PlayerID));
	_pDataFile->Read(&m_ClubID, sizeof(m_ClubID));
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerID::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerID::Write(CDataFile* _pDataFile)
{
	_pDataFile->Write(&m_PlayerID, sizeof(m_PlayerID));
	_pDataFile->Write(&m_ClubID, sizeof(m_ClubID));
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::Add
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Add(const ushort _PlayerID, const ClubID _ClubID)
{
	// Only add if not already there
	if (HasPlayed(_PlayerID) == false)
	{
		CClubAndPlayerID* pCupTiedPlayer = FD_ENGINE_NEW CClubAndPlayerID(_PlayerID, _ClubID);
		Add(pCupTiedPlayer);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::Add
	Access:    	public 
	Parameter: 	CClubAndPlayerID* pNeg
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
	Method:   	CClubAndPlayerIDList::HasPlayed
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
	Method:   	CClubAndPlayerIDList::DoFindListPosition
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
	return nRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::HasPlayedThisClub
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubAndPlayerIDList::HasPlayedThisClub(const ushort iPlayerID, const ClubID cClubID)
{
	bool bRet = false;
    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
        if (operator[](LoopCount).GetPlayerID() == iPlayerID)
		{
			if (operator[](LoopCount).GetClubID() == cClubID)
			{
				bRet = true;
			}
			break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::Delete
	Access:    	public 
	Parameter: 	CClubAndPlayerID* pCupTiedPlayer
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
			CClubAndPlayerID *pRemoved = m_List.GetAt(LoopCount);
			m_List.RemoveAt(LoopCount);
			delete pRemoved;
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::HasPlayedDifferentClub
	Access:    	public 
	Parameter: 	const ushort iPlayerID
	Parameter: 	const ushort iClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubAndPlayerIDList::HasPlayedDifferentClub(const ushort iPlayerID, const ClubID cClubID)
{
	bool bRet = false;
	EXT_ASSERT(false);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::Remove
	Access:    	public 
	Parameter: 	const ushort iPlayerId
	Parameter: 	const ClubID cClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Remove(const ushort _PlayerID, const ClubID _ClubID)
{
    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
		if (operator[](LoopCount).GetPlayerID() == _PlayerID && operator[](LoopCount).GetClubID() == _ClubID)
		{
			CClubAndPlayerID *pRemoved = m_List.GetAt(LoopCount);
			m_List.RemoveAt(LoopCount);
			delete pRemoved;
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::Remove
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Remove(const ushort _PlayerID)
{
	for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
		if (operator[](LoopCount).GetPlayerID())
		{
			CClubAndPlayerID *pRemoved = m_List.GetAt(LoopCount);
			m_List.RemoveAt(LoopCount);
			delete pRemoved;
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubAndPlayerIDList::Read(CDataFile* _pDataFile)
{
	ushort count;
    _pDataFile->Read(&count, sizeof(count));
    CClubAndPlayerID* p_CupTiedPlayer;
    RemoveList();

	for (uint i = 0; i < count; i++)
	{
        p_CupTiedPlayer = FD_ENGINE_NEW CClubAndPlayerID();
		p_CupTiedPlayer->Read(_pDataFile);
        Add(p_CupTiedPlayer);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::Write
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
    _pDataFile->Write(&count, sizeof(count));

    for (uint LoopCount = 0; LoopCount < m_List.GetSize(); LoopCount++)            // check list
	{
		m_List.GetAt(LoopCount)->Write(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::DeleteAll
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
	Method:   	CClubAndPlayerIDList::RemoveList
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
	Method:   	CClubAndPlayerIDList::operator[]
	Access:    	public 
	Parameter: 	const ushort x
	Returns:   	CClubAndPlayerID&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClubAndPlayerID& CClubAndPlayerIDList::operator [](const ushort x)
{
	return *(static_cast<CClubAndPlayerID*>(m_List.GetAt(x)));
}


/*------------------------------------------------------------------------------
	Method:   	CClubAndPlayerIDList::GetSize
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

