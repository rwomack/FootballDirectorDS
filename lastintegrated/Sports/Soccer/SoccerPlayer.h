#ifndef __PSKILLS_H__
#define __PSKILLS_H__


/*

    SoccerPlayer Class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */



class CContract;
class CPlayerSquad;
class CPerson;


class CSoccerPlayerTraining	
{
public:
    CSoccerPlayerTraining();
    CSoccerPlayerTraining& operator =(const CSoccerPlayerTraining& rTraining);

    // Methods
	byte		GetBeforeTrainingSkill() const
	{
		return m_BeforeTrainingSkill;
	}
	byte		GetTrainingSkillsPercent() const
	{
		return m_TrainingSkillsPercent;
	}
	byte		GetTrainingPacePercent() const
	{
		return m_TrainingPacePercent;
	}
	byte		GetTrainingStrengthPercent() const
	{
		return m_TrainingStrengthPercent;
	}
	void SetBeforeTrainingSkill(const byte _Value)
	{
		m_BeforeTrainingSkill = _Value;
	}

	void					DoResetVariables();
	byte					GetPersonalFreeTimePercent();
	void					SetTrainingSkillsPercent(const byte _Value);
	void					SetTrainingPacePercent(const byte _Value);
	void					SetTrainingStrengthPercent(const byte _Value);

protected:
	byte					m_BeforeTrainingSkill;
	byte					m_TrainingSkillsPercent;
	byte					m_TrainingPacePercent;
	byte					m_TrainingStrengthPercent;
};
/////////////////////////////////////////////////////////////////////////////

enum ePlayerWhichSkill
{
	SKILL_OVERALL,
	SKILL_HANDLING,
	SKILL_TACKLING,
	SKILL_PASSING,
	SKILL_SHOOTING,
	SKILL_HEADING,
};

class CSoccerPlayer
{
    // Variables
private:
    CPlayerRating           m_Rating;
	double					m_Experience;
	CSoccerPlayerTraining	m_Training;
	// Prevents players fees from changing upon every call
	uint        m_stablePrice;
	uint        m_lastUpdate;
	
	uint					m_Position:7;                     // Player's natural position, bit 7 is best foot
	uint					m_BestFoot:1;
    uint					m_OverallSkill:7;                 // Overall skill
    uint					m_Handling:7;
    uint					m_Tackling:7;

    uint					m_Passing:7;
    uint					m_Shooting:7;
    uint					m_Pace:7;
	uint					m_Strength:7;
	
	uint					m_Heading:7;
    uint					m_Fitness:7;
    uint					m_Energy:7;                      // How quickly will tire during match
	uint					m_Flair:7;
	uint					m_CareerPath:4;

	uint					m_PacePositivePoints:6;
	uint					m_PaceNegativePoints:6;
	uint					m_StrengthPositivePoints:6;


public:
    CSoccerPlayer();
    CSoccerPlayer& operator =(const CSoccerPlayer& Skills);

    ePlayerPosition			GetPosition();
	byte					GetOverallSkill() const;
    byte					GetHandling() const;
    byte					GetTackling() const;
    byte					GetPassing() const;
    byte					GetShooting() const;
    byte					GetPace() const;
    byte					GetRating();
    eBESTFOOT				GetBestFoot();
    byte					GetHeading() const;
    byte					GetStrength() const;
    byte					GetEnergy() const;
    byte					GetFlair() const;
    byte					GetFitness() const;
	double					GetExperience() const;

    void					SetPace(const byte _Value);
    void					SetHeading(const byte _Value);
    void					SetStrength(const byte _Value);
    void					SetEnergy(const byte _Value);
    void					SetFlair(const byte _Value);
    void					SetFitness(const byte _Value);

	void					DoIncrementHandling();
    void					DoIncrementTackling();
    void					DoIncrementPassing();
    void					DoIncrementShooting();
    void					DoIncrementPace();
	CString					GetBestFootStr();

