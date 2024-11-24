#ifndef __INJURY_H__
#define __INJURY_H__


/*
    Class - CInjury


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */



class CInjury
{

public:
	CInjury();
    CInjury& operator = (const CInjury& p);

    // Methods
	void					AverageImproveMent();
	void					BadImproveMent();
    bool                    CarriedOff();
    CString		            CapitalDescriptionStr();
	void					DoCreateInjury(CPlayer* _Player, CClub& _Club, CInjuryType::eInjurySeverity _Severity = CInjuryType::MAXINJURYSEVERITIES, bool bTraining = false);
	void					DoCreateEvent(CPlayer* _Player);
	void					DoCreateEventInjury(CPlayer* _Player, CClub* _Club);
	void					DoCreateTrainingInjury(CPlayer* _Player, CClub& _Club, const CInjuryType::eInjurySeverity _Severity);
    bool                    DailyImprove(CClub& _Club);
	CString					DescriptionStr();
    CString		            DetailsAndTimeOutStr();
	void					DoResetVariables();
	CPlayer*				GetPlayer();
	sbyte					GetImprovement() const;
	byte					GetHowLongInjured() const;
	byte					GetHowLongHasBeenInjured() const;
	byte					GetSeverity() const;
	byte					GetMatchPerformanceEffect() const;
	int						GetEventType(const byte _Value);
	void					GoodImproveMent();
    byte					HowLongRemainingUnableToPlay();
	byte					HowLongHasBeenUnableToPlay();
	CString					HowLongHasBeenUnableToPlayStr();
	CString					InjuryLengthStr();
	bool					IsRandomPlayerEvent();
	bool					IsEmpty();
    byte					MatchPerformanceEffect();
	void					SetMatchPerformanceEffect(const byte _Value);
  	float          			SkillMultiplier();
	CString					TotalInjuryLengthStr();
    int                     Text();
	void					VeryGoodImproveMent();
	void					VeryBadImproveMent();

	// Variables
protected:
	ushort					m_PlayerID;
	byte					m_Severity;
	byte					m_MatchPerformanceEffect;
    byte					m_HowLongRemainingInjured;
	byte					m_HowLongHasBeenInjured;
	bool					m_RandomPlayerEvent;
	sbyte					m_Improvement;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------
class CInjuryList : public CPointerArray<CInjury>
{
public:
    CInjuryList(); ;  // Constructor

        // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

        // Methods
	ushort					AddOne();
	CInjury* DoFindPlayer(CPlayer* _Player);
	const bool IsPlayerInjured(CPlayer* _Player);
};
#endif
