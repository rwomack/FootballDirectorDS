
/*

	File:- ManagerHistory.cpp

	Manager History classes

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CManagerClubHistory::DoResetVariables
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerClubHistory::DoResetVariables()
{
		m_ClubID.id = NOCLUB;
		m_TransferIncome = 0;
		m_TransferExpenditure = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerClubHistory::DoJoinClub
	Access:    	public
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerClubHistory::DoJoinClub(const ClubID _ClubID)
{
	m_ClubID = _ClubID;
    m_ClubDateJoined = WorldData().GetCurrentDate();
	m_TransferIncome = 0;
	m_TransferExpenditure = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerClubHistory::DoAddTransferIncome
	Access:    	public
	Parameter: 	const uint _Amount
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerClubHistory::DoAddTransferIncome(const uint _Amount)
{
	m_TransferIncome += _Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerClubHistory::DoAddTransferExpenditure
	Access:    	public
	Parameter: 	const uint _Amount
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerClubHistory::DoAddTransferExpenditure(const uint _Amount)
{
	m_TransferExpenditure += _Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerClubHistory::GetClub
	Access:    	public
	Returns:   	CClub&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub& CManagerClubHistory::GetClub()
{
	return WorldData().GetClubByID(m_ClubID);
}
/////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::CManagerHistory
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CManagerHistory::CManagerHistory()
{
	DoResetVariables();
}

CManagerHistory::~CManagerHistory()
{
	DoResetVariables();
}

/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::DoResetVariables
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::DoResetVariables()
{
	SetWon(0);
    SetDrawn(0);
    SetLost(0);
    SetPromotion(0);
    SetRelegation(0);
#ifdef DETAILEDMANAGERHISTORY
    m_Goals.DoInitialise();
#endif
#ifdef TROPHYHISTORY
	m_TrophyHistory.RemoveList();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::GetDrawn
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManagerHistory::GetDrawn() const
{
	return m_Data.m_Drawn;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::GetLost
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManagerHistory::GetLost() const
{
	return m_Data.m_Lost;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::GetGamesPlayed
	Access:    	public
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CManagerHistory::GetGamesPlayed() const
{
	return GetWon() + GetDrawn() + GetLost();
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::GetPromotion
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManagerHistory::GetPromotion() const
{
	return m_Data.m_Relegation;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::GetRelegation
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManagerHistory::GetRelegation() const
{
	return m_Data.m_Relegation;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::GetWon
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManagerHistory::GetWon() const
{
	return m_Data.m_Won;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::IncPromotion
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::IncPromotion()
{
	SetPromotion(GetPromotion() + 1);
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::IncRelegation
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::IncRelegation()
{
	SetRelegation(GetRelegation() + 1);
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::PlayedMatch
	Access:    	public
	Parameter: 	const byte _GoalsFor
	Parameter: 	const byte _GoalsAgainst
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::PlayedMatch(const byte _GoalsFor, const byte _GoalsAgainst)
{
#ifdef DETAILEDMANAGERHISTORY
    m_Goals.DoAddToTallies(_GoalsFor, _GoalsAgainst);
#endif
    if (_GoalsFor > _GoalsAgainst)
	{
        SetWon(GetWon() + 1);
	}
    if (_GoalsFor == _GoalsAgainst)
	{
        SetDrawn(GetDrawn() + 1);
	}
    if (_GoalsFor < _GoalsAgainst)
	{
        SetLost(GetLost() + 1);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::DoCalculateRating
	Access:    	public
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CManagerHistory::DoCalculateRating()
{
	int total = GetWon() + GetDrawn() + GetLost();
	if (total == 0)
	{
		return(100);
	}
	else
	{
		return((((GetWon() * 100) / total) + ((GetDrawn() * 50) / total)));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::SetDrawn
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::SetDrawn(const uint _Value)
{
	m_Data.m_Drawn = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::SetLost
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::SetLost(const uint _Value)
{
	m_Data.m_Lost = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::SetPromotion
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::SetPromotion(const uint _Value)
{
	m_Data.m_Promotion = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::SetRelegation
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::SetRelegation(const uint _Value)
{
	m_Data.m_Relegation = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::SetWon
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::SetWon(const uint _Value)
{
	m_Data.m_Won = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
  	_pDataFile->Read(&m_Data, sizeof(m_Data));
  	_pDataFile->Read(&m_Goals, sizeof(m_Goals));
#ifdef TROPHYHISTORY
	m_TrophyHistory.Read(_pDataFile);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CManagerHistory::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CManagerHistory::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
 	_pDataFile->Write(&m_Data, sizeof(m_Data));
  	_pDataFile->Write(&m_Goals, sizeof(m_Goals));
#ifdef TROPHYHISTORY
	m_TrophyHistory.Write(_pDataFile);
#endif
}
 /////////////////////////////////////////////////////////////////////////////////////////////////
