
 /*

    Program:- CharityShield.cpp


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShield::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_ID = _Resource.ReadUInt16();
	
	CCup::DoInitialiseFromResource(_Resource, NOCLUB, 0, 1);
	m_TheClubs[0] = CBaseCompetition::GetWinningClubID();
	m_TheClubs[1] = CBaseCompetition::GetRunnersUpClubID();
	m_TheClubs[0].id = 10;
	m_TheClubs[1].id = 4;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::pCup
	Access:    	public 
	Parameter: 	void
	Returns:   	CCup*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCup* CCharityShield::GetCup()
{
	return static_cast<CCup*>(this);
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::GetID
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CCharityShield::GetID() const
{
	return m_ID;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::AddClub
	Access:    	public 
	Parameter: 	ushort ClubNum
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShield::DoAddClub(ClubID _ClubID)
{
	if (m_TheClubs[0].id == NOCLUB)
	{
		m_TheClubs[0] = _ClubID;
	}
	else
	{
		m_TheClubs[1] = _ClubID;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::ClubQualified
	Access:    	public 
	Parameter: 	ushort ClubNum
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCharityShield::ClubQualified(ClubID _ClubID)
{
	if (m_TheClubs[0] == _ClubID || m_TheClubs[1] == _ClubID)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::DoEndOfSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShield::DoEndOfSeason()
{
	CCup::GetCupTiedPlayerList().RemoveList();
	CCup::GetRounds().DoEndOfSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::DoDraw
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShield::DoDraw()
{
    if (CCup::IsDrawThisWeek() == NULL)
	{
        return;         // No draw to create
    }

	CNumericArray<ClubID> DrawClubs;
#ifdef SPECIAL_CASE_CHARITY_SHIELDS
	switch(CCup::GetSpecialActionFlag())
	 
		case GERMANCHARITYSHIELDSPECIALEVENT:
			// Not implemented yet
			break;

		case MLSCHARITYSHIELDSPECIALEVENT:
			m_TheClubs[0] = CCup::GetCountry()->GetActiveCountry().DoFindDivisionTopPlayOffWinnerClubID(0);
			m_TheClubs[1] = CCup::GetCountry()->GetActiveCountry().DoFindDivisionTopPlayOffWinnerClubID(1);
			// Deliberately fall through to default
		default:
#endif
			EXT_ASSERT(m_TheClubs[0].id != NOCLUB && m_TheClubs[1].id != NOCLUB);
			DrawClubs.Add(m_TheClubs[0]);
			DrawClubs.Add(m_TheClubs[1]);
// 			DEBUG_OUTPUT(("Charity Shield %s v %s"), (WorldData().GetClub(m_TheClubs[0]).GetName(), WorldData().GetClub(m_TheClubs[1]).GetName()));
			CCup::GetRounds()[0].DoCreateMatchList(static_cast<CCup&> (*this), DrawClubs);
			m_TheClubs[0].id = NOCLUB;
			m_TheClubs[1].id = NOCLUB;
#ifdef SPECIAL_CASE_CHARITY_SHIELDS
			break;
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::DoPlayMatches
	Access:    	public 
	Parameter: 	const bool _ShowSummary
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCharityShield::DoPlayMatches(const bool _ShowSummary)
{
    bool bRet = CCup::DoPlayMatches(_ShowSummary);
	if (bRet == true && CCup::GetFinalRound()->HaveAllRoundMatchesBeenPlayed() == true)
	{
    ClubID csLoser;
    ClubID csWinner = CCup::GetFinalRound()->GetRoundFixtureWinners(0, csLoser, *this);

    //lLogFmt("Charity shield: winning club %s\n", WorldData().GetClubByID(csWinner).GetName());
    //lLogFmt("Charity shield: losing club %s\n", WorldData().GetClubByID(csLoser).GetName());

    WorldData().GetClubByID(csWinner).WonCharityShield(CCup::GetName(), 
#ifdef TROPHYHISTORY
			CCup::GetTrophyID()
#else
			0
#endif
			, true);


    WorldData().GetClubByID(csLoser).TrophyRunnersUp(CCup::GetName(), 
#ifdef TROPHYHISTORY
			CCup::GetTrophyID()
#else
			0
#endif
			, true);

		//@@str.FormatMessage(IDS_NEWSPAPERWINNERSHEADLINE, CCup::WinnerNameStr(), CCup::RunnerUpNameStr(), CCup::m_Title);
		//@@CCup::pCountry().SetMajorHeadline(str);
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShield::Read(CDataFile* _pDataFile)
{
    _pDataFile->Read(&m_ID, sizeof m_ID);
	_pDataFile->Read(&m_TheClubs[0], sizeof m_TheClubs[0] * NUMBER_CHARITY_SHIELD_CLUBS);
    CCup::Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShield::Write(CDataFile* _pDataFile)
{
    _pDataFile->Write(&m_ID, sizeof m_ID);
	_pDataFile->Write(&m_TheClubs[0], sizeof m_TheClubs[0] * NUMBER_CHARITY_SHIELD_CLUBS);
    CCup::Write(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShield::DeleteAll
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShield::DeleteAll()
{
    CCup::DeleteAll();
}
//////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::CCharityShieldList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCharityShieldList::CCharityShieldList()
{
	CBinaryReader theResource(0, IDR_CHARITYSHIELD, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);

	if (CExtArray::GetSize() > 0)
	{
		m_pList = FD_ENGINE_NEW CCharityShield[CExtArray::GetSize()];
		for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
		{
			m_pList[ListPos].DoInitialiseFromResource(theResource);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::~CCharityShieldList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCharityShieldList::~CCharityShieldList()
{
	if (CExtArray::GetSize() > 0)
	{
		delete[] m_pList;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShieldList::Read(CDataFile* File)
{
  EXT_ASSERT(File != NULL);
	if (CExtArray::GetSize() > 0)
	{
		for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
		{
      m_pList[ListPos].Read(File);
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::Write
	Access:    	public
  Parameter:  CDataFile* File
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShieldList::Write(CDataFile* File)
{
  EXT_ASSERT(File != NULL);
	if (CExtArray::GetSize() > 0)
	{
		for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
		{
      m_pList[ListPos].Write(File);
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::DoFindCompetitionTitleForFixture
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCharityShieldList::DoFindCompetitionTitleForFixture(CFixture* _Fixture)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	CString TitleStr;
	for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++) 
	{
		TitleStr = m_pList->DoFindCompetitionTitleForFixture(_Fixture);
		if (TitleStr.IsEmpty() == false)
		{
			break;
		}
	}
	return TitleStr;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::DoFindNeutralVenueNameForFixture
	Access:    	public 
	Parameter: 	CFixture * _Fixture
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCharityShieldList::DoFindNeutralVenueNameForFixture(CFixture* _Fixture)
{
	EXT_ASSERT_POINTER(_Fixture, CFixture);
	CString TitleStr = _T("");
	for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
	{
		TitleStr = m_pList->DoFindNeutralVenueNameForFixture(_Fixture);
		if (TitleStr.IsEmpty() == false)
		{
			break;
		}
	}
	return TitleStr;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::IsClubMatchToday
	Access:    	public 
	Parameter: 	const CCalendar& CurrentDate
	Parameter: 	ushort Club
	Parameter: 	CMatchInfo& Match
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCharityShieldList::IsClubMatchToday(const CCalendar& _CurrentDate, ClubID _ClubID, CMatchInfo& _Match)
{
    bool MatchPlayed = false;
	if (GetSize() > 0)
	{
		for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
		{
			if (m_pList[ListPos].IsClubMatchToday(_CurrentDate, _ClubID, _Match))
			{
				MatchPlayed = true;
				break;
			}
		}
	}
	return MatchPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::PlayMatches
	Access:    	public 
	Parameter: 	const bool bShowSummary
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CCharityShieldList::DoPlayMatches(const bool _ShowSummary)
{
    bool MatchPlayed = false;
	if (CExtArray::GetSize() > 0)
	{
		for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
		{
			if (m_pList[ListPos].DoPlayMatches(_ShowSummary))
			{
				MatchPlayed = true;
			}
		}
	}
	return MatchPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::CountryCharityShield
	Access:    	public 
	Parameter: 	ushort CountryId
	Returns:   	CCharityShield*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCharityShield* CCharityShieldList::CountryCharityShield(ushort CountryId)
{
    CCharityShield* ListPtr = NULL;
	if (CExtArray::GetSize() > 0)
	{
		for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
		{
			if (m_pList[ListPos].GetCountryID() == CountryId)
			{
				ListPtr = &m_pList[ListPos];
				break;
			}
		}
	}
	return ListPtr;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::FindPtr
	Access:    	public 
	Parameter: 	ushort Id
	Returns:   	CCharityShield*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCharityShield* CCharityShieldList::DoFind(ushort _ShieldID)
{
	CCharityShield* pCharityShield = NULL;
	if (CExtArray::GetSize() > 0)
	{
		for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
		{
			if (m_pList[ListPos].GetID() == _ShieldID)
			{
				pCharityShield = &m_pList[ListPos];
				break;
			}
		}
	}
	return pCharityShield;
}


/*------------------------------------------------------------------------------
	Method:   	CCharityShieldList::DoDraws
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCharityShieldList::DoDraws()
{
	if (CExtArray::GetSize() > 0)
	{
	    for (int ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
		{
			m_pList[ListPos].DoDraw();
		}
    }
}
