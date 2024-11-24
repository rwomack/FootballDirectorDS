
 /*

		Program:- MatchInfo.cpp

		The match info class routines

		Football Director

		(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "MatchInfo.h"


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::CMatchInfo
	Access:    	public
	Returns:
	Qualifier: 	:
	Purpose:
------------------------------------------------------------------------------*/
CMatchInfo::CMatchInfo()
	: m_CompTitle(TEXT(""))
	, m_pCup(NULL)
	, m_pFixture(NULL)
#ifdef	ACTIVEREFEREES
	, m_nReferee(wNOTFOUND)
#endif
	, m_eCompetitionType(NOTAMATCH)
{
	m_HomeClubID.id = NOCLUB;
	m_AwayClubID.id = NOCLUB;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::CMatchInfo
	Access:    	public
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchInfo::CMatchInfo(const ClubID _HomeClubID, const ClubID _AwayClubID)
	: m_CompTitle(TEXT(""))
	, m_pCup(NULL)
	, m_pFixture(NULL)
#ifdef	ACTIVEREFEREES
	, m_nReferee(wNOTFOUND)
#endif
	, m_eCompetitionType(NOTAMATCH)
{
	m_HomeClubID = _HomeClubID;
	m_AwayClubID = _AwayClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::CMatchInfo
	Access:    	public
	Parameter: 	const eMatchCompetitionType _eCompetitionType
	Parameter: 	const byte _nSelect
	Parameter: 	const byte _nUse
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchInfo::CMatchInfo(const eMatchCompetitionType _eCompetitionType, const byte _nSelect, const byte _nUse)
{
	m_eCompetitionType = _eCompetitionType;
	CAvailableSubstitutes::SetSubstitutes(_nSelect, _nUse);
	m_pCup = NULL;
	m_pFixture = NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::InitialiseClubs
	Access:    	public
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::DoInitialiseClubs(const ClubID _HomeClubID, const ClubID _AwayClubID)
{
	m_HomeClubID = _HomeClubID;
    m_StadiumID = WorldData().GetClubByID(m_HomeClubID).GetStadiumID();
	m_AwayClubID = _AwayClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::DoInitialiseFriendlyMatch
	Access:    	public
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::DoInitialiseFriendlyMatch(const ClubID _HomeClubID, const ClubID _AwayClubID)
{
	m_eCompetitionType = FRIENDLYMATCH;
	SetCup(NULL);
    SetFixture(NULL);
	DoInitialiseClubs(_HomeClubID, _AwayClubID);
	CAvailableSubstitutes::SetSubstitutes(5, 3);
	CString Str;
  Str.LoadString(IDS_FRIENDLYMATCH);
	SetCompetitionTitle(Str);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::DoInitialiseLeagueMatch
	Access:    	public
	Parameter: 	const ushort _HomeClubID
	Parameter: 	const ushort _AwayClubID
	Parameter: 	CDivision& _Division
	Parameter: 	CFixture* _pFixture
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::DoInitialiseLeagueMatch(const ClubID _HomeClubID, const ClubID _AwayClubID, CDivision& _Division, CFixture* _Fixture)
{
	m_eCompetitionType = LEAGUEMATCH;
	SetCup(NULL);
    SetFixture(_Fixture);
	DoInitialiseClubs(_HomeClubID, _AwayClubID);
	m_CompTitle = _Division.GetName();
	CAvailableSubstitutes::SetSubstitutes(_Division.GetSubsSelect(), _Division.GetSubsUse());
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetAwayName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchInfo::GetAwayName()
{
	return WorldData().GetClubByID(m_AwayClubID).GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetFixture
	Access:    	public
	Parameter: 	void
	Returns:   	CFixture*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CFixture* CMatchInfo::GetFixture()
{
	return m_pFixture;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetCompTitle
	Access:    	public
	Parameter: 	void
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CMatchInfo::GetCompetitionTitle()
{
	return m_CompTitle;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetCup
	Access:    	public
	Parameter: 	void
	Returns:   	CCup*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCup* CMatchInfo::GetCup()
{
	return m_pCup;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetHomeClubID
	Access:    	public const
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CMatchInfo::GetHomeClubID() const
{
	return m_HomeClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetHomeName
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CMatchInfo::GetHomeName()
{
	return WorldData().GetClubByID(m_HomeClubID).GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetAwayClub
	Access:    	public const
	Parameter: 	void
	Returns:   	const ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CMatchInfo::GetAwayClubID() const
{
	return m_AwayClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetCompetitionName
	Access:    	public
	Parameter: 	void
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CMatchInfo::GetCompetitionName()
{
	return m_CompTitle;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetOpponentClub
	Access:    	public
	Parameter: 	CClub* _pTheClub
	Returns:   	CClub&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub* CMatchInfo::GetOpponentClub(CClub* _TheClub)
{
	if (m_HomeClubID.id == NOCLUB || m_AwayClubID.id == NOCLUB)
	{
		// No opponent
		return NULL;
	}
	if (_TheClub == &WorldData().GetClubByID(m_HomeClubID))
	{
		return &WorldData().GetClubByID(m_AwayClubID);
	}
	return &WorldData().GetClubByID(m_HomeClubID);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetStadiumSize
	Access:    	public
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CMatchInfo::GetStadiumCapacity() const
{
	return WorldData().GetStadium(m_StadiumID)->GetCapacity();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetTheDate
	Access:    	public
	Parameter: 	void
	Returns:   	CMatchTime&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CCalendar& CMatchInfo::GetTheDate()
{
	return m_TheDate;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetStadiumName
	Access:    	public
	Returns:   	CString&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString& CMatchInfo::GetStadiumName()
{
	return WorldData().GetStadium(m_StadiumID)->GetName();
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::GetCompetitionType
	Access:    	public const
	Parameter: 	void
	Returns:   	const eCompetitionType
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
eMatchCompetitionType CMatchInfo::GetCompetitionType() const
{
	return m_eCompetitionType;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::SetCompetitionType
	Access:    	public
	Parameter: 	eMatchCompetitionType _MatchCompetitionType
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::SetCompetitionType(eMatchCompetitionType _eCompetitionType)
{
	m_eCompetitionType = _eCompetitionType;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::IsHomeClub
	Access:    	public
	Parameter: 	CClub * _Club
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchInfo::IsHomeClub(CClub* _Club)
{
	if (_Club->DoFindID() == m_HomeClubID)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::SetCompetitionTitle
	Access:    	public
	Parameter: 	CString _Str
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::SetCompetitionTitle(const CString &_Str)
{
	m_CompTitle = _Str;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::SetCup
	Access:    	public
	Parameter: 	CCup* x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::SetCup(CCup* _Cup)
{
	m_pCup = _Cup;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::SetFixture
	Access:    	public
	Parameter: 	CFixture* _nFixtureID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::SetFixture(CFixture* _nFixtureID)
{
	m_pFixture = _nFixtureID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::SetStadiumID
	Access:    	public
	Parameter: 	const ushort _ID
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::SetStadiumID(const ushort _ID)
{
	m_StadiumID = _ID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchInfo::SetTheDate
	Access:    	public
	Parameter: 	CMatchTime& _Date
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::SetTheDate(CCalendar& _Date)
{
	m_TheDate = _Date;
}


#ifdef	ACTIVEREFEREES
/*------------------------------------------------------------------------------
Method:   	CMatchInfo::SetReferee
Access:    	public
Parameter: 	const ushort _nReferee
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CMatchInfo::SetReferee(const ushort _nReferee)
{
	m_nReferee = _nReferee;
}
#endif
