
 /*

	SoccerMatchPlayer.cpp: implementation of the CSoccerMatchPlayer class.


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CSoccerMatchPlayer::DoScoredGoal
	Access:    	public 
	Parameter: 	byte _Time
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerMatchPlayer::DoScoredGoal(byte _Time)
{
	m_Goals++;
	m_GoalTimes.Add(_Time);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerMatchPlayer::CSoccerMatchPlayer
	Access:    	public 
	Parameter: 	CPlayer * pPtr
	Parameter: 	const ushort nSquadNumber
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSoccerMatchPlayer::CSoccerMatchPlayer(CPlayer* pPtr, const ushort nSquadNumber)
{
	OnInitialise(pPtr, nSquadNumber);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerMatchPlayer::OnInitialise
	Access:    	public 
	Parameter: 	CPlayer * pPtr
	Parameter: 	const ushort SquadNumber
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerMatchPlayer::OnInitialise(CPlayer* pPtr, const ushort SquadNumber)
{
    m_pPlayer = pPtr;
    m_SquadNumber = SquadNumber;
    //m_Energy.SetSizeFillWithValue(MAXIMUMMATCHLENGTH, 0);                  // 1 entry per minute
#ifdef MOTIVATEPLAYERSINMATCH
	m_MotivateFlag = 0;
#endif
	m_CurrentSkill = 0;
	m_Goals = 0;
	m_ShotsOnTarget = 0;
	m_ShotsOffTarget = 0;
	//m_FreeKicks = 0;
	//m_Fouls = 0;
}


 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayer::DoMinute(int time)
{
#ifdef MOTIVATEPLAYERSINMATCH
	if (m_MotivateFlag > 0)
	{
		m_MotivateFlag--;
	}
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerMatchPlayer::IncShotsOnTarget
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerMatchPlayer::IncShotsOnTarget()
{
	m_ShotsOnTarget++;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerMatchPlayer::IncShotsOffTarget
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerMatchPlayer::IncShotsOffTarget()
{
	m_ShotsOffTarget++;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerMatchPlayer::UpdateHowLongPlayed
	Access:    	public 
	Parameter: 	CPlayerPerformance * _PlayerPerformance
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerMatchPlayer::UpdateHowLongPlayed(CPlayerPerformance* _PlayerPerformance)
{
	byte HowLong = (byte)_PlayerPerformance->HowLongPlayed();
	m_pPlayer->SetHowLongPlayedLastMatch(HowLong);
}


#ifdef MOTIVATEPLAYERSINMATCH
 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayer::Motivate(int type, int time, int matchlength)
{
	CNumericArray<byte> SkillEffect;
	if (m_MotivateFlag == 0)
	{
		// Can't motivate again until this one wears off
		m_MotivateFlag = SkillEffect.GetSize();
		// Adjust his skill for the duration
		for (uint n = 0; n < SkillEffect.GetSize(); n++)
		{
			if ((n + time) < matchlength)
			{

			}
		}
	}
}
#endif

////////////////////////////////////////////////////////////////////////////////////////////////////////


 //-----------------------------------------------------------------------------------------------------
CSoccerMatchPlayerList::CSoccerMatchPlayerList()
{
    SetSize(PLAYERSINTEAM + MAXSUBS);
    for (uint num = 0; num <  PLAYERSINTEAM + MAXSUBS; num ++)
	{
        SetAt(num, new CSoccerMatchPlayer());
    }
}

 //-----------------------------------------------------------------------------------------------------
CSoccerMatchPlayerList::CSoccerMatchPlayerList(CClub* ClubPtr, int NumSubs)
{
    SetSize(PLAYERSINTEAM + NumSubs);
    for (uint num = 0; num <  PLAYERSINTEAM + NumSubs; num ++)
	{
        SetAt(num, new CSoccerMatchPlayer(&ClubPtr->GetPlayer(num), num));
    }
	DoResetVariables();
}


 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayerList::DoResetVariables()
{
    //m_Energy.SetSizeFillWithValue(MAXIMUMMATCHLENGTH, 0);
    //m_DefenceEnergy.SetSizeFillWithValue(MAXIMUMMATCHLENGTH, 0);
    //m_MidfieldEnergy.SetSizeFillWithValue(MAXIMUMMATCHLENGTH, 0);
    //m_AttackEnergy.SetSizeFillWithValue(MAXIMUMMATCHLENGTH, 0);
#ifdef MOTIVATEPLAYERSINMATCH
	m_MotivateZoneFlag[0] = m_MotivateZoneFlag[1] = m_MotivateZoneFlag[2] = 0;
#endif
}


 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayerList::OnInitialise(CClub& _Club, int _NumSubs)
{
	EXT_ASSERT(_NumSubs <= MAXSUBS);
    for (uint num = 0; num <  PLAYERSINTEAM + _NumSubs; num ++)
	{
        static_cast<CSoccerMatchPlayer*>(GetAt(num))->OnInitialise(&_Club.GetPlayer(num), num);
    }
	DoResetVariables();
}

 //-----------------------------------------------------------------------------------------------------
CSoccerMatchPlayerList::~CSoccerMatchPlayerList()
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        delete GetAt(ListPos);
    }
}


 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayerList::UpdateHowLongPlayed(CPlayerPerformanceList* pPtr)
{
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        static_cast<CSoccerMatchPlayer*>(GetAt(ListPos))->UpdateHowLongPlayed(pPtr->GetAt(ListPos));
    }
}


 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayerList::DoMinute(int time)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
#ifdef MOTIVATEPLAYERSINMATCH
	for (uint n = 0; n < 3; n++)
	{
		if (m_MotivateZoneFlag[n] > 0)
		{
			m_MotivateZoneFlag[n]--;
		}
	}
    for (uint num = 0; num <  PLAYERSINTEAM; num ++)
	{
        static_cast<CSoccerMatchPlayer*>(GetAt(num))->DoMinute(time);
    }
#endif
}


 //-----------------------------------------------------------------------------------------------------
/*void CSoccerMatchPlayerList::SetGoalkeeperCurrentSkill(byte x)
{
    GetAt(0)->SetCurrentSkill(x);
}


 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayerList::ResetCurrentSkills()
{
    m_DefenceCurrentSkill = 0;
    m_MidfieldCurrentSkill = 0;
    m_AttackCurrentSkill = 0;
}


 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayerList::SetCurrentSkills(byte a, byte b, byte c, byte StartPosn)
{
    m_DefenceCurrentSkill = a;
    m_MidfieldCurrentSkill = b;
    m_AttackCurrentSkill = c;
    m_DefenceSkill.FillWithValue(m_DefenceCurrentSkill, StartPosn);
    m_MidfieldSkill.FillWithValue(m_MidfieldCurrentSkill, StartPosn);
    m_AttackSkill.FillWithValue(m_AttackCurrentSkill, StartPosn);
} */