    // Methods
	void 					DoAddPerformance(const byte _Value);
	void 					DoAfterMatchUpdate(const byte _Age, const byte _Performance);
	void					AfterMatchEnergyAndFitnessUpdate(const byte HowLongPlayed);
    void                    DoCalculateOverallSkill();
	int						CalcSkillVal(const byte Age);
	int						CalcSkillValue(const byte Age);
	int						CharacteristicsAgeGroup(byte Age);
	void					DoEnsureInRange();
	byte					DoGenerateRandomSkill(const double aSkill, const byte aMinSkill, const byte aRndSkill);
	void 					DoCheckExperienceForLevelUp(double _CareerPathMultiplier);
	void 					DoCheckExperienceForLevelDown(double _CareerPathMultiplier);
	void 					DoCreate(const byte _Age, ePlayerPosition _Type, const byte _Skill);
	void 					DoCreatePlayer(const byte _Age, CPopularity* _Popularity, const byte _ClubDivStrength, ePlayerPosition _Position);
	void					DoCreateSkills();
	void					DoCreateYouth(const byte _Age, CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength);
    int                     EnergyText();
	double					GetEnergyEffect() const;
    byte					FindType();
	sbyte					GetPerformance();
	byte					GetPositionSkill();
	void					DoGenerateRandomContract(CContract& _ContractOffered, CCountry& _Country, const byte _Age, bool _Bosman);
	bool					HasImproved();
	bool					HasWorsened();
    void        			ImproveSkill(int IncreaseChance);
    uint      				DoCalculateMaximumSignOnFee(CCountry& rCountry, bool Bosman);
    bool        			MaxValue(double Amount, const byte Age, CCountry& rCountry, const byte Temper);
    uint					DoCalculateMaximumTransferFee(const byte Age, CCountry& rCountry, const byte Temper);
	uint					DoCalculateMaximumWage(CCountry& rCountry);
    uint					DoCalculateSignOnFee(CCountry& rCountry, bool Bosman);
    uint					DoCalculateMinimumWage(CCountry& rCountry);
	uint					DoCalculateMinimumTransferFee(const byte _Age, CCountry& _Country, const byte _Temper);
	ePlayerPosition			RandomPosition();
    void                    DoMakeInvincible();
    void                    DoResetVariables();
    void                    MakeVeryStrong();
    void                    DoMakeYoung();
    void                    DoMakeOld();
    void                    DoNewSeason();
    int                     PosnAbbr();
    int                     PositionTextID();
    CString		            PositionStr();
	int						PosnLongAbbr();
	ePlayerPosition			DoGenerateRandomPosition(CCountry& _Country);
	int						DoGenerateRandomContractLength(const byte _Age);
	uint					DoGenerateRandomWage(CCountry& _Country);
	uint					DoGenerateRandomSignOnFee(CCountry& _Country, bool _Bosman);
	uint					DoGenerateRandomTransferFee(const byte _Age, CCountry& _Country, const byte _Temper);
    void                    DoInitialiseFromResource(CBinaryReader& _Resource);
	void					DoReduceActualSkill(int _IncreaseChance);
	void					ReduceSkill(const byte _Age, int _IncreaseChance);
	void					DoRejuvenateEnergy(const byte _Age);
	double					SkillRatingMultiplier();
	uint					SuggestedTransferFee(const byte _Age, const byte _Temper, const byte _FinancialRatioPercentage);
    void                    Suspended();
	CSoccerPlayerTraining&	GetTraining();
	void 					DoTrain(const byte _Age, CClub& _Club);
	void 					DoTrainFitnessAndEnergy(const byte Hours);
	void 					DoTrainPace(const byte _Hours, const byte _Age);
	void 					DoTrainStrength(const byte _Hours, const byte _Age);
	void 					DoTrainSkills(double _Percentage, const byte _Age);
	void					TrainYouth(const byte Age, CClub& _Club);
	void					DoInjuredTraining();
	eContractOfferResponse  TryAcceptContract(CContract* ContractRequired, CContract* ContractOffered, const byte FinancialRatioPercentage, bool Bosman);
    CString		            TryAcceptTransferContract(CContract* ContractRequired, CContract* ContractOffered, const byte FinancialRatioPercentage, bool Bosman);
};

#endif
