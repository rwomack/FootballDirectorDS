
 /*

    Class:- CInjury


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CInjury::CInjury
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjury::CInjury()
{
    DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::DoResetVariables()
{
	m_PlayerID = wNOTFOUND;
	m_Severity = bNOTFOUND;
    m_HowLongRemainingInjured = 0;
    m_HowLongHasBeenInjured = 0;
	m_RandomPlayerEvent = false;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::IsEmpty
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CInjury::IsEmpty()
{
	if (m_Severity == 0xff)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::operator=
	Access:    	public 
	Parameter: 	const CInjury & p
	Returns:   	CInjury&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjury& CInjury::operator = (const CInjury& p)
{
	m_PlayerID = p.m_PlayerID;
    m_Severity = p.m_Severity;
    m_MatchPerformanceEffect = p.m_MatchPerformanceEffect;
    m_HowLongRemainingInjured = p.m_HowLongRemainingInjured;
	m_Improvement = p.m_Improvement;
	m_HowLongHasBeenInjured = p.m_HowLongHasBeenInjured;
	return *this;
}

/*------------------------------------------------------------------------------
	Method:   	CInjury::GetImprovement
	Access:    	public 
	Returns:   	sbyte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
sbyte CInjury::GetImprovement() const						
{ 
	return m_Improvement; 
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::GetHowLongInjured
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjury::GetHowLongInjured() const					
{ 
	return m_HowLongRemainingInjured; 
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::GetHowLongHasBeenInjured
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjury::GetHowLongHasBeenInjured() const			
{ 
	return m_HowLongHasBeenInjured; 
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::SetMatchPerformanceEffect
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::SetMatchPerformanceEffect(const byte _Value)		
{ 
	m_MatchPerformanceEffect = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::GetMatchPerformanceEffect
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjury::GetMatchPerformanceEffect() const			
{ 
	return m_MatchPerformanceEffect; 
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::GetSeverity
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CInjury::GetSeverity() const							
{ 
	return m_Severity; 
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::SkillMultiplier
	Access:    	public 
	Returns:   	double
	Qualifier: 	
	Purpose:	Skill should be multiplied by the returned value to give his skill
------------------------------------------------------------------------------*/
double CInjury::SkillMultiplier()
{
	double MatchPerformanceEffect = static_cast<double>(100 - m_MatchPerformanceEffect) / 100;
	EXT_ASSERT(m_MatchPerformanceEffect <= 100);
	return MatchPerformanceEffect;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::DoCreateTrainingInjury
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	CClub& _Club
	Parameter: 	const CInjuryType::eInjurySeverity _Severity
	Returns:   	void
	Qualifier: 	
	Purpose:	Injure the player and announce it
------------------------------------------------------------------------------*/
void CInjury::DoCreateTrainingInjury(CPlayer* _Player, CClub& _Club, const CInjuryType::eInjurySeverity _Severity)
{
	CExtString Str;
    DoCreateInjury(_Player, _Club, _Severity);

	// Display message box giving details of injury and likely period
	int tHowLongRemainingUnableToPlay = HowLongRemainingUnableToPlay();
	if (tHowLongRemainingUnableToPlay < 2)
	{
		// Less than 2 weeks injured
		if (WorldData().GetInjuryTypeList().GetAt(m_Severity).GetPlural() == true)
		{
			Str.Format(IDS_TRAININGBEENINJUREDSHORTPLURAL,
						_Player->GetName(),
						tHowLongRemainingUnableToPlay,
						DescriptionStr());
		}
		else
		{
			Str.Format(IDS_TRAININGBEENINJUREDSHORT,
						_Player->GetName(),
						tHowLongRemainingUnableToPlay,
						DescriptionStr());
		}
	}
	else
	{
		if (WorldData().GetInjuryTypeList().GetAt(m_Severity).GetPlural() == true)
		{
			Str.Format(IDS_TRAININGBEENINJUREDLONGPLURAL,
						_Player->GetName(),
						tHowLongRemainingUnableToPlay,
						DescriptionStr());
		}
		else
		{
			Str.Format(IDS_TRAININGBEENINJUREDLONG,
						_Player->GetName(),
						tHowLongRemainingUnableToPlay,
						DescriptionStr());
		}
	}
    if (_Club.IsUserControlled() == true)
	{
		_Club.AddToClubNews(Str);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::DoCreateEventInjury
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	CClub * _Club
	Returns:   	void
	Qualifier: 	
	Purpose:	Injure the player and announce it
------------------------------------------------------------------------------*/
void CInjury::DoCreateEventInjury(CPlayer* _Player, CClub* _Club)
{
    DoCreateEvent(_Player);

	// Display message box giving details of injury and likely period
	CString StringToUse = WorldData().GetInjuryEventTypeList().GetAt(m_Severity).GetDescription().GetStr();
	if (HowLongRemainingUnableToPlay() > 1)
	{
		StringToUse = WorldData().GetInjuryEventTypeList().GetAt(m_Severity).GetPluralDescription().GetStr();
	}
    CExtString Str;
    Str.Format(StringToUse, _Player->GetName(), HowLongRemainingUnableToPlay());
	_Club->AddToClubNews(Str);

	// Fiddle to allow physio screen to display correctly
	int ConvertToInjury[] =
	{
		NOTFOUND, 		 // 0 Flu
		NOTFOUND,		 // 1 Stomach Bug
		NOTFOUND,		 // 2 Food Poisoning
		NOTFOUND,		 // 3 Cold
		NOTFOUND,		 // 4 Mystery Illness
		NOTFOUND,		 // 5 Personal Issue
		5,				 // 6 Sprained Ankle
		17,				 // 7 Broken Arm
		6,				 // 8 Pulled Muscle
		6,				 // 9 Pulled Muscle
		NOTFOUND,		 // 10 Whiplash, car crash
		NOTFOUND,		 // 11 Bad Injuy, car crash
		NOTFOUND,		 // 12 Night club
		NOTFOUND,		 // 13 Collapsed in training
	};
	if (ConvertToInjury[m_Severity] != NOTFOUND)
	{
	    m_RandomPlayerEvent = false;
		m_Severity = ConvertToInjury[m_Severity];
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::GetEventType
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	int
	Qualifier: 	
	Purpose:	Find the injury relating to the given value < 100
------------------------------------------------------------------------------*/
int CInjury::GetEventType(const byte _Value)
{
    EXT_ASSERT(_Value < 100);
	sbyte sRet = 0;
	do
	{
		if (_Value < WorldData().GetInjuryEventTypeList().GetAt(sRet).GetMaxRnd())
		{
			break;
		}
		sRet++;
	}
	while(1);
	return sRet;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::DoCreateInjury
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Parameter: 	CClub * _Club
	Parameter: 	const CInjuryType::eInjurySeverity _Severity
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::DoCreateInjury(CPlayer* _Player, CClub& _Club, CInjuryType::eInjurySeverity _Severity /*= CInjuryType::MAXINJURYSEVERITIES*/)
{
    DoResetVariables();
	m_PlayerID = _Player->DoFindID();
	sbyte sRet;
	// Search for injury of specific severity
	do
	{
		m_Severity = RandomNumber.IntLessThan(99);
		sRet = 0;
		do
		{
			if (m_Severity < WorldData().GetInjuryTypeList().GetAt(sRet).GetMaxRnd())
			{
				break;
			}
			sRet++;
		}
		while(1);
		m_Severity = sRet;
	}
	while (_Severity < CInjuryType::MAXINJURYSEVERITIES || WorldData().GetInjuryTypeList().GetAt(m_Severity).GetSeverity() != _Severity);

    if (WorldData().GetInjuryTypeList().GetAt(m_Severity).GetMatchPerformanceEffect() < 100)
	{
		SetMatchPerformanceEffect(WorldData().GetInjuryTypeList().GetAt(m_Severity).GetMatchPerformanceEffect() + RandomNumber.IntLessThan(WorldData().GetInjuryTypeList().GetAt(m_Severity).GetRandomMatchPerformanceEffect()));
	}
	else
	{
		SetMatchPerformanceEffect(100);
	}

    m_HowLongRemainingInjured = WorldData().GetInjuryTypeList().GetAt(m_Severity).GetBasePeriod();
	if (WorldData().GetInjuryTypeList().GetAt(m_Severity).GetRandomPeriod() > 0)
	{
		m_HowLongRemainingInjured += RandomNumber.IntLessThan(WorldData().GetInjuryTypeList().GetAt(m_Severity).GetRandomPeriod());
	}
    m_HowLongRemainingInjured =  m_HowLongRemainingInjured << 1;      // Half weekly improvements
    m_HowLongRemainingInjured++;                 // Injuries during match will be decreased immediately after match

	m_Improvement = _Club.PhysioSkill() - RandomNumber.IntLessThan(100);
	if (m_Improvement < -49)
	{
		VeryGoodImproveMent();
	}
	else if (m_Improvement < -24)
	{
		GoodImproveMent();
	}
	else if (m_Improvement < 26)
	{
		AverageImproveMent();
	}
	else if (m_Improvement < - 51)
	{
		BadImproveMent();
	}
	else
	{
		VeryBadImproveMent();
	}

    if (_Club.IsUserControlled() == true)
	{
		CExtString Str;
		Str.Format(_T("%s %s"), _Player->GetName(), DetailsAndTimeOutStr());
		_Club.AddToClubNews(Str);
	}
//@@    pClub->NewsPaperEventPtr()->CheckIfNewsWorthyInjury(_Player, m_HowLongRemainingInjured / 2);
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::DoCreateEvent
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::DoCreateEvent(CPlayer* _Player)
{
	DoResetVariables();
	m_PlayerID = _Player->DoFindID();
    m_Severity = GetEventType(RandomNumber.IntLessThan(100));

    m_RandomPlayerEvent = true;
    SetMatchPerformanceEffect(100);    // Always unavailable for matches

    m_HowLongRemainingInjured = WorldData().GetInjuryEventTypeList().GetAt(m_Severity).GetBasePeriod();
	if (WorldData().GetInjuryEventTypeList().GetAt(m_Severity).GetRandomPeriod() > 0)
	{
		m_HowLongRemainingInjured += RandomNumber.IntLessThan(WorldData().GetInjuryEventTypeList().GetAt(m_Severity).GetRandomPeriod());
	}
    m_HowLongRemainingInjured = m_HowLongRemainingInjured << 1;              // Half weekly improvements
    m_HowLongRemainingInjured++;                 // Injuries during match will be decreased immediatly after match
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::VeryGoodImproveMent
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::VeryGoodImproveMent()
{
    if (m_HowLongRemainingInjured > 20)
	{
        m_Improvement = -4;
	}
    else if (m_HowLongRemainingInjured > 8)
	{
        m_Improvement = -2;
	}
    else if (m_HowLongRemainingInjured > 4)
	{
        m_Improvement = -2;
	}
	else
	{
		m_Improvement = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::GoodImproveMent
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::GoodImproveMent()
{
    if (m_HowLongRemainingInjured > 20)
	{
        m_Improvement = -4;
	}
    else if (m_HowLongRemainingInjured > 8)
	{
        m_Improvement = -2;
	}
    else if (m_HowLongRemainingInjured > 4)
	{
		m_Improvement = 0;
	}
	else
	{
		m_Improvement = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::AverageImproveMent
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::AverageImproveMent()
{
    if (m_HowLongRemainingInjured > 20)
	{
		m_Improvement = 0;
	}
    else if (m_HowLongRemainingInjured > 8)
	{
		m_Improvement = 0;
	}
    else if (m_HowLongRemainingInjured > 4)
	{
		m_Improvement = 0;
	}
	else
	{
		m_Improvement = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::BadImproveMent
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::BadImproveMent()
{
    if (m_HowLongRemainingInjured > 20)
	{
        m_Improvement = 4;
	}
    else if (m_HowLongRemainingInjured > 8)
	{
        m_Improvement = 2;
	}
    else if (m_HowLongRemainingInjured > 4)
	{
		m_Improvement = 0;
	}
	else
	{
		m_Improvement = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::VeryBadImproveMent
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjury::VeryBadImproveMent()
{
    if (m_HowLongRemainingInjured > 20)
	{
        m_Improvement = 4;
	}
    else if (m_HowLongRemainingInjured > 8)
	{
        m_Improvement = 2;
	}
    else if (m_HowLongRemainingInjured > 4)
	{
        m_Improvement = 2;
	}
	else
	{
		m_Improvement = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::DescriptionStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CInjury::DescriptionStr()
{
    if (m_RandomPlayerEvent == true)
	{
        return WorldData().GetInjuryEventTypeList().GetAt(m_Severity).GetDescription().GetStr();      // The injury description
    }
    else
	{
        return WorldData().GetInjuryTypeList().GetAt(m_Severity).GetDescription().GetStr();      // The Event description
    }
}


 // ---------------------------------------------------------------------------------------------------------------------------
CString CInjury::CapitalDescriptionStr()
{
	CString str, str1;
	str = DescriptionStr();		// The injury description
	str1 = str.Left(1);
	str1.MakeUpper();
	str = str.Right(str.GetLength() - 1);
	return str1 + str;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::InjuryLengthStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CInjury::InjuryLengthStr()
{
	CString Str;
	if (HowLongRemainingUnableToPlay() < 2)
	{
		// Less than 2 weeks injured
		Str.LoadString(IDS_TIMEINJUREDSHORT);
	}
	else
	{
		Str.Format(IDS_TIMEINJUREDLONG, HowLongRemainingUnableToPlay());
	}
	return Str;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::HowLongHasBeenUnableToPlayStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CInjury::HowLongHasBeenUnableToPlayStr()
{
	CString Str;
	if (HowLongHasBeenUnableToPlay() < 2)
	{
		// Less than 2 weeks injured
		Str.LoadString(IDS_TIMEINJUREDSHORT);
	}
	else
	{
		Str.Format(IDS_TIMEINJUREDLONG, HowLongHasBeenUnableToPlay());
	}
	return Str;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::DetailsAndTimeOutStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CInjury::DetailsAndTimeOutStr()
{
	CString Str;
	if (HowLongRemainingUnableToPlay() < 2)
	{
		// Less than 2 weeks injured
		if (WorldData().GetInjuryTypeList().GetAt(m_Severity).GetPlural() == true)
		{
			Str.Format(IDS_INJUREDSHORTPLURAL, DescriptionStr());
		}
		else
		{
			Str.Format(IDS_INJUREDSHORT, DescriptionStr());
		}
	}
	else
	{
		if (WorldData().GetInjuryTypeList().GetAt(m_Severity).GetPlural() == true)
		{
			Str.Format(IDS_INJUREDLONGPLURAL, DescriptionStr(), HowLongRemainingUnableToPlay());
		}
		else
		{
			Str.Format(IDS_INJUREDLONG, DescriptionStr(), HowLongRemainingUnableToPlay());
		}
	}
	return Str;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::CarriedOff
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:	Does the injury require the player to be carried off or can 
				he walk?
------------------------------------------------------------------------------*/
bool CInjury::CarriedOff()
{
    return WorldData().GetInjuryTypeList().GetAt(m_Severity).GetCarryOff();
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::TotalInjuryLengthStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CInjury::TotalInjuryLengthStr()
{
	byte Total = HowLongRemainingUnableToPlay() + HowLongHasBeenUnableToPlay();
	CString Str;
	if (Total < 2)
	{
		// Less than 2 weeks injured
		Str.LoadString(IDS_TIMEINJUREDSHORT);
	}
	else
	{
		Str.Format(IDS_TIMEINJUREDLONG, Total);
	}
	return Str;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::HowLongRemainingUnableToPlay
	Access:    	public 
	Returns:   	byte - how long player is likely to be out for
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CInjury::HowLongRemainingUnableToPlay()
{
    if (m_HowLongRemainingInjured == 1)
	{
        return m_HowLongRemainingInjured;
	}
    if (m_HowLongRemainingInjured > 1)
	{
        return m_HowLongRemainingInjured / 2;
	}
    else
	{
        return m_Improvement / 2;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CInjury::HowLongHasBeenUnableToPlay
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CInjury::HowLongHasBeenUnableToPlay()
{
    if (m_HowLongHasBeenInjured == 1)
	{
        return GetHowLongHasBeenInjured();
	}
    if (m_HowLongHasBeenInjured > 1)
	{
        return GetHowLongHasBeenInjured() / 2;
	}
    else
	{
        return 0;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CInjury::IsRandomPlayerEvent
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CInjury::IsRandomPlayerEvent()
{
	return m_RandomPlayerEvent;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::DailyImprove
	Access:    	public 
	Parameter: 	CClub & _Club
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CInjury::DailyImprove(CClub& _Club)
{
	m_HowLongRemainingInjured--;
	m_HowLongHasBeenInjured++;
	if (_Club.IsUserControlled() == true && m_Severity != CInjuryType::KNOCK && m_HowLongRemainingInjured == 0)
	{
		CExtString Str;
		CString StrPlayerName = WorldData().GetPlayer(m_PlayerID)->GetName();
		switch(m_Improvement)
		{
			case 2:
				Str.Format(IDS_INJURYVERYBADRECOVERED, StrPlayerName);
				break;
			case 1:
				Str.Format(IDS_INJURYBADRECOVERED, StrPlayerName);
				break;
			case 0:
				Str.Format(IDS_INJURYRECOVERED, StrPlayerName);
				break;
			case -1:
				Str.Format(IDS_INJURYGOODRECOVERED, StrPlayerName);
				break;
			case -2:
				Str.Format(IDS_INJURYVERYGOODRECOVERED, StrPlayerName);
				break;
			default:
				Str.Format(IDS_INJURYRECOVERED, StrPlayerName);
				break;
		}
		_Club.AddToClubNews(Str);
	}
	if (m_HowLongRemainingInjured == 0)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CInjury::GetPlayer
	Access:    	public 
	Returns:   	CPlayer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayer* CInjury::GetPlayer()
{
	if (m_PlayerID != NOPLAYER)
	{
		return WorldData().GetPlayer(m_PlayerID);
	}
	else
	{
		return null;
	}
}

//////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CInjuryList::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjuryList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    CInjury* pInjury;
	RemoveList();
	ushort count, Idx;
    _pDataFile->Read(&count, sizeof count);
	for (uint LoopCount = 0; LoopCount < count; LoopCount++)
	{
        _pDataFile->Read(&Idx, sizeof Idx);
		if (Idx != wNOTFOUND)
		{
			pInjury = new CInjury();
			EXT_ASSERT(pInjury != null);
			_pDataFile->Read(pInjury, sizeof CInjury);
		}
		else
		{
			pInjury = null;
		}
        SetAtGrow(LoopCount, pInjury);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryList::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CInjuryList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    CInjury* pInjury;
	ushort nHowMany = GetSize();
    _pDataFile->Write(&nHowMany, sizeof nHowMany);
	ushort nNull = wNOTFOUND;
    for (uint LoopCount = 0; LoopCount < nHowMany; LoopCount++)
	{
        pInjury = static_cast<CInjury*>(GetAt(LoopCount));
		if (pInjury != null && !pInjury->IsEmpty() )
		{
            _pDataFile->Write(&nHowMany, sizeof nHowMany);
			_pDataFile->Write(pInjury, sizeof CInjury);
		}
		else
		{
            _pDataFile->Write(&nNull, sizeof nNull);
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryList::AddOne
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CInjuryList::AddOne()
{
	ushort Id = wNOTFOUND;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (static_cast<CInjury*>(GetAt(LoopCount))->IsEmpty() == true)
		{
			Id = LoopCount;
			break;
		}
	}
	if (Id == wNOTFOUND)
	{
		return Add(new CInjury());
	}
	return Id;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryList::DoFindPlayer
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Returns:   	CInjury*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CInjury* CInjuryList::DoFindPlayer(CPlayer* _Player)
{
	CInjury* pInjury = null;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (static_cast<CInjury*>(GetAt(LoopCount))->GetPlayer() == _Player)
		{
			pInjury = static_cast<CInjury*>(GetAt(LoopCount));
			break;
		}
	}
	return pInjury;
}


/*------------------------------------------------------------------------------
	Method:   	CInjuryList::IsPlayerInjured
	Access:    	public 
	Parameter: 	CPlayer * _Player
	Returns:   	const bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
const bool CInjuryList::IsPlayerInjured(CPlayer* _Player)
{
	bool bRet  = false;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (static_cast<CInjury*>(GetAt(LoopCount))->GetPlayer() == _Player)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
