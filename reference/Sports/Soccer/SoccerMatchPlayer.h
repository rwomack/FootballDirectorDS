

/*

    SoccerMatchPlayer.h: interface for the CSoccerMatchPlayer class.


	Football Director

	(c) Rw Software 1994 - 2008

   ---------------------------------------------------------------
 */


#if !defined(AFX_SOCCERMATCHPLAYER_H__3F41B1A2_3EF3_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_SOCCERMATCHPLAYER_H__3F41B1A2_3EF3_11D1_A3F5_00001C303EC4__INCLUDED_


	class CPlayer;
	class CPlayerPerformance;
	class CPlayerPerformanceList;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*	
class CSoccerMatchPlayerAttribute
{
public:
    const byte      AverageValue(int x)                  
	{ 
		return m_Attribute.AverageValue(0, x); 
	}
    const byte      PeakValue()                      
	{ 
		return m_Attribute.HighestValue(); 
	}
	
    // Variables
private:
    CNumericArray<byte>     m_Attribute;                  // 1 entry per minute
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CSoccerMatchPlayerEnergy : public CSoccerMatchPlayerAttribute
{
public:
    const byte      Energy(int time)                      
	{ 
		return CSoccerMatchPlayerAttribute::m_Attribute.GetAt(time); 
	}
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CSoccerMatchPlayerSkill : public CSoccerMatchPlayerAttribute
{
public:
	void					ReCalculateSkill(byte StartPosn)		
	{ 
		CSoccerMatchPlayerAttribute::m_Attribute.FillWithRandomValue(m_CurrentSkill, 5, MAXPLAYSKILL, StartPosn); 
	}
	void					NotPlaying(byte StartPosn)				
	{ 
		CSoccerMatchPlayerAttribute::m_Attribute.FillWithValue(0, StartPosn); 
	}
};*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CSoccerMatchPlayer
{
    // Variables
private:
	CPlayer*				m_pPlayer;
	CNumericArray<byte>	m_GoalTimes;                 // 1 entry per goal
	ushort					m_SquadNumber;
	byte					m_Goals;
	byte					m_ShotsOnTarget;
	byte					m_ShotsOffTarget;
	//byte					m_Fouls;
	//byte					m_FreeKicks;
	byte					m_CurrentSkill;
#ifdef MOTIVATEPLAYERSINMATCH
	byte					m_MotivateFlag;
#endif

public:
    CSoccerMatchPlayer() {};
    CSoccerMatchPlayer(CPlayer* pPtr, const ushort SquadNumber);

	void					OnInitialise(CPlayer* pPtr, const ushort SquadNumber);
    void                    DoMinute(int time);
    const CPlayer*			pPlayer()
	{
		return m_pPlayer;
	}
    CString					PlayerName()
	{
		return m_pPlayer->GetName();
	}
    CString				    PlayerSurname()
	{
		return m_pPlayer->GetSurname();
	}
    CString				    PositionStr()
	{
		return m_pPlayer->GetSkills().PositionStr();
	}
    byte				Position()
	{
		return m_pPlayer->GetSkills().GetPosition();
	}
    ushort			GetNumber()
	{
		return m_SquadNumber;
	}
    void					SetCurrentSkill(byte _Value)
	{
		EXT_ASSERT(_Value < 100); 
		m_CurrentSkill = _Value;
	}
    byte				Goals()
	{
		return m_Goals;
	}
    byte				GoalsSoFar()
	{
		return m_pPlayer->GetNumberGoalsThisSeason();
	}
    byte				ShotsOnTarget()
	{
		return m_ShotsOnTarget;
	}
    byte				ShotsOffTarget()
	{
		return m_ShotsOffTarget;
	}
	void					DoScoredGoal(byte _Time);
	void UpdateHowLongPlayed(CPlayerPerformance* _PlayerPerformance);
	void					IncShotsOffTarget();
	void					IncShotsOnTarget();
	void SetNumber(ushort _SquadNumber)
	{
		m_SquadNumber = _SquadNumber;
	}
    //const byte				FreeKicks()							{ return m_FreeKicks; }
    //const byte				Fouls()								{ return m_Fouls; }
#ifdef MOTIVATEPLAYERSINMATCH
	void					Motivate(int type, int time, int matchlength);
#endif
};


//----------------------------------------------------------------------------
class CSoccerMatchPlayerList : public CPointerArray<CSoccerMatchPlayer>
{
public:
    CSoccerMatchPlayerList();								// Constructor
    CSoccerMatchPlayerList(CClub* ClubPtr, int NumSubs);   // Constructor
    ~CSoccerMatchPlayerList();                             // Destructor

    void					OnInitialise(CClub& _Club, int _NumSubs);
    void                    DoMinute(int time);
	void					UpdateHowLongPlayed(CPlayerPerformanceList* pPtr);
	void					DoResetVariables();

//     int              Skill(int time)                        { EXT_ASSERT(time < MAXIMUMMATCHLENGTH); return m_Skill.GetAt(time); }
  //   int              DefenceSkill(int time)                 { EXT_ASSERT(time < MAXIMUMMATCHLENGTH); return m_DefenceSkill.GetAt(time); }
    //int              DefenceEnergy(int time)               { EXT_ASSERT(time < MAXIMUMMATCHLENGTH); return m_DefenceEnergy.GetAt(time); }
    // int              MidfieldSkill(int time)                { EXT_ASSERT(time < MAXIMUMMATCHLENGTH); return m_MidfieldSkill.GetAt(time); }
    // int              MidfieldEnergy(int time)              { EXT_ASSERT(time < MAXIMUMMATCHLENGTH); return m_MidfieldEnergy.GetAt(time); }
    // int              AttackSkill(int time)                  { EXT_ASSERT(time < MAXIMUMMATCHLENGTH); return m_AttackSkill.GetAt(time); }
    // int              AttackEnergy(int time)                { EXT_ASSERT(time < MAXIMUMMATCHLENGTH); return m_AttackEnergy.GetAt(time); }
    //void                    SetGoalkeeperCurrentSkill(byte x);
    //void                    SetCurrentSkills(byte a, byte b, byte c, byte StartPosn);
	//void					ResetCurrentSkills();
#ifdef MOTIVATEPLAYERSINMATCH
	void					MotivateZone(int type, int time, int matchlength, int zone);
	void					MotivatePlayer(int type, int time, int matchlength, int who);
#endif
	void MakeSubstitution(int _When, int _Index, int _Swp1);

    // Variables
private:
    //CNumericArray<byte>           m_Skill;
    //CNumericArray<byte>           m_Energy;
    //CNumericArray<byte>           m_DefenceSkill;
    //CNumericArray<byte>           m_DefenceEnergy;
    //CNumericArray<byte>           m_MidfieldSkill;
    //CNumericArray<byte>           m_MidfieldEnergy;
    //CNumericArray<byte>           m_AttackSkill;
    //CNumericArray<byte>           m_AttackEnergy;
    //int						m_DefenceCurrentSkill;
    //int                       m_DefenceCurrentEnergy;
    //int						m_MidfieldCurrentSkill;
    //int                       m_MidfieldCurrentEnergy;
    //int						m_AttackCurrentSkill;
    //int                       m_AttackCurrentEnergy;
#ifdef MOTIVATEPLAYERSINMATCH
	byte					m_MotivateZoneFlag[3];
#endif
};
#endif // !defined(AFX_SOCCERMATCHPLAYER_H__3F41B1A2_3EF3_11D1_A3F5_00001C303EC4__INCLUDED_)
