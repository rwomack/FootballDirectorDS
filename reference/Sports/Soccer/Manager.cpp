
/*

	File:- Manager.cpp

	Manager class

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CManager::CManager
	Access:    	public 
	Parameter: 	CPlayer* _pPlayer
	Parameter: 	const uint _Wage
	Parameter: 	const byte _Contract
	Parameter: 	const ushort _ClubID
	Returns:   		
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManager::CManager(CPlayer* _pPlayer, const uint _Wage, const byte _Contract, const ushort _ClubID)
{
	DoMakeActive(_pPlayer, _Wage, _Contract, _ClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::CManager
	Access:    	public 
	Parameter: 	CEmployee* _pEmployee
	Parameter: 	const uint _Wage
	Parameter: 	const byte _Contract
	Parameter: 	const ushort _ClubID
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManager::CManager(CEmployee* _pEmployee, const uint _Wage, const byte _Contract, const ushort _ClubID)
{
	DoResetVariables();
    this->DoCopy(_pEmployee);
	OnInitialise(_Wage, _Contract, _ClubID);
	CPerson::SetNationalityID(_pEmployee->GetNationalityID());
	EXT_ASSERT(CPerson::GetAge()  > 25 && CPerson::GetAge() < 70);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::CManager
	Access:    	public 
	Parameter: 	const uint _Wage
	Parameter: 	const byte _Contract
	Parameter: 	const ushort _ClubID
	Returns:   		Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManager::CManager(const uint _Wage, const byte _Contract, const ushort _ClubID)
{
	DoResetVariables();
	OnInitialise(_Wage, _Contract, _ClubID);
	CPerson::DoGenerateDateOfBirth(30);
	EXT_ASSERT(CPerson::GetAge()  > 25 && CPerson::GetAge() < 70);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::CManager
	Access:    	public 
	Returns:   		
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManager::CManager()
{
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoEndOfSeason
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:	adjust contract, return whether manager is to retire etc
------------------------------------------------------------------------------*/
bool CManager::DoEndOfSeason()
{
    DoClearMOM();
    // Decide whether to retire or not
	if (CPerson::GetAge() > 66 || CPerson::GetAge() > 58 && RandomNumber.IntLessThan(6) == 1)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	DoResetVariables();
	CPerson::DoInitialiseFromResource(_Resource, true);
// 	DEBUG_OUTPUT(("%s, Age : %d"), (GetName(), GetAge()));
	EXT_ASSERT(CPerson::GetAge()  > 25 && CPerson::GetAge() < 70);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetClubName
	Access:    	public 
	Parameter: 	void
	Returns:   	CString&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString& CManager::GetClubName()
{
	return GetCurrentClub()->GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetClubUpperCaseName
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CManager::GetClubUpperCaseName()
{
	return GetCurrentClub()->GetUpperCaseName();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoCalculateRating
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CManager::DoCalculateRating()
{
	return m_SeasonHistory.DoCalculateRating();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetSeasonPoints
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManager::GetSeasonPoints() const
{
	return m_Data.m_SeasonPoints;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetTotalPoints
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManager::GetTotalPoints() const
{
	return m_Data.m_TotalPoints;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::IsUser
	Access:    	public const 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::IsUser() const
{
	return m_Data.m_IsUser;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::SetIsUser
	Access:    	public 
	Parameter: 	bool _IsUser
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetIsUser(bool _IsUser)
{
	m_Data.m_IsUser = _IsUser;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::SetPointsSinceMOM
	Access:    	public 
	Parameter: 	const byte _Points
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetPointsSinceMOM(const byte _Points)
{
	m_Data.m_PointsSinceMOM = _Points;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::SetTotalPoints
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetTotalPoints(const byte _Value)
{
	 m_Data.m_TotalPoints = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::SetSeasonPoints
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetSeasonPoints(const byte _Value)
{
	m_Data.m_SeasonPoints = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::SetSeasonTransferIncome
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetSeasonTransferIncome(const byte _Value)
{
	m_Data.m_SeasonTransferIncome = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::SetSeasonTransferExpenditure
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetSeasonTransferExpenditure(const byte _Value)
{
	m_Data.m_SeasonTransferExpenditure = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetRating
	Access:    	public 
	Parameter: 	void
	Returns:   	CRating&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRating& CManager::GetRating()
{
	return m_ratingManager;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoResetVariables
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoResetVariables()
{
	CPerson::DoResetVariables();
	CPerson::SetWeeksHere(0);
	DoClearMOM();
#ifdef DETAILEDMANAGERHISTORY
	m_ClubHistory.DoResetVariables();
	m_SeasonHistory.DoResetVariables();
	m_CareerHistory.DoResetVariables();
#endif
	DoClearTable();
    SetTotalPoints(0);

	for (uint LoopCount = 0; LoopCount < MAXMANAGERHISTORY; LoopCount++)
	{
		m_ManHistory[LoopCount].DoResetVariables();
	}
	m_ratingManager.DoResetVariables();
#ifdef MANAGEREXPENDITURE
	m_BankBalance = 0;
	m_WeekExpenditure = 0;
#endif
	m_BoardConfidence.DoResetVariables();
	m_SupportersConfidence.DoResetVariables();
#ifdef MANAGERSTRESS
	m_StressLevel = 0;
#endif
	SetIsUser(false);
 }


/*------------------------------------------------------------------------------
	Method:   	CManager::OnInitialise
	Access:    	public 
	Parameter: 	const uint _Wage
	Parameter: 	const byte _Contract
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::OnInitialise(const uint _Wage, const byte _Contract, const ushort _ClubID)
{
    CPerson::SetWage(_Wage);
	CPerson::GetContractEndDate().IncreaseLength(_Contract);
#ifdef MANAGEREXPENDITURE
	m_WeekExpenditure = CPerson::GetWage() * .8;
#endif
	// If manager of a club
	if (_ClubID != NOCLUB)
	{
		DoNewClub(WorldData().GetClub(_ClubID));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::CheckIfGotContract
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::CheckIfGotContract()
{
	return (CPerson::GetContractEndDate().SeasonsRemaining() > 0);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoRaiseRating
	Access:    	public 
	Parameter: 	const double howmany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoRaiseRating(const double howmany)
{
	EXT_ASSERT(IsManagingClub() == true);
	static const byte Adjuster[] =
	{
		0, 0, 0, 1, 1,
		1, 1, 1, 2, 2
	};
#ifdef MANAGERSTRESS
	static const byte StressAdjuster[] =
	{
		0, 0, 0, 1, 1,
		1, 1, 2, 2, 3
	};
#endif
	m_ratingManager.DoRaiseRating(howmany);
	ushort nHowMany = static_cast<ushort>(m_ratingManager.GetRatingPositiveMovement());
	if (nHowMany > 1)
	{
		m_ratingManager.SetRatingPositiveMovement(0);
		for (uint LoopCount = 0; LoopCount < nHowMany; LoopCount++)
		{
 			m_BoardConfidence.DoIncrease(Adjuster[RandomNumber.IntLessThan(9)]);
			m_SupportersConfidence.DoIncrease(Adjuster[RandomNumber.IntLessThan(9)]);
#ifdef MANAGERSTRESS
			m_StressLevel = max(MINIMUMCONFIDENCE, m_StressLevel - StressAdjuster[RandomNumber.IntLessThan(9)]);
#endif
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoLowerRating
	Access:    	public 
	Parameter: 	double howmany
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::DoLowerRating(double howmany)
{
	static byte Adjuster[] =
	{
		0, 0, 1, 1, 1,
		1, 1, 2, 2, 2
	};
#ifdef MANAGERSTRESS
	static const byte StressAdjuster[] =
	{
		0, 0, 0, 1, 1,
		1, 1, 2, 2, 3
	};
#endif
	EXT_ASSERT(IsManagingClub() == true);
	m_ratingManager.DoLowerRating(howmany);
	bool bRet = false;
	int Rnd;
	sbyte SupportersConfidence;
	ushort nHowMany = static_cast<ushort>(m_ratingManager.GetRatingNegativeMovement());
	if (nHowMany > 1)
	{
		m_ratingManager.SetRatingNegativeMovement(0);
		for (uint LoopCount = 0; LoopCount < nHowMany; LoopCount++)
		{
			m_SupportersConfidence.DoDecrease(Adjuster[RandomNumber.IntLessThan(9)]);
 			m_BoardConfidence.DoDecrease(Adjuster[RandomNumber.IntLessThan(9)]);

			// Lower board confidence if higher than supporter's
			Rnd = RandomNumber.IntLessThan(100);
			SupportersConfidence = m_SupportersConfidence.GetValue();
			if (SupportersConfidence < GetBoardConfidence() && \
				(SupportersConfidence < 30 && Rnd < 75) || \
				((SupportersConfidence < 60 && SupportersConfidence > 39) && Rnd < 25) || \
				((SupportersConfidence < 40 && SupportersConfidence > 30) && Rnd < 50))
			{
				m_BoardConfidence.DoDecrease(1);
			}
#ifdef MANAGERSTRESS
			m_StressLevel = min(max(GetStressLevel() + StressAdjuster[RandomNumber.IntLessThan(9)], MINIMUMCONFIDENCE), MAXIMUMCONFIDENCE);
#endif
			if (TrySackManager() == true)
			{
				bRet = true;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetBoardConfidence
	Access:    	public 
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CManager::GetBoardConfidence()
{
	if (IsManagingClub() == true)
	{
		return m_BoardConfidence.GetValue();
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetSupportersConfidence
	Access:    	public 
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CManager::GetSupportersConfidence()
{
	if (IsManagingClub() == true)
	{
		return m_SupportersConfidence.GetValue();
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetGamesPlayedClub
	Access:    	public 
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CManager::GetGamesPlayedClub() const
{
	return m_ClubHistory.GetGamesPlayed();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetSeasonTransferIncome
	Access:    	public 
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CManager::GetSeasonTransferIncome() const
{
	return m_Data.m_SeasonTransferIncome;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetSeasonTransferExpenditure
	Access:    	public 
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CManager::GetSeasonTransferExpenditure()	const
{
	return m_Data.m_SeasonTransferExpenditure;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetLastClubID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManager::GetLastClubID() const
{
	return m_ManHistory[1].GetClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetPointsSinceMOM
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CManager::GetPointsSinceMOM() const
{
	return m_Data.m_PointsSinceMOM;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoLeaveClub
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoLeaveClub()
{
	for (int ListPos = 1; ListPos < MAXMANAGERHISTORY; ListPos ++)
	{
		m_ManHistory[ListPos] = m_ManHistory[ListPos - 1];
	}
	m_ManHistory[0].SetClubID(NOMANAGER);
    m_ManHistory[1].SetClubDateLeft(WorldData().GetCurrentDate());
	CPerson::GetContractEndDate().IncreaseLength(0);
#ifdef DETAILEDMANAGERHISTORY
    m_ClubHistory.DoResetVariables();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CManager::IsMatchToday
	Access:    	public 
	Parameter: 	const CCalendar & _CurrentDate
	Parameter: 	CMatchInfo & _Match
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::IsMatchToday(const CCalendar& _CurrentDate, CMatchInfo& _Match)
{
	if (IsManagingClub() == false)
	{
		return false;
	}
	return GetCurrentClub()->IsMatchToday(_CurrentDate, _Match);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::WageStr
	Access:    	public 
	Parameter: 	CCountry & rCountry
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CManager::WageStr(CCountry& rCountry)
{
	if (IsManagingClub() == true)
	{
		return CPerson::WageStr(rCountry);
	}
	else
	{
		return _T("-");
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CManager::SalaryStr
	Access:    	public 
	Parameter: 	CCountry & rCountry
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CManager::SalaryStr(CCountry& rCountry)
{
	if (IsManagingClub() == true)
	{
	    return CPerson::SalaryStr(rCountry);
	}
	else
	{
		return _T("-");
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CManager::DoPlayedLeagueMatch
	Access:    	public 
	Parameter: 	CFixture & _Fixture
	Returns:   	bool - true if manager should be sacked
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::DoPlayedLeagueMatch(CFixture& _Fixture)
{
   byte GoalsFor, GoalsAgainst;
	bool bRet = false;
    if (_Fixture.GetHomeClubID() == GetCurrentClubID())
	{
        GoalsFor = _Fixture.GetNumberGoals(HOME);
        GoalsAgainst = _Fixture.GetNumberGoals(AWAY);
    }
    else
	{
        GoalsAgainst = _Fixture.GetNumberGoals(HOME);
        GoalsFor = _Fixture.GetNumberGoals(AWAY);
    }

    m_GoalsSinceMOM.DoInitialise(GoalsFor, GoalsAgainst);
#ifdef DETAILEDMANAGERHISTORY
	m_ClubHistory.PlayedMatch(GoalsFor, GoalsAgainst);
	m_SeasonHistory.PlayedMatch(GoalsFor, GoalsAgainst);
	m_CareerHistory.PlayedMatch(GoalsFor, GoalsAgainst);
#endif
	short RatingAdjustment;
	if (GetCurrentClub() == &_Fixture.GetClub(HOME))
	{
		RatingAdjustment = static_cast<short>(_Fixture.GetClub(HOME).GetPopularityID() - _Fixture.GetClub(AWAY).GetPopularityID());
	}
	else
	{
		RatingAdjustment = static_cast<short>(_Fixture.GetClub(AWAY).GetPopularityID() - _Fixture.GetClub(HOME).GetPopularityID());
	}

    // Won  match
    if (GoalsFor > GoalsAgainst)
	{
        AddPoints(GetCurrentClub()->GetDivision().GetDivisionStrength().GetManagerPointsWin());
		DoRatingAdjustmentWonMatch(RatingAdjustment);
    }
    // Drawn  match
    if (GoalsFor == GoalsAgainst)
	{
        AddPoints(GetCurrentClub()->GetDivision().GetDivisionStrength().GetManagerPointsDraw());
		bRet = DoRatingAdjustmentDrawnMatch(RatingAdjustment);
    }
    // Lost  match
    if (GoalsFor < GoalsAgainst)
	{
		bRet = DoRatingAdjustmentLostMatch(RatingAdjustment);
    }
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoRatingAdjustmentDrawnMatch
	Access:    	public 
	Parameter: 	short _PopularityDifference
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::DoRatingAdjustmentDrawnMatch(short _PopularityDifference)
{
	_PopularityDifference /= 15;
	return DoAdjustRating(_PopularityDifference);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoAdjustRating
	Access:    	public 
	Parameter: 	const short _PopularityDifference
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::DoAdjustRating(const short _PopularityDifference)
{
	bool bRet = false;
	if (_PopularityDifference > 0)
	{
		DoRaiseRating(_PopularityDifference);
	}
	else if (_PopularityDifference < 0)
	{
		if (DoLowerRating(abs(_PopularityDifference)) == true)
		{
			bRet = true;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoRatingAdjustmentLostMatch
	Access:    	public 
	Parameter: 	short _PopularityDifference
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::DoRatingAdjustmentLostMatch(short _PopularityDifference)
{
	return DoLowerRating(max(_PopularityDifference / 5, .3));
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoRatingAdjustmentWonMatch
	Access:    	public 
	Parameter: 	short _PopularityDifference
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoRatingAdjustmentWonMatch(short _PopularityDifference)
{
	_PopularityDifference = max(_PopularityDifference / 5, .3);
#ifdef DEBUG
//	afxDump << "Raise rating " << aPopularityDifference << "\n";
#endif
	DoRaiseRating(_PopularityDifference);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::AddPoints
	Access:    	public 
	Parameter: 	const ushort _Points
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::AddPoints(const ushort _Points)
{
	EXT_ASSERT(IsManagingClub() == true);
    SetPointsSinceMOM(GetPointsSinceMOM() + _Points);
	SetSeasonPoints(GetSeasonPoints() + _Points);
    SetTotalPoints(GetTotalPoints() + _Points);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::PlayedCupMatch
	Access:    	public 
	Parameter: 	CFixture & _Fixture
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::PlayedCupMatch(CFixture& _Fixture)
{
	EXT_ASSERT(IsManagingClub() == true);
    byte GoalsFor, GoalsAgainst;
	bool bRet = false;
	short RatingAdjustment;
	if (GetCurrentClub() == &_Fixture.GetClub(HOME))
	{
		RatingAdjustment = static_cast<short>(_Fixture.GetClub(HOME).GetPopularityID() - _Fixture.GetClub(AWAY).GetPopularityID());
        GoalsFor = _Fixture.GetNumberGoals(HOME);
        GoalsAgainst = _Fixture.GetNumberGoals(AWAY);
    }
    else
	{
		RatingAdjustment = static_cast<short>(_Fixture.GetClub(AWAY).GetPopularityID() - _Fixture.GetClub(HOME).GetPopularityID());
        GoalsAgainst = _Fixture.GetNumberGoals(HOME);
        GoalsFor = _Fixture.GetNumberGoals(AWAY);
    }

    m_GoalsSinceMOM.DoInitialise(GoalsFor, GoalsAgainst);
#ifdef DETAILEDMANAGERHISTORY
    m_ClubHistory.PlayedMatch(GoalsFor, GoalsAgainst);
	m_SeasonHistory.PlayedMatch(GoalsFor, GoalsAgainst);
	m_CareerHistory.PlayedMatch(GoalsFor, GoalsAgainst);
#endif
    // Won  match
    if ( GoalsFor > GoalsAgainst)
	{
		DoRatingAdjustmentWonMatch(RatingAdjustment);
    }
    // Drawn  match
    if ( GoalsFor == GoalsAgainst)
	{
		bRet = DoRatingAdjustmentDrawnMatch(RatingAdjustment);
    }
    // Lost  match
    if ( GoalsFor < GoalsAgainst)
	{
		bRet = DoRatingAdjustmentLostMatch(RatingAdjustment);
    }
	return bRet;
 }


/*------------------------------------------------------------------------------
	Method:   	CManager::DoClearMOM
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoClearMOM()
{
	SetPointsSinceMOM(0);
	m_GoalsSinceMOM.DoInitialise();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoNewClub
	Access:    	public 
	Parameter: 	CClub & _NewClub
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoNewClub(CClub& _NewClub)
{
 	m_ManHistory[0].DoJoinClub(_NewClub.DoFindID());
	CPerson::SetWeeksHere(0);
	CPerson::DoGiveNewContract(_NewClub.GetManagerContract().GetContractOffer(), _NewClub.GetManagerContract().GetWageOffer() / 52, false);
#ifdef DETAILEDMANAGERHISTORY
    m_ClubHistory.DoResetVariables();
#endif
    DoClearMOM();
#ifdef MANAGERSTRESS
	m_StressLevel = _NewClub.GetDivision().GetDivisionStrength().GetManagerInitialStress();
#endif
	m_BoardConfidence.DoRegenerate();
	m_SupportersConfidence.DoRegenerate();
	if (m_ratingManager.GetRating() == 0)
	{
		m_ratingManager.SetRating(_NewClub.GetPopularity().GetMinimumManagerRatingToManageClub());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::RenewClubContract
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::RenewClubContract()
{
	EXT_ASSERT(IsManagingClub() == true);
	SetWage(GetCurrentClub()->GetManagerContract().GetWageOffer() / 52);
	CPerson::GetContractEndDate().IncreaseLength(GetCurrentClub()->GetManagerContract().GetContractOffer());
 }


/*------------------------------------------------------------------------------
	Method:   	CManager::SetWage
	Access:    	public 
	Parameter: 	const uint x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetWage(const uint x)
{
#ifdef MANAGEREXPENDITURE
	EXT_ASSERT(IsManagingClub() == true);
	byte OldLifeStyle = GetLifeStyle();
	m_WeekExpenditure = x * .8;
#ifdef MANAGERSTRESS
	if (OldLifeStyle > GetLifeStyle())
	{
		m_StressLevel += (3 + RandomNumber.IntLessThan(2));
	}
	if (OldLifeStyle < GetLifeStyle())
	{
		m_StressLevel -= (3 + RandomNumber.IntLessThan(2));
	}
#endif
#endif
	CPerson::SetWage(x);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoNewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoNewSeason()
{
	DoClearTable();
    DoClearMOM();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoClearTable
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoClearTable()
{
    m_SeasonHistory.DoResetVariables();
	SetSeasonPoints(0);
	SetSeasonTransferIncome(0);
	SetSeasonTransferExpenditure(0);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::WonMOM
	Access:    	public 
	Parameter: 	CString _Season
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::WonMOM(const CString &_Season)
{
	EXT_ASSERT(IsManagingClub() == true);
#ifdef DETAILEDMANAGERHISTORY
    m_ClubHistory.GetTrophyHistory().Add(new CTrophyHistory(GetCurrentClub()->GetDivision().GetName(), GetCurrentClub()->GetName(), _Season, 0, MANAGEROFMONTH));
    m_SeasonHistory.GetTrophyHistory().Add(new CTrophyHistory(GetCurrentClub()->GetDivision().GetName(), GetCurrentClub()->GetName(), _Season, 0, MANAGEROFMONTH));
    m_CareerHistory.GetTrophyHistory().Add(new CTrophyHistory(GetCurrentClub()->GetDivision().GetName(), GetCurrentClub()->GetName(), _Season, 0, MANAGEROFMONTH));
#endif
	DoRaiseRating(1);
#ifdef MANAGEREXPENDITURE
	m_BankBalance += 2000;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CManager::WonTitle
	Access:    	public 
	Parameter: 	bool _IsTopDivision
	Parameter: 	CString _SeasonStr
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::WonTitle(bool _IsTopDivision, const CString &_SeasonStr)
{
	EXT_ASSERT(IsManagingClub() == true);
	DoFinishedTopOfDivision(_IsTopDivision);
#ifdef DETAILEDMANAGERHISTORY
	m_ClubHistory.GetTrophyHistory().Add(new CTrophyHistory(GetCurrentClub()->GetDivision().GetName(), GetCurrentClub()->GetName(), _SeasonStr, 0, DIVISIONWINNER));
    m_CareerHistory.GetTrophyHistory().Add(new CTrophyHistory(GetCurrentClub()->GetDivision().GetName(), GetCurrentClub()->GetName(), _SeasonStr, 0, DIVISIONWINNER));
#endif
#ifdef MANAGEREXPENDITURE
	m_BankBalance += (CPerson::GetWage() * 52) *.1;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CManager::WonCup
	Access:    	public 
	Parameter: 	CCup * _Cup
	Parameter: 	CString _SeasonStr
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::WonCup(CCup* _Cup, const CString &_SeasonStr)
{
	EXT_ASSERT(IsManagingClub() == true);
#ifdef DETAILEDMANAGERHISTORY
	m_ClubHistory.GetTrophyHistory().Add(new CTrophyHistory(_Cup->GetName(), GetCurrentClub()->GetName(), _SeasonStr, 0, CUPWINNER));
    m_CareerHistory.GetTrophyHistory().Add(new CTrophyHistory(_Cup->GetName(), GetCurrentClub()->GetName(), _SeasonStr, 0, CUPWINNER));
#endif
	DoRaiseRating(_Cup->GetWinnersManagerRatingPoints());
	m_BoardConfidence.DoIncrease(2 + RandomNumber.IntLessThan(2));
	m_SupportersConfidence.DoIncrease(2 + RandomNumber.IntLessThan(2));
#ifdef MANAGEREXPENDITURE
	m_BankBalance += (CPerson::GetWage() * 52) *.08;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CManager::RunnerUpInCup
	Access:    	public 
	Parameter: 	CCup * pCup
	Parameter: 	CString Season
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::RunnerUpInCup(CCup* _Cup, const CString &_SeasonStr)
{
	EXT_ASSERT(IsManagingClub() == true);
#ifdef DETAILEDMANAGERHISTORY
    m_ClubHistory.GetTrophyHistory().Add(new CTrophyHistory(_Cup->GetName(), GetCurrentClub()->GetName(), _SeasonStr, 0, CUPRUNNERUP));
    m_CareerHistory.GetTrophyHistory().Add(new CTrophyHistory(_Cup->GetName(), GetCurrentClub()->GetName(), _SeasonStr, 0, CUPRUNNERUP));
#endif
	DoRaiseRating(_Cup->GetRunnersUpManagerRatingPoints());
#ifdef MANAGEREXPENDITURE
	m_BankBalance += (CPerson::GetWage() * 52) *.04;
#endif
 }


/*------------------------------------------------------------------------------
	Method:   	CManager::BeenRelegated
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::BeenRelegated()
{
	EXT_ASSERT(IsManagingClub() == true);
#ifdef DETAILEDMANAGERHISTORY
    m_ClubHistory.IncRelegation();
    m_CareerHistory.IncRelegation();
#endif
	DoLowerRating(3);
	m_BoardConfidence.DoDecrease(3 + RandomNumber.IntLessThan(2));
	m_SupportersConfidence.DoDecrease(3 + RandomNumber.IntLessThan(2));
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoFinishedBottomOfDivision
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoFinishedBottomOfDivision()
{
	EXT_ASSERT(IsManagingClub() == true);
	DoLowerRating(1);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoFinishedTopOfDivision
	Access:    	public 
	Parameter: 	bool _IsTopDivision
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoFinishedTopOfDivision(bool _IsTopDivision)
{
	EXT_ASSERT(IsManagingClub() == true);
	if (_IsTopDivision == true)
	{
		DoRaiseRating(5);
	}
	else
	{
		DoRaiseRating(3);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoQualifiedTopPlayOff
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoQualifiedTopPlayOff()
{
	EXT_ASSERT(IsManagingClub() == true);
	DoRaiseRating(2);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoQualifiedBottomPlayOff
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::DoQualifiedBottomPlayOff()
{
	EXT_ASSERT(IsManagingClub() == true);
	return DoLowerRating(1);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::BeenPromoted
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::BeenPromoted()
{
	EXT_ASSERT(IsManagingClub() == true);
#ifdef DETAILEDMANAGERHISTORY
    m_ClubHistory.IncPromotion();
	m_CareerHistory.IncPromotion();
#endif
	DoRaiseRating(2);
	m_BoardConfidence.DoIncrease(3 + RandomNumber.IntLessThan(2));
	m_SupportersConfidence.DoIncrease(3 + RandomNumber.IntLessThan(2));
}


/*------------------------------------------------------------------------------
	Method:   	CManager::IsManagingClub
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::IsManagingClub()
{
	return m_ManHistory[0].GetClubID() != NOMANAGER;
}


/*------------------------------------------------------------------------------
	Method:   	CUser::IsManagingAClubInCountry
	Access:    	public 
	Parameter: 	CActiveCountry & _Country
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::IsManagingAClubInCountry(CActiveCountry& _Country)
{
	if (m_ManHistory[0].GetClubID() == NOCLUB)
	{
		return false;
	}
	return &m_ManHistory[0].GetClub().GetActiveCountry() == &_Country;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::TrySackManager
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::TrySackManager()
{
	EXT_ASSERT(IsManagingClub() == true);
	int SackChance = RandomNumber.IntLessThan(100);
	bool bRet = false;
	if (GetBoardConfidence() < 10 ||
		(GetBoardConfidence() <  20 && GetBoardConfidence() > 9 && SackChance < 90) ||
		(GetBoardConfidence() <  30 && GetBoardConfidence() > 19 && SackChance < 30) ||
		(GetBoardConfidence() <  40 && GetBoardConfidence() > 29 && SackChance < 5) ||
		(GetBoardConfidence() <  50 && GetBoardConfidence() > 39 && SackChance < 0) ||
		(GetBoardConfidence() <  60 && GetBoardConfidence() > 49 && SackChance < 0) ||
		(GetBoardConfidence() <  70 && GetBoardConfidence() > 59 && SackChance < 0))
	{
		bRet = true;
	}
    return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoAddTransferIncome
	Access:    	public 
	Parameter: 	const uint _Amount
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoAddTransferIncome(const uint _Amount)
{
	EXT_ASSERT(IsManagingClub() == true);
	m_ManHistory[0].DoAddTransferIncome(_Amount);
	SetSeasonTransferIncome(GetSeasonTransferIncome() + _Amount);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoAddTransferExpenditure
	Access:    	public 
	Parameter: 	const uint _Amount
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoAddTransferExpenditure(const uint _Amount)
{
	EXT_ASSERT(IsManagingClub() == true);
	EXT_ASSERT(_Amount >= 0);
	m_ManHistory[0].DoAddTransferExpenditure(_Amount);
	SetSeasonTransferExpenditure(GetSeasonTransferExpenditure() + _Amount);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetCurrentClubID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManager::GetCurrentClubID() const
{
	return m_ManHistory[0].GetClubID();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetCareerTransferIncome
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CManager::GetCareerTransferIncome()
{
	uint x = 0;
	for (uint n = 0; n < MAXMANAGERHISTORY; n++)
	{
		x += m_ManHistory[n].GetTransferIncome();
	};
	return x;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetCareerTransferExpenditure
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CManager::GetCareerTransferExpenditure()
{
	uint x = 0;
	for (uint n = 0; n < MAXMANAGERHISTORY; n++)
	{
		x += m_ManHistory[n].GetTransferExpenditure();
	};
	return x;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::SetClubTransferIncome
	Access:    	public 
	Parameter: 	const uint x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetClubTransferIncome(const uint x)
{
	EXT_ASSERT(IsManagingClub() == true);
	EXT_ASSERT(x >= 0);
	m_ManHistory[0].SetTransferIncome(x);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::SetClubTransferExpenditure
	Access:    	public 
	Parameter: 	const uint x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::SetClubTransferExpenditure(const uint x)
{
	EXT_ASSERT(IsManagingClub() == true);
	EXT_ASSERT(x >= 0);
	m_ManHistory[0].SetTransferExpenditure(x);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoEndOfWeekTasks
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::DoEndOfWeekTasks()
{
	CPerson::DoWeeklyTasks();
#ifdef MANAGEREXPENDITURE
	CString str;
	int income = CPerson::GetWage() * IsManagingClub();
	if (IsManagingClub() == true)
	{
		m_WeekExpenditure = CPerson::GetWage() * .8;
	}
	m_BankBalance +=  income - GetWeekExpenditure();
	if (IsUser() == true)
	{
		if (m_BankBalance < 0)
		{
			str.Format(IDS_GAMEOVER, GetName());
			UserMessageBox(str);
			return true;
		}
		if ((GetBankBalance() - GetWeekExpenditure()) < 0 && IsManagingClub() == false)
		{
			UserMessageBox(IDS_BANKBALANCELOW);
		}
	}
#endif
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetClubTransferIncome
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CManager::GetClubTransferIncome()
{
	if (IsManagingClub() == true)
	{
		return m_ManHistory[0].GetTransferIncome();
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetClubTransferExpenditure
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CManager::GetClubTransferExpenditure()
{
	if (IsManagingClub() == true)
	{
		return m_ManHistory[0].GetTransferExpenditure();
	}
	else
	{
		return 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetCurrentClub
	Access:    	public 
	Returns:   	CClub*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub* CManager::GetCurrentClub()
{
	if (IsManagingClub() == true)
	{
		return &WorldData().GetClub(GetCurrentClubID());
	}
	else
	{
		return GetLastClub();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::LastClubName
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CManager::LastClubName()
{
	if (GetLastClubID() != NOMANAGER)
	{
		return GetLastClub()->GetName();
	}
	else
	{
		return _T("-");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::LastClubUpperCaseName
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CManager::LastClubUpperCaseName()
{
	if (GetLastClubID() != NOMANAGER)
	{
		return GetLastClub()->GetUpperCaseName();
	}
	else
	{
		return(CString)_T("-");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GetLastClub
	Access:    	public 
	Returns:   	CClub*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub* CManager::GetLastClub()
{
	if (GetLastClubID() != NOMANAGER)
	{
		return &WorldData().GetClub(GetLastClubID());
	}
	else
	{
		return null;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoSwapClubIDs
	Access:    	public 
	Parameter: 	ushort _Club1ID
	Parameter: 	const ushort _Club2ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoSwapClubIDs(ushort _Club1ID, const ushort _Club2ID)
{
	for (int LoopCount = 0; LoopCount < MAXMANAGERHISTORY; LoopCount ++)
	{
		if (m_ManHistory[LoopCount].GetClubID() == _Club1ID)
		{
			m_ManHistory[LoopCount].SetClubID(_Club2ID);
		}
		else
		{
			if (m_ManHistory[LoopCount].GetClubID() == _Club2ID)
			{
				m_ManHistory[LoopCount].SetClubID(_Club1ID);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManager::GoalsSinceMOM
	Access:    	public 
	Returns:   	CGameMatchScore&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGameMatchScore& CManager::GoalsSinceMOM()
{
	return m_GoalsSinceMOM;
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoMakeActive
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	const uint _Wage
	Parameter: 	const byte _Contract
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoMakeActive(CPlayer* _Player, const uint _Wage, const byte _Contract, const ushort _ClubID)
{
	DoResetVariables();
    this->DoCopy(_Player);
	OnInitialise(_Wage, _Contract, _ClubID);
	CPerson::SetNationalityID(_Player->GetNationalityID());
}


/*------------------------------------------------------------------------------
	Method:   	CManager::IsActive
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CManager::IsActive()
{
	return GetRating().IsNull();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::DoRetire
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::DoRetire()
{
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CManager::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CPerson::Read(_pDataFile);
	_pDataFile->Read(&m_Data, sizeof m_Data);
	_pDataFile->Read(&m_ManHistory[0] , sizeof CManagerClubHistory * MAXMANAGERHISTORY);
	m_ClubHistory.Read(_pDataFile);
	m_SeasonHistory.Read(_pDataFile);
	m_CareerHistory.Read(_pDataFile);
	_pDataFile->Read(&m_ratingManager, sizeof m_ratingManager);
	_pDataFile->Read(&m_GoalsSinceMOM, sizeof m_GoalsSinceMOM);
	_pDataFile->Read(&m_BoardConfidence, sizeof m_BoardConfidence);
	_pDataFile->Read(&m_SupportersConfidence, sizeof m_SupportersConfidence);
}


/*------------------------------------------------------------------------------
	Method:   	CManager::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManager::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	CPerson::Write(_pDataFile);
	_pDataFile->Write(&m_Data, sizeof m_Data);
	_pDataFile->Write(&m_ManHistory[0] , sizeof CManagerClubHistory * MAXMANAGERHISTORY);
	m_ClubHistory.Write(_pDataFile);
	m_SeasonHistory.Write(_pDataFile);
	m_CareerHistory.Write(_pDataFile);
	_pDataFile->Write(&m_ratingManager, sizeof m_ratingManager);
	_pDataFile->Write(&m_GoalsSinceMOM, sizeof m_GoalsSinceMOM);
	_pDataFile->Write(&m_BoardConfidence, sizeof m_BoardConfidence);
	_pDataFile->Write(&m_SupportersConfidence, sizeof m_SupportersConfidence);
}


#ifdef ACTIVEFEDERATIONCUPS
/*------------------------------------------------------------------------------
Method:   	CManager::QualifiedForFederationCup
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CManager::QualifiedForFederationCup()
{
	EXT_ASSERT(IsManagingClub() == true);
	DoRaiseRating(2);
}
#endif


#ifdef MANAGEREXPENDITURE
//----------------------------------------------------------------------------------------------------------------
uint CManager::NextWeeksBalanceIfNoClub()
{
	return m_BankBalance - m_WeekExpenditure;
}

//----------------------------------------------------------------------------------------------------------------
byte CManager::GetLifeStyle()
{
	int WeekExpenditure = CPerson::GetWage() * .8;
	return  (WeekExpenditure >= 500 && WeekExpenditure < 1000) + \
		(WeekExpenditure >= 1000 && WeekExpenditure < 2000) * 2 + \
		(WeekExpenditure >= 2000 && WeekExpenditure < 4000) * 3 + \
		(WeekExpenditure >= 4000 && WeekExpenditure < 7000) * 4 + \
		(WeekExpenditure >= 7000 && WeekExpenditure < 11000) * 5 + \
		(WeekExpenditure >= 11000 && WeekExpenditure < 16000) * 6 + \
		(WeekExpenditure >= 16000 && WeekExpenditure < 22000) * 7 + \
		(WeekExpenditure >= 22000 && WeekExpenditure < 30000) * 8 + \
		(WeekExpenditure >= 30000) * 9;
}
#endif


#ifdef MANAGEREXPENDITURE
//---------------------------------------------------------------------------------------------------------------------------
CString	CManager::WeeklyExpenditureStr(CCountry& rCountry)
{
	return rCountry.GetCurrency().GetForeignStr(GetWeekExpenditure());
}


//---------------------------------------------------------------------------------------------------------------------------
CString	CManager::WeeklyProfitLossStr(CCountry& rCountry)
{
	if (IsManagingClub() == true)
	{
		return rCountry.GetCurrency().GetForeignStr(CPerson::GetWage() - GetWeekExpenditure());
	}
	else
	{
		return rCountry.GetCurrency().GetForeignStr(GetWeekExpenditure());
	}
}

//---------------------------------------------------------------------------------------------------------------------------
CString	CManager::BalanceStr(CCountry& rCountry)
{
	return rCountry.GetCurrency().GetForeignStr(GetBankBalance());
}
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CManagerList::~CManagerList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManagerList::~CManagerList()
{
}


/*------------------------------------------------------------------------------
	Method:   	CManagerList::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerList::DoInitialise()
{
	CBinaryReader theResource(0, IDR_MANAGER, TEXT("FDDATA"));
	ushort nHowMany = theResource.ReadUInt16();
	SetSize(nHowMany);
    for (uint LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
		CManager *pManager = new CManager;
		SetAt(LoopCount, pManager);
		pManager->DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManagerList::DoAddNewManager
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	const uint _Wage
	Parameter: 	const byte _Contract
	Parameter: 	const ushort _ClubID
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CManagerList::DoAddNewManager(CPlayer* _Player, const uint _Wage, const byte _Contract, const ushort _ClubID)
{
	CManager* pManager;
	ushort nRet = NOMANAGER;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		pManager = GetAt(ListPos);
		if (pManager->IsActive() == false)
		{
			nRet = ListPos;
			pManager->DoMakeActive(_Player, _Wage, _Contract, _ClubID);
			break;
		}
	}
	if (nRet == NOMANAGER)
	{
		nRet = WorldData().GetManagerList().Add(new CManager(_Player, _Wage, _Contract, _ClubID));
	}
	return nRet;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerList::DoNewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerList::DoNewSeason()
{
    CManager* pManager;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
         pManager = GetAt(ListPos);
		 if (pManager->IsActive() == true)
		 {
            pManager->DoNewSeason();
            if (pManager->GetAge() == EMPLOYEERETIREMENTAGE)
			{
				WorldData().GetClubList().SetManagerID(pManager->GetCurrentClubID(), NOMANAGER);
                GetAt(ListPos)->DoRetire();
            }
         }
    }
}


/*------------------------------------------------------------------------------
	Method:   	CManagerList::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	ushort nCount = GetSize();
    _pDataFile->Write(&nCount, sizeof nCount);
	for (int ListPos = 0; ListPos < nCount; ListPos ++)
	{
		GetAt(ListPos)->Write(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CManagerList::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	ushort nCount;
    _pDataFile->Read(&nCount, sizeof nCount);
	RemoveList();
	SetSize(nCount);
	for (int ListPos = 0; ListPos < nCount; ListPos ++)
	{
		CManager *pManager = new CManager();
		pManager->Read(_pDataFile);
		SetAt(ListPos, pManager);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CManagerList::DoEndOfWeekTasks
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerList::DoEndOfWeekTasks()
{
	for (short LoopCount = GetUpperBound(); LoopCount >= 0; LoopCount--)
	{
		if (GetAt(LoopCount)->IsActive() == true)
		{
            if (GetAt(LoopCount)->DoEndOfWeekTasks() == true && GetAt(LoopCount)->IsUser() == true)
			{
				WorldData().MakeUserInActive(LoopCount);
				GetAt(LoopCount)->DoRetire();
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManagerList::DoBuildUnemployedList
	Access:    	public 
	Parameter: 	CManagerList * _TheList
	Parameter: 	const ushort _Country
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerList::DoBuildUnemployedList(CManagerList& _TheList, const ushort _Country)
{
    CManager* pManager;
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount ++)
	{
		if (GetAt(LoopCount)->IsActive() == true)
		{
            pManager = GetAt(LoopCount);
			if (pManager->IsUser() == false && pManager->GetCurrentClubID() == NOMANAGER && pManager->GetNationalityID() == _Country)
			{
				_TheList.Add(pManager);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CManagerList::DoFindID
	Access:    	public 
	Parameter: 	CManager * _Manager
	Returns:   	ushort - the index(position in list) of the pointer
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CManagerList::DoFindID(CManager* _Manager)
{
    EXT_ASSERT(_Manager != null);
    ushort iRet = NOMANAGER;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (GetAt(ListPos)->IsActive() == false && GetAt(ListPos) == _Manager)
		{
            iRet = ListPos;
            break;
        }
    }
    EXT_ASSERT(iRet != NOMANAGER);
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerList::DoSwapClubIDs
	Access:    	public 
	Parameter: 	const ushort _Club1ID
	Parameter: 	const ushort _Club2ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerList::DoSwapClubIDs(const ushort _Club1ID, const ushort _Club2ID)
{
	// Check for human manager and change club number
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		if (GetAt(LoopCount)->IsActive() == false)
		{
	        GetAt(LoopCount)->DoSwapClubIDs(_Club1ID, _Club2ID);
		}
	}
}


#ifdef MANAGERTABLES
//----------------------------------------------------------------------------------------------------------------
void CManagerList::SortSeasonPoints(int left, int right)
{
	EXT_ASSERT(right > left && left > -1);
	int i, j, x;

	i = left;
	j = right;
	x = GetAt((left + right) / 2)->GetSeasonPoints();

	do 	{
		while((GetAt(i)->GetSeasonPoints() > x) && i < right) i++;
		while((GetAt(j)->GetSeasonPoints() < x) && j > left) j--;

		if(i <= j)
		{
			DoSwapListPositions(i, j);
			i++;
			j--;
		}
	}
	while(i <= j);

	if (left < j)
	{
		SortSeasonPoints(left, j);
	}
	if (i < right)
	{
		SortSeasonPoints(i, right);
	}
}


//----------------------------------------------------------------------------------------------------------------
void CManagerList::SortPointsSinceMOM (int left, int right)
{
	int i, j, x;

	i = left;
	j = right;
	x = GetAt((left + right) / 2)->GetPointsSinceMOM ();

	do
	{
		while((GetAt(i)->GetPointsSinceMOM () > x) && i < right) i++;
		while((GetAt(j)->GetPointsSinceMOM () < x) && j > left) j--;
		if(i <= j)
		{
			DoSwapListPositions(i, j);
			i++;
			j--;
		}
	}
	while(i <= j);

	if (left < j)
	{
		SortPointsSinceMOM (left, j);
	}
	if (i < right)
	{
		SortPointsSinceMOM (i, right);
	}
}


//----------------------------------------------------------------------------------------------------------------
void CManagerList::SortTotalPoints(int left, int right)
{
	int i, j, x;

	i = left;
	j = right;
	x = GetAt((left + right) / 2)->GetTotalPoints();

	do
	{
		while((GetAt(i)->GetTotalPoints() > x) && (i < right))
		{
			i++;
		}
		while((GetAt(j)->GetTotalPoints() < x) && (j > left))
		{
			j--;
		}
		if (i <= j)
		{
			DoSwapListPositions(i, j);
			i++;
			j--;
		}
	}
	while(i <= j);

	if (left < j)
	{
		SortTotalPoints(left, j);
	}
	if (i < right)
	{
		SortTotalPoints(i, right);
	}
}


//---------------------------------------------------------------------------------------
void CManagerList::DoSwapListPositions(const ushort aElement1, const ushort aElement2)
{
	CManager* pManager = GetAt(aElement1);
	SetAt(aElement1, GetAt(aElement2));
	SetAt(aElement2, pManager);
}
#endif
