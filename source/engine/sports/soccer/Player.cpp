
 /*

    Class:- Player

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#endif


#include "stdafx.h"

#include "..\system\MessageDisplay.h"
#include "ClubHistory.h"
#include "PlayerSearch.h"


	const int				YOUNGPLAYERTOOGOODFORCONTRACT = 60;
	const int				MINMOVEABROADAGE = 22;
	const int				MAXMOVEABROADAGE = 31;
	const int   			OUTOFCONTRACTMOVEABROAD = 4;
	const int				CONTRACTNEARLYOVERMOVEABROAD = 8;
	const int				CONTRACTOKOVERMOVEABROAD = 6;
    const int				OFFLISTCHANCE = 20;
	const int				REVALUECHANCE = 10;		// Change his valuation?
	const int				CHANCEPLAYERRENEWCONTRACT = 90;
	const int				CHANCEBOSMAN = 85;
	const int				CHANCERESIGN = 7;
	const int				STARTONBOSMANCHANCE = 200;
	const int				MAXOUTOFCONTRACTPLAYERSSIGNFORCLUBTHISWEEK = 2;		// If greater than this
	const int				MAXOUTOFCONTRACTPLAYERSSIGNFORCLUB = 6;				// If greater than this
	const int				CHANCELOWERMORALENOTPLAYED = 85;					// %age chance of morale reduction if not played in match

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef STOREMANOFMATCH
/*------------------------------------------------------------------------------
	Method:   	CPlayerManOfMatchAwards::AwardedManOfMatch
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerManOfMatchAwards::AwardedManOfMatch()
{
	m_ManOfMatchAwardsThisSeason++;
	m_ManOfMatchAwardsThisClub++;
	m_ManOfMatchAwardsCareer++;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerManOfMatchAwards::DoResetVariables
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerManOfMatchAwards::DoResetVariables()
{
	m_ManOfMatchAwardsThisSeason = 0;
	m_ManOfMatchAwardsThisClub = 0;
	m_ManOfMatchAwardsCareer = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerManOfMatchAwards::DoNewSeason
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerManOfMatchAwards::DoNewSeason()
{
	m_ManOfMatchAwardsThisSeason = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerManOfMatchAwards::DoJoinedNewClub
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerManOfMatchAwards::DoJoinedNewClub()
{
	m_ManOfMatchAwardsThisClub = 0;
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CPlayer::CPlayer
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer::CPlayer()
{
#ifdef NDS
	m_bitfields.m_bDontSave = false;
#else
	m_ID = NOPLAYER;
#endif
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::CPlayer
	Access:    	public
	Parameter: 	CClub* apClub
	Parameter: 	ePlayerPosition aPosition
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer::CPlayer(CClub* apClub, ePlayerPosition aPosition)
{
#ifdef NDS
	m_bitfields.m_bDontSave = false;
#else
	m_ID = NOPLAYER;
#endif
	DoResetVariables();
	CPerson::DoCreatePlayer(GetSkills().DoGenerateRandomWage(apClub->GetCountry()), apClub->GetCountry());
    GetSkills().DoCreatePlayer(CPerson::GetAge(), &apClub->GetPopularity(), apClub->GetDivisionStrengthID(), aPosition);
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::operator=
	Access:    	public
	Parameter: 	const CPlayer & rhs
	Returns:   	CPlayer &
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayer &CPlayer::operator = (const CPlayer &rhs)
{
	CPerson::operator=(rhs);
	m_Skills = rhs.m_Skills;
	m_Career = rhs.m_Career;
	m_CupCareer = rhs.m_CupCareer;
	m_ClubCareer = rhs.m_ClubCareer;
#ifdef INTERNATIONAL
	m_InternationalCareer = rhs.m_InternationalCareer;
#endif
	m_ClubSeasonCareer = rhs.m_ClubSeasonCareer;
	m_CupSeasonCareer = rhs.m_CupSeasonCareer;
	m_LeagueSeasonCareer = rhs.m_LeagueSeasonCareer;
	m_LastSeasonCareer = rhs.m_LastSeasonCareer;

	m_Discipline = rhs.m_Discipline;

	m_bitfields.m_HowLongPlayedLastMatch = rhs.m_bitfields.m_HowLongPlayedLastMatch;
	m_bitfields.m_NumberMatchesSinceLastAppeared = rhs.m_bitfields.m_NumberMatchesSinceLastAppeared;
#ifdef NDS
	m_bitfields.m_bOnLoan = rhs.m_bitfields.m_bOnLoan;
#else
	m_bitfields.m_HasTransferRecord = rhs.m_bitfields.m_HasTransferRecord;
	m_bitfields.m_HasLoanRecord = rhs.m_bitfields.m_HasLoanRecord;
#endif

#ifdef PLAYERTRANSFERHISTORY
	m_PlayerTransferHistory = rhs.m_PlayerTransferHistory;
#endif

	return *this;
}

/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoInitialiseFromResource
	Access:    	public
	Parameter: 	CBinaryResource& aResource
	Parameter: 	CClub* apClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoInitialiseFromResource(CBinaryReader& _Resource, CClub* _Club)
{
	CPerson::DoInitialiseFromResource(_Resource);
	GetSkills().DoInitialiseFromResource(_Resource);
// 	if (GetSkills().GetPace() == 0)
// 	{
// 		GetSkills().DoCreatePlayer(CPerson::GetAge(), &_Club->GetPopularity(), _Club->GetDivisionStrengthID(), GetSkills().GetPosition());
// 	}
	if (_Club->IsNonLeagueClub() == false)
	{
		CPerson::SetWage(GetSkills().DoGenerateRandomWage(_Club->GetCountry()));
	}
	byte nAgeInYears = CPerson::GetAge() - 17;
	ushort nCareerAppearances = 30 + ((RandomNumber.IntLessThan( 1 + nAgeInYears / 2  * 30) + nAgeInYears / 2 * 30)) - 30;
	m_Career.DoGenerateInitialData(nCareerAppearances, GetSkills().GetPosition());
	int nPreviousClubs = 1 + RandomNumber.IntLessThan(2);
	if (nAgeInYears > 22)
	{
		nPreviousClubs += RandomNumber.IntLessThan(3);
	}
	nCareerAppearances /= nPreviousClubs;
	m_ClubCareer.DoGenerateInitialData(nCareerAppearances, GetSkills().GetPosition());

	if (IsOnLoan() == false)
	{
	//	DoCheckClubPlayerRecords(*_Club);
	}
}


#ifdef NDS
void CPlayer::SetDontSave()
{
	m_bitfields.m_bDontSave = true;
}

bool CPlayer::IsDontSave() const
{
	return m_bitfields.m_bDontSave;
}
#endif

/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoCheckClubPlayerRecords
	Access:    	public
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoCheckClubPlayerRecords(CClub& _Club)
{
	EXT_ASSERT(IsOnLoan() == false);
	_Club.GetClubHistory().CheckIfRecordAppear(*this, GetClubAppearances());
	if (GetSkills().GetPosition() != GOALKEEPER)
	{
		_Club.GetClubHistory().CheckIfRecordGoals(*this, GetClubGoal());
		_Club.GetClubHistory().CheckIfRecordSeasonGoals(*this, GetClubSeasonGoal());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoFindID
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

ushort CPlayer::DoFindID() const
{
#ifdef NDS
	return WorldData().GetPlayerList().Find(this);
#else
	EXT_ASSERT(m_ID != NOPLAYER);
	return m_ID;
#endif
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif


#ifdef PLAYERTRANSFERHISTORY
/*------------------------------------------------------------------------------
	Method:   	CPlayer::AddTransferHistory
	Access:    	public
	Parameter: 	CString From
	Parameter: 	const uint HowMuch
	Parameter: 	CString When
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::AddTransferHistory(const CString &From, const uint HowMuch, const CString &When)
{
	m_PlayerTransferHistory.AddTransferHistory(From, HowMuch, When);
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoLeaveClub
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoLeaveClub()
{
	m_ClubCareer.DoResetVariables();
	m_ClubSeasonCareer.DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoLoanComplete
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoLoanComplete()
{
	WorldData().GetPlayerLoanManager().DoInitialise(DoFindID());
  m_bitfields.m_bOnLoan = false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoWeeklyTasks
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

void CPlayer::DoWeeklyTasks()
{
	CPerson::DoWeeklyTasks();
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoGiveNewContract
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoGiveNewContract()
{
	CPerson::DoGiveNewContract(m_Skills.DoGenerateRandomContractLength(GetAge()));                             // Seasons
	WorldData().GetTransferListManager().DoJoinedNewClub(DoFindID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoAddPerformance
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoAddPerformance(const byte _Value)
{
    GetSkills().DoAddPerformance(_Value);
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetNumberBookingsThisSeason
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::GetNumberBookingsThisSeason() const
{
	return m_LeagueSeasonCareer.GetBookings() + m_CupSeasonCareer.GetBookings();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetNumberSentOffThisSeason
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::GetNumberSentOffThisSeason() const
{
	return m_LeagueSeasonCareer.GetSendingOffs() + m_CupSeasonCareer.GetSendingOffs();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetDisciplinaryPointsThisSeason
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::GetDisciplinaryPointsThisSeason()
{
	return GetNumberBookingsThisSeason() * BOOKINGPOINTS + GetNumberSentOffThisSeason() * SENDINGOFFPOINTS;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetRating
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::GetRating()
{
    return GetSkills().GetRating();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetPerformance
	Access:    	public
	Parameter: 	void
	Returns:   	sbyte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
sbyte CPlayer::GetPerformance()
{
    return GetSkills().GetPerformance();
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoMakeInvincible
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoMakeInvincible()
{
#ifdef _DEBUG
    GetSkills().DoMakeInvincible();
#endif // !_DEBUG
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoMakeOld
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoMakeOld()
{
	CPerson::DoGenerateDateOfBirth(30 + RandomNumber.IntLessThan(5));
	GetSkills().DoMakeOld();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoMakeYoung
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoMakeYoung()
{
	CPerson::DoGenerateDateOfBirth(18 + RandomNumber.IntLessThan(5));
	GetSkills().DoMakeYoung();
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsTransferListed
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsTransferListed()
{
	if (WorldData().GetTransferListManager().IsTransferListed(DoFindID()) == true)
	{
		return WillingToTransfer();
	}
	return QualifiesForBosmanFreeTransfer();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::ContractLengthInDays
	Access:    	public
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayer::ContractLengthInDays()
{
	return GetContractEndDate().ContractLengthInDays();
}

/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsLoanListed
	Access:    	public
	Parameter: 	void
	Returns:   	const bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsLoanListed()
{
	if (WorldData().GetTransferListManager().IsLoanListed(DoFindID()) == true)
	{
		return WillingToTransfer();
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::WillingToTransfer
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::WillingToTransfer()
{
	if (CPerson::GetWeeksHere() < MINWEEKSATCLUB)
	{
		return false;
	}
 	if (CPerson::GetContractRenewalStatus() == RETIRING)
	{
		return false;
	}
    if (IsOnLoan() == true)
	{
		return false;
	}
	return IsAvailableForMatch();
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif

/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsWillingToPlayInDivision
	Access:    	public
	Parameter: 	const byte DivisionStrength
	Returns:   	const bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
const bool CPlayer::IsWillingToPlayInDivision(const byte DivisionStrength)
{
	bool bWillChangeDivision = true;
	switch(GetClub().GetDivisionStrengthID() - DivisionStrength)
	{
		case -1:
			if (RandomNumber.IntLessThan(100) > 25)
			{
				bWillChangeDivision = false;
			}
			break;
		case -2:
			if (RandomNumber.IntLessThan(100) > 15)
			{
				bWillChangeDivision = false;
			}
			break;
		case -3:
			if (RandomNumber.IntLessThan(100) > 5)
			{
				bWillChangeDivision = false;
			}
			break;
	}
	return bWillChangeDivision;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoFindSuitableNewClub
	Access:    	public
	Parameter: 	CClub* _ClubToAvoid
	Returns:   	CClub*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub* CPlayer::DoFindSuitableNewClub(CClub* _ClubToAvoid)
{
	CClubList SuitableClubs;
	WorldData().GetClubList().DoAddClubsSuitableForPlayer(*this, SuitableClubs, _ClubToAvoid);
	if (SuitableClubs.GetSize() > 0)
	{
		CClub* pNewClub = SuitableClubs.DoFindRandomClub();
		EXT_ASSERT(pNewClub->IsUserControlled() == false);
		return pNewClub;
	}
	return NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::AskingPriceStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::AskingPriceStr()
{
	const ushort usPlayerID = DoFindID();
	if (WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == true)
	{
		const uint uiAskingPrice = WorldData().GetTransferListManager().GetAskingPrice(usPlayerID);
		if (uiAskingPrice > 0)
		{
			return GetClub().GetForeignStr(uiAskingPrice);
		}
		else
		{
			CString szStr;
      szStr.LoadString(IDS_FREETRANSFER);
			return szStr;
		}
	}
	else if (QualifiesForBosmanFreeTransfer() == true)
	{
		CString szStr;
    szStr.LoadString(IDS_BOSMAN);
		return szStr;
	}
	else
	{
		return GetClub().GetForeignStr(GetValuation(GetClub().GetCountry()));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::BecomeManager
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:	Does the player wish to become manager?
------------------------------------------------------------------------------*/
bool CPlayer::BecomeManager()
{
	int PosnChance[] =
	{
		10,		// Goalkeeper
		 2,		// Defender
		 2,		// Midfielder
		 8,		// Attacker
	};

	if (CPerson::GetAge() < 36 || RandomNumber.IntLessThan(PosnChance[GetSkills().GetPosition()]) != 1)
	{
		return false;
	}

  if( WorldData().GetManagerList().GetSize() >= 200 )
  {
    // too many managers already, don't add to list
    return false;
  }

	return true;
}

extern bool g_injuriesDisabled;

/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTryInjurePlayer
	Access:    	public
	Parameter: 	CClub & _Club
	Parameter: 	const CInjuryType::eInjurySeverity aSeverity
	Returns:   	bool
	Qualifier:
	Purpose:	The specified player has been injured, decide how long for,
				update player file and inform manager