#ifdef MOTIVATEPLAYERSINMATCH
 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayerList::MotivatePlayer(int type, int time, int matchlength, int who)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
    static_cast<CSoccerMatchPlayer*>(GetAt(who))->Motivate(type, time, matchlength);
}


 //-----------------------------------------------------------------------------------------------------
void CSoccerMatchPlayerList::MotivateZone(int type, int time, int matchlength, int zone)
{
	EXT_ASSERT(time < MAXIMUMMATCHLENGTH);
	CNumericArray<byte> SkillEffect;
	if (m_MotivateZoneFlag[zone] == 0)
	{
		// Can't motivate again until this one wears off
		m_MotivateZoneFlag[zone] = SkillEffect.GetSize();
		// Adjust his skill for the duration
		for (uint n = 0; n < SkillEffect.GetSize(); n++)
		{
			if ((n + time) < matchlength)
			{
				switch(zone)
				{
					case 0:
						break;
					case 1:
						break;
					case 2:
						break;
					default:
						EXT_ASSERT(false);
						break;
				}
			}
		}
	}
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CSoccerMatchPlayerList::MakeSubstitution
	Access:    	public 
	Parameter: 	int _When
	Parameter: 	int _Index
	Parameter: 	int _Swp1
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerMatchPlayerList::MakeSubstitution(int _When, int _Index, int _Swp1)
{
	Swap(_Index, _Swp1);
    int Num = static_cast<CSoccerMatchPlayer*>(GetAt(_Index))->GetNumber();
    static_cast<CSoccerMatchPlayer*>(GetAt(_Index))->SetNumber(static_cast<CSoccerMatchPlayer*>(GetAt(_Swp1))->GetNumber());
    static_cast<CSoccerMatchPlayer*>(GetAt(_Swp1))->SetNumber(Num);
}
