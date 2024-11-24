

 /*

	Class:- PlayerSquad

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#endif

#include "stdafx.h"

#include "PlayerSquad.h"
#include "LeadingGoalScorer.h"
#include "leadingDisciplinaryPoints.h"
#include "PlayerSearch.h"
#include "FansReaction.h"
#include "SortedStringList.h"

	const uint ASKFORTRANSFERCHANCE = 75;


//#define FORCETRANSFERREQUESTS


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CPlayerSquad
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSquad::CPlayerSquad()
	: m_PlayersInOnLoanThisSeason(0)
	, m_PlayersOutOnLoan(0)
	, m_PlayersInOnLoan(0)
{
	for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
		m_listSquadID[LoopCount] = NOPLAYER;
	}
	DoUpdateTeamStrengths(0, 0, 0, 0);
	InitialiseSpecialPlayers();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CPlayerSquad
	Access:    	public
	Parameter: 	CPlayerSquad& _rPlayerSquad
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSquad::CPlayerSquad(CPlayerSquad& _rPlayerSquad)
{
	DoCopy(_rPlayerSquad);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::operator=
	Access:    	public
	Parameter: 	CPlayerSquad& _rPlayerSquad
	Returns:   	CPlayerSquad&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayerSquad& CPlayerSquad::operator = (CPlayerSquad& _rPlayerSquad)
{
	DoCopy(_rPlayerSquad);
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::BuildFitPlayerNameList
	Access:    	public
	Parameter: 	CSortedStringList & _theList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::BuildFitPlayerNameList(CSortedStringList& _theList)
{
	CPlayer* pPlayer = NULL;
	for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
		if (GetPlayer(num).IsInjured() == false)
		{
			_theList.Add(GetPlayer(num).GetName(), num);
		}
	}
}

/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCopy
	Access:    	public
	Parameter: 	CPlayerSquad& _rPlayerSquad
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoCopy(CPlayerSquad& _rPlayerSquad)
{
	uint LoopCount;
	for (LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
	    m_listSquadID[LoopCount] = _rPlayerSquad.m_listSquadID[LoopCount];
	}
    m_CaptainID = _rPlayerSquad.m_CaptainID;
    m_PenaltyTakerID = _rPlayerSquad.m_PenaltyTakerID;
    m_CornerTakerID = _rPlayerSquad.m_CornerTakerID;
    m_FreeKickTakerID = _rPlayerSquad.m_FreeKickTakerID;
    m_PlayersInOnLoanThisSeason = _rPlayerSquad.m_PlayersInOnLoanThisSeason;
    m_PlayersOutOnLoan = _rPlayerSquad.m_PlayersOutOnLoan;
    m_PlayersInOnLoan = _rPlayerSquad.m_PlayersInOnLoan;
	for (LoopCount = 0; LoopCount < MAXPLAYERPOSITIONS; LoopCount++)
    {
		m_AverageStrengths[LoopCount] = _rPlayerSquad.m_AverageStrengths[LoopCount];
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetCaptainID
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetCaptainID() const
{
	return m_CaptainID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetCaptainSquadNum
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetCaptainSquadNum()
{
	return DoFindPlayerIDInSquad(m_CaptainID);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetSquad
	Access:    	public
	Parameter: 	const ushort _Index
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetSquad(const ushort _Index)
{
	EXT_ASSERT(_Index < PLAYERSINSQUAD); //, "Index out of range");
	return m_listSquadID[_Index];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::SetSquad
	Access:    	public
	Parameter: 	const ushort _SquadNum
	Parameter: 	const ushort _PlayerID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::SetSquad(const ushort _SquadNum, const ushort _PlayerID)
{
	EXT_ASSERT(_SquadNum < PLAYERSINSQUAD); //, "SquadNum out of range");
	EXT_ASSERT(_PlayerID < WorldData().GetPlayerList().GetSize() || _PlayerID == NOPLAYER); //, "PlayerID out of range");
	m_listSquadID[_SquadNum] = _PlayerID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::SetCaptainSquadNum
	Access:    	public
	Parameter: 	const ushort _SquadNum
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::SetCaptainSquadNum(const ushort _SquadNum)
{
	EXT_ASSERT(_SquadNum < PLAYERSINSQUAD);
	m_CaptainID = m_listSquadID[_SquadNum];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetCornerTakerSquadNum
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetCornerTakerSquadNum()
{
	return DoFindPlayerIDInSquad(m_CornerTakerID);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetCornerTakerID
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetCornerTakerID() const
{
	return m_CornerTakerID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetFreeKickTakerID
	Access:    	public
	Returns:   	USHORT
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetFreeKickTakerID() const
{
	return m_FreeKickTakerID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetFreeKickTakerSquadNum
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetFreeKickTakerSquadNum()
{
	return DoFindPlayerIDInSquad(m_FreeKickTakerID);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::SetPenaltyTakerNum
	Access:    	public
	Parameter: 	const ushort _SquadNum
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::SetPenaltyTakerSquadNum(const ushort _SquadNum)
{
	EXT_ASSERT(_SquadNum < PLAYERSINSQUAD);
	m_PenaltyTakerID = m_listSquadID[_SquadNum];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetPenaltyTakerID
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetPenaltyTakerID() const
{
	return m_PenaltyTakerID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetPenaltyTakerSquadNum
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetPenaltyTakerSquadNum()
{
	return DoFindPlayerIDInSquad(m_PenaltyTakerID);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::SetCornerTakerSquadNum
	Access:    	public
	Parameter: 	const ushort _SquadNum
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::SetCornerTakerSquadNum(const ushort _SquadNum)
{
	EXT_ASSERT(_SquadNum < PLAYERSINSQUAD);
	m_CornerTakerID = m_listSquadID[_SquadNum];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::SetFreeKickTakerNum
	Access:    	public
	Parameter: 	const ushort _SquadNum
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::SetFreeKickTakerSquadNum(const ushort _SquadNum)
{
	EXT_ASSERT(_SquadNum < PLAYERSINSQUAD);
	m_FreeKickTakerID = m_listSquadID[_SquadNum];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::SetLoansThisSeason
	Access:    	public
	Parameter: 	const ushort _LoanCount
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::SetLoansThisSeason(const byte _LoanCount)
{
	m_PlayersInOnLoanThisSeason = _LoanCount;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoAddPlayerToSquad
	Access:    	public
	Parameter: 	const ushort _nPlayerID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoAddPlayerToSquad(const ushort _nPlayerID)
{
	EXT_ASSERT(TotalSquadSize() <= PLAYERSINSQUAD);
    for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
        if (m_listSquadID[LoopCount] == _nPlayerID)
		{
            EXT_ASSERT(false);
		}
        if (m_listSquadID[LoopCount] == NOPLAYER)
		{
			SetSquad(LoopCount, _nPlayerID);
            break;
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindPlayerIDInSquad
	Access:    	public
	Parameter: 	const ushort _PlayerID
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoFindPlayerIDInSquad(const ushort _PlayerID)
{
	ushort nFoundPlayerID = NOPLAYER;
	for (uint count = 0; count < PLAYERSINSQUAD; count ++)
	{
		if (m_listSquadID[count] != NOPLAYER && m_listSquadID[count] == _PlayerID)
		{
			nFoundPlayerID = count;
			break;
		}
	}
	return nFoundPlayerID;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::ClearSpecialPlayerIfCurrent
	Access:    	public
	Parameter: 	const ushort _nPlayerToRemove
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::ClearSpecialPlayerIfCurrent(const ushort _nPlayerToRemove)
{
	ushort PlayerListNumber = m_listSquadID[_nPlayerToRemove];
	// Check if he had special duties
	if (m_CaptainID == PlayerListNumber)
	{
		m_CaptainID = NOPLAYER;		// Discard player
	}
	if (m_PenaltyTakerID == PlayerListNumber)
	{
		m_PenaltyTakerID = NOPLAYER;
	}
	if (m_CornerTakerID == PlayerListNumber)
	{
		m_CornerTakerID = NOPLAYER;
	}
	if (m_FreeKickTakerID == PlayerListNumber)
	{
		m_FreeKickTakerID = NOPLAYER;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::TotalSquadSize
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:	Count the actual number of players in the squad
------------------------------------------------------------------------------*/
ushort CPlayerSquad::TotalSquadSize()
{
    ushort iRet = 0;
    for (int num = 0; num < PLAYERSINSQUAD; num ++)
	{
		if (m_listSquadID[num] != NOPLAYER)
		{
			iRet++;
		}
    }
	EXT_ASSERT(iRet <= PLAYERSINSQUAD);
    return iRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::InitialiseSpecialPlayers
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::InitialiseSpecialPlayers()
{
	m_CaptainID = NOPLAYER;
	m_PenaltyTakerID = NOPLAYER;
	m_CornerTakerID = NOPLAYER;
	m_FreeKickTakerID = NOPLAYER;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindWeakestPosition
	Access:    	public const
	Parameter: 	void
	Returns:   	ePlayerPosition
	Qualifier: 	const
	Purpose:	Finds the weakest area of the team in the last match
------------------------------------------------------------------------------*/
ePlayerPosition CPlayerSquad::DoFindWeakestPosition() const
{
	ePlayerPosition WeakestPosition = GOALKEEPER;
	byte WeakestSkill = m_AverageStrengths[GOALKEEPER];
	if (WeakestSkill > m_AverageStrengths[DEFENDER])
	{
		WeakestPosition = DEFENDER;
	}
	if (WeakestSkill > m_AverageStrengths[MIDFIELDER])
	{
		WeakestPosition = MIDFIELDER;
	}
	if (WeakestSkill > m_AverageStrengths[ATTACKER])
	{
		WeakestPosition = ATTACKER;
	}
	return WeakestPosition;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoUpdateTeamStrengths
	Access:    	public
	Parameter: 	const byte _goalstren
	Parameter: 	const byte _defstren
	Parameter: 	const byte _midstren
	Parameter: 	const byte _attstren
	Returns:   	void
	Qualifier:
	Purpose:	The zonal effectiveness for the last match
------------------------------------------------------------------------------*/
void CPlayerSquad::DoUpdateTeamStrengths(const byte _goalstren, const byte _defstren, const byte _midstren, const byte _attstren)
{
	m_AverageStrengths[GOALKEEPER] = _goalstren;
	m_AverageStrengths[DEFENDER] = _defstren;
	m_AverageStrengths[MIDFIELDER] = _midstren;
	m_AverageStrengths[ATTACKER] = _attstren;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateAverageStrengths
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoCalculateAverageStrengths()
{
    CPlayerIDList ThePlayers;
    ThePlayers.DoBuildFromTeamPlayingPosn(*this, WorldData().GetPlayerList(), GOALKEEPER);
	m_AverageStrengths[GOALKEEPER] = ThePlayers.AverageSkill();
	ThePlayers.RemoveAll();

    ThePlayers.DoBuildFromTeamPlayingPosn(*this, WorldData().GetPlayerList(), DEFENDER);
	m_AverageStrengths[DEFENDER] = ThePlayers.AverageSkill();
	ThePlayers.RemoveAll();

	ThePlayers.DoBuildFromTeamPlayingPosn(*this, WorldData().GetPlayerList(), MIDFIELDER);
	m_AverageStrengths[MIDFIELDER] = ThePlayers.AverageSkill();
	ThePlayers.RemoveAll();

	ThePlayers.DoBuildFromTeamPlayingPosn(*this, WorldData().GetPlayerList(), ATTACKER);
	m_AverageStrengths[ATTACKER] = ThePlayers.AverageSkill();
	ThePlayers.RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::PostInitialise
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::PostInitialise(CClub& _Club)
{
	DoCalculateAverageStrengths();
	byte DivStrength = _Club.GetDivisionStrengthID();
	int StarPlayerCount = DoCountStarPlayers(_Club.GetPopularity().MinimumStarPlayerSkill(DivStrength));
	int TooGoodPlayerSkill = _Club.GetPopularity().GetMaximumStarPlayerSkill(DivStrength);
	int TooPoorPlayerSkill = _Club.GetPopularity().GetMinimumPlayerSkill(DivStrength);
	ushort TooGoodCount = 0;
	ushort TooPoorCount = 0;
	for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
		if (m_listSquadID[num] != NOPLAYER)
		{
			TooGoodCount += (GetPlayer(num).GetSkills().GetOverallSkill() >= TooGoodPlayerSkill);

			if (GetPlayer(num).GetSkills().GetOverallSkill() < TooPoorPlayerSkill)
			{
// 				g_DebugStr.Format(_T("%s old %d"), GetPlayer(num).GetName(), GetPlayer(num).GetSkills().GetOverallSkill());
 //				g_DebugStr.OutputDebug();
// 				GetPlayer(num).GetSkills().SetOverallSkill(_Club.GetPopularity().GetRandomPlayerSkill(DivStrength));
//  				g_DebugStr.Format(_T("%s new %d"), GetPlayer(num).GetName(), GetPlayer(num).GetSkills().GetOverallSkill());
//  				g_DebugStr.OutputDebug();
// 				g_DebugStr.Format(_T("Ps %d Ov %d Ha %d Ta %d Pa %d Sh %d He %d"), GetPosition(), GetOverallSkill(),
// 					m_bitfields.m_Handling, m_bitfields.m_Tackling, m_bitfields.m_Passing, m_bitfields.m_Shooting, m_bitfields.m_Heading);
// 				g_DebugStr.OutputDebug();
				if (GetPlayer(num).GetSkills().GetOverallSkill() < TooPoorPlayerSkill)
				{
					TooPoorCount++;
				}
			}
		}
	}
 //	g_DebugStr.Format(_T("Squad %d, Stars %d, Too good %d, To poor %d, G %d, D %d, M %d, A %d"), TotalSquadSize(), StarPlayerCount, TooGoodCount, TooPoorCount,
 //		m_AverageStrengths[GOALKEEPER], m_AverageStrengths[DEFENDER], m_AverageStrengths[MIDFIELDER], m_AverageStrengths[ATTACKER]);
 //	g_DebugStr.OutputDebug();
	DoSelectTeam(5, _Club);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountAllPlayersPosn
	Access:    	public
	Parameter: 	const ePlayerPosition _Posn
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoCountAllPlayersPosn(const ePlayerPosition _Posn)
{
    ushort iRet = 0;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			if (GetPlayer(num).GetSkills().GetPosition() == _Posn)
			{
				iRet++;
			}
        }
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CountFootedPlayersPosn
	Access:    	public
	Parameter: 	const ePlayerPosition _Posn
	Parameter: 	const eBESTFOOT _eFoot
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::CountFootedPlayersPosn(const ePlayerPosition _Posn, const eBESTFOOT _eFoot)
{
    ushort iRet = 0;
    CPlayer* pPlayer;

    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
            pPlayer = &GetPlayer(num);
			EXT_ASSERT_POINTER(pPlayer, CPlayer);
			if (pPlayer->GetSkills().GetBestFoot() == _eFoot && pPlayer->GetSkills().GetPosition() == _Posn)
			{
				iRet++;
			}
        }
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GotPlayerOnLoan
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::GotPlayerOnLoan()
{
	m_PlayersInOnLoan++;
	m_PlayersInOnLoanThisSeason++;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoRemovePlayerIDFromNonUserControlledSquad
	Access:    	public
	Parameter: 	const ushort a_nPlayerID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoRemovePlayerIDFromNonUserControlledSquad(const ushort _nPlayerID)
{
	DoRemoveFromSquad(DoFindPlayerSquadNumber(_nPlayerID));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoRemoveFromSquad
	Access:    	public
	Parameter: 	ushort _PlayerToRemoveSquadNumber
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoRemoveFromSquad(ushort _PlayerToRemoveSquadNumber)
{
	// If the player is in the team
    if (_PlayerToRemoveSquadNumber < PLAYERSINTEAM)
	{
		// Try to replace with a player of the same type
        for (uint nSquadNumber = PLAYERSINTEAM; nSquadNumber < PLAYERSINSQUAD; nSquadNumber++)
		{
			if (m_listSquadID[nSquadNumber] != NOPLAYER)
			{
				// If is same position
				if (GetPlayer(_PlayerToRemoveSquadNumber).GetSkills().GetPosition() == GetPlayer(nSquadNumber).GetSkills().GetPosition())
				{
					// replace him
					SetSquad(_PlayerToRemoveSquadNumber, m_listSquadID[nSquadNumber]);
					_PlayerToRemoveSquadNumber = nSquadNumber;
					break;	// Now overwrite old position
				}
			}
        }
	}
	// If not the last player in the squad move the rest down one.
    if (_PlayerToRemoveSquadNumber < PLAYERSINSQUAD - 1)
	{
        for (uint nSquadNumber = _PlayerToRemoveSquadNumber; nSquadNumber < PLAYERSINSQUAD - 1; nSquadNumber++)
		{
            SetSquad(nSquadNumber, m_listSquadID[nSquadNumber + 1]);
        }
	}
	// Remove the last player from the squad
    SetSquad(PLAYERSINSQUAD - 1, NOPLAYER);
}


#ifdef _DEBUGv3
/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DumpSquad
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DumpSquad()
{
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
		if (m_listSquadID[num] != NOPLAYER)
		{
			afxDump << num << " " << m_listSquadID[num] << " " << GetPlayer(num).GetName() << "\n";
		}
	}
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CountPlayersScoredAtLeast
	Access:    	public
	Parameter: 	const uint _nMin
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerSquad::CountPlayersScoredAtLeast(const uint _nMin)
{
	int iCount = 0;
  	for (uint nSquadNumber = 0; nSquadNumber < PLAYERSINTEAM; nSquadNumber++)
	{
    	if (m_listSquadID[nSquadNumber] != NOPLAYER)
		{
      		if (GetPlayer(nSquadNumber).GetNumberGoalsThisSeason() >= _nMin && GetPlayer(nSquadNumber).GetSkills().GetPosition() > GOALKEEPER)
			{
				iCount++;
			}
		}
	}
	return iCount;
}


#ifdef ASSISTS
/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CountPlayersAssistedAtLeast
	Access:    	public
	Parameter: 	const uint _nMin
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::CountPlayersAssistedAtLeast(const uint _nMin)
{
	int iCount = 0;
	for (uint count = 0; count < PLAYERSINTEAM; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
			if (GetPlayer(count).GetSeasonAssist() >= _nMin)
			{
				iCount++;
			}
		}
	}
	return iCount;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CountPlayersFouledAtLeast
	Access:    	public
	Parameter: 	const uint _nMin
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerSquad::CountPlayersFouledAtLeast(const uint _nMin)
{
	int iCount = 0;
	for (uint count = 0; count < PLAYERSINTEAM; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
			if (GetPlayer(count).GetDiscipline().GetDisciplinaryPoints() >= _nMin)
			{
				iCount++;
			}
		}
	}
	return iCount;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetLeadingScorer
	Access:    	public
	Parameter: 	CPlayer* _pIgnorePlayer
	Parameter: 	const bool _bIgnoreUnavailablePlayers
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CPlayerSquad::GetLeadingScorer(CPlayer* _pIgnorePlayer, const bool _bIgnoreUnavailablePlayers /*= false*/)
{
	CPlayer* pBestPlayer = NULL;
	CPlayer* pPlayer = NULL;
	int iCount = 0;
    for (uint count = 0; count < PLAYERSINTEAM; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
			pPlayer = &GetPlayer(count);
			if (pPlayer != _pIgnorePlayer && pPlayer->GetSkills().GetPosition() > GOALKEEPER)
			{
				if (pPlayer->GetNumberGoalsThisSeason() > 0 && pPlayer->GetNumberGoalsThisSeason() >= iCount)
				{
					if (pPlayer->IsAvailableForMatch() == false && _bIgnoreUnavailablePlayers)
					{
					}
					else
					{
						pBestPlayer = pPlayer;
						iCount = pBestPlayer->GetNumberGoalsThisSeason();
					}
				}
			}
		}
	}
	return pBestPlayer;
}


#ifdef ASSISTS
/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetLeadingAssister
	Access:    	public
	Parameter: 	CPlayer* _pIgnorePlayer
	Parameter: 	const bool _bIgnoreUnavailablePlayers
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CPlayerSquad::GetLeadingAssister(CPlayer* _pIgnorePlayer, const bool _bIgnoreUnavailablePlayers /*= false*/)
{
	CPlayer* pPlayer = NULL;
	CPlayer* pBestPlayer = NULL;
	int iCount = 0;
	CExtString szStr;
	for (uint count = 0; count < PLAYERSINTEAM; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
			pPlayer = &GetPlayer(count);
			if (pPlayer != _pIgnorePlayer)
			{
				if (pPlayer->GetSeasonAssist() > 0 && pPlayer->GetSeasonAssist() >= iCount)
				{
					if (pPlayer->IsAvailableForMatch() == false && _bIgnoreUnavailablePlayers)
					{
					}
					else
					{
						iCount = pPlayer->GetSeasonAssist();
						pBestPlayer = pPlayer;
					}
				}
			}
		}
	}
//	EXT_ASSERT_POINTER(pBestPlayer, CPlayer);
	return pBestPlayer;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetLeadingFouler
	Access:    	public
	Parameter: 	CPlayer* _pIgnorePlayer
	Parameter: 	const bool _bIgnoreUnavailablePlayers
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CPlayerSquad::GetLeadingFouler(CPlayer* _pIgnorePlayer, const bool _bIgnoreUnavailablePlayers /*= false*/)
{
	CPlayer* pPlayer = NULL;
	CPlayer* pPlayerToCheck;
	int iCount = 0;
	for (uint count = 0; count < PLAYERSINTEAM; count ++)
	{
    if (m_listSquadID[count] != NOPLAYER)
		{
			pPlayerToCheck = &GetPlayer(count);
			if (pPlayerToCheck != _pIgnorePlayer && pPlayerToCheck->IsAvailableForMatch() == true)
			{
				if (GetPlayer(count).GetDiscipline().GetDisciplinaryPoints() >= iCount)
				{
					if (pPlayerToCheck->IsAvailableForMatch() == false && _bIgnoreUnavailablePlayers)
					{
					}
					else
					{
						pPlayer = &GetPlayer(count);
						iCount = pPlayer->GetDiscipline().GetDisciplinaryPoints();
					}
				}
			}
		}
	}
	return pPlayer;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CountPlayersSeekingMove
	Access:    	public
	Parameter: 	void
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayerSquad::CountPlayersSeekingMove()
{
    uint Count = 0;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			if (GetPlayer(num).GetContractRenewalStatus() == LISTED)
			{
				Count++;
			}
		}
	}
    return Count;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountAvailablePlayers
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:	Count the number of players available to play in the next match
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoCountAvailablePlayers()
{
    ushort iRet = 0;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			//afxDump << "Player Id " << m_listSquadID[num] << "\n";
            iRet += GetPlayer(num).IsAvailableForMatch();
        }
    }
    return iRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountSuspendedPlayers
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:	Count the number of players unable to play in the next match
				due to suspension
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCountSuspendedPlayers()
{
    byte iRet = 0;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
            iRet += (GetPlayer(num).GetDiscipline().GetMatchesSuspended() > 0);
        }
    }
    return iRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountInjuredPlayers
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:	Count the number of players unable to play in the next match
				due to injury
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCountInjuredPlayers()
{
    byte iRet = 0;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
            iRet += GetPlayer(num).IsInjured();
        }
    }
    return iRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountAvailableNonLoanedPlayers
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:	Count the number of players available to play in the next match
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCountAvailableNonLoanedPlayers()
{
    int iRet = 0;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
            if (GetPlayer(num).IsAvailableForMatch() == true && GetPlayer(num).IsOnLoan() == false)
			{
                iRet++;
			}
        }
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountAvailablePlayersPosn
	Access:    	public
	Parameter: 	ePlayerPosition _Posn
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoCountAvailablePlayersPosn(ePlayerPosition _ePosn)
{
    ushort iRet = 0;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			if (GetPlayer(num).GetSkills().GetPosition() == _ePosn && GetPlayer(num).IsAvailableForMatch() == true)
			{
				iRet++;
			}
        }
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountRecentlySignedPlayers
	Access:    	public
	Parameter: 	const ushort _WeeksLessThan
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoCountRecentlySignedPlayers(const ushort _WeeksLessThan)
{
    ushort iRet = 0;
    for (int num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
            if (GetPlayer(num).GetWeeksHere() < _WeeksLessThan)
			{
				iRet++;
			}
        }
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CountStarPlayers
	Access:    	public
	Parameter: 	const byte _MinStarPlayerSkill
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoCountStarPlayers(const byte _MinStarPlayerSkill)
{
	ushort Count = 0;
	for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
		if (m_listSquadID[num] != NOPLAYER)
		{
			Count += (GetPlayer(num).GetSkills().GetOverallSkill() >= _MinStarPlayerSkill);
		}
	}
	return Count;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::AddLeadingDiscipinaryPoints
	Access:    	public
	Parameter: 	const ushort _CurrentClubID
	Parameter: 	const int _MinDiscPoints
	Parameter: 	CLeadingDisciplinaryList* _pList
	Returns:   	void
	Qualifier:
	Purpose:	Add all the players from this club that have more
				than _MinDiscPoints
------------------------------------------------------------------------------*/
void CPlayerSquad::AddLeadingDiscipinaryPoints(const ClubID _CurrentClubID, const int _MinDiscPoints, CLeadingDisciplinaryList* _pList)
{
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			if (GetPlayer(num).GetDisciplinaryPointsThisSeason() > _MinDiscPoints)
			{
				_pList->Add(FD_ENGINE_NEW CLeadingDiscipinaryPoints(m_listSquadID[num], _CurrentClubID,
														   GetPlayer(num).GetNumberBookingsThisSeason(),
														   GetPlayer(num).GetNumberSentOffThisSeason())
				);
			}
		}
    }
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::AddLeadScorers
	Access:    	public
	Parameter: 	const ushort _CurrentClubID
	Parameter: 	const int _MinGoals
	Parameter: 	CLeadingGoalScorerList* _pList
	Returns:   	void
	Qualifier:
	Purpose:	Add all the players from this club that have scored more
				than MinGoals
------------------------------------------------------------------------------*/
void CPlayerSquad::AddLeadScorers(const ClubID _CurrentClubID, const byte _MinGoals, CLeadingGoalScorerList* _pList)
{
	byte LGoals, CGoals;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			if (GetPlayer(num).GetSkills().GetPosition() > GOALKEEPER)
			{
   				LGoals = GetPlayer(num).GetLeagueSeasonGoal();
   				CGoals = GetPlayer(num).GetCupGoalsScoredThisSeason();

				if (LGoals + CGoals > _MinGoals)
				{
					_pList->Add(FD_ENGINE_NEW CLeadingGoalScorer(m_listSquadID[num], _CurrentClubID, LGoals, CGoals));
				}
			}
		}
    }
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountListedPlayers
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:	Count the number of players on the transfer list or out of
				contract
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoCountListedPlayers()
{
    ushort iRet = 0;
	for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			CPlayer& thePlayer = GetPlayer(num);
            if (thePlayer.IsTransferListed())
			{
                iRet++;
			}
        }
		else
		{
			// All players checked
			break;
		}
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::RandomPlayingGoalkeeperSurName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayerSquad::RandomPlayingGoalkeeperSurName()
{
    CString sRet;
    CPlayer* pPlayer;
    for (uint count = 0; count < PLAYERSINTEAM; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
            pPlayer = &GetPlayer(count);
			EXT_ASSERT_POINTER(pPlayer, CPlayer);
            if (pPlayer->GetSkills().GetPosition() == GOALKEEPER && pPlayer->IsPlaying() == true)
			{
                sRet = pPlayer->GetSurname();
                break;
            }
        }
    }
    return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::AverageSquadMorale
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerSquad::AverageSquadMorale()
{
	int iRet = 0;
    for (int count = 0; count < PLAYERSINSQUAD; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
            iRet += GetPlayer(count).GetMorale();
        }
    }
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::RandomPlayingSquadNumber
	Access:    	public
	Parameter: 	const ePlayerPosition _Posn
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::RandomPlayingSquadNumber(const ePlayerPosition _Posn/* = MAXPLAYERPOSITIONS*/)
{
	EXT_ASSERT(_Posn <= MAXPLAYERPOSITIONS);
    CNumericArray<ushort> ThePlayers;
    for (uint ListPos = 0; ListPos < PLAYERSINTEAM; ListPos ++)
	{
		if (m_listSquadID[ListPos] != NOPLAYER)
		{
            if ((GetPlayer(ListPos).GetSkills().GetPosition() == _Posn || _Posn == MAXPLAYERPOSITIONS) && GetPlayer(ListPos).IsPlaying() == true)
			{
                ThePlayers.Add(ListPos);
            }
		}
	}
    return ThePlayers.RandomValue();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindWorstPlayingOutfieldPlayer
	Access:    	public
	Parameter: 	const int _WorstValue
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoFindWorstPlayingOutfieldPlayer(const int _WorstValue)
{
    CPlayerIDList ThePlayers;
    ThePlayers.DoBuildFromTeamPlayingOutField(*this, WorldData().GetPlayerList());
	return DoFindSquadNumber(ThePlayers.DoFindWorstPlayer(_WorstValue));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindWorstPlayingOutfieldPlayerPosition
	Access:    	public
	Parameter: 	const int _WorstValue
	Parameter: 	ePlayerPosition _Position
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoFindWorstPlayingOutfieldPlayerPosition(const int _WorstValue, ePlayerPosition _Position)
{
	ushort iRet = NOPLAYER;
    CPlayerIDList ThePlayers;
    ThePlayers.DoBuildFromTeamPlayingPosn(*this, WorldData().GetPlayerList(), _Position);
	return DoFindSquadNumber(ThePlayers.DoFindWorstPlayer(_WorstValue));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::RandomPlayingPositionSurName
	Access:    	public
	Parameter: 	const ePlayerPosition _ePosition
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayerSquad::RandomPlayingPositionSurName(const ePlayerPosition _ePosition)
{
    CPlayerIDList ThePlayers;
    ThePlayers.DoBuildFromTeamPlayingPosn(*this, WorldData().GetPlayerList(), _ePosition);
    return ThePlayers.RandomSurName();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::RandomPlayingVictimSurName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayerSquad::RandomPlayingVictimSurName()
{
    CPlayerIDList ThePlayers;
    ThePlayers.DoBuildFromTeamPlayingPosn(*this, WorldData().GetPlayerList(), ATTACKER);
    return ThePlayers.RandomSurName();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::RandomPlayingFoulerSurName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayerSquad::RandomPlayingFoulerSurName()
{
    CPlayerIDList ThePlayers;
    ThePlayers.DoBuildFromTeamPlayingPosn(*this, WorldData().GetPlayerList(), ATTACKER);
    return ThePlayers.RandomSurName();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::FindPlayerID
	Access:    	public
	Parameter: 	CPlayer* _pPlayer
	Returns:   	ushort
	Qualifier:
	Purpose:	Returns position in the squad of the specified player
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoFindPlayerID(CPlayer* _Player)
{
	return m_listSquadID[DoFindSquadNumber(_Player)];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindSquadNumber
	Access:    	public
	Parameter: 	const ushort _PlayerID
	Returns:   	ushort - position in the squad of the specified player
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoFindSquadNumber(const ushort _PlayerID)
{
	EXT_ASSERT(_PlayerID != NOPLAYER && _PlayerID < WorldData().GetPlayerList().GetSize()); //, "PlayerId Out Of Range");
	ushort pNum = NOPLAYER;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] == _PlayerID)
		{
			pNum = num;
			break;
		}
	}
	EXT_ASSERT(pNum != NOPLAYER); //, "Player not found");
	return pNum;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::FindSquadNumber
	Access:    	public
	Parameter: 	CPlayer* _pPlayer
	Returns:   	ushort - position in the squad of the specified player
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoFindSquadNumber(CPlayer* _pPlayer)
{
	ushort pNum = NOPLAYER;
	for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
		if (m_listSquadID[num] != NOPLAYER)
		{
			if (_pPlayer == &GetPlayer(num))
			{
				pNum = num;
				break;
			}
		}
	}
	EXT_ASSERT(pNum != NOPLAYER);
	return pNum;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountGoodFlairDefendersPlaying
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCountGoodFlairDefendersPlaying()
{
    CPlayer* pPlayer;
	byte bRet = 0;
    for (uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
        if (m_listSquadID[LoopCount] != NOPLAYER)
		{
            pPlayer = &GetPlayer(LoopCount);
			if (pPlayer->GetSkills().GetPosition() == DEFENDER && pPlayer->IsPlaying() == true)
			{
				bRet += (pPlayer->GetSkills().GetFlair() > GOODFLAIR);
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountGoodFlairMidfieldersPlaying
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCountGoodFlairMidfieldersPlaying()
{
    CPlayer* pPlayer;
	byte bRet = 0;
    for (uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
        if (m_listSquadID[LoopCount] != NOPLAYER)
		{
            pPlayer = &GetPlayer(LoopCount);
			if (pPlayer->GetSkills().GetPosition() == MIDFIELDER && pPlayer->IsPlaying() == true)
			{
				bRet += (pPlayer->GetSkills().GetFlair() > GOODFLAIR);
			}
		}
	}
	return bRet;
}


byte CPlayerSquad::DoCountGoodFlairAttackersPlaying()
{
    CPlayer* pPlayer;
	byte bRet = 0;
    for (uint LoopCount = 0; LoopCount < PLAYERSINTEAM; LoopCount++)
	{
        if (m_listSquadID[LoopCount] != NOPLAYER)
		{
            pPlayer = &GetPlayer(LoopCount);
			if (pPlayer->GetSkills().GetPosition() == ATTACKER && pPlayer->IsPlaying() == true)
			{
				bRet += (pPlayer->GetSkills().GetFlair() > GOODFLAIR);
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoDeleteAllPlayers
	Access:    	public
	Parameter: 	const ushort _nClubID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoDeleteAllPlayers(const ClubID _nClubID)
{
    for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
        if (m_listSquadID[LoopCount] != NOPLAYER)
		{
            GetPlayer(LoopCount).DoRetired(_nClubID);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::IsInTeam
	Access:    	public
	Parameter: 	const ushort _PlayerID
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::IsInTeam(const ushort _PlayerID)
{
	if (_PlayerID == NOPLAYER)
	{
		return NOPLAYER;
	}
    for (uint n = 0; n < PLAYERSINTEAM; n++)
	{
		if (_PlayerID == m_listSquadID[n])
		{
			return n;
		}
	}
	return NOPLAYER;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindFitPlayerID
	Access:    	public
	Parameter: 	const ushort _FirstPlayer
	Parameter: 	const ushort _LastPlayer
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoFindFitPlayerID(const ushort _FirstPlayer, const ushort _LastPlayer)
{
	ushort iRet = NOPLAYER;
	while(iRet == NOPLAYER)
	{
		iRet = m_listSquadID[_FirstPlayer + RandomNumber.IntLessThan((_LastPlayer - _FirstPlayer) + 1)];
		if (iRet != NOPLAYER)
		{
			if (WorldData().GetPlayer(iRet)->IsAvailableForMatch() != true)
			{
				iRet = NOPLAYER;
			}
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoSelectSpecialPlayers
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoSelectSpecialPlayers()
{
	ushort PlayerIDToUse = NOPLAYER;
	if (IsInTeam(m_CaptainID) == NOPLAYER)
	{
		// Try to find a reasonably mature player
		for (uint LoopCount = 0; LoopCount < 8; LoopCount++)
		{
			PlayerIDToUse = DoFindFitPlayerID(1, 7);
			if (WorldData().GetPlayer(PlayerIDToUse)->GetAge() > 24)
			{
				break;
			}
			PlayerIDToUse = NOPLAYER;		// Discard player
		}
		if (PlayerIDToUse == NOPLAYER)
		{
			PlayerIDToUse = DoFindFitPlayerID(1, 7);
		}
//  		DEBUG_OUTPUT(("%d, Captain %s"), (PlayerIDToUse, WorldData().GetPlayer(PlayerIDToUse)->GetName()));
		m_CaptainID = PlayerIDToUse;
	}
	if (IsInTeam(m_PenaltyTakerID) == NOPLAYER)
	{
		m_PenaltyTakerID = DoFindFitPlayerID(5, 10);
	}
	if (IsInTeam(m_CornerTakerID) == NOPLAYER)
	{
		m_CornerTakerID = DoFindFitPlayerID(2, 6);
	}
	if (IsInTeam(m_FreeKickTakerID) == NOPLAYER)
	{
		m_FreeKickTakerID = DoFindFitPlayerID(3, 9);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::BeenPromoted
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::BeenPromoted()
{
	// Increase player's skills
    for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
		if (m_listSquadID[LoopCount] != NOPLAYER)
		{
            GetPlayer(LoopCount).BeenPromoted();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateWageBill
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerSquad::DoCalculateWageBill()
{
	int totalplaywage = 0;
    for (int LoopCount = 0 ; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
		if (m_listSquadID[LoopCount] != NOPLAYER)
		{
       		totalplaywage += GetPlayer(LoopCount).GetWage();
		}
    }
	return totalplaywage;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoSackPlayersIfContractExpired
	Access:    	public
	Parameter: 	const bool _HumanManager
	Parameter: 	CCountry* _pCountry
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoSackPlayersIfContractExpired(const bool _HumanManager, CCountry* _pCountry)
{
    CPlayer* pPlayer;
	bool bListHim;
	CString str;
    for (uint n = 0; n < PLAYERSINSQUAD; n++)
	{
		bListHim = false;
        if (m_listSquadID[n] != NOPLAYER)
		{
            pPlayer = &GetPlayer(n);
			EXT_ASSERT_POINTER(pPlayer, CPlayer);
			if (pPlayer->GetContractEndDate().SeasonsRemaining() == 0)
			{
				if (_HumanManager == true)
				{
					const uint uiAskingPrice = WorldData().GetTransferListManager().GetAskingPrice(pPlayer->DoFindID());
					if (pPlayer->QualifiesForBosmanFreeTransfer() == true)
					{
                  str.Format(IDS_PLAYERNOCONTRACTBOSMAN, CNewsItem::PreprocessPlayerName(pPlayer).CStr(), uiAskingPrice);
					}
					else
					{
                  str.Format(IDS_PLAYERNOCONTRACTNOBOSMAN, CNewsItem::PreprocessPlayerName(pPlayer).CStr(), uiAskingPrice);
					}
					UserMessageBox(str, OKBOX);
					bListHim = true;
				}
				else
				{
					bListHim = IsPlayerOkToLeave(pPlayer);
				}
				if (bListHim == true)
				{
					pPlayer->DoTransferList(pPlayer->GetSuggestedAskingPrice(*_pCountry), *_pCountry);
				}
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::IsPlayerOkToLeave
	Access:    	public
	Parameter: 	CPlayer* _pPlayer
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerSquad::IsPlayerOkToLeave(CPlayer* _pPlayer)
{
    // Not if injured or on loan
	if (_pPlayer->IsTransferListed() == false)
	{
		return false;
	}
	// Not if squad too small
	if (DoCountAvailablePlayers() + GetPlayersInOnLoan() < MINSQUADSIZE + 2)
	{
		return false;
	}
	// If didn't play much last season and over 23
	if (_pPlayer->GetNumberAppearancesThisSeason() < 4 && _pPlayer->GetAge())
	{
		return true;
	}
	// If got too many in this position
    if (DoCountAllPlayersPosn(_pPlayer->GetSkills().GetPosition()) > MaximumPlayersInPosition(_pPlayer->GetSkills().GetPosition()))
	{
		return true;
	}
	// If squad too big
    if (TotalSquadSize() > PLAYERSINSQUAD - 2)
	{
		return true;
	}
	// If short of money
	if (_pPlayer->GetClub().GetCashBalance() < 50000)
	{
		return true;
	}
	// If far to good for club
	if (_pPlayer->GetClub().GetPopularity().GetMaximumPlayerSkill())
	{
		return true;
	}
	// Random chance
	if (RandomNumber.IntLessThan(8) > 2)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::IsPlayerInSquad
	Access:    	public
	Parameter: 	const ushort _PlayerID
	Returns:   	bool
	Qualifier:
	Purpose:	Is the requested player at this club?
------------------------------------------------------------------------------*/
bool CPlayerSquad::IsPlayerInSquad(const ushort _PlayerID)
{
	EXT_ASSERT(_PlayerID != NOPLAYER && _PlayerID < WorldData().GetPlayerList().GetSize()); //, "PlayerId Out Of Range");
    for (int count = 0; count < PLAYERSINSQUAD; count ++)
	{
        if (m_listSquadID[count] == _PlayerID)
		{
            return true;
        }
    }
    return false;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindPlayerSquadNumber
	Access:    	public
	Parameter: 	const ushort _PlayerID
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoFindPlayerSquadNumber(const ushort _PlayerID)
{
	EXT_ASSERT(_PlayerID != NOPLAYER && _PlayerID < WorldData().GetPlayerList().GetSize()); //, "PlayerId Out Of Range");
	ushort nPlayerSquadNumber = wNOTFOUND;
    for (uint count = 0; count < PLAYERSINSQUAD; count ++)
	{
        if (m_listSquadID[count] == _PlayerID)
		{
			nPlayerSquadNumber = count;
			break;
        }
    }
	EXT_ASSERT(nPlayerSquadNumber != wNOTFOUND); //, "Player not found in squad");
    return nPlayerSquadNumber;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::IsPlayerInSquad
	Access:    	public
	Parameter: 	CPlayer* _pPlayer
	Returns:   	bool
	Qualifier:
	Purpose:	Is the requested player at this club?
------------------------------------------------------------------------------*/
bool CPlayerSquad::IsPlayerInSquad(CPlayer* _Player)
{
    EXT_ASSERT_POINTER(_Player, CPlayer);
    for (uint count = 0; count < PLAYERSINSQUAD; count ++)
	{
		if (m_listSquadID[count] != NOPLAYER)
		{
            if (&GetPlayer(count) == _Player)
			{
				return true;
	        }
		}
    }
    return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateMaximumPlayerSkill
	Access:    	public
	Parameter: 	CClub* _Club
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCalculateMaximumPlayerSkill(CClub* _Club)
{
    EXT_ASSERT_POINTER(_Club, CClub);
	return _Club->GetPopularity().GetMaximumPlayerSkill(_Club->GetDivisionStrengthID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateMinimumPlayerSkill
	Access:    	public
	Parameter: 	CClub* _Club
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCalculateMinimumPlayerSkill(CClub* _Club)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	return _Club->GetPopularity().GetMinimumPlayerSkill(_Club->GetDivisionStrengthID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCreateSquad
	Access:    	public
	Parameter: 	CClub* _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoCreateSquad(CClub* _Club)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	static const ePlayerPosition Positions[] =
	{
		GOALKEEPER,
		DEFENDER, DEFENDER, DEFENDER, DEFENDER,
		MIDFIELDER, MIDFIELDER, MIDFIELDER,
		ATTACKER, ATTACKER, ATTACKER,
		GOALKEEPER,
		DEFENDER, DEFENDER, DEFENDER,
		MIDFIELDER,	MIDFIELDER, MIDFIELDER, MIDFIELDER,
		ATTACKER, ATTACKER, ATTACKER
	};
	for (uint LoopCount =0; LoopCount < 22; LoopCount++)
	{
		if (m_listSquadID[LoopCount] == NOPLAYER)
		{
			SetSquad(LoopCount, WorldData().GetPlayerList().AddNew(_Club, Positions[LoopCount]));
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::MinimumAvailablePlayersInPosition
	Access:    	public const
	Parameter: 	const ePlayerPosition _ePosn
	Returns:   	uint
	Qualifier: 	const
	Purpose:	What is the minimum number of players for the specified zone
				if a match is imminent
------------------------------------------------------------------------------*/
uint CPlayerSquad::MinimumAvailablePlayersInPosition(const ePlayerPosition _ePosn) const
{
	EXT_ASSERT(_ePosn >= GOALKEEPER || _ePosn <= ATTACKER);
	static const uint MinPlayersInPosition[] =
	{
		// GDMA
		2, 4, 4, 4
	};
	return MinPlayersInPosition[_ePosn];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::MaximumPlayersInPosition
	Access:    	public const
	Parameter: 	const ePlayerPosition _ePosn
	Returns:   	uint
	Qualifier: 	const
	Purpose:	What is the suggested maximum number of players for the
				specified zone
------------------------------------------------------------------------------*/
uint CPlayerSquad::MaximumPlayersInPosition(const ePlayerPosition _ePosn) const
{
	EXT_ASSERT(_ePosn >= GOALKEEPER || _ePosn <= ATTACKER);
	static const uint TotalPlayersInPosition[] =
	{
		// GDMA
		3, 8, 8, 5
	};
	return TotalPlayersInPosition[_ePosn];
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::CanAffordToLosePlayer
	Access:    	public
	Parameter: 	CPlayer* _pPlayer
	Parameter: 	const bool _LoanPlayer
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerSquad::CanAffordToLosePlayer(CPlayer* _Player, const bool _LoanPlayer /*= false*/)
{
    EXT_ASSERT_POINTER(_Player, CPlayer);
	if (DoCountAvailablePlayers() < MINSQUADSIZE)
	{
		return false;
	}
	if (DoCountAvailablePlayersPosn(_Player->GetSkills().GetPosition()) <= MinimumAvailablePlayersInPosition(_Player->GetSkills().GetPosition()))
	{
		return false;
	}
	if (_LoanPlayer == true && m_PlayersOutOnLoan > 2)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::TakeAllPlayersOffTransferList
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoTakeAllPlayersOffTransferList()
{
	CPlayer* pPlayer;
	for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
		if (m_listSquadID[LoopCount] != NOPLAYER)
		{
			pPlayer = &GetPlayer(LoopCount);
			EXT_ASSERT_POINTER(pPlayer, CPlayer);
			if (WorldData().GetTransferListManager().IsTransferListed(pPlayer->DoFindID()) == true)
			{
				pPlayer->DoTakeOffTransferList();
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::FreeSquadNumber
	Access:    	public
	Parameter: 	void
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::FreeSquadNumber()
{
    for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount++)
	{
        if (m_listSquadID[LoopCount] == NOPLAYER)
		{
            return LoopCount;
		}
    }
	return NOPLAYER;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoNewSeason
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoNewSeason()
{
	m_PlayersInOnLoanThisSeason = 0;
    for (uint LoopCount = 0; LoopCount < PLAYERSINSQUAD; LoopCount ++)
	{
		if (m_listSquadID[LoopCount] != NOPLAYER)
		{
            GetPlayer(LoopCount).DoNewSeason();
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::PromoteYouth
	Access:    	public
	Parameter: 	CContract* _Contract
	Parameter: 	CYouth* _pYouth
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::PromoteYouth(CContract* _Contract, CYouth* _pYouth)
{
	// Find squad number
   ushort SquadNumber = FreeSquadNumber();
   EXT_ASSERT(SquadNumber < PLAYERSINSQUAD);
   // Transfer youth data to player
   SetSquad(SquadNumber, WorldData().GetPlayerList().TransferFromYouth(_pYouth, _Contract));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFansReactionToSale
	Access:    	public
	Parameter: 	CPlayer& _Player
	Parameter: 	const uint _Cost
	Returns:   	void
	Qualifier:
	Purpose:	Check if just sold a top player(within club) then send message
				from fans expressing their feelings
------------------------------------------------------------------------------*/
void CPlayerSquad::DoFansReactionToSale(CPlayer& _Player, const uint _Cost)
{
	CFansReaction::DoFansReactionToSale(_Player, _Cost, DoRankInSquad(_Player), DoRankInPosition(_Player));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFansReactionToSigning
	Access:    	public
	Parameter: 	CPlayer& _Player
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayerSquad::DoFansReactionToSigning(CPlayer& _Player)
{
	return CFansReaction::DoFansReactionToSigning(_Player, DoRankInSquad(_Player), DoRankInPosition(_Player));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::RatingPositionInSquad
	Access:    	public
	Parameter: 	CPlayer& _Player
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoRankInSquad(CPlayer& _Player)
{
    CPlayerIDList ThePlayers;
	ThePlayers.DoBuildFromSquad(*this, WorldData().GetPlayerList());
	ThePlayers.DoSortByOverallSkill();
	return ThePlayers.indexOf(_Player.DoFindID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::RatingPositionInPosition
	Access:    	public
	Parameter: 	CPlayer& _Player
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoRankInPosition(CPlayer& _Player)
{
    CPlayerIDList ThePlayers;
	ThePlayers.DoBuildFromSquad(*this, WorldData().GetPlayerList());
	ThePlayers.DoRemoveNotPosition(_Player.GetSkills().GetPosition());
	ThePlayers.DoSortByOverallSkill();
	return ThePlayers.indexOf(_Player.DoFindID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::NextPlayerNumber
	Access:    	public
	Parameter: 	const ushort _CurrPlayer
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::NextPlayerNumber(const ushort _CurrPlayer)
{
	ushort iRet = _CurrPlayer + 1;
	if (iRet == PLAYERSINSQUAD)
	{
		iRet = 0;
	}
	while(m_listSquadID[iRet] == NOPLAYER)
	{
		iRet++;
		if (iRet == PLAYERSINSQUAD)
		{
			iRet = 0;
		}
	}
	return iRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::BuildTeamTempersArray
	Access:    	public
	Parameter: 	CNumericArray<byte>* _PlayerTempers
	Returns:   	void
	Qualifier:
	Purpose:	Calculate the cumulative aggression factor of the team
------------------------------------------------------------------------------*/
void CPlayerSquad::DoBuildTeamTempersArray(CNumericArray<byte>* _PlayerTempers)
{
    CPlayer* pDiscPlayer;
	byte PositionAdjuster[] = { 0, 10, 5, 5 };
    for (uint num = 0; num < PLAYERSINTEAM; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
	        pDiscPlayer = &GetPlayer(num);
		    _PlayerTempers->Add(pDiscPlayer->GetTemperament() + PositionAdjuster[pDiscPlayer->GetSkills().GetPosition()]);	// Transfer to single dimension array
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::OffListAllPlayersInPosition
	Access:    	public
	Parameter: 	const ePlayerPosition _ePosn
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::OffListAllPlayersInPosition(const ePlayerPosition _ePosn)
{
    for (int num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			if (GetPlayer(num).GetSkills().GetPosition() == _ePosn || _ePosn == MAXPLAYERPOSITIONS)
			{
				GetPlayer(num).DoTakeOffTransferList();
			}
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::OnOffListPlayers
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::OnOffListPlayers()
{
	if (DoCountAvailablePlayers() < MINSQUADSIZE)
	{
		OffListAllPlayersInPosition(MAXPLAYERPOSITIONS);
		return;
	}

    uint HowMany;
	for (int LoopCount = 0; LoopCount < MAXPLAYERPOSITIONS; LoopCount++)
	{
		HowMany = DoCountAvailablePlayersPosn(static_cast<ePlayerPosition>(LoopCount));
		if (HowMany < MinimumAvailablePlayersInPosition(static_cast<ePlayerPosition>(LoopCount)))
		{
			OffListAllPlayersInPosition(static_cast<ePlayerPosition>(LoopCount));
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::PenaltyTakerSquadNumber
	Access:    	public
	Parameter: 	void
	Returns:   	ushort - the squad number of the penalty taker
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::PenaltyTakerSquadNumber()
{
	ushort iRet = wNOTFOUND;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] == m_PenaltyTakerID)
		{
			iRet = num;
			break;
		}
	}
	EXT_ASSERT(iRet != wNOTFOUND);
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::AllowInjuries
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerSquad::AllowInjuries()
{
	if (DoCountAvailablePlayers() < MINSQUADSIZE + 2)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::AllowToHirePlayer
	Access:    	public
	Parameter: 	CPlayer* _pPlayer
	Parameter: 	const bool _bShowMessage
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayerSquad::AllowToHirePlayer(CPlayer* _Player, const bool _ShowMessage /*= false*/)
{
	EXT_ASSERT_POINTER(_Player, CPlayer);
	static const uint MaxPlayerPosition[] =
	{
		MAXIMUMGOALKEEPERS,
		MAXIMUMDEFENDERS,
		MAXIMUMMIDFIELDERS,
		MAXIMUMATTACKERS,
	};
	if (TotalSquadSize() >= PLAYERSINSQUAD)
	{
		if (_ShowMessage == true)
		{
      UserMessageBox(IDS_SQUADFULL);
			return NOTFOUND;
		}
		else
		{
      return IDS_SQUADFULL;
		}
	}

	if (DoCountAllPlayersPosn(_Player->GetSkills().GetPosition()) >= MaxPlayerPosition[_Player->GetSkills().GetPosition()])
	{
		if (_ShowMessage == true)
		{
      UserMessageBox(IDS_POSITIONFULL);
			return NOTFOUND;
		}
		else
		{
      return IDS_POSITIONFULL;
		}
	}
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoRequirePlayerPosition
	Access:    	public
	Parameter: 	const ePlayerPosition _Position
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerSquad::DoRequirePlayerPosition(const ePlayerPosition _Position)
{
	if (DoCountAllPlayersPosn(_Position) < MaximumPlayersInPosition(_Position))
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCountFitPlayers
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoCountFitPlayers()
{
	ushort iRet = 0;
	for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
		if (GetPlayer(num).IsInjured() == false)
		{
			iRet++;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetFitPlayer
	Access:    	public
	Parameter: 	const ushort _SquadNum
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CPlayerSquad::GetFitPlayer(const ushort _SquadNum)
{
	CPlayer* pPlayer = NULL;
	ushort iRet = 0;
	for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
		if (GetPlayer(num).IsInjured() == false)
		{
			if (iRet == _SquadNum)
			{
				pPlayer = &GetPlayer(num);
				break;
			}
			iRet++;
		}
	}
	EXT_ASSERT_POINTER(pPlayer, CPlayer);
	return pPlayer;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::NextInjuredPlayer
	Access:    	public
	Parameter: 	const ushort _CurrentPlayer
	Returns:   	ushort
	Qualifier:
	Purpose:	Find the next injured player in the squad
------------------------------------------------------------------------------*/
ushort CPlayerSquad::NextInjuredPlayer(const ushort _CurrentPlayer)
{
	int iRet = _CurrentPlayer;
	do
	{
		iRet++;
        if (iRet < 0 || iRet > PLAYERSINSQUAD - 1)
		{
			iRet = 0;
		}

		if (m_listSquadID[iRet] != NOPLAYER)
		{
            if (GetPlayer(iRet).IsInjured() == true)
			{
				return iRet;
			}
		}
	}
	while(1);
	return NULL;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoMakeFitPlayer
	Access:    	public
	Parameter: 	const ePlayerPosition _Posn
	Returns:   	bool
	Qualifier:
	Purpose:	If there is a player nearly fit then make him fit
				Best only used on non human managed clubs.
------------------------------------------------------------------------------*/
bool CPlayerSquad::DoMakeFitPlayer(const ePlayerPosition _Posn)
{
	bool bRet = false;
    for (uint num = MINSQUADSIZE + 1; num < PLAYERSINSQUAD; num ++)
	{
		if (m_listSquadID[num] != NOPLAYER)
		{
            if (GetPlayer(num).GetSkills().GetPosition() == _Posn && GetPlayer(num).HowLongUnableToPlay() < 2)
			{
				GetPlayer(num).DoMakeFit();
				bRet = true;
				break;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoGivePlayerFreeTransferToClub
	Access:    	public
	Parameter: 	const ePlayerPosition _ePosn
	Parameter: 	CClub* _pClub
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerSquad::DoGivePlayerFreeTransferToClub(const ePlayerPosition _ePosn, CClub* _pClub)
{
	EXT_ASSERT_POINTER(_pClub, CClub); //, "Free transfer to no club");
	CContract ContractRequired;
    CPlayer* pPlayer;
	bool bRet = false;
    for (int num = MINSQUADSIZE + 1; num < PLAYERSINSQUAD; num++)
	{
		if (m_listSquadID[num] != NOPLAYER)
		{
            pPlayer = &GetPlayer(num);
			EXT_ASSERT_POINTER(pPlayer, CPlayer);
			if (pPlayer->GetAge() > 38)
			{
				continue;
			}
            if (pPlayer->GetContractRenewalStatus() != RETIRING &&
				pPlayer->GetAge() > 30 &&
				pPlayer->GetSkills().GetPosition() == _ePosn &&
				pPlayer->IsAvailableForMatch() == true &&
				pPlayer->GetWeeksHere() > MINWEEKSATCLUB &&
				pPlayer->IsOnLoan() == false)
			{
				CClubTransferOffer TheOffer(pPlayer, 0, _pClub);
				pPlayer->GetClub().DoSoldPlayer(*pPlayer, TheOffer, ContractRequired, false);	// Sell the player
				bRet = true;
				break;
			}
		}
    }
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetAllTransferListed
	Access:    	public
	Parameter: 	const ushort _ClubID
	Parameter: 	CPlayerSearch& _PlayerSearch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::GetAllTransferListed(const ClubID _ClubID, CPlayerSearch& _PlayerSearch)
{
  for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
    if (m_listSquadID[num] != NOPLAYER)
		{
      if (GetPlayer(num).ValidSearchListed(_PlayerSearch) == true)
			{
				_PlayerSearch.DoAddToList(m_listSquadID[num], _ClubID, GetPlayer(num).GetValuation(WorldData().GetCurrentUser()->GetCountry()));
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::PreviousPlayerNumber
	Access:    	public
	Parameter: 	const ushort _CurrPlayer
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::PreviousPlayerNumber(const ushort _CurrPlayer)
{
	ushort iRet = min((ushort)(_CurrPlayer - 1), PLAYERSINSQUAD - 1);
	while(m_listSquadID[iRet] == NOPLAYER)
	{
		iRet--;
		if (iRet < 0)
		{
			iRet = PLAYERSINSQUAD - 1;
		}
	}
	return iRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::SelectNonUserControlledTeam
	Access:    	public
	Parameter: 	const byte _NumSubs
	Parameter: 	CClub& _rClub
	Returns:   	void
	Qualifier:
	Purpose:	Select the strongest team for the next match
------------------------------------------------------------------------------*/
void CPlayerSquad::DoSelectNonUserControlledTeam(const byte _NumSubs, CClub& _Club, const bool _GenerateNewIfShort /*= true*/)
{
	if (WorldData().IsFastTest() == false)
	{
		EXT_ASSERT(_Club.IsUserControlled() == false);
	}
	if (_Club.GetCurrentTacticsID() >= NUMFORMATIONS)
	{
		_Club.DoSelectRandomTactics();
	}

	if (_GenerateNewIfShort == true)
	{
		ushort AvailablePlayers = DoCountAvailablePlayers();
		if (AvailablePlayers < (PLAYERSINTEAM + _NumSubs))
		{
			DoGenerateYoungPlayers((PLAYERSINTEAM + _NumSubs) - AvailablePlayers, _Club);
		}
	}
	DoSelectTeam(_NumSubs, _Club);
	DoSelectSpecialPlayers();
}


/*------------------------------------------------------------------------------
	Method:   	GetPlayer
	Access:    	public
	Parameter: 	const ushort _SquadNumber
	Returns:   	CPlayer&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer& CPlayerSquad::GetPlayer(const ushort _SquadNumber)
{
	EXT_ASSERT(_SquadNumber != NOPLAYER && _SquadNumber < PLAYERSINSQUAD);
	return WorldData().GetPlayerList()[m_listSquadID[_SquadNumber]];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoSelectTeam
	Access:    	public
	Parameter: 	const byte _NumSubs
	Parameter: 	CClub& _rClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoSelectTeam(const byte _NumSubs, CClub& _Club)
{
	const ushort uiNumPlayers = TotalSquadSize();
	if (uiNumPlayers == 0)
	{
		return;
	}

	DoFindBestPlayer(0, GOALKEEPER);			// Goalkeepers, 1 in team
	for (uint nCount = 1; nCount < PLAYERSINTEAM; nCount++)
	{
		if (nCount >= uiNumPlayers)
		{
			break;
		}
		DoFindBestPlayer(nCount, _Club.GetTactics().PlayerPosition(nCount - 1));
	}

	ushort BestNumber, Count;
	// Now the substitutes
	for (uint PlayerCount = PLAYERSINTEAM; PlayerCount < PLAYERSINTEAM + _NumSubs - 1; PlayerCount++)
	{
		if ((m_listSquadID[PlayerCount] == NOPLAYER) ||
			(WorldData().GetPlayerList()[m_listSquadID[PlayerCount]].IsAvailableForMatch() == false) ||
			(WorldData().GetPlayerList()[m_listSquadID[PlayerCount]].GetSkills().GetPosition() == GOALKEEPER))
		{
			BestNumber = PlayerCount;
			Count = 0;
			while((Count < 10) &&
				  ((m_listSquadID[BestNumber] == NOPLAYER) ||
				   (WorldData().GetPlayerList()[m_listSquadID[BestNumber]].IsAvailableForMatch() == false) ||
				   (WorldData().GetPlayerList()[m_listSquadID[BestNumber]].GetSkills().GetPosition() == GOALKEEPER)))
			{
				do
				{
					BestNumber = PlayerCount + RandomNumber.IntLessThan(PLAYERSINSQUAD - (PLAYERSINTEAM + _NumSubs) - 1);
					Count++;
				}
				while((m_listSquadID[BestNumber] == NOPLAYER) && (Count < 10));
			}

			if (Count < 10)
			{
				DoSwapPlayers(PlayerCount, BestNumber);
			}
		}
	}
	int FirstSquadPosition = PLAYERSINTEAM + _NumSubs - 1;
 	int Attempts = 0;
	CPlayer* pPlayer = NULL;
	if (m_listSquadID[FirstSquadPosition] != NOPLAYER)
	{
		pPlayer = &GetPlayer(FirstSquadPosition);
	}
	if (pPlayer == NULL || pPlayer->IsAvailableForMatch() == false || pPlayer->GetSkills().GetPosition() != GOALKEEPER)
	{
		BestNumber = FirstSquadPosition;
		while((Attempts < 10) &&
			  (((this->GetSquad(BestNumber) != NOPLAYER) && (GetPlayer(BestNumber).IsAvailableForMatch() == false)) ||
			  ((pPlayer != NULL) && (pPlayer->GetSkills().GetPosition() != GOALKEEPER))))
		{
			do
			{
				Attempts++;
				BestNumber = FirstSquadPosition + RandomNumber.IntLessThan(PLAYERSINSQUAD - FirstSquadPosition - 1);
			}
			while(Attempts < 10 && m_listSquadID[BestNumber] == NOPLAYER);
			if (m_listSquadID[BestNumber] != NOPLAYER)
			{
				pPlayer = &GetPlayer(BestNumber);
			}
		}
		if (m_listSquadID[BestNumber] != NOPLAYER && FirstSquadPosition != BestNumber)
		{
			DoSwapPlayers(FirstSquadPosition, BestNumber);
		}
	}
	if (Attempts == 10 || m_listSquadID[FirstSquadPosition] == NOPLAYER)
	{
		// Need to ensure that we don't go into an infinite loop.
		bool bHasAnyMorePlayers = false;
		for (int i = FirstSquadPosition; i < PLAYERSINSQUAD; i++)
		{
			if (GetSquad(i) != NOPLAYER)
			{
				bHasAnyMorePlayers = true;
				break;
			}
		}

		if (bHasAnyMorePlayers == true)
		{
			DoFindBestPlayer(FirstSquadPosition, static_cast<ePlayerPosition>(DEFENDER + RandomNumber.IntLessThan(2)));
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoSwapPlayers
	Access:    	public
	Parameter: 	const ushort _playernum
	Parameter: 	const ushort _playernum1
	Returns:   	void
	Qualifier:
	Purpose:	Swap 2 player numbers
------------------------------------------------------------------------------*/
void CPlayerSquad::DoSwapPlayers(const ushort _playernum, const ushort _playernum1)
{
    EXT_ASSERT(_playernum != NOPLAYER && _playernum < PLAYERSINSQUAD);
    EXT_ASSERT(_playernum1 != NOPLAYER && _playernum1 < PLAYERSINSQUAD);

  	ushort tempplayer = m_listSquadID[_playernum];
    SetSquad(_playernum, m_listSquadID[_playernum1]);
    SetSquad(_playernum1, tempplayer);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindBestPlayer
	Access:    	public
	Parameter: 	const ushort _FirstPlayer
	Parameter: 	const ePlayerPosition _ePosn
	Returns:   	void
	Qualifier:
	Purpose:	Select the strongest player in the requested player ignoring
				those above {FirstPlayer} and leaving the selected player
				in {squad[FirstPlayer]}
------------------------------------------------------------------------------*/
void CPlayerSquad::DoFindBestPlayer(const ushort _FirstPlayer, const ePlayerPosition _ePosn)
{
    EXT_ASSERT(_FirstPlayer != NOPLAYER && _FirstPlayer < TotalSquadSize());
	EXT_ASSERT(_ePosn >= FIRSTPOSN && _ePosn <= LASTPOSN);

	int BestNumber = 0;
    byte BestSkill = 0;
    CPlayer* pPlayer;
	byte nPlayerEffectiveSkill;
    for (uint count = _FirstPlayer; count < PLAYERSINSQUAD; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
            pPlayer = &GetPlayer(count);
			nPlayerEffectiveSkill = pPlayer->DoCalculateEffectiveSkill(SKILL_OVERALL);
            if (pPlayer->GetSkills().GetPosition() == _ePosn && pPlayer->IsAvailableForMatch() == true && nPlayerEffectiveSkill > BestSkill)
			{
                BestNumber = count;
                BestSkill = nPlayerEffectiveSkill;
            }
        }
    }

	// Not found a suitable player
    if (BestNumber == 0 && _ePosn != GOALKEEPER)
	{
		// Find an outfield player if position required is outfield
        for (uint count = _FirstPlayer; count < PLAYERSINSQUAD; count ++)
		{
			if (m_listSquadID[count] != NOPLAYER)
			{
                pPlayer = &GetPlayer(count);
				if (pPlayer->GetSkills().GetPosition() != GOALKEEPER && pPlayer->IsAvailableForMatch() == true)
				{
					BestNumber =  count;
					break;
				}
			}
		}
	}

    if (BestNumber != 0 && BestNumber != _FirstPlayer)
	{
        DoSwapPlayers(_FirstPlayer, BestNumber);
    }
	// Ensure an available player
	BestNumber = _FirstPlayer;
	while(m_listSquadID[_FirstPlayer] == NOPLAYER || GetPlayer(_FirstPlayer).IsAvailableForMatch() == false)
	{
		do
		{
			BestNumber = _FirstPlayer + RandomNumber.IntLessThan(PLAYERSINSQUAD - _FirstPlayer - 1);
		}
		while(m_listSquadID[BestNumber] == NOPLAYER);
        DoSwapPlayers(_FirstPlayer, BestNumber);
	}
    EXT_ASSERT(GetPlayer(_FirstPlayer).IsAvailableForMatch() == true);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateAverageSkill
	Access:    	public
	Parameter: 	const ePlayerPosition _ePosn
	Returns:   	byte
	Qualifier:
	Purpose:	Calculate the average skill of all the players in the specified
				position
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCalculateAverageSkill(const ePlayerPosition _ePosn)
{
	int Number = 0;
	byte TotalSkill = 0;
	for (uint count = 0; count < PLAYERSINSQUAD; count ++)
	{
		if (m_listSquadID[count] != NOPLAYER)
		{
			if (GetPlayer(count).GetSkills().GetPosition() == _ePosn)
			{
				TotalSkill += GetPlayer(count).GetSkills().GetOverallSkill();
				Number++;
			}
		}
	}
	if (Number > 0)
	{
		return TotalSkill / Number;
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateAverageTeamPlayerSkill
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:	Calculate the average skill of all the players in the team
------------------------------------------------------------------------------*/
byte CPlayerSquad::DoCalculateAverageTeamPlayerSkill()
{
	int TotalSkill = 0;
  for (uint count = 0; count < PLAYERSINTEAM; count ++)
	{
    if (m_listSquadID[count] != NOPLAYER)
		{
            TotalSkill += GetPlayer(count).GetSkills().GetOverallSkill();
		}
	}
    return static_cast <byte>(TotalSkill / PLAYERSINTEAM);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateAveragePositionAge
	Access:    	public
	Parameter: 	const ePlayerPosition _ePosn
  	Returns:    float
	Qualifier:
	Purpose:	Calculate the average skill of all the players in the specified
				position
------------------------------------------------------------------------------*/
float CPlayerSquad::DoCalculateAveragePositionAge(const ePlayerPosition _ePosn)
{
	int Number = 0;
  	float TotalAge = 0;
    for (uint count = 0; count < PLAYERSINSQUAD; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
            if (GetPlayer(count).GetSkills().GetPosition() == _ePosn)
			{
                TotalAge += GetPlayer(count).GetAge();
				Number++;
			}
		}
	}
	return TotalAge / Number;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateAverageTeamPlayerAge
	Access:    	public
	Parameter: 	void
  	Returns:    float
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
float CPlayerSquad::DoCalculateAverageTeamPlayerAge()
{
  	float TotalAge = 0;
    for (uint count = 0; count < PLAYERSINTEAM; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
            TotalAge += GetPlayer(count).GetAge();
		}
	}
    return TotalAge / PLAYERSINTEAM;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoCalculateAverageSquadPlayerAge
	Access:    	public
	Parameter: 	void
  	Returns:    float
	Qualifier:
	Purpose:	Calculate the average skill of all the players in the squad
------------------------------------------------------------------------------*/
float CPlayerSquad::DoCalculateAverageSquadPlayerAge()
{
  	float TotalAge = 0;
    for (uint count = 0; count < PLAYERSINSQUAD; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
            TotalAge += GetPlayer(count).GetAge();
		}
	}
    return TotalAge / PLAYERSINSQUAD;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::ReduceSquadSize
	Access:    	public
	Parameter: 	const int _nHowMany
	Returns:   	void
	Qualifier:
	Purpose:	Set the squad size by removing players from the end of the squad
------------------------------------------------------------------------------*/
void CPlayerSquad::ReduceSquadSize(const int _nHowMany)
{
	EXT_ASSERT(_nHowMany < TotalSquadSize());
    for (short num = TotalSquadSize() - 1; num > _nHowMany; num--)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
            GetPlayer(num).GetClub().DoRemoveFromSquad(num);
            WorldData().GetPlayerList().DoDelete(m_listSquadID[num]);
        }
    }
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindRandomPlayer
	Access:    	public
	Parameter: 	void
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CPlayerSquad::DoFindRandomPlayer()
{
	EXT_ASSERT(m_listSquadID[0] != NOPLAYER);		// Ensure club initialised
    int Player;
    CPlayer* pPlayer = NULL;
    do
	{
        Player = RandomNumber.IntLessThan(PLAYERSINSQUAD);
        if (m_listSquadID[Player] != NOPLAYER)
		{
            pPlayer = &GetPlayer(Player);
			EXT_ASSERT_POINTER(pPlayer, CPlayer);
        }
    }
	while(pPlayer == NULL);
    return pPlayer;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindRandomAttackingPlayer
	Access:    	public
	Parameter: 	void
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CPlayerSquad::DoFindRandomAttackingPlayer()
{
	EXT_ASSERT(m_listSquadID[0] != NOPLAYER);		// Ensure club initialised
    int Player;
	int count = 0;
    CPlayer* pPlayer = NULL;
	do
	{
		do
		{
			Player = RandomNumber.IntLessThan(PLAYERSINSQUAD);
			if (m_listSquadID[Player] != NOPLAYER)
			{
	            pPlayer = &GetPlayer(Player);
		    }
		}
		while(pPlayer == NULL);
	}
	while(count++ < 100 && (pPlayer->GetSkills().GetPosition() == MIDFIELDER || pPlayer->GetSkills().GetPosition() == DEFENDER));
    return pPlayer;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoFindRandomSquadPlayer
	Access:    	public
	Parameter: 	void
	Returns:   	CPlayer*
	Qualifier:
	Purpose:	Find a random player who isn't in the team
------------------------------------------------------------------------------*/
CPlayer* CPlayerSquad::DoFindRandomSquadPlayer()
{
    EXT_ASSERT(m_listSquadID[0] != NOPLAYER);		// Ensure club initialised
    ushort PlayerID;
    CPlayer* pPlayer = NULL;
    do
	{
        PlayerID = PLAYERSINTEAM + RandomNumber.IntLessThan(PLAYERSINSQUAD - PLAYERSINTEAM);
        if (m_listSquadID[PlayerID] != NOPLAYER)
		{
            pPlayer = &GetPlayer(PlayerID);
        }
    }
    while(pPlayer == NULL);
    return pPlayer;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::BestPlayingPosition
	Access:    	public
	Parameter: 	const bool _bSquadNumber
	Parameter: 	const ePlayerPosition _ePosition
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::BestPlayingPosition(const bool _bSquadNumber, const ePlayerPosition _ePosition)
{
    CPlayer* pPlayer;
	ushort iRet = NOPLAYER;
	byte BestSkill = 0;
	bool bToAdd;
    for (uint num = 0; num < PLAYERSINTEAM; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			bToAdd = false;
            pPlayer = &GetPlayer(num);
			if (pPlayer->IsPlayingPosition(_ePosition) == true)
			{
				switch (_ePosition)
				{
					case DEFENDER:
						if (pPlayer->GetSkills().GetTackling() > BestSkill)
						{
							BestSkill = pPlayer->GetSkills().GetTackling();
							bToAdd = true;
						}
						break;
					case MIDFIELDER:
						if (pPlayer->GetSkills().GetPassing() > BestSkill)
						{
							BestSkill = pPlayer->GetSkills().GetPassing();
							bToAdd = true;
						}
						break;
					case ATTACKER:
						if (pPlayer->GetSkills().GetShooting() > BestSkill)
						{
							BestSkill = pPlayer->GetSkills().GetShooting();
							bToAdd = true;
						}
						break;
					default:
						EXT_ASSERT(false);			// Invalid player position
				}
				if (bToAdd == true)
				{
					if (_bSquadNumber == true)
					{
						iRet = num;
					}
					else
					{
						iRet = m_listSquadID[num];
					}
				}
			}
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::RandomPlayingPosition
	Access:    	public
	Parameter: 	const bool _bSquadNumber
	Parameter: 	const ePlayerPosition _ePosition
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::RandomPlayingPosition(const bool _bSquadNumber, const ePlayerPosition _ePosition)
{
	ushort iRet = NOPLAYER;
	CNumericArray<ushort> PlayerNumbers;
    for (uint num = 0; num < PLAYERSINTEAM; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			if (GetPlayer(num).IsPlayingPosition(_ePosition) == true)
			{
				if (_bSquadNumber == true)
				{
					PlayerNumbers.Add(num);
				}
				else
				{
					PlayerNumbers.Add(m_listSquadID[num]);
				}
			}
		}
	}
	if (PlayerNumbers.GetSize() > 0)
	{
		PlayerNumbers.DoShuffleArray();

		int iIndex = 0;
		while (((iRet = PlayerNumbers[iIndex]) == NOPLAYER) && (iIndex < PLAYERSINTEAM))
		{
			iIndex++;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoRandomPlayerIDByPosition
	Access:    	public
	Parameter: 	const ePlayerPosition _ePosition
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::DoRandomPlayerIDByPosition(const ePlayerPosition _ePosition)
{
	CNumericArray<ushort> PlayerNumbers;
    for (uint num = 0; num < PLAYERSINSQUAD; num ++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			if (GetPlayer(num).IsPlayingPosition(_ePosition) == true)
			{
				PlayerNumbers.Add(m_listSquadID[num]);
			}
		}
	}
	EXT_ASSERT(PlayerNumbers.GetSize() > 0);
	PlayerNumbers.DoShuffleArray();

	int iIter = 0;
	while ((PlayerNumbers[iIter] == NOPLAYER) && (iIter < PLAYERSINSQUAD))
		iIter++;

	if (iIter < PLAYERSINSQUAD)
		return PlayerNumbers[iIter];
	return NOPLAYER;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoGenerateYoungPlayers
	Access:    	public
	Parameter: 	const ushort _nHowMany
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoGenerateYoungPlayers(const ushort _nHowMany, CClub& _Club)
{
	EXT_ASSERT((TotalSquadSize() + _nHowMany) < PLAYERSINSQUAD);
	ushort nPlayerID;
	for (int LoopCount = 0; LoopCount < _nHowMany; LoopCount++)
	{
		nPlayerID = WorldData().GetPlayerList().AddNew();
		DoAddPlayerToSquad(nPlayerID);
		WorldData().GetPlayer(nPlayerID)->DoGenerateYoungPlayer(_Club, static_cast<ePlayerPosition>(RandomNumber.IntLessThan(MAXPLAYERPOSITIONS)));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoGenerateNonLeagueSquad
	Access:    	public
	Parameter: 	const ushort _nHowMany
	Parameter: 	CCountry& _rCountry
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoGenerateNonLeagueSquad(const ushort _HowMany, CCountry& _Country)
{
	EXT_ASSERT((TotalSquadSize() + _HowMany) < PLAYERSINSQUAD);
	ushort PlayerID;
	for (int LoopCount = 0; LoopCount < _HowMany; LoopCount++)
	{
		PlayerID = WorldData().GetPlayerList().AddNew();
		DoAddPlayerToSquad(PlayerID);
		WorldData().GetPlayer(PlayerID)->DoGenerateNonLeaguePlayer(_Country);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoEndOfSeason
	Access:    	public
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoEndOfSeason(CClub& _Club)
{
    for (short PlayerListNumber = PLAYERSINSQUAD - 1; PlayerListNumber > -1; PlayerListNumber--)
	{
		if (m_listSquadID[PlayerListNumber] != NOPLAYER)
		{
			if (GetPlayer(PlayerListNumber).DoEndOfSeason(_Club) == true)
			{
				GetPlayer(PlayerListNumber).DoRetired(_Club.DoFindID());
        GetPlayer(PlayerListNumber).DoRegenerate(GetPlayer(PlayerListNumber).DoFindID(), _Club.GetCountry());
        DoRemoveFromSquad(PlayerListNumber);
			}
			else if (GetPlayer(PlayerListNumber).IsOnLoan() == true /*&& GetPlayer(PlayerListNumber).GetContractEndDate().SeasonsRemaining() == 0*/)
			{
				_Club.DoCheckSendPlayerBackFromLoan(PlayerListNumber);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoNonLeagueCLubEndOfSeason
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoNonLeagueClubEndOfSeason()
{
	for (int PlayerListNumber = 0; PlayerListNumber < PLAYERSINSQUAD; PlayerListNumber++)
	{
		if (m_listSquadID[PlayerListNumber] != NOPLAYER)
		{
			CPlayer& thePlayer = GetPlayer(PlayerListNumber);
			if (thePlayer.GetAge() > 39)
			{
				thePlayer.DoGenerateDateOfBirth(20);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoEnsureEnoughPlayers
	Access:    	public
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:	Ensure doesn't run out of players
------------------------------------------------------------------------------*/
void CPlayerSquad::DoEnsureEnoughPlayers(CClub& _Club)
{
	if (TotalSquadSize() < MINSQUADSIZE)
	{
		DoGenerateYoungPlayers((MINSQUADSIZE - TotalSquadSize()) + RandomNumber.IntLessThan(3), _Club);
//		g_DebugStr.Format(_T("Squad size %d"), TotalSquadSize());
//		g_DebugStr.OutputDebug();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoIncrementPlayersOutOnLoan
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoIncrementPlayersOutOnLoan()
{
	m_PlayersOutOnLoan++;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoDecrementPlayersInOnLoan
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoDecrementPlayersInOnLoan()
{
	EXT_ASSERT(m_PlayersInOnLoan > 0);
	m_PlayersInOnLoan--;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DecrementPlayersOutOnLoan
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoDecrementPlayersOutOnLoan()
{
	EXT_ASSERT(m_PlayersOutOnLoan > 0);
	m_PlayersOutOnLoan--;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::NewContractRequests
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoNewContractRequests()
{
    CPlayer* pPlayer;
	// Check the players
    for(int count = 0; count < PLAYERSINSQUAD; count ++)
	{
        if (m_listSquadID[count] != NOPLAYER)
		{
            pPlayer = &GetPlayer(count);
			if (RandomNumber.IntLessThan(CHANCENEWCONTRACTREQUEST) == 1)
			{
				pPlayer->DoNewContractRequest(pPlayer->GetClub());
                break;  // Only one a turn
            }
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::WeeklyUpdate
	Access:    	public
	Parameter: 	CClub& _rClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoWeeklyUpdate(CClub& _Club)
{
    int num = 0;
	bool DeletedOne;
    CPlayer* pPlayer;
	ushort NumberAvailablePlayers = DoCountAvailableNonLoanedPlayers();
    while(num < PLAYERSINSQUAD)
	{
		DeletedOne = true;		// Go round inner loop
		while(DeletedOne == true)
		{
			DeletedOne = false;
		     if (m_listSquadID[num] != NOPLAYER)
			 {
                pPlayer = &GetPlayer(num);
				pPlayer->DoWeeklyTasks();
				if (pPlayer->IsOnLoan() == true)
				{
					DeletedOne = _Club.DoCheckSendPlayerBackFromLoan(num);
				}
				else if (NumberAvailablePlayers > MINSQUADSIZE)
				{
					// Should player demand a transfer?
#ifndef FORCETRANSFERREQUESTS
					if (RandomNumber.IntLessThan(ASKFORTRANSFERCHANCE) == 2 && pPlayer->CheckAskForTransfer() == true)
#else
					if (RandomNumber.IntLessThan(ASKFORTRANSFERCHANCE) == 2)
#endif
					{
						pPlayer->GetClub().OnPlayerAskedForMove(pPlayer);
					}
				}
			}
		}
		num++;
  	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoAfterMatchUpdate
	Access:    	public
	Parameter: 	const byte _PrimaryTactics
	Parameter: 	const byte _OurGoals
	Parameter: 	const byte _TheirGoals
	Parameter: 	CFixture& _Fixture
	Parameter: 	const eWhichTeam _eWhichTeam
	Parameter: 	CClub& _rClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoAfterMatchUpdate(const ushort _PrimaryTactics, const byte _OurGoals, const byte _TheirGoals, CFixture& _Fixture, const eWhichTeam _WhichTeam, CClub& _Club)
{
	ushort NumSubs = _Fixture.GetSubsSelect();
	for (ushort num = 0; num < PLAYERSINTEAM + NumSubs; num ++)
	{
    	// If the club has played a match
        if (_OurGoals != -1 && m_listSquadID[num] != NOPLAYER)
		{
            GetPlayer(num).DoAfterMatchInTeamUpdate(_Club, _Fixture.GetOverallPlayerPerformance(_WhichTeam, num));
		}
  	}

	for (ushort num = PLAYERSINTEAM + NumSubs; num < PLAYERSINSQUAD; num ++)
	{
    	// If the club has played a match
        if (_OurGoals != -1 && m_listSquadID[num] != NOPLAYER)
		{
            GetPlayer(num).DoAfterMatchInSquadUpdate(_Club);
		}
  	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetPenaltyTaker
	Access:    	public
	Parameter: 	void
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CPlayerSquad::GetPenaltyTaker()
{
	return WorldData().GetPlayer(m_PenaltyTakerID);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::PlayersRequestNewContracts
	Access:    	public
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoCheckIfRequestNewContracts(CClub& _Club)
{
    for (int num = 0; num < PLAYERSINSQUAD; num++)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
            GetPlayer(num).DoCheckIfRequestNewContract(_Club);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoGivePlayersInjury
	Access:    	public
	Parameter: 	const ushort _nHowMany
	Parameter: 	const CInjuryType::eInjurySeverity _eSeverity
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoGivePlayersInjury(const ushort _HowMany, const CInjuryType::eInjurySeverity _Severity)
{
	int num;
	for (int LoopCount = 0; LoopCount < _HowMany; LoopCount++)
	{
		// Select a random player
		do
		{
			num = RandomNumber.IntLessThan(PLAYERSINSQUAD);
		}
		while(m_listSquadID[num] == NOPLAYER);
		// Injure him if not injured already
		//GetPlayer(num).DoTryInjurePlayerInTraining(_Severity);
	}
}

extern bool g_injuriesDisabled;

/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::DoDailyTasks
	Access:    	public
	Parameter: 	CClub * _Club
	Parameter: 	const bool _IsWeekEnd
	Parameter: 	const bool _DoTraining
	Parameter: 	const uint _TimeTillNextMatch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerSquad::DoDailyTasks(CClub* _Club, const bool _IsWeekEnd, const bool _DoTraining, const uint _TimeTillNextMatch)
{
	EXT_ASSERT_POINTER(_Club, CClub);
    for (int num = PLAYERSINSQUAD - 1; num >= 0; num--)
	{
        if (m_listSquadID[num] != NOPLAYER)
		{
			GetPlayer(num).DailyTasks(_Club, _IsWeekEnd, _DoTraining);
		}
	}
	// Training injures
#ifdef TRAININGINJURIES
	if (WorldData().IsFastTest() == false && _TimeTillNextMatch < 10 && DoCountAvailablePlayers() > MINSQUADSIZE + 1 && !g_injuriesDisabled )
	{
		int HowMany = RandomNumber.IntLessThan(100);

		if (HowMany > 49 && HowMany < 70)
		{
			DoGivePlayersInjury(1, CInjuryType::KNOCK);
		}

		if (HowMany > 69 && HowMany < 80)
		{
			DoGivePlayersInjury(2, CInjuryType::KNOCK);
		}

		if (HowMany > 79 && HowMany < 90)
		{
			DoGivePlayersInjury(1, CInjuryType::KNOCK);
			DoGivePlayersInjury(1, CInjuryType::MINOR);
		}

		if (HowMany > 89 && HowMany < 95)
		{
			DoGivePlayersInjury(2, CInjuryType::MINOR);
		}

		if (HowMany > 94)
		{
			DoGivePlayersInjury(1, CInjuryType::MAJOR);
		}
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetLoansThisSeason
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerSquad::GetLoansThisSeason() const
{
	return m_PlayersInOnLoanThisSeason;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetPlayersOutOnLoan
	Access:    	public
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayerSquad::GetPlayersOutOnLoan() const
{
	return m_PlayersOutOnLoan;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerSquad::GetPlayersInOnLoan
	Access:    	public
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayerSquad::GetPlayersInOnLoan() const
{
	return m_PlayersInOnLoan;
}