------------------------------------------------------------------------------*/
bool CPlayer::DoTryInjurePlayer(CClub& _Club, const CInjuryType::eInjurySeverity _Severity /*= CInjuryType::MAXINJURYSEVERITIES*/)
{
	if (IsInjured() == true || RandomNumber.IntLessThan(10) < 5 || g_injuriesDisabled)
	{
		// Already injured
		return false;
	}
	return DoTryInjurePlayerInTraining(_Club, _Severity);
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTryCreateRandomEventPlayer
	Access:    	public
	Parameter: 	CClub * _Club
	Returns:   	bool
	Qualifier:
	Purpose:	The specified player has been the victim of a random event
				decide how long for, update player file and inform manager
------------------------------------------------------------------------------*/
bool CPlayer::DoTryCreateRandomEventPlayer(CClub* _Club)
{
		// Already injured
	if (IsInjured() == false && !g_injuriesDisabled )
	{
		static_cast<CInjury*>(WorldData().GetInjuryList().GetAt(WorldData().GetInjuryList().AddOne()))->DoCreateEventInjury(this, _Club);
		CPerson::DoLowerMorale(20);
	    return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTryInjurePlayerInTraining
	Access:    	public
	Parameter: 	CClub & _Club
	Parameter: 	const CInjuryType::eInjurySeverity _Severity
	Returns:   	bool
	Qualifier:
	Purpose:	The specified player has been injured, decide how long for,
				update player file and inform manager
------------------------------------------------------------------------------*/
bool CPlayer::DoTryInjurePlayerInTraining(CClub& _Club, const CInjuryType::eInjurySeverity _Severity /*= CInjuryType::MAXINJURYSEVERITIES*/)
{
	if (IsInjured() == true || g_injuriesDisabled)
	{
		// Already injured
		return false;
	}
	static_cast<CInjury*>(WorldData().GetInjuryList().GetAt(WorldData().GetInjuryList().AddOne()))->DoCreateTrainingInjury(this, _Club, _Severity);
	CPerson::DoLowerMorale(20);

  return true;
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_begin.h>
#endif

/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetInjury
	Access:    	public
	Returns:   	CInjury*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CInjury* CPlayer::GetInjury()
{
	return WorldData().GetInjuryList().DoFindPlayer(this);
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif

/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetInjuryEffect
	Access:    	public
  	Returns:    float
	Qualifier:
	Purpose:	Skill should be multiplied by the returned value to give his skill
------------------------------------------------------------------------------*/
float CPlayer::GetInjuryEffect()
{
	if (IsInjured() == true)
	{
		return GetInjury()->SkillMultiplier();
	}
	else
	{
		// Not injured
		return 1;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoCheckIfRequestNewContract
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoCheckIfRequestNewContract(CClub& _Club)
{
	if (IsOnLoan() == false && WorldData().IsFastTest() == false && CPerson::GetContractEndDate().SeasonsRemaining() == 1 && RandomNumber.IntLessThan(CHANCEREQUESTNEWCONTRACT) == 1)
	{
		if (WillSignMsg(_Club, false) == true)
		{
			CExtString str;
      str.Format(IDS_PLAYERCONTRACTNEARLYEXPIRED, GetName().CStr());
			//Club.AddToClubNews(str);
			WorldData().DoNegotiatePlayerContract(this, &_Club, str);
			if (CPerson::GetContractEndDate().SeasonsRemaining() == 1)
			{
				CPerson::DoLowerMorale(10);
				SetContractRenewalStatus(CLUBREFUSED);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::WillSignMsg
	Access:    	public
	Parameter: 	CClub & _Club
	Parameter: 	const bool _Messages
	Returns:   	bool
	Qualifier:
	Purpose:	Is the player willing to sign for the specified club?
------------------------------------------------------------------------------*/
bool CPlayer::WillSignMsg(CClub& _Club, const bool _Messages)
{
	CExtString str;
	switch(CPerson::GetContractRenewalStatus())
	{
		case RETIRING:
			// Retiring soon
			if (_Messages == true)
			{
        str.Format(IDS_WONTSIGNRETIRE, CNewsItem::PreprocessPlayerName(this).CStr());
				UserMessageBox(str, 0);
				_Club.AddToClubNews(str);
			}
			return false;
		case REFUSED:
		case LISTED:
		case ACCEPTED:
		case CLUBREFUSED:
			if (_Club.GetFirstTeamSquad().IsPlayerInSquad(this) == true)
			{
				return false;
			}

		default :
			if (_Club.GetPopularity().IsPlayerSkillInRange(GetSkills().GetOverallSkill(), _Club.GetDivisionStrengthID()) == true)
			{
				return true;
			}
			else
			{
				// Skills not appropriate for the club
				if (_Messages == true)
				{
          str.Format(IDS_WONTSIGN, CNewsItem::PreprocessPlayerName(this).CStr());
					UserMessageBox(str, 0);
					_Club.AddToClubNews(str);
				}
				return false;
			}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::SetLoanListed
	Access:    	public
	Parameter: 	const bool _State
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::SetLoanListed(const bool _State)
{
	WorldData().GetTransferListManager().SetLoanListed(DoFindID(), _State);
}


 // Is the player willing to sign for the specified club?
 // Returns empty string if true
 //---------------------------------------------------------------------------
CString CPlayer::WillSignTransferMsg(CClub& _Club, const bool _RenewContract /*= false*/)
{
	CString str;
	if (WorldData().GetTransferListManager().IsOutOfContract(DoFindID()) == true)
	{
		if (_Club.GetFirstTeamSquad().DoCountRecentlySignedPlayers(1) > MAXOUTOFCONTRACTPLAYERSSIGNFORCLUBTHISWEEK)
		{
      str.Format(IDS_WONTSIGN, CNewsItem::PreprocessPlayerName(this).CStr());
			return str;
		}
		if (_Club.GetFirstTeamSquad().DoCountRecentlySignedPlayers(6) > MAXOUTOFCONTRACTPLAYERSSIGNFORCLUB)
		{
      str.Format(IDS_WONTSIGN, CNewsItem::PreprocessPlayerName(this).CStr());
			return str;
		}
	}
	if (CPerson::GetContractRenewalStatus() == RETIRING)
	{
		// Retiring soon
    str.Format(IDS_WONTSIGNRETIRE, CNewsItem::PreprocessPlayerName(this).CStr());
        return str;
	}
    if (_Club.GetPopularity().IsPlayerSkillInRange(GetSkills().GetOverallSkill(), _Club.GetDivisionStrengthID()) == true)
	{
        return TEXT("");
	}
	else
	{
		// Skills not appropriate for the club
		if (_RenewContract != true)
		{
      str.Format(IDS_WONTSIGN, CNewsItem::PreprocessPlayerName(this).CStr());
		}
		else
		{
      str.Format(IDS_WONTRENEW, CNewsItem::PreprocessPlayerName(this).CStr());
		}
        return str;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoUpdateContractOffered
	Access:    	public
	Parameter: 	CContract & _ContractOffered
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoUpdateContractOffered(CContract& _ContractOffered)
{
	EXT_ASSERT(GetAge()+ _ContractOffered.GetContractLength() < 42);
	CPerson::DoUpdateContractOffered(_ContractOffered);
}


 //---------------------------------------------------------------------------
bool CPlayer::MaxValueMsg(const uint aAmount, CCountry& arCountry)
{
	CString str;
    if (GetSkills().MaxValue(aAmount, CPerson::GetAge(), arCountry, CPerson::GetTemperament()) == true)
	{
    str.Format(IDS_BIDBLOCKED, CNewsItem::PreprocessPlayerName(this).CStr());
		UserMessageBox(str, 0);
        return true;
	}
	else
	{
        return false;
	}
 }


 // Calculate the minimum wage for this player
 //---------------------------------------------------------------------------
uint CPlayer::DoCalculateMinimumWage(CCountry& _Country, CPopularity* _Popularity)
{
    float Minwage[]=
	{
        0, 2.5, 3.75, 6.0, 10.0,
        15.0, 20.0, 30.0, 50.0, 150.0
    };

    int SkillClass = static_cast<int> (GetSkills().GetOverallSkill() / 10);
    float wage1 = Minwage[SkillClass] * _Country.GetFinancialRatioPercentage();
    if (CPerson::GetAge() < 21)
	{
        wage1 *=  .85f;
    }
	return (uint)(_Country.GetCurrency().RoundedPounds((uint)((wage1 * 10) / 10)));
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoResetVariables
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoResetVariables()
{
	GetSkills().DoResetVariables();
    CPerson::DoResetVariables();
	DoInitialise();
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoInitialise
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoInitialise()
{
	const ushort usPlayerID = DoFindID();
	if (usPlayerID != NOPLAYER)
	{
		WorldData().GetTransferListManager().DoResetVariables(usPlayerID);
		WorldData().GetPlayerLoanManager().DoInitialise(usPlayerID);
	}

  	m_bitfields.m_NumberMatchesSinceLastAppeared = 0;
#ifdef NDS
  	m_bitfields.m_bOnLoan = false;
#endif

	m_Career.DoResetVariables();
	m_ClubCareer.DoResetVariables();
	m_ClubSeasonCareer.DoResetVariables();
	m_LastSeasonCareer.DoResetVariables();
	m_CupCareer.DoResetVariables();
	m_CupSeasonCareer.DoResetVariables();
	m_LeagueSeasonCareer.DoResetVariables();

	m_Discipline.DoResetVariables();
	DoClearSeasonDisciplinaryPoints();
//    SinceGoal = SuccessiveGoal = 0;
//	SuccessiveAppear = 0;

	m_bitfields.m_HowLongPlayedLastMatch = 0;
#ifdef STOREMANOFMATCH
	m_ManOfMatchAwards.MakeDetailsNull();
#endif
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::HowLongUnableToPlay
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CPlayer::HowLongUnableToPlay()
{
	if (IsInjured() == true)
	{
		return GetInjury()->HowLongRemainingUnableToPlay();
	}
	else
	{
		return 0;
	}
}


 //---------------------------------------------------------------------------
byte CPlayer::HowLongHasBeenUnableToPlay()
{
	if (IsInjured() == true)
	{
		return GetInjury()->HowLongHasBeenUnableToPlay();
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::HowLongUnableToPlayStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::HowLongUnableToPlayStr()
{
	if (IsInjured() == true)
	{
		return GetInjury()->InjuryLengthStr();
	}
	else
	{
		return TEXT("");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::InjuryDetailsStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::InjuryDetailsStr()
{
	if (IsInjured() == true)
	{
		return GetInjury()->CapitalDescriptionStr();
	}
	else
	{
		return TEXT("");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::InjuryMatchPerformanceEffect
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::InjuryMatchPerformanceEffect()
{
    if (GetInjury() != NULL)
	{
		return GetInjury()->GetMatchPerformanceEffect();
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::InjuryCarriedOff
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::InjuryCarriedOff()
{
    EXT_ASSERT_POINTER(GetInjury(), CInjury);
    return GetInjury()->CarriedOff();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoMakeFit
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoMakeFit()
{
	if (IsInjured() == true)
	{
		DoBackFromInjury();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsAtCurrentUserControlledClub
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsAtCurrentUserControlledClub()
{
	if 	(WorldData().GetCurrentUser() != NULL && WorldData().GetCurrentUserClub() != NULL)
	{
		return WorldData().GetCurrentUserClub()->GetFirstTeamSquad().IsPlayerInSquad(this);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoEndOfSeason
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	bool - true if needs removing
	Qualifier:
	Purpose:	End of season clear down
------------------------------------------------------------------------------*/
bool CPlayer::DoEndOfSeason(CClub& _Club)
{
	CPerson::DoEndOfSeason();

	m_LastSeasonCareer = m_LeagueSeasonCareer;
	m_LastSeasonCareer += m_CupSeasonCareer;
	m_ClubSeasonCareer.DoResetVariables();
	m_CupSeasonCareer.DoResetVariables();
	m_LeagueSeasonCareer.DoResetVariables();

    // Finished if got a contract
  if (CPerson::GetContractEndDate().SeasonsRemaining() == 0)
	{
		if (_Club.IsUserControlled() == false || WorldData().IsFastTest() == true)
		{
			return DoTryRenewNonUserPlayerContract(_Club);
		}

		// User controlled club players only
    if (CPerson::GetContractRenewalStatus() == RETIRING || CPerson::GetContractRenewalStatus() == RETIRED)
		{
			CExtString str;
      str.Format(IDS_PLAYERRETIRED, CNewsItem::PreprocessPlayerName(this).CStr());
			_Club.AddToClubNews(str);
			return true;            // Delete the object
		}
	}
  return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTryListPlayerAtEndOfContract
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::DoTryListPlayerAtEndOfContract(CClub& _Club, const bool _bSacked)
{
	const ushort usPlayerID = DoFindID();
	SetLoanListed(false);
	if (QualifiesForBosmanFreeTransfer() == false && CPerson::GetContractRenewalStatus() != RETIRING)
	{
		if (_bSacked == true)
		{
			WorldData().GetTransferListManager().DoTakeOffTransferList(DoFindID());
			CPerson::SetContractRenewalStatus(NONEINPROGRESS);
		}
		else
		{
			if (IsTransferListed() == false)
			{
				DoTryListPlayer(NOTFOUND, _Club.GetCountry(), _bSacked);
			}

			if (_Club.IsUserControlled() == true)
			{
				CExtString str;
        str.Format(IDS_PLAYERNOCONTRACTBOSMANUNDERAGE, CNewsItem::PreprocessPlayerName(this).CStr());
				_Club.AddToClubNews(str);
			}
			return true;
		}
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTryRenewNonUserPlayerContract
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::DoTryRenewNonUserPlayerContract(CClub& _Club)
{
    if (CPerson::GetAge() > 32 && RandomNumber.IntLessThan(10) >= (CHANCERESIGN >> 1))
	{
        CPerson::SetContractRenewalStatus(RETIRING);
    }

    if ((CPerson::GetAge() > 36 && RandomNumber.IntLessThan(10) >= CHANCERESIGN))
	{
        CPerson::SetContractRenewalStatus(RETIRING);
    }

    if (CPerson::GetAge() > 39 || CPerson::GetContractRenewalStatus() == RETIRING)
	{
        DoGenerateYoungPlayer(_Club, GetSkills().GetPosition());
        return false;			// Only return true if <CPlayer> is to be destroyed
    }

    EXT_ASSERT(CPerson::GetAge() < 40);			// Player getting old
	if (RandomNumber.IntLessThan(100) < CHANCEBOSMAN || IsOnLoan() == true)
	{
		DoGiveNewContract();
	}
	else
	{
		DoTryListPlayerAtEndOfContract(_Club, false);
	}
    return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoContractTerminated
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoContractTerminated(CClub& _Club, const bool _bWasSacked)
{
	DoTakeOffTransferList();
	SetLoanListed(false);

	if (CPerson::GetAge() > 37)
	{
		CPerson::SetContractRenewalStatus(RETIRING);
	}
	if (CPerson::GetContractRenewalStatus() != RETIRING)
	{
    if (_bWasSacked == false)
    {
			if (DoTryListPlayerAtEndOfContract(_Club, _bWasSacked) == false && _Club.IsUserControlled() == true)
			{
					CExtString str;
            str.Format(IDS_PLAYERNOCONTRACTLEFTONBOSMAN, CNewsItem::PreprocessPlayerName(this).CStr());
					_Club.AddToClubNews(str);
			}
    }

		DoLeaveClub();
		ushort PlayerID = DoFindID();
  //  lLogFmt("ContractTerminated: %s\n", GetName());
		if (_Club.GetFirstTeamSquad().DoFindPlayerIDInSquad(PlayerID) != NOPLAYER)
		{
			// Remove player from squad if not done so already
    //  lLogFmt("  Removing from squad\n");
			_Club.GetFirstTeamSquad().DoRemovePlayerIDFromNonUserControlledSquad(PlayerID);
		}
		WorldData().GetTransferListManager().SetOutOfContract(PlayerID, true);
		WorldData().GetOutOfContractPlayerList().Add(PlayerID, _Club.DoFindID());
  }
	else
	{
		DoRetired(_Club.DoFindID());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetSkills
	Access:    	public
	Returns:   	CSoccerPlayer&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerPlayer& CPlayer::GetSkills()
{
	return m_Skills;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoGenerateNonLeaguePlayer
	Access:    	public
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoGenerateNonLeaguePlayer(CCountry& _Country)
{
	byte ClubDivStrength = WorldData().GetDivisionStrengthList().GetSize() - 1;
	DoCreatePlayer(18 + RandomNumber.IntLessThan(15), m_Skills.RandomPosition(),
		WorldData().GetPopularityList()[WorldData().GetPopularityList().GetSize() - 1].GetRandomPlayerSkill(ClubDivStrength), _Country);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoGenerateYoungPlayer
	Access:    	public
	Parameter: 	CClub & _Club
	Parameter: 	ePlayerPosition _Position
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoGenerateYoungPlayer(CClub& _Club, ePlayerPosition _Position)
{
	byte ClubDivStrength = _Club.GetDivisionStrengthID();
	CPopularity* pPopularity = &_Club.GetPopularity();
	byte Skill;
	if (_Club.GetFirstTeamSquad().DoCountStarPlayers(pPopularity->MinimumStarPlayerSkill(ClubDivStrength)) < pPopularity->HowManyStarPlayers(ClubDivStrength))
	{
		Skill = pPopularity->RandomStarPlayerSkill(ClubDivStrength);
	}
	else
	{
		Skill = pPopularity->GetRandomPlayerSkill(ClubDivStrength);
	}

	DoCreatePlayer(18 + RandomNumber.IntLessThan(2), _Position, Skill, _Club.GetCountry());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoCreatePlayer
	Access:    	public
	Parameter: 	const ushort _Age
	Parameter: 	ePlayerPosition _Position
	Parameter: 	const byte _Skill
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoCreatePlayer(const ushort _Age, ePlayerPosition _Position, const byte _Skill, CCountry& _Country)
{
	EXT_ASSERT(_Age >= MINIMUPLAYERAGE && _Age < MAXIMUMPLAYERAGE - 3);
	EXT_ASSERT(_Skill > 10 && _Skill < 100);
	DoResetVariables();

	CPerson::DoCreatePlayer(GetSkills().DoGenerateRandomWage(_Country), _Country);
	CPerson::DoGenerateDateOfBirth(_Age);
	GetSkills().DoCreate(CPerson::GetAge(), _Position, _Skill);
	WorldData().GetTransferListManager().DoResetVariables(DoFindID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoNewSeason
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Start of season clear down
------------------------------------------------------------------------------*/
void CPlayer::DoNewSeason()
{
#ifdef STOREMANOFMATCH
	m_ManOfMatchAwards.DoNewSeason();
#endif
    m_LastSeasonCareer = m_LeagueSeasonCareer + m_CupSeasonCareer;

	m_ClubSeasonCareer.DoResetVariables();
	m_CupSeasonCareer.DoResetVariables();
	m_LeagueSeasonCareer.DoResetVariables();

	DoClearSeasonDisciplinaryPoints();
    GetSkills().DoNewSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetSkillViewStatus
	Access:    	public
	Returns:   	ePlayerStatus
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ePlayerStatus CPlayer::GetSkillViewStatus()
{
	if (IsInjured() == true)
	{
		return 	PLAYERSTATUS_INJURED;
	}
	if (IsCupTied() == true)
	{
		return PLAYERSTATUS_CUPTIED;
	}
	if (GetDiscipline().GetMatchesSuspended() != 0)
	{
		return PLAYERSTATUS_SUSPENDED;
	}
	return MAXPLAYERSTATUSES;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetMatchStatus
	Access:    	public
	Returns:   	ePlayerStatus
	Qualifier:
	Purpose:	Is the player still on the pitch?
------------------------------------------------------------------------------*/
ePlayerStatus CPlayer::GetMatchStatus()
{
    if (IsInjured() == true)
	{
		return 	PLAYERSTATUS_INJURED;
	}
	else if (BeenSentOff() == true)
	{
		return PLAYERSTATUS_SUSPENDED;
	}
	return MAXPLAYERSTATUSES;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetStatus
	Access:    	public
	Parameter: 	const ushort _ClubID
	Returns:   	ePlayerStatus
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ePlayerStatus CPlayer::GetStatus(const ClubID _ClubID)
{
	if (IsInjured() == true)
	{
		return 	PLAYERSTATUS_INJURED;
	}
	if (IsCupTied() == true)
	{
		return PLAYERSTATUS_CUPTIED;
	}
	if (GetDiscipline().GetMatchesSuspended() != 0)
	{
		return PLAYERSTATUS_SUSPENDED;
	}

	const ushort usPlayerID = DoFindID();
	if (IsOnLoan() == true)
	{
		if (WorldData().GetPlayerLoanManager().IsLoanedOut(usPlayerID, _ClubID) == true)
		{
			return PLAYERSTATUS_LOANEDIN;
		}
		return PLAYERSTATUS_LOANEDOUT;
	}
	if (WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == true)
	{
		return PLAYERSTATUS_TRANSFERLISTED;
	}
	if (GetDiscipline().IsNearSuspension() == true)
	{
		return PLAYERSTATUS_SUSPENSIONNEAR;
	}
	if (WorldData().GetTransferListManager().IsLoanListed(usPlayerID) == true)
	{
		return PLAYERSTATUS_LOANLISTED;
	}
	if (CPerson::GetContractRenewalStatus() == RETIRING)
	{
		return PLAYERSTATUS_RETIRING;
	}
	if (CPerson::GetContractEndDate().SeasonsRemaining() == 1 && CPerson::GetContractRenewalStatus() != ACCEPTED)
	{
		return PLAYERSTATUS_CONTRACTEXPIRING;
	}
	return MAXPLAYERSTATUSES;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoClearSeasonDisciplinaryPoints
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoClearSeasonDisciplinaryPoints()
{
    m_Discipline.DoNewSeason();
	m_LeagueSeasonCareer.DoClearSeasonDisciplinaryPoints();
	m_CupSeasonCareer.DoClearSeasonDisciplinaryPoints();
	m_ClubSeasonCareer.DoClearSeasonDisciplinaryPoints();
}


 // Decide whether or not to retire the player through injury
 //---------------------------------------------------------------------------
bool CPlayer::DoesPlayerRetireDoToInjury()
{
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoAdjustFeeForContractRemaining
	Access:    	public
	Parameter: 	const uint _Value
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayer::DoAdjustFeeForContractRemaining(const uint _Value)
{
	float dRet = _Value;
	// Adjust fee if not long left
	if (CPerson::GetContractEndDate().SeasonsRemaining() == 1 && CPerson::GetContractRenewalStatus() != ACCEPTED)
	{
		dRet *= .9f;
	}
	return (uint)dRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetCurrentValuationStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::GetCurrentValuationStr()
{
	if (WorldData().GetTransferListManager().IsOutOfContract(DoFindID()) == true)
	{
		CString str;
    str.LoadString(IDS_FREETRANSFER);
		return str;
	}
	return GetClub().GetForeignStr(GetSellingPrice());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetSellingPrice
	Access:    	public
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayer::GetSellingPrice()
{
	return GetSellingPrice(GetClub().GetCountry());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetSellingPrice
	Access:    	public
	Parameter: 	CCountry & _Country
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayer::GetSellingPrice(CCountry& _Country)
{
	if (WorldData().GetTransferListManager().IsTransferListed(DoFindID()))
	{
		return WorldData().GetTransferListManager().GetAskingPrice(DoFindID());
	}
	else
	{
		return GetValuation(_Country);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetValuation
	Access:    	public
	Parameter: 	CCountry & _Country
	Returns:   	uint
	Qualifier:
	Purpose:	Get a fair but random transfer valuation of player
------------------------------------------------------------------------------*/
uint CPlayer::GetValuation(CCountry& _Country)
{
	return WorldData().GetTransferListManager().GetValuation(DoFindID(), _Country, DoCalculateAdjustedFee(_Country));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoCalculateAdjustedFee
	Access:    	public
	Parameter: 	CCountry & _Country
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayer::DoCalculateAdjustedFee(CCountry& _Country)
{
	if (QualifiesForBosmanFreeTransfer())
	{
		return 0;
	}
	uint nRet = GetSkills().DoGenerateRandomTransferFee(CPerson::GetAge(), _Country, CPerson::GetTemperament());
	return DoAdjustFeeForContractRemaining(nRet);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetBidCountry
	Access:    	public
	Parameter: 	CCountry & _Country
	Parameter: 	CCountryList & _CountryList
	Returns:   	CCountry&
	Qualifier:
	Purpose:	Select a country to try and bid for player
------------------------------------------------------------------------------*/
CCountry& CPlayer::GetBidCountry(CCountry& _Country, CCountryList& _CountryList)
{
	if (ShouldMoveAbroad() != true)
	{
		return _Country;		// Same country
	}
	else
	{
		return _CountryList.DoSelectRandomActive();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::PreMatchAvailabilityReport
	Access:    	public
	Parameter: 	CMessageDisplay * _MessageDisplay
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::PreMatchAvailabilityReport(CMessageDisplay* _MessageDisplay)
{
	CString szStr;
#ifdef TEST_MATCH_REPORT
  szStr.Format(IDS_MISSMATCHINJURY, CNewsItem::PreprocessPlayerName(this).CStr());
	_MessageDisplay->AddString(szStr);
  szStr.Format(IDS_MISSMATCHSUSPENDED, CNewsItem::PreprocessPlayerName(this).CStr());
	_MessageDisplay->AddString(szStr);
#else
	if (IsInjured() == true)
	{
    szStr.Format(IDS_MISSMATCHINJURY, CNewsItem::PreprocessPlayerName(this).CStr());
		_MessageDisplay->AddString(szStr);
	}
	else if (GetDiscipline().GetMatchesSuspended() != 0)
	{
    szStr.Format(IDS_MISSMATCHSUSPENDED, CNewsItem::PreprocessPlayerName(this).CStr());
		_MessageDisplay->AddString(szStr);
	}
#endif
}

/*------------------------------------------------------------------------------
	Method:   	CPlayer::PreMatchAvailabilityReport
	Access:    	public
	Parameter: 	CMessageDisplay * _MessageDisplay
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::PreMatchAvailabilityReportFDL(lArray<CString> * _pStringList)
{
	CString szStr;
#ifdef TEST_MATCH_REPORT
  szStr.Format(IDS_MISSMATCHINJURY, CNewsItem::PreprocessPlayerName(this).CStr());
	_pStringList->Add(szStr, 1);
  szStr.Format(IDS_MISSMATCHSUSPENDED, CNewsItem::PreprocessPlayerName(this).CStr());
	_pStringList->Add(szStr, 1);
#else
	if (IsInjured() == true)
	{
    szStr.Format(IDS_MISSMATCHINJURY, CNewsItem::PreprocessPlayerName(this).CStr());
		_pStringList->Add(szStr,1);
	}
	else if (GetDiscipline().GetMatchesSuspended() != 0)
	{
    szStr.Format(IDS_MISSMATCHSUSPENDED, CNewsItem::PreprocessPlayerName(this).CStr());
		_pStringList->Add(szStr,1);
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::ShouldMoveAbroad
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::ShouldMoveAbroad()
{
	if (CPerson::GetAge() < MINMOVEABROADAGE || CPerson::GetAge() > MINMOVEABROADAGE)
	{
		return false;
	}
	switch (CPerson::GetContractEndDate().SeasonsRemaining())
	{
		case 0:
			if (RandomNumber.IntLessThan(OUTOFCONTRACTMOVEABROAD) == 2)
			{
				return true;
			}
			break;
		case 1:
			if (RandomNumber.IntLessThan(CONTRACTNEARLYOVERMOVEABROAD) == 2)
			{
				return true;
			}
			break;
		default :
			if (RandomNumber.IntLessThan(CONTRACTOKOVERMOVEABROAD) == 2)
			{
				return true;
			}
			break;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoCalculateMinimumTransferFee
	Access:    	public
	Parameter: 	CCountry & arCountry
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayer::DoCalculateMinimumTransferFee(CCountry& arCountry)
{
    return GetSkills().DoCalculateMinimumTransferFee(CPerson::GetAge(), arCountry, CPerson::GetTemperament());
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoCalculateMaximumTransferFee
	Access:    	public
	Parameter: 	CCountry & _Country
	Returns:   	uint
	Qualifier:
	Purpose:	Get the maximum transfer valuation of player
------------------------------------------------------------------------------*/
uint CPlayer::DoCalculateMaximumTransferFee(CCountry& _Country)
{
	return GetSkills().DoCalculateMaximumTransferFee(CPerson::GetAge(), _Country, CPerson::GetTemperament());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoIncrementAppearances
	Access:    	public
	Parameter: 	const eMatchCompetitionType _eMatchVenue
	Parameter: 	CClub & _Club
	Parameter: 	const byte _HowLongPlayed
	Returns:   	void
	Qualifier:
	Purpose:	Player has played in last match
------------------------------------------------------------------------------*/
void CPlayer::DoIncrementAppearances(const eMatchCompetitionType _eMatchType, CClub& _Club, const byte _HowLongPlayed)
{
	SetHowLongPlayedLastMatch(_HowLongPlayed);
    m_Career.DoIncrementAppearances();
	if (IsOnLoan() == false)
	{
		m_ClubCareer.DoIncrementAppearances();
		m_ClubSeasonCareer.DoIncrementAppearances();
		DoCheckClubPlayerRecords(_Club);
	}
	switch(_eMatchType)
	{
		case LEAGUEMATCH:
			m_LeagueSeasonCareer.DoIncrementAppearances();
			break;
		case CUPMATCH:
		case REPLAYMATCH:
		case FEDERATIONCUPMATCH:
		case CUPMATCHNEUTRALVENUE:
		case REPLAYMATCHNEUTRALVENUE:
		case FEDERATIONCUPMATCHNEUTRALVENUE:
			m_CupSeasonCareer.DoIncrementAppearances();
			m_CupCareer.DoIncrementAppearances();
        case FRIENDLYMATCH:
			break;
		default:
			EXT_ASSERT(false);
	}
    m_bitfields.m_NumberMatchesSinceLastAppeared = 0;
//	SuccessiveAppear ++;
//	SinceGoal ++;
}


 //------------------------------------------------------------------------------------------------------
CPopularity& CPlayer::GetPlayerClubPopularity()
{
	return GetClub().GetPopularity();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::NotAppear
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Player has not played in the last match
------------------------------------------------------------------------------*/
void CPlayer::NotAppear()
{
	m_bitfields.m_NumberMatchesSinceLastAppeared++;
	if (GetAge() > 20 && RandomNumber.IntLessThan(100) < CHANCELOWERMORALENOTPLAYED)
	{
		CPerson::DoLowerMorale(1);
	}
	if (GetSkills().GetFitness() > 75)
	{
		GetSkills().DoLowerFitness(5);
	}
    //SinceGoal ++;
	//SuccessiveGoal = NULL;
	//SuccessiveAppear = NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTryBookPlayer
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:	Can we book the player, if he has not been booked already and
				is still on the pitch
------------------------------------------------------------------------------*/
bool CPlayer::DoTryBookPlayer()
{
    if (GetDiscipline().GetDisciplinaryPointsThisMatch() == 0 && IsPlaying() == true)
	{
        return true;
    }
    else
	{
        return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoBookPlayer
	Access:    	public
	Parameter: 	const eMatchCompetitionType _eMatchVenue
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoBookPlayer(const eMatchCompetitionType _eMatchVenue)
{
	// Book this player
	switch(_eMatchVenue)
	{
		case LEAGUEMATCH:
			m_LeagueSeasonCareer.DoIncrementBookings();
			break;
		case CUPMATCH:
		case REPLAYMATCH:
		case FEDERATIONCUPMATCH:
		case CUPMATCHNEUTRALVENUE:
		case REPLAYMATCHNEUTRALVENUE:
		case FEDERATIONCUPMATCHNEUTRALVENUE:
			m_CupSeasonCareer.DoIncrementBookings();
			m_CupCareer.DoIncrementBookings();
        case FRIENDLYMATCH:
			break;
		default:
			EXT_ASSERT(false);
			break;
	}
    m_Discipline.DoBookPlayer();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoSendPlayerOff
	Access:    	public
	Parameter: 	const eMatchCompetitionType _eMatchVenue
	Returns:   	void
	Qualifier:
	Purpose:	Send this player off (red card)
------------------------------------------------------------------------------*/
void CPlayer::DoSendPlayerOff(const eMatchCompetitionType _eMatchVenue)
{
	switch(_eMatchVenue)
	{
		case LEAGUEMATCH:
			m_LeagueSeasonCareer.DoIncrementSendingOffs();
			break;
		case CUPMATCH:
		case REPLAYMATCH:
		case FEDERATIONCUPMATCH:
		case CUPMATCHNEUTRALVENUE:
		case REPLAYMATCHNEUTRALVENUE:
		case FEDERATIONCUPMATCHNEUTRALVENUE:
			m_CupSeasonCareer.DoIncrementSendingOffs();
			m_CupCareer.DoIncrementSendingOffs();
        case FRIENDLYMATCH:
			break;
		default:
			EXT_ASSERT(false);
	}
	GetDiscipline().DoSendPlayerOff();
}


 // Is the player still on the pitch?
 //------------------------------------------------------------------------------------------------------
bool CPlayer::IsPlaying()
{
    if (IsInjured() == false && BeenSentOff() == false)
	{
        return true;
	}
    else
	{
        return false;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsPlayingPosition
	Access:    	public
	Parameter: 	const ePlayerPosition _Position
	Returns:   	bool
	Qualifier:
	Purpose:	Is the player still on the pitch?
------------------------------------------------------------------------------*/
bool CPlayer::IsPlayingPosition(const ePlayerPosition _Position)
{
	if (IsPlaying() == true && GetSkills().GetPosition() == _Position)
	{
        return true;
	}
    else
	{
        return false;
	}
 }


 // Is the player still on the pitch?
 //------------------------------------------------------------------------------------------------------
bool CPlayer::BeenSentOff()
{
	if (GetDiscipline().GetDisciplinaryPointsThisMatch() == SENDINGOFFPOINTS)
	{
        return true;
	}
    else
	{
        return false;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::BeenPromoted
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::BeenPromoted()
{
	CPerson::DoRaiseMorale(5);
}


 //------------------------------------------------------------------------------------------------------
void CPlayer::BeenRelegated()
{
	CPerson::DoLowerMorale(5);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoCalculateEffectiveSkill
	Access:    	public
	Parameter: 	const ePlayerWhichSkill _WhichSkill
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::DoCalculateEffectiveSkill(const ePlayerWhichSkill _WhichSkill)
{
	if (GetInjuryEffect() == 0)
	{
		return 0;
	}
	float Value = 0.0f;
	switch (_WhichSkill)
	{
		case SKILL_OVERALL:
			Value = static_cast<float>(GetSkills().GetOverallSkill());
			break;
		case SKILL_HANDLING:
			Value = static_cast<float>(GetSkills().GetHandling());
			break;
		case SKILL_TACKLING:
			Value = static_cast<float>(GetSkills().GetTackling());
			break;
		case SKILL_PASSING:
			Value = static_cast<float>(GetSkills().GetPassing());
			break;
		case SKILL_SHOOTING:
			Value = static_cast<float>(GetSkills().GetShooting());
			break;
		case SKILL_HEADING:
			Value = static_cast<float>(GetSkills().GetHeading());
			break;
		default:
			EXT_ASSERT(false);
			break;
	}
	Value *= GetSkills().SkillRatingMultiplier();
  EXT_ASSERT(Value <= 160);
	Value *= GetInjuryEffect();
  EXT_ASSERT(Value <= 170);
	Value *= CPerson::GetMoraleEffect();
	Value *= GetSkills().GetEnergyEffect();
	return min(MAXPLAYSKILL, max(MINPLAYSKILL, static_cast<byte> (Value + 0.5)));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoGoingOnLoan
	Access:    	public
	Parameter: 	CClub * _ClubFrom
	Parameter: 	int _HowLong
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoGoingOnLoan(CClub* _ClubFrom, byte _HowLong)
{
	EXT_ASSERT(_HowLong > 0 && _HowLong <= MAXLOANPLAYERWEEKS);		// Loan Period invalid
	ClubID _ClubID;
	_ClubID.id = NOCLUB;
	if (_ClubFrom != NULL)
	{
		_ClubID = _ClubFrom->DoFindID();
	}
	WorldData().GetPlayerLoanManager().DoGoingOnLoan(DoFindID(), _ClubID, _HowLong);
#ifdef NDS
  	m_bitfields.m_bOnLoan = true;
#else
	m_bitfields.m_HasLoanRecord = true;
#endif
	DoTransferRequestCanceled();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsOnLoan
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsOnLoan()
{
#ifdef NDS
	return m_bitfields.m_bOnLoan;
#else
	if (m_bitfields.m_HasLoanRecord == true)
	{
		return WorldData().GetPlayerLoanManager().IsOnLoan(m_ID);
	}
	return false;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoScoredGoal
	Access:    	public
	Parameter: 	const eMatchCompetitionType _MatchType
	Returns:   	void
	Qualifier:
	Purpose:	Has scored a goal
------------------------------------------------------------------------------*/
void CPlayer::DoScoredGoal(const eMatchCompetitionType _MatchType)
{
    switch(_MatchType)
	{
        case FRIENDLYMATCH:
            return;
            break;
        case LEAGUEMATCH:
			m_LeagueSeasonCareer.DoIncrementGoals();
            break;
        case CUPMATCH:
        case REPLAYMATCH:
        case FEDERATIONCUPMATCH:
        case CUPMATCHNEUTRALVENUE:
        case REPLAYMATCHNEUTRALVENUE:
        case FEDERATIONCUPMATCHNEUTRALVENUE:
    		m_CupSeasonCareer.DoIncrementGoals();
			m_CupCareer.DoIncrementGoals();
            break;
        default :
            EXT_ASSERT(false);
    }
    // Don't add to club totals if on loan
    if (IsOnLoan() == false)
	{
		m_ClubCareer.DoIncrementGoals();
		m_ClubSeasonCareer.DoIncrementGoals();
    }
	m_Career.DoIncrementGoals();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetClub
	Access:    	public
	Returns:   	CClub&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub& CPlayer::GetClub()
{
	return *WorldData().GetClubList().DoFindPlayerClub(this);
}


/*------------------------------------------------------------------------------
  Method:     CPlayer::GetClubPtr
	Access:    	public
  Returns:    CClub*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub *CPlayer::GetClubPtr()
{
  return WorldData().GetClubList().DoFindPlayerClub(this);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::ClubName
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::ClubName()
{
	if (WorldData().GetTransferListManager().IsOutOfContract(DoFindID()) == false)
    {
		return GetClub().GetName();
	}
	else
	{
		return TEXT("-");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::PlayerAndClubName
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::PlayerAndClubName()
{
	CString str;
  str.Format(IDS_PLAYERANDCLUB, CNewsItem::PreprocessPlayerName(this).CStr(), CNewsItem::PreprocessClubName(GetClubPtr()).CStr());
    return str;
}


 //------------------------------------------------------------------------------------------------------
/*ushort CPlayer::GetPlayerClubID()
{
	return WorldData().GetClubList().DoFindPlayerClubNumber(this);
}*/


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoConcedeGoal
	Access:    	public
	Parameter: 	const eMatchCompetitionType _eMatchType
	Returns:   	void
	Qualifier:
	Purpose:	If a goalkeeper then has conceded a goal
------------------------------------------------------------------------------*/
void CPlayer::DoConcedeGoal(const eMatchCompetitionType _eMatchType)
{
    if (GetSkills().GetPosition() == GOALKEEPER)
	{
		DoScoredGoal(_eMatchType);
    }
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::QualifiesForBosmanFreeTransfer
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::QualifiesForBosmanFreeTransfer()
{
	if (WorldData().GetTransferListManager().IsOutOfContract(DoFindID()) == true ||
		(CPerson::GetContractEndDate().ContractLengthInDays() < 182 && CPerson::GetAge() >= BOSMANAGE))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsAvailableOnBosmanFreeTransfer
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsAvailableOnBosmanFreeTransfer()
{
	if (CPerson::GetAge() >= BOSMANAGE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoBackFromInjury
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoBackFromInjury()
{
	GetInjury()->DoResetVariables();
	CPerson::DoRaiseMorale(20);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DailyTasks
	Access:    	public
	Parameter: 	CClub * _pClub
	Parameter: 	const bool _IsWeekEnd
	Parameter: 	const bool _DoTraining
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DailyTasks(CClub* _pClub, const bool _IsWeekEnd, const bool _DoTraining)
{
  // 1 date less injured
  const ushort usPlayerID = DoFindID();
  if (IsInjured() == true)
	{
        if (GetInjury()->DailyImprove(*_pClub) == true)
		{
			DoBackFromInjury();
        }
    }
	int ContractSeasonsLeft = GetContractEndDate().SeasonsRemaining();
    if (_pClub != NULL)
	{
    if (ContractSeasonsLeft == 0 && IsOnLoan() == false)
		{
      if(QualifiesForBosmanFreeTransfer() == true)
      {
        bool bWillSign = WillSignMsg(*_pClub, false);
        if ( bWillSign )
        {
          if (_pClub->IsUserControlled() == true)
          {
            CExtString str;
            str.Format(IDS_PLAYERCONTRACTEXPIRESTODAY, CNewsItem::PreprocessPlayerName(this).CStr());
            _pClub->AddToClubNews(str);

            // The dialog box can't have a link in it.
            str.Format(IDS_PLAYERCONTRACTEXPIRESTODAY, GetName().CStr());
            WorldData().DoNegotiatePlayerContract(this, _pClub, str);
          }
        }
        else
        {
          // Non user club
          if (_pClub->IsUserControlled() != true)
          {
            if (CPerson::GetAge() < 39)
            {
              if (_pClub->GetFirstTeamSquad().CanAffordToLosePlayer(this) == false || RandomNumber.IntLessThan(100) < CHANCEPLAYERRENEWCONTRACT)
              {
                CContract ContractRequired;
                GetSkills().DoGenerateRandomContract(ContractRequired, _pClub->GetCountry(), GetAge(), true);
                ContractRequired.SetContractLength(ContractRequired.GetContractLength() + 1);
                DoUpdateContractOffered(ContractRequired);
                ContractSeasonsLeft = GetContractEndDate().SeasonsRemaining();    // Update, as may have been extended
                EXT_ASSERT(GetAge() + ContractSeasonsLeft < 42);          // No player play past 42 years old
              }
            }
          }
        }
      }
			if (ContractSeasonsLeft == 0)
			{
				DoContractTerminated(*_pClub, false);
				return;
			}
		}
		if (_IsWeekEnd == true)
		{
			if (_DoTraining == true)
			{
				DoTraining(*_pClub);
			}

			// Been here a week longer if not on loan
			if (IsOnLoan() == false)
			{
				CPerson::AddWeeksHere(1);
			}
			// Been on transfer list a week longer if listed
			WorldData().GetTransferListManager().WeeklyTasks(usPlayerID);
		}

		if (_pClub->IsUserControlled() == false && CPerson::GetContractRenewalStatus() != RETIRING)
		{
			// Take off transfer list?
			if (WorldData().GetTransferListManager().IsTransferListed(usPlayerID) && RandomNumber.IntLessThan(OFFLISTCHANCE) == 2)
			{
				DoTakeOffTransferList();
			}
			/*
			if (WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == false && RandomNumber.IntLessThan(REVALUECHANCE) == 2)
			{
				WorldData().GetTransferListManager().SetAskingPrice(usPlayerID, GetValuation(_pClub->GetCountry()));
			}
			*/
		}
		else
		{
			if (IsOnLoan() == false && CPerson::GetAge() > 31 && RandomNumber.IntLessThan(100) < 16)
			{
				//if ((_pClub->GetActiveCountry().IsOneYearSeason() == true && (WorldData().GetCurrentDate().GetMonth() > MAY && WorldData().GetCurrentDate().GetMonth() < NOVEMBER)) ||
				//	((_pClub->GetActiveCountry().IsOneYearSeason() == false && WorldData().GetCurrentDate().GetMonth() > JUNE)))
				{
					if (ContractSeasonsLeft > 0)
					{
						DoCheckIfRetire(*_pClub);
					}
				}
			}
		}
	}
    GetSkills().DoRejuvenateEnergy(CPerson::GetAge());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTakeOffTransferList
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoTakeOffTransferList()
{
	WorldData().GetTransferListManager().DoTakeOffTransferList(DoFindID());
	CPerson::SetContractRenewalStatus(NONEINPROGRESS);
	CPerson::DoRaiseMorale(10);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoUpdateTransferStatus
	Access:    	public
	Parameter: 	bool bToList
	Parameter: 	const uint anFee
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoUpdateTransferStatus(bool bToList, const uint anFee)
{
	if (bToList)
	{
		if (WorldData().GetTransferListManager().IsTransferListed(DoFindID()))
		{
			// Already listed so just update the asking price
			WorldData().GetTransferListManager().SetAskingPrice(DoFindID(), anFee);
		}
		else
		{
			DoTransferList(anFee, GetClub().GetCountry());
		}
	}
	else
	{
		DoTakeOffTransferList();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsFairPrice
	Access:    	public
	Parameter: 	const float adAmount
	Parameter: 	CCountry arCountry
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsFairPrice(const float adAmount, CCountry arCountry)
{
  	float FairPrice = (float)GetSellingPrice(arCountry);
	if (adAmount < FairPrice * .94f || \
    	IsBetween(adAmount, FairPrice * .95f,  FairPrice * .104f) && RandomNumber.IntLessThan(100) > 4 || \
    	IsBetween(adAmount, FairPrice * 1.05f, FairPrice * 1.19f) && RandomNumber.IntLessThan(100) > 8 || \
    	IsBetween(adAmount, FairPrice * 1.20f, FairPrice * 1.34f) && RandomNumber.IntLessThan(100) > 15 || \
    	IsBetween(adAmount, FairPrice * 1.35f, FairPrice * 1.49f) && RandomNumber.IntLessThan(100) > 25 || \
    	IsBetween(adAmount, FairPrice * 1.50f, FairPrice * 1.74f) && RandomNumber.IntLessThan(100) > 40 || \
    	IsBetween(adAmount, FairPrice * 1.75f, FairPrice * 1.99f) && RandomNumber.IntLessThan(100) > 90 || \
		adAmount > FairPrice * 2 && RandomNumber.IntLessThan(100) > 90)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoAfterMatchInTeamUpdate
	Access:    	public
	Parameter: 	CClub & _Club
	Parameter: 	const byte _Performance
	Returns:   	void
	Qualifier:
	Purpose:	After match update(if played last match)
------------------------------------------------------------------------------*/
void CPlayer::DoAfterMatchInTeamUpdate(CClub& _Club, const byte _Performance)
{
	GetSkills().DoAfterMatchUpdate(CPerson::GetAge(), _Performance);
	GetSkills().AfterMatchEnergyAndFitnessUpdate(GetHowLongPlayedLastMatch());
	m_Discipline.DoAfterMatchInTeamUpdate(_Club, *this);
	SetInjuryMatchPerformanceEffect(100);
	m_bitfields.m_HowLongPlayedLastMatch = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::SetInjuryMatchPerformanceEffect
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::SetInjuryMatchPerformanceEffect(const byte _Value)
{
	if (GetInjury() != NULL)
	{
		// If played on in match then flag as unavailable until injury is cleared
		GetInjury()->SetMatchPerformanceEffect(_Value);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoAfterMatchInSquadUpdate
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:	After match update if did not play in the match
------------------------------------------------------------------------------*/
void CPlayer::DoAfterMatchInSquadUpdate(CClub& _Club)
{
	NotAppear();
	// If is suspended, 1 match less suspended
	GetDiscipline().AfterMatchInSquadUpdate(_Club, *this);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetContractResponseStr
	Access:    	public
	Parameter: 	eContractOfferResponse eResponse
	Parameter: 	CContract * ContractOffered
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::GetContractResponseStr(eContractOfferResponse eResponse, CContract* ContractOffered)
{
	EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
	CString szStr;
	switch(eResponse)
	{
		case ACCEPTED:
          szStr.Format(IDS_CONTRACTACCEPTED, CNewsItem::PreprocessPlayerName(this).CStr(),
                    CNewsItem::PreprocessClubName(WorldData().GetCurrentUserClub()).CStr());
			break;
        case MORESIGNON:
          szStr.Format(IDS_SIGNONFEETOOLOW, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
        case LESSSIGNON:
      szStr.Format(IDS_SIGNONFEETOOHIGH, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
        case MOREWAGE:
      szStr.Format(IDS_WAGETOOLOW, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
		case LESSWAGE:
      szStr.Format(IDS_WAGETOOHIGH, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
        case LONGERCONTRACT:
      szStr.Format(IDS_CONTRACTTOOSHORTCLUB, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
        case SHORTERCONTRACT:
      szStr.Format(IDS_CONTRACTTOOLONG, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
		case LISTED:
      szStr.Format(IDS_WONTRENEWWANTSRANSFER, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
		case RETIRING:
      szStr.Format(IDS_WONTRENEWRETIRE, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
        case REFUSED:
      szStr.Format(IDS_NOTINTERESTEDINCONTRACT, CNewsItem::PreprocessPlayerName(this).CStr());
			break;
		case NONEINPROGRESS:
        case NEGOTIATING:
		default :
			EXT_ASSERT(false);		// Unhandled result
	}
    return szStr;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::TryAcceptContract
	Access:    	public
	Parameter: 	CContract * ContractRequired
	Parameter: 	CContract * ContractOffered
	Parameter: 	CClub & _OfferingClub
	Returns:   	eContractOfferResponse
	Qualifier:
	Purpose:	Offer the player a new contract
------------------------------------------------------------------------------*/
eContractOfferResponse CPlayer::TryAcceptContract(CContract* ContractRequired, CContract* ContractOffered, CClub& _OfferingClub)
{
	EXT_ASSERT_POINTER(ContractRequired, CContract);
	EXT_ASSERT_POINTER(ContractOffered, CContract);
	if (GetAge() > 37 || CPerson::GetContractRenewalStatus() == RETIRING)
	{
		return RETIRING;
	}

	if (WillSignTransferMsg(_OfferingClub) != TEXT(""))
	{
		return REFUSED;
	}

	// Do checks if requirements not exactly met
	if (ContractOffered->IsBetterThan(ContractRequired) == false)
	{
		eContractOfferResponse Reason = GetSkills().TryAcceptContract(ContractRequired, ContractOffered, _OfferingClub.GetCountry().GetFinancialRatioPercentage(), QualifiesForBosmanFreeTransfer());
	    if (Reason != NONEINPROGRESS)
		{
		    return Reason;
		}

		if ((ContractOffered->GetContractLength() < ContractRequired->GetContractLength()) &&
			((int)RandomNumber.IntLessThan(ContractRequired->GetContractLength() + 1) > (ContractRequired->GetContractLength() - ContractOffered->GetContractLength())))
		{
			return LONGERCONTRACT;
		}

		if (GetAge() > 32 && ContractOffered->GetContractLength() > 2)
		{
			return SHORTERCONTRACT;
		}

		if (ContractOffered->GetContractLength() > 6 || ((ContractOffered->GetContractLength() > ContractRequired->GetContractLength()) &&
			((int)RandomNumber.IntLessThan(ContractOffered->GetContractLength() + 1) > (ContractOffered->GetContractLength() - ContractRequired->GetContractLength()))))
		{
			return SHORTERCONTRACT;
		}

	}

    // Player has accepted
    return ACCEPTED;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoGenerateRandomContract
	Access:    	public
	Parameter: 	CContract & _ContractOffered
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoGenerateRandomContract(CContract& _ContractOffered, CCountry& _Country)
{
	GetSkills().DoGenerateRandomContract(_ContractOffered, _Country, GetAge(), QualifiesForBosmanFreeTransfer());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::PercentageGamesPlayedThisSeason
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayer::PercentageGamesPlayedThisSeason()
{
	return (GetClub().DoCountLeagueGamesPlayedThisSeason() / 100) * GetClubSeasonAppear();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsInFirstTeam
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsInFirstTeam()
{
    if (GetClub().GetFirstTeamSquad().DoFindSquadNumber(WorldData().GetPlayerList().Find(this)) < PLAYERSINTEAM)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::TryAcceptLoanTransferBid
	Access:    	public
	Parameter: 	CClub * pBiddingClub
	Parameter: 	CString * szResponse
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::TryAcceptLoanTransferBid(CClub* pBiddingClub, CString* szResponse)
{
	CString RefusedStr;
  szResponse->Format(IDS_PLAYERREFUSEDLOAN, CNewsItem::PreprocessPlayerName(this).CStr());
	// Check if injured
	if (IsAvailableForMatch() == false)
	{
		return false;
	}
	// Check if nearly suspended
	if (GetDiscipline().GetDisciplinaryPoints() > POINTSFORSUSPENSION - RandomNumber.IntLessThan(POINTSFORSUSPENSION / 2))
	{
		return false;
	}
	// Check if played in approx 50% of games this season
	if (PercentageGamesPlayedThisSeason() > (40 + RandomNumber.IntLessThan(20)))
	{
    szResponse->Format(IDS_PLAYERREFUSEDLOANWHILSTPLAYING, CNewsItem::PreprocessPlayerName(this).CStr());
		return false;
	}
	// Check if in team at the moment
	if (IsInFirstTeam() == true)
	{
    szResponse->Format(IDS_PLAYERREFUSEDLOANWHILSTPLAYING, CNewsItem::PreprocessPlayerName(this).CStr());
		return false;
	}
	// Check skills appropriate for the club
    if (pBiddingClub->GetPopularity().IsPlayerSkillInRange(GetSkills().GetOverallSkill(), pBiddingClub->GetDivisionStrengthID()) == false)
	{
    szResponse->Format(IDS_PLAYERREFUSEDLOANCHANCESLIMITED, CNewsItem::PreprocessPlayerName(this).CStr());
        return false;
	}
  szResponse->Format(IDS_PLAYERLOANAGREED, CNewsItem::PreprocessClubName(GetClubPtr()).CStr(), CNewsItem::PreprocessPlayerName(this).CStr());
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::NumberMatchesSinceLastAppeared
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::NumberMatchesSinceLastAppeared()
{
	return m_bitfields.m_NumberMatchesSinceLastAppeared;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::SetHowLongPlayedLastMatch
	Access:    	public
	Parameter: 	const byte _HowLongPlayed
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::SetHowLongPlayedLastMatch(const byte _HowLongPlayed)
{
	m_bitfields.m_HowLongPlayedLastMatch = _HowLongPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetHowLongPlayedLastMatch
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::GetHowLongPlayedLastMatch() const
{
	return m_bitfields.m_HowLongPlayedLastMatch;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoCheckIfRetire
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:	See if he wants to retire
------------------------------------------------------------------------------*/
void CPlayer::DoCheckIfRetire(CClub& _Club)
{
	if (CPerson::GetContractRenewalStatus() != RETIRING && CPerson::GetContractEndDate().SeasonsRemaining() == 1 && CPerson::GetAge() > 31)
	{
// 		DEBUG_OUTPUT(("Retirement Attempt %d"), (CPerson::GetAge()));
		if ((RandomNumber.IntLessThan(100) > (42 - static_cast<uint>(CPerson::GetAge())) * 8) || CPerson::GetAge() > 39)
		{
			CPerson::SetContractRenewalStatus(RETIRING);
			CExtString str;
      str.Format(IDS_RETIREATENDOFCONTRACT, CNewsItem::PreprocessPlayerName(this).CStr());
			_Club.AddToClubNews(str);
//@@	_pClub->NewsPaperEventPtr()->CheckIfNewsWorthyPlayerRetirement(WorldData().PlayerList()->Find(this));
		}
	}
}


 // Decide whether to retire or leave at end of contract
 //---------------------------------------------------------------------------
bool CPlayer::WillingToNegotiateNewContract(CClub& _Club)
{
	EXT_ASSERT(IsOnLoan() == false);		// Not on loan
	DoCheckIfRetire(_Club);
	if (CPerson::GetContractRenewalStatus() == RETIRING || CPerson::GetAge() > 38)
	{
		return false;
	}
	CExtString str;
 	// See if he wishes to move to a better club
	if (CPerson::GetAge() < 26 && GetSkills().GetOverallSkill() > _Club.GetPopularity().GetGoodPlayer() && RandomNumber.IntLessThan(100) > YOUNGPLAYERTOOGOODFORCONTRACT)
	{
		CPerson::DoLowerMorale(10);
		CPerson::SetContractRenewalStatus(LISTED);
        str.Format(IDS_LEAVEATENDOFCONTRACTBETTERCLUB, CNewsItem::PreprocessPlayerName(this).CStr());
		_Club.AddToClubNews(str);
//@@	_Club.NewsPaperEventPtr()->CheckIfNewsWorthyPlayerLeaveAtEndOfContract(WorldData().PlayerList()->Find(this));
		return false;
	}

	// See if he wishes to move due to lack of games
	if (GetClubAppearances() < (CPerson::GetWeeksHere() / 10))
	{
		CPerson::DoLowerMorale(10);
		CPerson::SetContractRenewalStatus(REFUSED);
        str.Format(IDS_LEAVEATENDOFCONTRACTFIRSTTEAM, CNewsItem::PreprocessPlayerName(this).CStr());
		_Club.AddToClubNews(str);
//@@	_Club.NewsPaperEventPtr()->CheckIfNewsWorthyPlayerLeaveAtEndOfContract(WorldData().PlayerList()->Find(this));
		return false;
	}

	// See if he wishes to move anyway
	if (RandomNumber.IntLessThan(CHANCEREFUSECONTRACT) == 1)
	{
		CPerson::DoLowerMorale(10);
		CPerson::SetContractRenewalStatus(REFUSED);
        str.Format(IDS_LEAVEATENDOFCONTRACT, CNewsItem::PreprocessPlayerName(this).CStr(), CNewsItem::PreprocessClubName(&_Club).CStr());
		_Club.AddToClubNews(str);
//@@	_Club.NewsPaperEventPtr()->CheckIfNewsWorthyPlayerLeaveAtEndOfContract(WorldData().PlayerList()->Find(this));
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::ValidSearchListed
	Access:    	public
	Parameter: 	CPlayerSearch & _PlayerSearch
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::ValidSearchListed(CPlayerSearch& _PlayerSearch)
{
	if (_PlayerSearch.GetFindPosn() !=  MAXPLAYERPOSITIONS && _PlayerSearch.GetFindPosn() != GetSkills().GetPosition())
	{
		return false;
	}

	// Check if skill in range first
	if (GetSkills().GetOverallSkill() < _PlayerSearch.GetMinSkill() || GetSkills().GetOverallSkill() > _PlayerSearch.GetMaxSkill())
	{
		return false;
	}

	// Not while on loan or is retiring
	if (IsOnLoan() == true || CPerson::GetContractRenewalStatus() == RETIRING)
	{
		return false;
	}

	const ushort usPlayerID = DoFindID();
	if (_PlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDEOUTOFCONTRACT) == false && WorldData().GetTransferListManager().IsOutOfContract(usPlayerID) == true)
	{
		return false;
	}

	if (_PlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDEINJURED) == false && IsInjured() == true)
	{
		return false;
	}

	if (_PlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDEONTRANSFERLIST) == false && WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == true)
	{
		return false;
	}

	if (_PlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDEONLOANLIST) == false && WorldData().GetTransferListManager().IsLoanListed(usPlayerID) == true)
	{
		return false;
	}

	if (_PlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDENONLISTED) == false && WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == false && WorldData().GetTransferListManager().IsOutOfContract(usPlayerID) == false)
	{
		return false;
	}

	if (_PlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDEOUTOFCONTRACT) == false && WorldData().GetTransferListManager().IsOutOfContract(usPlayerID) == true)
	{
		return false;
	}

	if (_PlayerSearch.GetIncludeFlag(CPlayerSearch::INCLUDESUSPENDED) == false && GetDiscipline().GetMatchesSuspended() > 0)
	{
		return false;
	}

	if (CPerson::GetAge() < _PlayerSearch.GetMinAge() || CPerson::GetAge() > _PlayerSearch.GetMaxAge())
	{
		return false;
	}

	uint Value;
	if (WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == true)
	{
		Value = WorldData().GetTransferListManager().GetAskingPrice(usPlayerID);
	}
	else
	{
    Value = GetValuation(WorldData().GetCurrentUser()->GetCountry());
    //GetSkills().SuggestedTransferFee(CPerson::GetAge(), CPerson::GetTemperament(), 100);
	}
	if (Value < _PlayerSearch.GetMinPrice() || Value > _PlayerSearch.GetMaxPrice())
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTransferList
	Access:    	public
	Parameter: 	const uint _AskingPrice
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier:
	Purpose:	Player is available for transfer
------------------------------------------------------------------------------*/
void CPlayer::DoTransferList(const uint _AskingPrice, CCountry& _Country)
{
	EXT_ASSERT(IsOnLoan() == false);	// Not while on loan
	if (GetRating() > 60)
	{
		EXT_ASSERT(_AskingPrice > 0);
	}
	WorldData().GetTransferListManager().DoTransferList(DoFindID(), _Country.GetCurrency().RoundedPounds(_AskingPrice));
	CPerson::DoLowerMorale(20);
	CPerson::SetContractRenewalStatus(LISTED);
// 	g_DebugStr.Format(_T("%s listed by %s"), GetName(), GetClub().GetName());
// 	g_DebugStr.OutputDebug();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::LastClubName
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::LastClubName()
{
#ifdef PLAYERTRANSFERHISTORY
	return m_PlayerTransferHistory.LastClubName();
#else
	CString szStr;
  szStr.LoadString(IDS_NODATA);
	return szStr;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTryListPlayer
	Access:    	public
	Parameter: 	uint _AskingPrice
	Parameter: 	CCountry & _Country
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::DoTryListPlayer(uint _AskingPrice, CCountry& _Country, const bool _bSacked)
{
	EXT_ASSERT(IsOnLoan() == false);	// Not while on loan
	if (_bSacked == false)
	{
		if (_AskingPrice == NOTFOUND)
		{
			_AskingPrice = GetSuggestedAskingPrice(_Country);
		}
		if (_AskingPrice > GetSkills().DoCalculateMaximumTransferFee(CPerson::GetAge(), _Country, CPerson::GetTemperament()))
		{
			// Inform user too high
			CString str;
      str.Format(IDS_LISTTOOHIGH,
						_Country.GetCurrency().GetForeignStr(GetSkills().DoCalculateMaximumTransferFee(CPerson::GetAge(), _Country, CPerson::GetTemperament())),
						CNewsItem::PreprocessPlayerName(this).CStr()
					);
			UserMessageBox(str, OKBOX);

			// List at maximum price
			DoTransferList(GetSkills().DoCalculateMaximumTransferFee(CPerson::GetAge(), _Country, CPerson::GetTemperament()), _Country);
			return false;
		}
		if (_AskingPrice < GetSkills().DoCalculateMinimumTransferFee(CPerson::GetAge(), _Country, CPerson::GetTemperament()))
		{
			// Inform user too low
			CString str;
      str.Format(IDS_LISTTOOLOW, CNewsItem::PreprocessPlayerName(this).CStr(), _Country.GetCurrency().GetForeignStr(GetSkills().DoCalculateMinimumTransferFee(CPerson::GetAge(), _Country, CPerson::GetTemperament())));
    	    UserMessageBox(str, OKBOX);

			// List at minimum price
			DoTransferList(GetSkills().DoCalculateMinimumTransferFee(CPerson::GetAge(), _Country, CPerson::GetTemperament()), _Country);
			return false;
		}
	}
	DoTransferList(_AskingPrice, _Country);
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTraining
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoTraining(CClub& _Club)
{
	if (IsInjured() == false)
	{
        GetSkills().DoTrain(CPerson::GetAge(), _Club);
	}
	else
	{
		GetSkills().DoInjuredTraining();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoJoinedNewClub
	Access:    	public
	Parameter: 	CContract & _PlayerContract
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoJoinedNewClub(CContract& _PlayerContract)
{
	m_ClubCareer.DoResetVariables();
	m_ClubSeasonCareer.DoResetVariables();
    CPerson::SetWeeksHere(0);
	EXT_ASSERT(GetAge() + _PlayerContract.GetContractLength() < 42);
	CPerson::DoGiveNewContract(_PlayerContract.GetContractLength(), _PlayerContract.GetWage(), false);
	WorldData().GetTransferListManager().DoJoinedNewClub(DoFindID());
	DoTakeOffTransferList();						// Pass true to ensure leaves contract alone
#ifdef STOREMANOFMATCH
	m_ManOfMatchAwards.DoJoinedNewClub();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoNewContractRequest
	Access:    	public
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoNewContractRequest(CClub& _Club)
{
	// Not while on loan
	if (IsOnLoan() == true /*|| WorldData().IsFastTest() == true*/)
	{
			return;
	}
	if (CPerson::GetContractEndDate().SeasonsRemaining() < 2)
	{
		switch(CPerson::GetContractRenewalStatus())
		{
			case NONEINPROGRESS:
				if (WillingToNegotiateNewContract(_Club) == true)
				{
					CExtString str;
          str.Format(IDS_REQUESTSNEWCONTRACT, CNewsItem::PreprocessPlayerName(this).CStr());
					_Club.AddToClubNews(str);
				}
				break;
			case ACCEPTED:
			case NEGOTIATING:
			case REFUSED:
			case MORESIGNON:
			case LESSSIGNON:
			case MOREWAGE:
			case LESSWAGE:
			case LONGERCONTRACT:
			case SHORTERCONTRACT:
			case LISTED:
			case CLUBREFUSED:
			case RETIRING:
			case RETIRED:
				break;
			default :
				EXT_ASSERT(false);

		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::ContractValue
	Access:    	public
	Returns:   	uint
	Qualifier:
	Purpose:	How much will it cost to release this player from his contract?
------------------------------------------------------------------------------*/
uint CPlayer::ContractValue()
{
	EXT_ASSERT(IsOnLoan() == false);	// Not while on loan
	if (CPerson::GetContractEndDate().SeasonsRemaining() > 0)
	{
		return((CPerson::GetContractEndDate().SeasonsRemaining() * 52) - WorldData().GetCurrentDate().GetWeek()) * CPerson::GetWage();
	}
	else
	{
		// Out of contract
		return 0;
	}
 }


 // Calculate the player's value
 //---------------------------------------------------------------------------
CString CPlayer::SuggestedAskingPriceStr()
{
	return GetClub().GetForeignStr(GetSkills().SuggestedTransferFee(CPerson::GetAge(),
																	 CPerson::GetTemperament(),
																	 GetClub().GetCountry().GetFinancialRatioPercentage()));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetSuggestedAskingPrice
	Access:    	public
	Parameter: 	CCountry & _Country
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayer::GetSuggestedAskingPrice(CCountry& _Country)
{
  	uint Value = GetSkills().SuggestedTransferFee(CPerson::GetAge(), CPerson::GetTemperament(), _Country.GetFinancialRatioPercentage());
	return _Country.GetCurrency().RoundedPounds(DoAdjustFeeForContractRemaining(Value));
}


 // create the player from a youth player
 //---------------------------------------------------------------------------------------
void CPlayer::TransferFromYouth(CYouth* _Youth, CContract* _Contract)
{
	EXT_ASSERT_POINTER(_Youth, CYouth);
	EXT_ASSERT(_Contract != null);
    DoResetVariables();
    this->DoCopy(_Youth);
	EXT_ASSERT(GetAge() + _Contract->GetContractLength() < 42);
	CPerson::DoGiveNewContract(_Contract->GetContractLength(), _Contract->GetWage(), false);
	WorldData().GetTransferListManager().DoJoinedNewClub(DoFindID());
	GetSkills() = _Youth->GetSkills();
 }


 // Determine if the player should be sacked, if his value is to high then refuse
 //---------------------------------------------------------------------------------------
bool CPlayer::AllowSack()
{
	if (IsOnLoan() == true)
	{
		return false;
	}
	if (GetAge() < 33 && GetAge() > 18)
	{
		if (GetPlayerClubPopularity().GetSackSkill() < GetSkills().GetOverallSkill())
		{
			return false;
		}
	}
	return true;
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsInjured
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsInjured()
{
	CInjury* ptrInjury = GetInjury();
	if (ptrInjury != NULL)
	{
		if (ptrInjury->GetMatchPerformanceEffect() == 100)
		{
			return true;
		}
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsAvailableForMatch
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::IsAvailableForMatch()
{
	if (IsInjured() == true || IsCupTied() == true || BeenSentOff() == true || GetDiscipline().GetMatchesSuspended() != 0)
	{
        return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::MatchAvailabilityStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString	CPlayer::MatchAvailabilityStr()
{
	CString szRet;
  szRet.LoadString(IDS_FULLY_AVAILABLE);

	if (IsInjured() == true)
	{
		if (GetInjury()->HowLongRemainingUnableToPlay() == 1)
		{
      szRet.LoadString(IDS_INJURED);
		}
		else
		{
      szRet.Format(IDS_INJUREDPLURAL, GetInjury()->HowLongRemainingUnableToPlay());
		}
	}

	if (GetDiscipline().GetMatchesSuspended() != 0)
	{
		if (GetDiscipline().GetMatchesSuspended() == 1)
		{
      szRet.LoadString(IDS_SUPSENDED);
		}
		else
		{
      szRet.Format(IDS_SUPSENDEDPLURAL, GetDiscipline().GetMatchesSuspended());
		}
	}

	if (IsCupTied() == true)
	{
    szRet.LoadString(IDS_CUPTIED);
	}
	return szRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::IsCupTied
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:	Is the player cup tied for the next match?
------------------------------------------------------------------------------*/
bool CPlayer::IsCupTied()
{
	bool bRet = false;
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::CheckAskForTransfer
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:	Should the player request a transfer?
------------------------------------------------------------------------------*/
bool CPlayer::CheckAskForTransfer()
{
	// Not while on loan or is already on transfer list
	if (IsOnLoan() == true || WorldData().GetTransferListManager().IsTransferListed(DoFindID()) == true)
	{
		return false;
	}

	// Not if played a game recently or Not unless played at least 10 games for the club
	if (m_bitfields.m_NumberMatchesSinceLastAppeared < 10 || GetClubAppearances() < 10)
	{
		return false;
	}

	if (IsInjured() == true)
	{
		return false;
	}

	if (CPerson::GetAge() < 21 || CPerson::GetAge()  > 33 || CPerson::GetContractRenewalStatus() != NONEINPROGRESS)
	{
		return false;
	}
	// Not if in last year of contract as will be able to get a Bosman at end of season
	if (CPerson::GetContractEndDate().SeasonsRemaining() < 2)
	{
		return false;
	}
	return true;
}


 /*------------------------------------------------------------------------------
	Method:   	CPlayer::OnTransferRequestGranted
	Access:    	public
	Parameter: 	CCountry & _Country
	Returns:   	bool
	Qualifier:
	Purpose:	Club has granted a transfer request
------------------------------------------------------------------------------*/
bool CPlayer::OnTransferRequestGranted(CCountry& _Country)
{
	EXT_ASSERT(IsOnLoan() == false);	// Not while on loan
	bool bRet = false;
	CPerson::DoRaiseMorale(10);
	CPerson::SetContractRenewalStatus(LISTED);
	// If not already got an asking price then give him one.
	const ushort usPlayerID = DoFindID();
	if (WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == false)
	{
		WorldData().GetTransferListManager().DoTransferList(usPlayerID, GetValuation(_Country));
		bRet = true;
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoTransferRequestCanceled
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoTransferRequestCanceled()
{
	const ushort usPlayerID = DoFindID();
	if (WorldData().GetTransferListManager().IsTransferListed(usPlayerID) == true)
	{
		CPerson::SetContractRenewalStatus(NONEINPROGRESS);
		CPerson::DoRaiseMorale(5);
		WorldData().GetTransferListManager().DoTransferRequestCanceled(usPlayerID);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::OnTransferRequestRefused
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	His current Club has refused a transfer request by him
------------------------------------------------------------------------------*/
void CPlayer::OnTransferRequestRefused()
{
	for (int LoopCount = 0; LoopCount < 3; LoopCount++)
	{
		GetSkills().DoReduceActualSkill(1);
	}
	CPerson::DoLowerMorale(15);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoRespondTransferRequestDenied
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CPlayer::DoRespondTransferRequestDenied()
{
	OnTransferRequestRefused();
	uint RefusalMessage[] =
	{
    IDS_REFUSETRANREQ1, IDS_REFUSETRANREQ2, IDS_REFUSETRANREQ3,
    IDS_REFUSETRANREQ4, IDS_REFUSETRANREQ5, IDS_REFUSETRANREQ6,
    IDS_REFUSETRANREQ7
	};
	CString str;
	str.Format(RefusalMessage[RandomNumber.IntLessThan(6)], GetName().CStr());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::RequiresSellOnClause
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayer::RequiresSellOnClause()
{
	bool bRet;
	ushort Required = RandomNumber.IntLessThan(10);
	ushort Value = 100;			// If over 26 then will always return false
	ushort Chance16[] = { 0, 0, 0, 5, 20, 45, 60, 70, 75, 80 };
	ushort Chance19[] = { 0, 0, 0, 0,  5, 25, 40, 50, 55, 60 };
	ushort Chance21[] = { 0, 0, 0, 0,  0,	 5,	20, 30, 35, 40 };
	ushort Chance23[] = { 0, 0, 0, 0,  0,  0,  5, 10, 15, 20 };
	ushort Chance25[] = { 0, 0, 0, 0,  0,	 0,	 0,  0,  5, 10 };

	switch(CPerson::GetAge())
	{
		case 16:
		case 17:
		case 18:
			Value = Chance16[Required];
			break;

		case 19:
		case 20:
			Value = Chance19[Required];
			break;

		case 21:
		case 22:
			Value = Chance21[Required];
			break;

		case 23:
		case 24:
			Value = Chance23[Required];
			break;

		case 25:
		case 26:
			Value = Chance25[Required];
			break;

	}
	bRet = (RandomNumber.IntLessThan(100) < Value);
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoGenerateSellOnClauseForPlayer
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::DoGenerateSellOnClauseForPlayer()
{
	byte bRet = 0;
	// Do we require one?
	//if (RequiresSellOnClause() == true)
	{
		switch(CPerson::GetAge())
		{
			case 16:
				bRet = 25;
				break;
			case 17:
				bRet = 25;
				break;
			case 18:
				bRet = 20;
				break;
			case 19:
				bRet = 20;
				break;
			case 20:
				bRet = 15;
				break;
			case 21:
				bRet = 15;
				break;
			case 22:
				bRet = 10;
				break;
			case 23:
				bRet = 10;
				break;
			case 24:
				bRet = 5;
				break;
			case 25:
				bRet = 5;
				break;
			case 26:
				bRet = 5;
				break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetDiscipline
	Access:    	public
	Returns:   	CPlayerDiscipline&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayerDiscipline& CPlayer::GetDiscipline()
{
	return m_Discipline;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoRetired
	Access:    	public
	Parameter: 	const ushort _ClubId
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::DoRetired(const ClubID _ClubID)
{
  //lLogFmt("%s(%d) retiring from club %d\n", GetName(), DoFindID(), _ClubID.id);
 	CPerson::SetContractRenewalStatus(RETIRED);

  if( WorldData().GetClubByID(_ClubID).IsUserControlled() == false)
  {
    WorldData().GetPlayerLoanManager().SetClubOnLoanFrom(DoFindID(), _ClubID);        // This is the club the regenerated player will join
  }

	if (BecomeManager() == true)
	{
		ClubID noClub;
		ushort ManagerID = WorldData().GetManagerList().DoAddNewManager(this, 0, 0, noClub);
		if (ManagerID != NOMANAGER)
		{
			WorldData().GetManager(ManagerID).GetRatingController().SetRating(WorldData().GetClubByID(_ClubID).GetPopularity().GetMinimumManagerRatingToManageClub() * .6);
		}

	}
}



/*------------------------------------------------------------------------------
	Method:   	CPlayer::DoRegenerate
	Access:    	public
	Parameter: 	const ushort _PlayerID
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier:
	Purpose:	Creates a youth player from a retired one and adds to his old
				club providing the squad isnt full
------------------------------------------------------------------------------*/
void CPlayer::DoRegenerate(const ushort _PlayerID, CCountry& _Country)
{
	WorldData().GetContractClauseList().RemoveAllForPlayer(DoFindID());
	CClub* pClub;
	// Players can start the game on loan from a non existant club
	if (GetClubIDOnLoanFrom().id != NOCLUB)
	{
		pClub = &WorldData().GetClubByID(GetClubIDOnLoanFrom());
	}
	else
	{
		pClub = _Country.GetActiveCountry().DoFindRandomClub();
	}
	while (pClub->IsUserControlled() == true || pClub->GetFirstTeamSquad().TotalSquadSize() >= PLAYERSINSQUAD || &_Country != &(pClub->GetCountry()))
	{
		pClub = _Country.GetActiveCountry().DoFindRandomClub();
	}
	EXT_ASSERT_POINTER(pClub, CClub);
	DoMakeYoung();
	DoInitialise();
	CPerson::DoCreate(18 + RandomNumber.IntLessThan(3), 2, pClub->GetCountry());
	CPerson::SetWage(GetSkills().DoGenerateRandomWage(pClub->GetCountry()));
	pClub->GetFirstTeamSquad().DoAddPlayerToSquad(_PlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetClubIDOnLoanFrom
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CPlayer::GetClubIDOnLoanFrom() const
{
	return WorldData().GetPlayerLoanManager().GetClubOnLoanFrom(DoFindID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetWeeksOnLoan
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayer::GetWeeksOnLoan()
{
	return WorldData().GetPlayerLoanManager().GetWeeksOnLoan(DoFindID());
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::SetWeeksOnLoan
	Access:    	public
	Parameter: 	const byte _WeeksOnLoan
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayer::SetWeeksOnLoan(const byte _WeeksOnLoan)
{
	WorldData().GetPlayerLoanManager().SetWeeksOnLoan(DoFindID(), _WeeksOnLoan);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::Write
	Access:    	public
	Parameter: 	CDataFile *_pDataFile
	Returns:   	void
	Qualifier:
	Purpose:	Serializes the CPlayer.
------------------------------------------------------------------------------*/
void CPlayer::Write(CDataFile* _pDataFile)
{
  lAssert( !"Use Write1/2/3/4/5/6" );
//  CPerson::Write(_pDataFile);
//  m_Skills.Write(_pDataFile);
//
//  _pDataFile->EnterBitMode(true);
//  m_Career.Write(_pDataFile);
//  m_CupCareer.Write(_pDataFile);
//  m_ClubCareer.Write(_pDataFile);
//  m_ClubSeasonCareer.Write(_pDataFile);
//  m_CupSeasonCareer.Write(_pDataFile);
//  m_LeagueSeasonCareer.Write(_pDataFile);
//  m_LastSeasonCareer.Write(_pDataFile);
//#ifdef INTERNATIONAL
//  m_InternationalCareer.Write(_pDataFile);
//#endif
//  _pDataFile->ExitBitMode();
//
//  _pDataFile->Write(&m_Discipline, sizeof(m_Discipline));
//  _pDataFile->Write(&m_bitfields, sizeof(m_bitfields));
}

void CPlayer::Write1(CDataFile* _pDataFile)
{
  CPerson::Write(_pDataFile);
}

void CPlayer::Write2(CDataFile* _pDataFile)
{
  m_Skills.Write(_pDataFile);

}

void CPlayer::Write3(CDataFile* _pDataFile)
{
	_pDataFile->EnterBitMode(true);
	m_Career.Write(_pDataFile);
	m_CupCareer.Write(_pDataFile);
	m_ClubCareer.Write(_pDataFile);
#ifdef INTERNATIONAL
	m_InternationalCareer.Write(_pDataFile);
#endif
	_pDataFile->ExitBitMode();
}

void CPlayer::Write4(CDataFile* _pDataFile)
{
	_pDataFile->EnterBitMode(true);
	m_ClubSeasonCareer.Write(_pDataFile);
  m_CupSeasonCareer.Write(_pDataFile);
  m_LeagueSeasonCareer.Write(_pDataFile);
  m_LastSeasonCareer.Write(_pDataFile);
	_pDataFile->ExitBitMode();
}

void CPlayer::Write5(CDataFile* _pDataFile)
{
  _pDataFile->Write(&m_Discipline, sizeof(m_Discipline));
}

void CPlayer::Write6(CDataFile* _pDataFile)
{
  _pDataFile->Write(&m_bitfields, sizeof(m_bitfields));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::Read
	Access:    	public
	Parameter: 	CDataFile *_pDataFile
	Returns:   	void
	Qualifier:
	Purpose:	Deserializes the CPlayer.
------------------------------------------------------------------------------*/
void CPlayer::Read(CDataFile* _pDataFile)
{
  lAssert( !"Use Read1/2/3/4/5/6" );
/*
	CPerson::Read(_pDataFile);

	m_Skills.Read(_pDataFile);

	_pDataFile->EnterBitMode(false);
	m_Career.Read(_pDataFile);
	m_CupCareer.Read(_pDataFile);
	m_ClubCareer.Read(_pDataFile);
	m_ClubSeasonCareer.Read(_pDataFile);
	m_CupSeasonCareer.Read(_pDataFile);
	m_LeagueSeasonCareer.Read(_pDataFile);
	m_LastSeasonCareer.Read(_pDataFile);
#ifdef INTERNATIONAL
	m_InternationalCareer.Read(_pDataFile);
#endif
	_pDataFile->ExitBitMode();

	_pDataFile->Read(&m_Discipline, sizeof(m_Discipline));
  _pDataFile->Read(&m_bitfields, sizeof(m_bitfields));
*/
}

void CPlayer::Read1(CDataFile* _pDataFile)
{
  CPerson::Read(_pDataFile);
}

void CPlayer::Read2(CDataFile* _pDataFile)
{
  m_Skills.Read(_pDataFile);

}

void CPlayer::Read3(CDataFile* _pDataFile)
{
	_pDataFile->EnterBitMode(true);
  m_Career.Read(_pDataFile);
  m_CupCareer.Read(_pDataFile);
  m_ClubCareer.Read(_pDataFile);
#ifdef INTERNATIONAL
  m_InternationalCareer.Read(_pDataFile);
#endif
	_pDataFile->ExitBitMode();
}

void CPlayer::Read4(CDataFile* _pDataFile)
{
	_pDataFile->EnterBitMode(true);
  m_ClubSeasonCareer.Read(_pDataFile);
  m_CupSeasonCareer.Read(_pDataFile);
  m_LeagueSeasonCareer.Read(_pDataFile);
  m_LastSeasonCareer.Read(_pDataFile);
	_pDataFile->ExitBitMode();
}

void CPlayer::Read5(CDataFile* _pDataFile)
{
  _pDataFile->Read(&m_Discipline, sizeof(m_Discipline));
}

void CPlayer::Read6(CDataFile* _pDataFile)
{
  _pDataFile->Read(&m_bitfields, sizeof(m_bitfields));
}


/*------------------------------------------------------------------------------
	Method:   	CPlayer::GetBosmanValuation
	Access:    	public
	Parameter: 	const ClubID & _ClubID
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CPlayer::GetBosmanValuation(const ClubID& _ClubID)
{
	int Valuation = 0;
	if (IsAvailableOnBosmanFreeTransfer() == false)
	{
		Valuation = GetSkills().DoCalculateMinimumTransferFee(GetAge(), WorldData().GetClubByID(_ClubID).GetCountry(), GetTemperament());
	}
	return Valuation;
}

#ifdef SURGICALINJURIES
//------------------------------------------------------------------------------------------------------
CString CPlayer::SurgeryResultStr()
{
	if (IsInjured() == true)
	{
		return GetInjury()->SurgeryResultStr(&GetClub());
	}
	return TEXT("");
}
#endif


// Has scored a goal
//------------------------------------------------------------------------------------------------------
#ifdef ASSISTS
void CPlayer::AssistedGoal(const eMatchCompetitionType MatchType)
{
	switch(MatchType)
	{
	case FRIENDLYMATCH:
		return;
		break;
	case LEAGUEMATCH:
		m_LeagueSeasonCareer.DoIncrementAssists();
		break;
	case CUPMATCH:
	case REPLAYMATCH:
	case FEDERATIONCUPMATCH:
	case CUPMATCHNEUTRALVENUE:
	case REPLAYMATCHNEUTRALVENUE:
	case FEDERATIONCUPMATCHNEUTRALVENUE:
		m_CupSeasonCareer.DoIncrementAssists();
		m_CupCareer.DoIncrementAssists();
		break;
	default :
		EXT_ASSERT(false);
	}
	m_Career.DoIncrementAssists();
	// Don't add to club totals if on loan
	if (IsOnLoan() == false)
	{
		m_ClubCareer.DoIncrementAssists();
		m_ClubSeasonCareer.DoIncrementAssists();
	}
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CPlayerList::~CPlayerList
	Access:    	public
	Returns:   		Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPlayerList::~CPlayerList()
{
	RemoveList();
}

void CPlayerList::RemoveList()
{
	delete[] m_pPlayers;
	m_pPlayers = NULL;
	m_uiSize = 0;
	m_uiMaxAllocated = 0;
}

void CPlayerList::SetSize(const unsigned int uiSize)
{
	EXT_ASSERT(m_pPlayers == NULL);

  m_pPlayers = FD_ENGINE_LARGE_NEW CPlayer[uiSize];
	m_uiMaxAllocated = 0;
	m_uiSize = uiSize;
}

unsigned int CPlayerList::GetSize() const
{
	return m_uiSize;
}

CPlayer *CPlayerList::GetNextFreePlayer()
{
  //lLogFmt("GetNextFree %d\n", m_uiMaxAllocated);
	EXT_ASSERT(m_uiMaxAllocated < m_uiSize);
	return &(m_pPlayers[m_uiMaxAllocated++]);
}

unsigned int CPlayerList::Find(const CPlayer* _Element) const
{
	if ((_Element < m_pPlayers) || (_Element >= (m_pPlayers + m_uiSize)))
		return NOPLAYER;

	EXT_ASSERT(((unsigned int)(_Element - m_pPlayers)) < ((unsigned int)(m_pPlayers + m_uiSize)));
	return (_Element - m_pPlayers);
}

CPlayer *CPlayerList::GetAt(const int iIndex)
{
	return &m_pPlayers[iIndex];
}

const CPlayer *CPlayerList::GetAt(const int iIndex) const
{
	return &m_pPlayers[iIndex];
}

void CPlayerList::Swap(const int iIndex1, const int iIndex2)
{
	CPlayer cTemp = m_pPlayers[iIndex1];
	m_pPlayers[iIndex1] = m_pPlayers[iIndex2];
	m_pPlayers[iIndex2] = cTemp;
}



/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoInitialise
	Access:    	public
	Parameter: 	ushort _MaxClubs
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerList::DoInitialise(ushort _MaxClubs)
{
	ushort nClubID;
	ushort ClubSquadNumber = wNOTFOUND, OldClubNum = wNOTFOUND;
	bool UsePlayerData;
	CClub* pClub;
	CPlayer DummyPlayer;

	CBinaryReader theResource(0, IDR_PLAYER, TEXT("FDDATA"));
	ushort HowMany = theResource.ReadUInt16();
  EXT_ASSERT( HowMany < MAXNUMPLAYERS );
  SetSize(MAXNUMPLAYERS);    // Add a few extra to save constantly allocating space used for non league clubs and youths

	int LoopCount;
	for (LoopCount = 0; LoopCount < HowMany; LoopCount++)
	{
    if( (LoopCount & 0xFF) == 0 )
    {
      // 256 processed
      ProcessingCallback( "PlayerInitLoop" );

    }


        nClubID = theResource.ReadUInt16();
		EXT_ASSERT(nClubID < _MaxClubs); // Invalid ClubID
		if (nClubID != OldClubNum)
		{
        	OldClubNum = nClubID;				// The player is the first in the squad of the next club
            EXT_ASSERT(ClubSquadNumber > 15);  		// Ensure last club had some players
            pClub = &WorldData().GetClub(OldClubNum);
			if (pClub->GetCountryRanking() < 2)
			{
				UsePlayerData = true;
			}
			else
			{
				UsePlayerData = false;
			}
            ClubSquadNumber = 0;                // Start of squad
     	}

		if (ClubSquadNumber < PLAYERSINSQUAD)
		{
			if (UsePlayerData == true)
			{
				CPlayer *pNewPlayer = GetNextFreePlayer();
				pClub->GetFirstTeamSquad().SetSquad(ClubSquadNumber++, LoopCount);
				pNewPlayer->DoInitialiseFromResource(theResource, pClub);
				//DEBUG_OUTPUT(("%d/%d, Added Player to (%d)%s, squad size : %d"), (LoopCount, HowMany, nClubID, pClub->GetName(), ClubSquadNumber));
			}
		}
    };
}


 /*------------------------------------------------------------------------------
	Method:   	CPlayerList::AddNew
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerList::AddNew()
{
	CPlayer *pNewPlayer = GetNextFreePlayer();
	const ushort usIndex = (pNewPlayer - m_pPlayers);
	return usIndex;
}


 //---------------------------------------------------------------------------------------
ushort CPlayerList::AddNew(CClub* _pClub, ePlayerPosition _Position)
{
	/*
	int ListPos;
    for (ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        if (GetAt(ListPos) == null)
		{
			EXT_ASSERT(false);
			break;
        }
    }
	EXT_ASSERT(false);
    SetAtGrow(ListPos, FD_ENGINE_NEW CPlayer(_pClub, _Position));
	return ListPos;
	*/
	EXT_ASSERT(false);
	return 0;
 }


 //---------------------------------------------------------------------------------------
ushort CPlayerList::TransferFromYouth(CYouth* pYouth, CContract* Contract)
{
	ushort ListPos = AddNew();
    operator[](ListPos).TransferFromYouth(pYouth, Contract);
	return ListPos;
 }


 //---------------------------------------------------------------------------------------
void CPlayerList::Write(CDataFile* _pDataFile)
{
	byte bInUse;
	ushort nHowMany = m_uiMaxAllocated;
//  ushort nWritten = 0;
	_pDataFile->Write(&nHowMany, sizeof(nHowMany));  // List size

  _pDataFile->Debug("PlayerListInit");

  // Write out skip flag + initial data
  for (uint ListPos = 0; ListPos < nHowMany; ListPos++)
  {
    CPlayer *pPlayer = GetAt(ListPos);
    if (pPlayer->IsDontSave() == true)
    {
      byte bSkipFlag = 1;
      _pDataFile->Write(&bSkipFlag, sizeof(bSkipFlag));
      continue;
    }
    else
    {
//      nWritten++;
      byte bSkipFlag = 0;
      _pDataFile->Write(&bSkipFlag, sizeof(bSkipFlag));
      pPlayer->Write1(_pDataFile);
    }
  }
  _pDataFile->Debug("PlayerList1");

  for (uint ListPos = 0; ListPos < nHowMany; ListPos++)
  {
    CPlayer *pPlayer = GetAt(ListPos);
    if (pPlayer->IsDontSave() == false)
    {
      // if we should write
      pPlayer->Write2(_pDataFile);
    }
  }
  _pDataFile->Debug("PlayerList2");

  for (uint ListPos = 0; ListPos < nHowMany; ListPos++)
  {
    CPlayer *pPlayer = GetAt(ListPos);
    if (pPlayer->IsDontSave() == false)
    {
      // if we should write
      pPlayer->Write3(_pDataFile);
    }
  }
  _pDataFile->Debug("PlayerList3");

  for (uint ListPos = 0; ListPos < nHowMany; ListPos++)
  {
    CPlayer *pPlayer = GetAt(ListPos);
    if (pPlayer->IsDontSave() == false)
    {
      // if we should write
      pPlayer->Write4(_pDataFile);
    }
  }
  _pDataFile->Debug("PlayerList4");

  for (uint ListPos = 0; ListPos < nHowMany; ListPos++)
  {
    CPlayer *pPlayer = GetAt(ListPos);
    if (pPlayer->IsDontSave() == false)
    {
      // if we should write
      pPlayer->Write5(_pDataFile);
    }
  }
  _pDataFile->Debug("PlayerList5");

  for (uint ListPos = 0; ListPos < nHowMany; ListPos++)
  {
    CPlayer *pPlayer = GetAt(ListPos);
    if (pPlayer->IsDontSave() == false)
    {
      // if we should write
      pPlayer->Write6(_pDataFile);
    }
  }
  _pDataFile->Debug("PlayerList6");

//  lLogFmt("%d / %d written\n", nWritten, nHowMany);
}


 //---------------------------------------------------------------------------------------
void CPlayerList::Read(CDataFile* _pDataFile)
{
	ushort n;

	ushort HowMany;
  _pDataFile->Read(&HowMany, sizeof(HowMany));

  // remove existing
  RemoveList();

  // set size
  const unsigned int uiNormalAmount = MAXNUMPLAYERS;
	if (HowMany > uiNormalAmount)
	{
		SetSize(HowMany + (24 * 2));
	}
	else
  {
		SetSize(uiNormalAmount);
  }

  // read in
	for (uint ListPos = 0; ListPos < HowMany; ListPos++)
	{
    if( (ListPos & 0xFF) == 0 )
       ProcessingCallback( "PlayerInitLoop" );
  // processing callback every 256 players

		byte bSkipFlag = 0;
		_pDataFile->Read(&bSkipFlag, sizeof(bSkipFlag));

    CPlayer *curPlayer = GetNextFreePlayer();

    // we make the assumption that the array position will match up for the further reads
    lAssert( curPlayer->DoFindID() == ListPos );

		if (bSkipFlag == 1)
    {
      curPlayer->SetDontSave();
			continue;
		}

    curPlayer->Read1(_pDataFile);
	}

  // read in
	for (uint ListPos = 0; ListPos < HowMany; ListPos++)
	{
    if( (ListPos & 0xFF) == 0 )
       ProcessingCallback( "PlayerInitLoop" );
  // processing callback every 256 players

    CPlayer *curPlayer = GetAt( ListPos );
    if( !curPlayer->IsDontSave() )
      curPlayer->Read2(_pDataFile);
	}

	for (uint ListPos = 0; ListPos < HowMany; ListPos++)
	{
    if( (ListPos & 0xFF) == 0 )
       ProcessingCallback( "PlayerInitLoop" );
  // processing callback every 256 players

    CPlayer *curPlayer = GetAt( ListPos );
    if( !curPlayer->IsDontSave() )
      curPlayer->Read3(_pDataFile);
	}

	for (uint ListPos = 0; ListPos < HowMany; ListPos++)
	{
    if( (ListPos & 0xFF) == 0 )
       ProcessingCallback( "PlayerInitLoop" );
  // processing callback every 256 players

    CPlayer *curPlayer = GetAt( ListPos );
    if( !curPlayer->IsDontSave() )
      curPlayer->Read4(_pDataFile);
	}

	for (uint ListPos = 0; ListPos < HowMany; ListPos++)
	{
    if( (ListPos & 0xFF) == 0 )
       ProcessingCallback( "PlayerInitLoop" );
  // processing callback every 256 players

    CPlayer *curPlayer = GetAt( ListPos );
    if( !curPlayer->IsDontSave() )
      curPlayer->Read5(_pDataFile);
	}

	for (uint ListPos = 0; ListPos < HowMany; ListPos++)
	{
    if( (ListPos & 0xFF) == 0 )
       ProcessingCallback( "PlayerInitLoop" );
  // processing callback every 256 players

    CPlayer *curPlayer = GetAt( ListPos );
    if( !curPlayer->IsDontSave() )
      curPlayer->Read6(_pDataFile);
	}
}


 //------------------------------------------------------------------------------------------------------
void CPlayerList::DoDelete(const ushort a_nPlayerID)
{
	EXT_ASSERT(a_nPlayerID != NOPLAYER && a_nPlayerID < GetSize());
    EXT_ASSERT_POINTER(GetAt(a_nPlayerID), CPlayer);
	EXT_ASSERT(false);

    //delete GetAt(a_nPlayerID);
    //SetAt(a_nPlayerID, null);

	WorldData().GetContractClauseList().RemoveAllForPlayer(a_nPlayerID);
#ifdef _DEBUGv3
	afxDump << "Retiring Player Immediately " << a_nPlayerID << "\n";
#endif
 }


 // Returns the index(position in list) of the pointer
 //------------------------------------------------------------------------------------------------------
ushort CPlayerList::DoFindID(CPlayer* _Player)
{
    EXT_ASSERT_POINTER(_Player, CPlayer);
  ushort iRet = NOPLAYER;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        if (GetAt(ListPos) == _Player)
		{
            iRet = ListPos;
            break;
        }
    }
    EXT_ASSERT(iRet != NOPLAYER);
    lLogFmt("Looked up player %x to index %d\n", _Player, iRet);
    return iRet;
}

/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoRegenerateRetiredPlayers
	Access:    	public
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerList::DoRegenerateRetiredPlayers(CCountry& _Country)
{
	CPlayer* pPlayer;
	for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		pPlayer = &operator [](ListPos);
		if (pPlayer != null && pPlayer->IsRetired() == true)
		{
			pPlayer->DoRegenerate(ListPos, _Country);
		}
	}
}


 //------------------------------------------------------------------------------------------------------
CPlayer& CPlayerList::operator [](const ushort nIndex)
{
	return *(GetAt(nIndex));
}

//////////////////////////////////////////////////////////////////////////
CPlayerIDList::CPlayerIDList()
{
}

/*------------------------------------------------------------------------------
	Method:   	CPlayerIDList::DoBuildFromSquad
	Access:    	public
	Parameter: 	CPlayerSquad & _PlayerIDList
	Parameter: 	CPlayerList & _OldList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerIDList::DoBuildFromSquad(CPlayerSquad& _PlayerIDList, CPlayerList& _OldList)
{
    for (int ListPos = 0; ListPos < PLAYERSINSQUAD; ListPos ++)
	{
		if (_PlayerIDList.GetSquad(ListPos) != NOPLAYER)
		{
			Add(_OldList.GetAt(_PlayerIDList.GetSquad(ListPos))->DoFindID());
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoRemoveNotPosition
	Access:    	public
	Parameter: 	ePlayerPosition _Posn
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerIDList::DoRemoveNotPosition(ePlayerPosition _Posn)
{
	for (int LoopCount = GOALKEEPER; LoopCount < MAXPLAYERPOSITIONS; LoopCount++)
	{
		// Remove all players not in this
		if (LoopCount != _Posn)
		{
			DoRemoveAllPosition((ePlayerPosition)LoopCount);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoBuildFromTeamPlayingPosn
	Access:    	public
	Parameter: 	CPlayerSquad & aPlayerIDList
	Parameter: 	CPlayerList & OldList
	Parameter: 	ePlayerPosition Posn
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerIDList::DoBuildFromTeamPlayingPosn(CPlayerSquad& aPlayerIDList, CPlayerList& OldList, ePlayerPosition Posn)
{
    for (int ListPos = 0; ListPos < PLAYERSINTEAM; ListPos ++)
	{
		if (aPlayerIDList.GetSquad(ListPos) != NOPLAYER)
		{
            if (OldList[aPlayerIDList.GetSquad(ListPos)].GetSkills().GetPosition() == Posn && OldList[aPlayerIDList.GetSquad(ListPos)].IsPlaying() == true)
			{
				Add(OldList[aPlayerIDList.GetSquad(ListPos)].DoFindID());
            }
		}
	}
}

/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoBuildFromTeamPlayingOutField
	Access:    	public
	Parameter: 	CPlayerSquad & _PlayerIDList
	Parameter: 	CPlayerList & _OldList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerIDList::DoBuildFromTeamPlayingOutField(CPlayerSquad& _PlayerIDList, CPlayerList& _OldList)
{
    for (int ListPos = 0; ListPos < PLAYERSINTEAM; ListPos ++)
	{
		if (_PlayerIDList.GetSquad(ListPos) != NOPLAYER)
		{
            if (_OldList[_PlayerIDList.GetSquad(ListPos)].IsPlaying() == true && _OldList[_PlayerIDList.GetSquad(ListPos)].GetSkills().GetPosition() != GOALKEEPER)
			{
				Add(_OldList[_PlayerIDList.GetSquad(ListPos)].DoFindID());
            }
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoFindWorstPlayer
	Access:    	public
	Parameter: 	const int _WorstValue
	Returns:   	CPlayer*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerIDList::DoFindWorstPlayer(const int _WorstValue)
{
	DoSortByEffectiveSkill();
	ushort PlayerRet = NOPLAYER;
	// Check for players carrying knocks
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		CPlayer *pPlayer = WorldData().GetPlayer(operator[](LoopCount));
		if (pPlayer->InjuryMatchPerformanceEffect() > 10)
		{
			PlayerRet = operator [](LoopCount);
			break;
		}

	}
	if (PlayerRet == NOPLAYER && _WorstValue < GetSize())
	{
		// None so just find worst player
		PlayerRet = operator [](_WorstValue);
	}
	EXT_ASSERT(PlayerRet != NOPLAYER);
	return PlayerRet;
}

/*------------------------------------------------------------------------------
	Method:   	CPlayerList::AverageSkill
	Access:    	public
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerIDList::AverageSkill()
{
	if (GetSize() > 0)
	{
		uint TotalSkill = 0;
		for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
		{
			CPlayer *pPlayer = WorldData().GetPlayer(operator[](ListPos));
			TotalSkill += pPlayer->GetSkills().GetOverallSkill();
		}
		return static_cast<byte>(TotalSkill / GetSize());
	}
	else
	{
		return 0;
	}
}

 //---------------------------------------------------------------------------------------
CString CPlayerIDList::RandomSurName()
{
    CString sRet;
    CPlayer* pPlayer = null;
    if (GetSize() > 1)
	{
        pPlayer = WorldData().GetPlayer(operator [](RandomNumber.IntLessThan(GetSize())));
    }
    if (GetSize() == 1)
	{
        pPlayer = WorldData().GetPlayer(operator [](0));
    }
    if (pPlayer != null)
	{
        sRet = pPlayer->GetSurname();
    }
    return sRet;
}

/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoRemoveAllPosition
	Access:    	public
	Parameter: 	ePlayerPosition Posn
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerIDList::DoRemoveAllPosition(ePlayerPosition Posn)
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        CPlayer *pPlayer = WorldData().GetPlayer(operator [](ListPos));
        if (pPlayer != null)
		{
            if (pPlayer->GetSkills().GetPosition() == Posn)
			{
                RemoveAt(ListPos);
            }
        }
    }
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoSortByOverallSkill
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerIDList::DoSortByOverallSkill()
{
    bool t_Swap = true;
    int ListPos;
    while(t_Swap == true)
	{
        t_Swap = false;
        for (ListPos = 0; ListPos < GetSize() - 1; ListPos++)
		{
			CPlayer *pPlayer = WorldData().GetPlayer(operator[](ListPos));
			CPlayer *pPlayer2 = WorldData().GetPlayer(operator[](ListPos + 1));
            if (pPlayer->GetSkills().GetOverallSkill() < pPlayer2->GetSkills().GetOverallSkill())
			{
                DoSwap(ListPos, ListPos + 1);
                t_Swap = true;
            }
        }
    }
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerList::DoSortByEffectiveSkill
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Sort by effective skill, poorest player first
------------------------------------------------------------------------------*/
void CPlayerIDList::DoSortByEffectiveSkill()
{
    bool t_Swap = true;
    while(t_Swap == true)
	{
        t_Swap = false;
        for (int n = 0; n < GetSize() - 1; n++)
		{
			CPlayer *pPlayer = WorldData().GetPlayer(operator[](n));
			CPlayer *pPlayer2 = WorldData().GetPlayer(operator[](n + 1));
            if (pPlayer->DoCalculateEffectiveSkill(SKILL_OVERALL) > pPlayer2->DoCalculateEffectiveSkill(SKILL_OVERALL))
			{
                DoSwap(n, n + 1);
                t_Swap = true;
            }
        }
    }
}

