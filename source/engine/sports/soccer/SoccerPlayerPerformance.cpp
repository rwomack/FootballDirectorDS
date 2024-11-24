
 /*

        SoccerPlayerPerformance.cpp: implementation of the CSoccerPlayerPerformance class.

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


    const sbyte EXCELLENTSHOT = 6;
    const sbyte GOODSHOT = 4;
    const sbyte AVERAGESHOT = 2;
    const sbyte POORSHOT = -2;
    const sbyte BADSHOT = -4;
    const sbyte APPALLINGSHOT = -6;

    const sbyte EXCELLENTSAVE = 6;
    const sbyte GOODSAVE = 4;
    const sbyte AVERAGESAVE = 2;
    const sbyte POORSAVE = -2;
    const sbyte BADSAVE = -4;
    const sbyte APPALLINGSAVE = -6;

    const sbyte AVERAGEASSIST = 2;
    const sbyte GOODASSIST = 4;
    const sbyte EXCELLENTASSIST = 6;

    const sbyte AVERAGEASSISTDEFEND = -2;
    const sbyte GOODASSISTDEFEND = -4;
    const sbyte EXCELLENTASSISTDEFEND = -6;

    const sbyte NODEFEND = 0;
    const sbyte AVERAGEDEFEND = 2;
    const sbyte GOODDEFEND = 4;
    const sbyte EXCELLENTDEFEND = 6;
    const sbyte POORDEFEND = -2;
    const sbyte BADDEFEND = -4;
    const sbyte APPALLINGDEFEND = -6;

    const sbyte GOODSHOTUPPERLIMIT = 50;
    const sbyte AVERAGESHOTUPPERLIMIT = 25;
    const sbyte POORSHOTUPPERLIMIT = -1;
    const sbyte BADSHOTUPPERLIMIT = -25;
    const sbyte APPALLINGSHOTUPPERLIMIT = -50;

    const sbyte GOODSAVEUPPERLIMIT = -50;
    const sbyte AVERAGESAVEUPPERLIMIT = -25;
    const sbyte POORSAVEUPPERLIMIT = 1;
    const sbyte BADSAVEUPPERLIMIT = 25;
    const sbyte APPALLINGSAVEUPPERLIMIT = 50;


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::CSoccerPlayerPerformance
	Access:    	public 
	Parameter: 	byte Value
	Parameter: 	byte MatchLength
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerPerformance::CPlayerPerformance(byte Value, byte MatchLength)
{
	m_Performance = 0;
    m_PerformanceByMinute.SetSize(MatchLength, Value);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::~CSoccerPlayerPerformance
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerPerformance::~CPlayerPerformance()
{
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::GetOverallRating
	Access:    	public 
	Parameter: 	byte PeriodLength
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerPerformance::GetOverallRating(int _PeriodLength)
{
	return min(m_PerformanceByMinute.AverageNonZeroValue(_PeriodLength), 100);
}



/*------------------------------------------------------------------------------
	Method:   	CPlayerPerformance::HowLongPlayed
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CPlayerPerformance::HowLongPlayed()
{
	return m_PerformanceByMinute.NumberNonZero();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::Init
	Access:    	public 
	Parameter: 	byte Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformance::DoInitialise(byte Value)
{
	EXT_ASSERT(Value < 100);
	m_Performance = 0;
    m_Skill = Value;
	m_PerformanceByMinute.FillWithValue(Value);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::InitExtraTime
	Access:    	public 
	Parameter: 	byte ExtraTimeLength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformance::DoInitialiseExtraTime(byte _ExtraTimeLength)
{
    m_PerformanceByMinute.FillWithValue(m_PerformanceByMinute[89], 90);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::AttemptGoal
	Access:    	public 
	Parameter: 	sbyte ShotSaveSkillDifference
	Parameter: 	byte when
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CPlayerPerformance::AttemptGoal(sbyte _ShotSaveSkillDifference, byte _When)
{
    sbyte Amount = EXCELLENTSHOT;
    if (_ShotSaveSkillDifference > BADSHOTUPPERLIMIT)
	{
        Amount = APPALLINGSHOT;
    }
    if (IsBetween(_ShotSaveSkillDifference, BADSHOTUPPERLIMIT, static_cast<sbyte>(POORSHOTUPPERLIMIT + 1)))
	{
        Amount = BADSHOT;
    }
    if (IsBetween(_ShotSaveSkillDifference, POORSHOTUPPERLIMIT, static_cast<sbyte>(AVERAGESHOTUPPERLIMIT + 1)))
	{
        Amount = POORSHOT;
    }
    if (IsBetween(_ShotSaveSkillDifference, AVERAGESHOTUPPERLIMIT, static_cast<sbyte>(GOODSHOTUPPERLIMIT + 1)))
	{
        Amount = AVERAGESHOT;
    }
    if (_ShotSaveSkillDifference > GOODSHOTUPPERLIMIT)
	{
        Amount = GOODSHOT;
    }

	Raise(_When, Amount);
	return Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::AssistedShot
	Access:    	public 
	Parameter: 	sbyte ShotSaveSkillDifference
	Parameter: 	byte when
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CPlayerPerformance::AssistedShot(sbyte _ShotSaveSkillDifference, byte _When)
{
    sbyte Amount;
    switch(_ShotSaveSkillDifference)
	{
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            Amount = AVERAGEASSIST;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            Amount = GOODASSIST;
            break;
        case 9:
            Amount = EXCELLENTASSIST;
            break;
        default:
            EXT_ASSERT(false);
			return 0;
    }
	Raise(_When, Amount);
	return Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::ConcededShot
	Access:    	public 
	Parameter: 	sbyte ShotSaveSkillDifference
	Parameter: 	byte TheTime
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CPlayerPerformance::DoConcededShot(const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
    sbyte Amount;
    switch(_ShotSaveSkillDifference)
	{
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
            Amount = POORDEFEND;
            break;
        case 5:
        case 6:
        case 7:
        case 8:
            Amount = BADDEFEND;
            break;
        case 9:
            Amount = APPALLINGDEFEND;
            break;
        default:
            EXT_ASSERT(false);
			return 0;
    }
	Raise(_TheTime, Amount);
	return Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::DefendAttempt
	Access:    	public 
	Parameter: 	byte ShotSaveSkillDifference
	Parameter: 	byte TheTime
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CPlayerPerformance::DefendAttempt(byte ShotSaveSkillDifference, byte TheTime)
{
    sbyte Amount;
    switch(ShotSaveSkillDifference)
	{
        case 0:
        case 1:
        case 2:
        case 3:
            return 0;
        case 4:
        case 5:
        case 6:
            Amount = AVERAGEDEFEND;
            break;
        case 7:
        case 8:
            Amount = GOODDEFEND;
            break;
        case 9:
            Amount = EXCELLENTDEFEND;
            break;
        default:
            EXT_ASSERT(false);
			return 0;
    }
	Raise(TheTime, Amount);
	return Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerPerformance::DoAttemptSave
	Access:    	public 
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CPlayerPerformance::DoAttemptSave(const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
    sbyte Amount = EXCELLENTSAVE;
    if (_ShotSaveSkillDifference > BADSAVEUPPERLIMIT)
	{
        Amount = APPALLINGSAVE;
    }
    if (IsBetween(_ShotSaveSkillDifference, BADSAVEUPPERLIMIT, static_cast<sbyte>(POORSAVEUPPERLIMIT + 1)))
	{
        Amount = BADSAVE;
    }
    if (IsBetween(_ShotSaveSkillDifference, POORSAVEUPPERLIMIT, static_cast<sbyte>(AVERAGESAVEUPPERLIMIT + 1)))
	{
        Amount = POORSAVE;
    }
    if (IsBetween(_ShotSaveSkillDifference, AVERAGESAVEUPPERLIMIT, static_cast<sbyte>(GOODSAVEUPPERLIMIT + 1)))
	{
        Amount = AVERAGESAVE;
    }
    if (_ShotSaveSkillDifference < GOODSAVEUPPERLIMIT)
	{
        Amount = GOODSAVE;
    }
	Raise(_TheTime, Amount);
	return Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::SetCurrentSkill
	Access:    	public 
	Parameter: 	byte x
	Parameter: 	byte when
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformance::SetCurrentSkill(byte _Value, byte _Time)
{
	EXT_ASSERT(_Value < 100);
	m_Skill = _Value;
	sbyte CurrentRating = m_Skill + m_Performance;
	CurrentRating = max(MINPLAYSKILL, CurrentRating);
    m_PerformanceByMinute.FillWithValue(min(CurrentRating, MAXPLAYSKILL), _Time);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::Raise
	Access:    	public 
	Parameter: 	int When
	Parameter: 	sbyte HowMuch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformance::Raise(int _When, sbyte _HowMuch)
{
	EXT_ASSERT(_When < m_PerformanceByMinute.GetSize());
	m_Performance += _HowMuch;
	if (_HowMuch > 0)
	{
		if ((m_PerformanceByMinute[_When] + _HowMuch) > MAXPLAYSKILL)
		{
			_HowMuch = MAXPLAYSKILL - m_PerformanceByMinute[_When];
		}
		if (_HowMuch > 0)
		{
			m_PerformanceByMinute.RaiseValue(_HowMuch, _When);
		}
	}
	else
	{
		EXT_ASSERT((m_PerformanceByMinute[_When]  - abs(_HowMuch) ) < 100);
		if (static_cast <sbyte>(m_PerformanceByMinute[_When]  - abs(_HowMuch)) > MINPLAYSKILL)
		{
			m_PerformanceByMinute.LowerValue(abs(_HowMuch), _When);
		}
		else
		{
			m_PerformanceByMinute.FillWithValue(MINPLAYSKILL, _When);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::GetSkill
	Access:    	public 
	Parameter: 	byte time
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerPerformance::GetSkill(byte time)
{
	EXT_ASSERT(time < m_PerformanceByMinute.GetSize());
	return m_PerformanceByMinute[time];
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::ClearSkillFrom
	Access:    	public 
	Parameter: 	byte TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformance::DoClearSkillFrom(byte _TheTime)
{
    m_PerformanceByMinute.FillWithValue(0, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::ClearSkillTo
	Access:    	public 
	Parameter: 	byte TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformance::DoClearSkillTo(byte _TheTime)
{
	//								Val, first, howmany
    m_PerformanceByMinute.FillWithValue(0, 0, _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::Write
	Access:    	public 
	Parameter: 	CDataFile* File
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformance::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    m_PerformanceByMinute.Write(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformance::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformance::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    m_PerformanceByMinute.Read(_pDataFile);
}
//////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::CSoccerPlayerPerformanceList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerPerformanceList::CPlayerPerformanceList()
{
    m_GoalSkill.SetSize(MAXIMUMMATCHLENGTH, 0);
    m_DefenceSkill.SetSize(MAXIMUMMATCHLENGTH, 0);
    m_MidfieldSkill.SetSize(MAXIMUMMATCHLENGTH, 0);
    m_AttackSkill.SetSize(MAXIMUMMATCHLENGTH, 0);
	SetSize(PLAYERSINTEAM + MAXSUBS);
    for (uint ListPos = 0; ListPos < PLAYERSINTEAM + MAXSUBS; ListPos ++)
	{
		SetAt(ListPos, FD_ENGINE_NEW CPlayerPerformance(0, MAXIMUMMATCHLENGTH));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::~CSoccerPlayerPerformanceList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerPerformanceList::~CPlayerPerformanceList()
{
	RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	ushort HowMany = GetSize();
    _pDataFile->Write(&HowMany, sizeof(HowMany));  // List size
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        GetAt(ListPos)->Write(_pDataFile);
	}
 }


 //---------------------------------------------------------------------------------------
void CPlayerPerformanceList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	ushort HowMany;
	RemoveList();
    _pDataFile->Read(&HowMany, sizeof(HowMany));
    for (uint ListPos = 0; ListPos < HowMany; ListPos ++)
	{
        SetAtGrow(ListPos, FD_ENGINE_NEW CPlayerPerformance());
        GetAt(ListPos)->Read(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::Init
	Access:    	public 
	Parameter: 	CClub* apClub
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::DoInitialise(CClub* _Club)
{
	CPlayer* pPlayer;
	m_GoalPerformance = 0;
	m_DefencePerformance = 0;
	m_MidfieldPerformance = 0;
	m_AttackPerformance = 0;

	int ListPos;
	// Team start with rating
    for (ListPos = 0; ListPos < PLAYERSINTEAM; ListPos ++)
	{
		if (_Club->GetFirstTeamSquad().GetSquad(ListPos) != NOPLAYER)
		{
			pPlayer = WorldData().GetPlayer(_Club->GetFirstTeamSquad().GetSquad(ListPos));
			GetAt(ListPos)->DoInitialise(pPlayer->GetRating());
		}
	}

	// Subs start with 0
    for (ListPos = PLAYERSINTEAM; ListPos < GetSize(); ListPos ++)
	{
		if (_Club->GetFirstTeamSquad().GetSquad(ListPos) != NOPLAYER)
		{
			pPlayer = WorldData().GetPlayer(_Club->GetFirstTeamSquad().GetSquad(ListPos));
			GetAt(ListPos)->DoInitialise(0);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::InitExtraTime
	Access:    	public 
	Parameter: 	byte ExtraTimeLength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::DoInitialiseExtraTime(byte _ExtraTimeLength)
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        GetAt(ListPos)->DoInitialiseExtraTime(_ExtraTimeLength);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::AttemptGoal
	Access:    	public 
	Parameter: 	int who
	Parameter: 	sbyte ShotSaveSkillDifference
	Parameter: 	byte when
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::AttemptGoal(int _Who, sbyte _ShotSaveSkillDifference, byte _When)
{
	AdjustAttackPerformance(GetAt(_Who)->AttemptGoal(_ShotSaveSkillDifference, _When), _When);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::AssistedShot
	Access:    	public 
	Parameter: 	int who
	Parameter: 	sbyte ShotSaveSkillDifference
	Parameter: 	byte when
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::AssistedShot(int _Who, sbyte _ShotSaveSkillDifference, byte _When)
{
    AdjustAttackPerformance(GetAt(_Who)->AssistedShot(_ShotSaveSkillDifference, _When), _When);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::ConcededShot
	Access:    	public 
	Parameter: 	int who
	Parameter: 	sbyte ShotSaveSkillDifference
	Parameter: 	byte when
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::DoConcededShot(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
    AdjustDefencePerformance(GetAt(_PlayerSquadID)->DoConcededShot(_ShotSaveSkillDifference, _TheTime), _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerPerformanceList::DoAttemptSave
	Access:    	public 
	Parameter: 	const ushort _PlayerSquadID
	Parameter: 	const sbyte _ShotSaveSkillDifference
	Parameter: 	const byte _TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::DoAttemptSave(const ushort _PlayerSquadID, const sbyte _ShotSaveSkillDifference, const byte _TheTime)
{
    AdjustGoalPerformance(GetAt(_PlayerSquadID)->DoAttemptSave(_ShotSaveSkillDifference, _TheTime), _TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::DefendAttempt
	Access:    	public 
	Parameter: 	int who
	Parameter: 	byte ShotSaveSkillDifference
	Parameter: 	byte when
	Parameter: 	bool WasMidfielder
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::DoDefendAttempt(int PlayerNum, byte _ShotSaveSkillDifference, byte _TheTime, bool _WasMidfielder)
{
	if (_WasMidfielder == false)
	{
		AdjustDefencePerformance(GetAt(PlayerNum)->DefendAttempt(_ShotSaveSkillDifference, _TheTime), _TheTime);
	}
	else
	{
		AdjustMidfieldPerformance(GetAt(PlayerNum)->DefendAttempt(_ShotSaveSkillDifference, _TheTime), _TheTime);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerPerformanceList::AdjustGoalPerformance
	Access:    	public 
	Parameter: 	int _Performance
	Parameter: 	byte _When
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::AdjustGoalPerformance(int _Performance, byte _When)
{
	m_GoalPerformance += _Performance;
    m_GoalSkill.RaiseValue(_Performance, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerPerformanceList::AdjustDefencePerformance
	Access:    	public 
	Parameter: 	int _Performance
	Parameter: 	byte _When
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::AdjustDefencePerformance(int _Performance, byte _When)
{
	m_DefencePerformance += _Performance;
    m_DefenceSkill.RaiseValue(_Performance, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::AdjustMidfieldPerformance
	Access:    	public 
	Parameter: 	int Performance
	Parameter: 	byte when
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::AdjustMidfieldPerformance(int Performance, byte when)
{
	m_MidfieldPerformance += Performance;
    m_MidfieldSkill.RaiseValue(Performance, when);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::AdjustAttackPerformance
	Access:    	public 
	Parameter: 	int Performance
	Parameter: 	byte when
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::AdjustAttackPerformance(int _Performance, byte _When)
{
	m_AttackPerformance += _Performance;
    m_AttackSkill.RaiseValue(_Performance, _When);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::AttackSkill
	Access:    	public 
	Parameter: 	const int time
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerPerformanceList::AttackSkill(const int time)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
	return m_AttackSkill[time];
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::MidfieldSkill
	Access:    	public 
	Parameter: 	const int time
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerPerformanceList::MidfieldSkill(const int time)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
	return m_MidfieldSkill[time];
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::DefenceSkill
	Access:    	public 
	Parameter: 	int time
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerPerformanceList::DefenceSkill(int time)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
	return m_DefenceSkill[time];
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::SetPlayerCurrentSkill
	Access:    	public 
	Parameter: 	int _SquadNumber
	Parameter: 	byte _Skill
	Parameter: 	byte when
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::SetPlayerCurrentSkill(int _SquadNumber, byte _Skill, byte _Time)
{
    GetAt(_SquadNumber)->SetCurrentSkill(_Skill, _Time);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::GetPlayerSkill
	Access:    	public 
	Parameter: 	int who
	Parameter: 	byte time
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerPerformanceList::GetPlayerSkill(const ushort _SquadNumber, const byte _TheTime)
{
    return GetAt(_SquadNumber)->GetSkill(_TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::ClearSkillFrom
	Access:    	public 
	Parameter: 	int who
	Parameter: 	byte TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::DoClearSkillFrom(const ushort _SquadNumber, const byte _TheTime)
{
    GetAt(_SquadNumber)->DoClearSkillFrom(_TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::ClearSkillTo
	Access:    	public 
	Parameter: 	int who
	Parameter: 	byte TheTime
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::DoClearSkillTo(const ushort _SquadNumber, const byte TheTime)
{
    GetAt(_SquadNumber)->DoClearSkillTo(TheTime);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::GetOverallRating
	Access:    	public 
	Parameter: 	int who
	Parameter: 	byte PeriodLength
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerPerformanceList::GetOverallRating(const ushort _SquadNumber, const byte PeriodLength)
{
    return GetAt(_SquadNumber)->GetOverallRating(PeriodLength);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerPerformanceList::SetCurrentSkills
	Access:    	public 
	Parameter: 	byte _GoalSkill
	Parameter: 	int _DefSkill
	Parameter: 	int _MidSkill
	Parameter: 	int _AttSkill
	Parameter: 	int _StartPosn
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::SetCurrentSkills(byte _GoalSkill, int _DefSkill, int _MidSkill, int _AttSkill, int _StartPosn)
{
    m_GoalCurrentSkill = _GoalSkill;
    m_DefenceCurrentSkill = _DefSkill;
    m_MidfieldCurrentSkill = _MidSkill;
    m_AttackCurrentSkill = _AttSkill;
    m_GoalSkill.FillToEndWithValue(m_GoalCurrentSkill + m_GoalPerformance, _StartPosn);
    m_DefenceSkill.FillToEndWithValue(m_DefenceCurrentSkill + m_DefencePerformance, _StartPosn);
    m_MidfieldSkill.FillToEndWithValue(m_MidfieldCurrentSkill + m_MidfieldPerformance, _StartPosn);
    m_AttackSkill.FillToEndWithValue(m_AttackCurrentSkill + m_AttackPerformance, _StartPosn);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::Skill
	Access:    	public 
	Parameter: 	int time
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerPerformanceList::Skill(int time)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
    int Skill = (m_GoalSkill[time] + (m_DefenceSkill[time] / 4) + (m_MidfieldSkill[time] / 3)+ (m_AttackSkill[time] / 3)) / 4;
	return Skill;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::GoalSkill
	Access:    	public 
	Parameter: 	int time
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPlayerPerformanceList::GoalSkill(int time)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
	return m_GoalSkill[time];
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::SetGoalSkill
	Access:    	public 
	Parameter: 	int time
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::SetGoalSkill(int time)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
	m_GoalSkill.SetAt(time, GetAt(0)->GetSkill(time));
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerPerformanceList::SetGoalSkill
	Access:    	public 
	Parameter: 	int time
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerPerformanceList::SetGoalSkill(int time, const byte x)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
	m_GoalSkill.SetAt(time, x);
}
