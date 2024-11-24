
/*
	File:- SoccerPlayer.cpp

	Player skills class for soccer

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "SoccerSquadTraining.h"

#ifdef NDS
template<class T> inline const T myabs(const T& x)                                    { return (x>0) ? (x) : (-x); }
#endif

    const int				MINHANDLING = 10;
    const int				MAXHANDLING = 99;
    const int				MINTACKLING = 10;
    const int				MAXTACKLING = 99;
    const int				MINPASSING = 10;
    const int				MAXPASSING = 99;
    const int				MINSHOOTING = 10;
    const int				MAXSHOOTING = 99;

	const int				MAXTRAINVAL = 100;		// training aggression 0 - 100

  	const float      		GKHANDLING = 0.89f;
  	const float      		GKTACKLING = 0.01f;
	const float      		GKPASSING =  0.01f;
  	const float      		GKSHOOTING = 0.0f;
  	const float      		GKHEADING =  0.01f;
  	const float      		GKPACE =     0.01f;
  	const float      		GKSTRENGTH = 0.05f;
  	const float      		GKFLAIR =    0.01f;

  	const float      		DEFTACKLING = 0.8f;
  	const float      		DEFPASSING =  0.05f;
  	const float      		DEFSHOOTING = 0.04f;
  	const float      		DEFHEADING =  0.05f;
  	const float      		DEFPACE =     0.02f;
  	const float      		DEFSTRENGTH = 0.03f;
  	const float      		DEFFLAIR =    0.01f;

  	const float      		MIDTACKLING = 0.04f;
  	const float      		MIDPASSING =  0.8f;
  	const float      		MIDSHOOTING = 0.03f;
  	const float      		MIDHEADING =  0.03f;
  	const float      		MIDPACE =     0.04f;
  	const float      		MIDSTRENGTH = 0.03f;
  	const float      		MIDFLAIR =    0.03f;

  	const float      		ATTTACKLING = 0.02f;
  	const float      		ATTPASSING =  0.05f;
  	const float      		ATTSHOOTING = 0.7f;
  	const float      		ATTHEADING =  0.06f;
  	const float      		ATTPACE =     0.04f;
  	const float      		ATTSTRENGTH = 0.08f;
  	const float      		ATTFLAIR =    0.05f;

	const int				POINTSNEEDEDFORPACEADJUSTMENT = 8;
	const int				POINTSNEEDEDFORSTRENGTHADJUSTMENT = 10;
  	const float      		EXPERIENCEPOINTSUP = 0.8f;
  	const float      		EXPERIENCEPOINTSDOWN = -0.8f;

	const byte				MINIMUMPACE = 10;
	const byte				MAXIMUMPACE = 99;
	const byte				MAXIMUMENERGY = 99;
	const byte				MINIMUMENERGY = 30;
	const byte				MAXIMUMSTRENGTH = 99;
    const byte				FULLYFIT = 99;					// Players maximum Energy

	const int				BOSMANSIGNONFEEMULTIPLIER = 3;

	const byte				LOWENERGY = 70;
  	const float      		LOWENERGYMULTIPLIER = .9f;
	const byte				MAXIMUMFITNESS = 99;
	const byte				MINIMUMFITNESS = 30;


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerTraining::CSoccerPlayerTraining
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerPlayerTraining::CSoccerPlayerTraining()
{
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerTraining::DoResetVariables
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayerTraining::DoResetVariables()
{
	m_TrainingSkillsPercent = 20;
	m_TrainingPacePercent = 40;
	m_TrainingStrengthPercent = 40;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerTraining::GetPersonalFreeTimePercent
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayerTraining::GetPersonalFreeTimePercent()
{
	return 100 - (GetTrainingSkillsPercent() + GetTrainingPacePercent() + GetTrainingStrengthPercent());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerTraining::operator=
	Access:    	public
	Parameter: 	const CSoccerPlayerTraining& rTraining
	Returns:   	CSoccerPlayerTraining&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerPlayerTraining& CSoccerPlayerTraining::operator = (const CSoccerPlayerTraining& rTraining)
{
	m_BeforeTrainingSkill = rTraining.m_BeforeTrainingSkill;
	m_TrainingSkillsPercent = rTraining.m_TrainingSkillsPercent;
	m_TrainingPacePercent = rTraining.m_TrainingPacePercent;
	m_TrainingStrengthPercent = rTraining.m_TrainingStrengthPercent;
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerTraining::SetTrainingSkillsPercent
	Access:    	public
	Parameter: 	const byte x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayerTraining::SetTrainingSkillsPercent(const byte _Value)
{
	EXT_ASSERT(_Value < 101);
	m_TrainingSkillsPercent = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerTraining::SetTrainingPacePercent
	Access:    	public
	Parameter: 	const byte x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayerTraining::SetTrainingPacePercent(const byte _Value)
{
	EXT_ASSERT(_Value < 101);
	m_TrainingPacePercent = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayerTraining::SetTrainingStrengthPercent
	Access:    	public
	Parameter: 	const byte x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayerTraining::SetTrainingStrengthPercent(const byte _Value)
{
	EXT_ASSERT(_Value < 101);
	m_TrainingStrengthPercent = _Value;
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::CSoccerPlayer
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerPlayer::CSoccerPlayer()
{
	DoResetVariables();
	DoNewSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoResetVariables
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoResetVariables()
{
    m_Rating.DoResetVariables();
	m_bitfields.m_Position = FIRSTPOSN;
    m_bitfields.m_OverallSkill = MINPLAYSKILL;
    m_bitfields.m_Handling = MINPLAYSKILL;
    m_bitfields.m_Tackling = MINPLAYSKILL;
    m_bitfields.m_Passing = MINPLAYSKILL;
    m_bitfields.m_Shooting = MINPLAYSKILL;
    m_bitfields.m_Pace = MINPLAYSKILL;
    m_bitfields.m_Strength = MINPLAYSKILL;
    m_bitfields.m_Heading = MINPLAYSKILL;
    m_bitfields.m_Flair = MINPLAYSKILL;
    m_bitfields.m_Fitness = MINPLAYSKILL;
    m_bitfields.m_Energy = 0;
    m_Experience = 0;
	m_bitfields.m_CareerPath = 0;
    m_bitfields.m_PacePositivePoints = 0;
    m_bitfields.m_StrengthPositivePoints = 0;
    m_bitfields.m_PaceNegativePoints = 0;
    m_stablePrice = 0;
    m_bitfields.m_lastUpdate = 0;
	m_Training.DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::operator=
	Access:    	public
	Parameter: 	const CSoccerPlayer& Skills
	Returns:   	CSoccerPlayer&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerPlayer& CSoccerPlayer::operator = (const CSoccerPlayer& Skills)
{
    m_bitfields.m_OverallSkill = Skills.m_bitfields.m_OverallSkill;
    m_bitfields.m_Position = Skills.m_bitfields.m_Position;
    m_bitfields.m_Handling = Skills.m_bitfields.m_Handling;
    m_bitfields.m_Tackling = Skills.m_bitfields.m_Tackling;
    m_bitfields.m_Passing = Skills.m_bitfields.m_Passing;
    m_bitfields.m_Shooting = Skills.m_bitfields.m_Shooting;
    m_bitfields.m_Pace = Skills.m_bitfields.m_Pace;
    m_bitfields.m_Strength = Skills.m_bitfields.m_Strength;
    m_bitfields.m_Heading = Skills.m_bitfields.m_Heading;
    m_bitfields.m_Flair = Skills.m_bitfields.m_Flair;
    m_Rating = Skills.m_Rating;
    m_bitfields.m_Fitness = Skills.m_bitfields.m_Fitness;
    m_bitfields.m_Energy = Skills.m_bitfields.m_Energy;
    m_Experience = Skills.m_Experience;
	m_bitfields.m_CareerPath = Skills.m_bitfields.m_CareerPath;
    m_bitfields.m_PacePositivePoints = Skills.m_bitfields.m_PacePositivePoints;
    m_bitfields.m_StrengthPositivePoints = Skills.m_bitfields.m_StrengthPositivePoints;
    m_bitfields.m_PaceNegativePoints = Skills.m_bitfields.m_PaceNegativePoints;
	m_Training = Skills.m_Training;
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoLowerFitness
	Access:    	public
	Parameter: 	const uint _Amount
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoLowerFitness(const uint _Amount)
{
	m_bitfields.m_Fitness -= _Amount;
	m_bitfields.m_Fitness  = min(MAXIMUMFITNESS, max(MINIMUMFITNESS, GetFitness()));
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetOverallSkill
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetOverallSkill() const
{
	return m_bitfields.m_OverallSkill;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetHandling
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetHandling() const
{
	return m_bitfields.m_Handling;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetTackling
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetTackling() const
{
	return m_bitfields.m_Tackling;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetPassing
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetPassing() const
{
	return m_bitfields.m_Passing;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetShooting
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetShooting() const
{
	return m_bitfields.m_Shooting;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetPace
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetPace() const
{
	return m_bitfields.m_Pace;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetRating
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetRating()
{
	return m_Rating.CurrentRating();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetHeading
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetHeading() const
{
	return m_bitfields.m_Heading;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetStrength
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetStrength() const
{
	return m_bitfields.m_Strength;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetEnergy
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetEnergy() const
{
	return m_bitfields.m_Energy;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetFlair
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetFlair() const
{
	return m_bitfields.m_Flair;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetFitness
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetFitness() const
{
	return m_bitfields.m_Fitness;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetFitnessStrID
	Access:    	public
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CSoccerPlayer::GetFitnessStrID() const
{
	return FitnessStrID(m_bitfields.m_Fitness);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::FitnessStrID
	Access:    	public
	Parameter: 	const int _Fitness
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CSoccerPlayer::FitnessStrID(const int _Fitness) const
{
	if (_Fitness < 50)
	{
#ifndef NDS
		return IDS_FITNESSTEXT1;
#else
    return IDS_FITNESSTEXT1;
#endif
	}
	if (_Fitness >= 50 && _Fitness <= 60)
	{
#ifndef NDS
		return IDS_FITNESSTEXT2;
#else
    return IDS_FITNESSTEXT2;
#endif
	}
	if (_Fitness >= 61 && _Fitness <= 70)
	{
#ifndef NDS
		return IDS_FITNESSTEXT3;
#else
    return IDS_FITNESSTEXT3;
#endif
	}

	if (_Fitness >= 71 && _Fitness <= 80)
	{
#ifndef NDS
		return IDS_FITNESSTEXT4;
#else
    return IDS_FITNESSTEXT4;
#endif
	}

	if (_Fitness >= 81 && _Fitness <= 90)
	{
#ifndef NDS
		return IDS_FITNESSTEXT5;
#else
    return IDS_FITNESSTEXT5;
#endif
	}
#ifndef NDS
	return IDS_FITNESSTEXT6;
#else
    return IDS_FITNESSTEXT6;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetExperience
	Access:    	public
	Parameter: 	void
  	Returns:    float
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
float CSoccerPlayer::GetExperience() const
{
	return m_Experience;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SetHeading
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::SetHeading(const byte _Value)
{
	m_bitfields.m_Heading = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SetStrength
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::SetStrength(const byte _Value)
{
	m_bitfields.m_Strength = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SetEnergy
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::SetEnergy(const byte _Value)
{
	m_bitfields.m_Energy = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SetFlair
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::SetFlair(const byte _Value)
{
	m_bitfields.m_Flair = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SetFitness
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::SetFitness(const byte _Value)
{
	m_bitfields.m_Fitness = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoIncrementHandling
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoIncrementHandling()
{
	m_bitfields.m_Handling++;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoIncrementTackling
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoIncrementTackling()
{
	m_bitfields.m_Tackling++;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoIncrementPassing
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoIncrementPassing()
{
	m_bitfields.m_Passing++;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoIncrementShooting
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoIncrementShooting()
{
	m_bitfields.m_Shooting++;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoIncrementPace
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoIncrementPace()
{
	m_bitfields.m_Pace++;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoTrainSkills
	Access:    	public
  	Parameter:  float _Percentage
	Parameter: 	const byte _Age
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoTrainSkills(float _Percentage, const byte _Age)
{
	m_Experience += WorldData().GetCareerPaths().GetAt(m_bitfields.m_CareerPath)->PositiveMultiplier(_Age) * _Percentage;
	DoCheckExperienceForLevelUp(WorldData().GetCareerPaths().GetAt(m_bitfields.m_CareerPath)->PositiveMultiplier(_Age));
    DoCalculateOverallSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoTrainPace
	Access:    	public
	Parameter: 	const byte _Hours
	Parameter: 	const byte _Age
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoTrainPace(const byte _Hours, const byte _Age)
{
	int AgeGroup = CharacteristicsAgeGroup(_Age);
	static const byte PositiveEffects[6][11] =
	{
//Hours 0  1  2	 3	4  5  6	 7	8  9  10
		0, 1, 1, 1, 2, 2, 2, 3, 3, 3, 4,		// Age 16 - 18
		0, 0, 1, 1, 1, 2, 2, 2, 3, 3, 3,		//     19 - 21
		0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 3,		//     22 - 24
		0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 2,		//     25 - 27
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		//     28 - 30
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		//     31+
	};
	static const byte NegativeEffects[6][11] =
	{
// Hrs  0  1  2	 3	4  5  6	 7	8  9  10
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		// Age 16 - 18
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		//     19 - 21
		1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,		//     22 - 24
		2, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0,		//     25 - 27
		3, 3, 2, 2, 1, 1, 0, 0, 0, 0, 0,		//     28 - 30
		4, 4, 3, 3, 2, 2, 0, 0, 0, 0, 0,		//     31+
	};

 	int NumChances = PositiveEffects[AgeGroup][min(_Hours, 10)];
	int LoopCount;
	for (LoopCount = 0; LoopCount < NumChances; LoopCount++)
	{
		if( RandomNumber.IntLessThan(140) > GetPace())
		{
			m_bitfields.m_PacePositivePoints +=	1;
			break;
		}
	}
	NumChances = NegativeEffects[AgeGroup][min(_Hours, 10)];
	for (LoopCount = 0; LoopCount < NumChances; LoopCount++)
	{
		if( RandomNumber.IntLessThan(140) > GetPace())
		{
			m_bitfields.m_PaceNegativePoints += 1;
			break;
		}
	}
	// Adjust pace if enough points have been gained
	if (m_bitfields.m_PacePositivePoints >= POINTSNEEDEDFORPACEADJUSTMENT && m_bitfields.m_Pace < MAXIMUMPACE)
	{
		m_bitfields.m_Pace++;
		m_bitfields.m_PacePositivePoints = 0;
	}
	if (m_bitfields.m_PaceNegativePoints >= POINTSNEEDEDFORPACEADJUSTMENT && m_bitfields.m_Pace > MINIMUMPACE)
	{
		m_bitfields.m_Pace--;
		m_bitfields.m_PaceNegativePoints = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::TrainStrength
	Access:    	public
	Parameter: 	const byte _Hours
	Parameter: 	const byte _Age
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoTrainStrength(const byte _Hours, const byte _Age)
{
	static const byte PositiveEffects[6][11] =
	{
		0, 0, 0, 0, 2, 2, 2, 2, 3, 3, 3,
		0, 0, 0, 0, 0, 0, 2, 2, 2, 2, 3,
		0, 0, 0, 0, 0, 0, 0, 0, 2, 2, 2,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	};

	uint NumChances = PositiveEffects[CharacteristicsAgeGroup(_Age)][min(_Hours, 10)];
	for (uint n = 0; n < NumChances; n++)
	{
		if (RandomNumber.IntLessThan(140) > GetStrength())
		{
			m_bitfields.m_StrengthPositivePoints += 1;
			break;
		}
	}
	// Adjust strength if enough points have been gained
	if (m_bitfields.m_StrengthPositivePoints >= POINTSNEEDEDFORSTRENGTHADJUSTMENT && m_bitfields.m_Strength < MAXIMUMSTRENGTH)
	{
		m_bitfields.m_Strength++;
		m_bitfields.m_StrengthPositivePoints = 0;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoTrainFitnessAndEnergy
	Access:    	public
	Parameter: 	const byte _Hours
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoTrainFitnessAndEnergy(const byte _Hours)
{
	static const byte EnergyEffects[] =
	{
		0, 3, 6, 9, 16, 19, 22, 25, 28, 31, 34
	};
	static const sbyte FitnessEffects[] =
	{
		-2, -1, 0, 1, 2, 2, 3, 3, 4, 4, 5
	};
	m_bitfields.m_Energy -= EnergyEffects[_Hours / 4];
	m_bitfields.m_Fitness += FitnessEffects[_Hours / 4];
	m_bitfields.m_Energy  = min(MAXIMUMENERGY, max(MINIMUMENERGY, GetEnergy()));
	m_bitfields.m_Fitness  = min(MAXIMUMFITNESS, max(MINIMUMFITNESS, GetFitness()));
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoAfterMatchUpdate
	Access:    	public
	Parameter: 	const byte _Age
	Parameter: 	const byte _Performance
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoAfterMatchUpdate(const byte _Age, const byte _Performance)
{
	DoAddPerformance(_Performance);
	CPlayerCareerPath& theCareerPath = *WorldData().GetCareerPaths().GetAt(m_bitfields.m_CareerPath);
	float PerformanceDifference = (m_Rating.GetLastPerformance() - m_Rating.CurrentRating()) / 10;
	if (PerformanceDifference > 0)
	{
		m_Experience += theCareerPath.PositiveMultiplier(_Age) * PerformanceDifference;
		DoCheckExperienceForLevelUp(theCareerPath.PositiveMultiplier(_Age));
	}
	if (PerformanceDifference < 0)
	{
		m_Experience -= theCareerPath.NegativeMultiplier(_Age) * myabs(PerformanceDifference);
		DoCheckExperienceForLevelDown(theCareerPath.PositiveMultiplier(_Age));
	}
    DoCalculateOverallSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::TrainYouth
	Access:    	public
	Parameter: 	const byte Age
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::TrainYouth(const byte Age, CClub& _Club)
{
	float PersonalTrainingHoursPercentage = static_cast <float>(_Club.GetPersonalTrainingHours()) / 100;
	float TrainHours = m_Training.GetTrainingSkillsPercent() * PersonalTrainingHoursPercentage;
	m_Training.SetBeforeTrainingSkill(GetOverallSkill());
	DoTrainSkills( (0.05 * (TrainHours) ), Age);
    DoCalculateOverallSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoTrain
	Access:    	public
	Parameter: 	const byte _Age
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoTrain(const byte _Age, CClub& _Club)
{
	// Hours Percentage Individual Training - Pace, Strength
  float PersonalTrainingHoursPercentage = static_cast <float>(_Club.GetPersonalTrainingHours()) / 100;

	// Hours Percentage Team Training - Skills
  float TeamTrainingHoursPercentage = static_cast <float>(_Club.GetFirstTeamSquadTrainingDetails().GetTeamTrainingHours()) / 100;
  float SkillsTrainingHours =  TeamTrainingHoursPercentage * (100 - _Club.GetFirstTeamSquadTrainingDetails().GetRestingPercent());
  float SkillsTrainingHoursPercentage = SkillsTrainingHours / 100;

	// Hours Team & Individual Training
  float FitnessHours = PersonalTrainingHoursPercentage * (100 - m_Training.GetPersonalFreeTimePercent());
	FitnessHours += SkillsTrainingHoursPercentage * (100 - _Club.GetFirstTeamSquadTrainingDetails().GetRestingPercent());

	// Do the Team Training
	m_Training.SetBeforeTrainingSkill(GetOverallSkill());

	// Skill hours = 5 a side + skills
  float side5  = _Club.GetFirstTeamSquadTrainingDetails().Get5aSidePercent() * TeamTrainingHoursPercentage;
  float skills = m_Training.GetTrainingSkillsPercent() * PersonalTrainingHoursPercentage;
	DoTrainSkills( (0.025 * (side5 + skills) ), _Age );

	// Do the Individual Training
	DoTrainPace(static_cast<byte>(PersonalTrainingHoursPercentage * m_Training.GetTrainingPacePercent()), _Age);
	DoTrainStrength(static_cast<byte>(PersonalTrainingHoursPercentage * m_Training.GetTrainingStrengthPercent()), _Age);

	// Works on both Training
	DoTrainFitnessAndEnergy(static_cast<byte>(FitnessHours));
    DoCalculateOverallSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoInjuredTraining
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoInjuredTraining()
{
	m_bitfields.m_Fitness -= 2;
	m_bitfields.m_Fitness = min(MAXIMUMFITNESS, max(MINIMUMFITNESS, m_bitfields.m_Fitness));
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::RejuvenateEnergy
	Access:    	public
	Parameter: 	const byte Age
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoRejuvenateEnergy(const byte _Age)
{
	static const byte AgeAdjustment[] =
	{
		28, 28, 28, 28,		// 16 - 19
		26, 26, 26, 26,		// 20 - 23
		24, 24, 24, 24,		// 24 - 27
		22, 22, 22, 22,		// 28 - 31
		20, 20, 20,			// 32 - 34
		18, 18, 18, 18,		// 35 - 38
		16, 16, 16, 16,		// 39 - 42
		16, 16, 16, 16
	};
	EXT_ASSERT(_Age >= MINIMUPLAYERAGE && _Age <= MAXIMUMPLAYERAGE);
	EXT_ASSERT(AgeAdjustment[_Age - 16] < 30);
	m_bitfields.m_Energy += AgeAdjustment[_Age - 16];
	m_bitfields.m_Energy  = min(MAXIMUMENERGY, max(MINIMUMENERGY, GetEnergy()));
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::CharacteristicsAgeGroup
	Access:    	public
	Parameter: 	byte _Age
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CSoccerPlayer::CharacteristicsAgeGroup(byte _Age)
{
	EXT_ASSERT(_Age >= MINIMUPLAYERAGE && _Age <= MAXIMUMPLAYERAGE);
	static const int AgeGroups[] =
	{
		0, 0, 0, 1, 1, 1,			// < 22
		2, 2, 2, 3, 3, 3,			// < 28
		4, 4, 4, 4, 5, 5,			// < 34
		5, 5, 5, 5, 5, 5,			// < 40
		5, 5, 5, 5, 5, 5, 5
	};
	EXT_ASSERT(AgeGroups[_Age - 16] < 6);
	return AgeGroups[_Age - 16];
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SkillRatingMultiplier
	Access:    	public
	Parameter: 	void
  	Returns:    float
	Qualifier:
	Purpose:	Calculate the players current rating as a fraction of his overall
				skill
				If {skill} and {rating} are equal then returns 1
				If {rating} is 90% of {skill} then returns .9
------------------------------------------------------------------------------*/
float CSoccerPlayer::SkillRatingMultiplier()
{
  return static_cast<float>(m_Rating.CurrentRating()) / static_cast<float>(GetOverallSkill());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoMakeInvincible
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoMakeInvincible()
{
#ifdef _DEBUG
    m_bitfields.m_Handling = MAXPLAYSKILL;
    m_bitfields.m_Tackling = MAXPLAYSKILL;
    m_bitfields.m_Passing = MAXPLAYSKILL;
    m_bitfields.m_Shooting = MAXPLAYSKILL;
    m_bitfields.m_Pace = MAXPLAYSKILL;
#endif // !_DEBUG
    if (GetHandling() > 90 && GetTackling() > 90 && GetPassing  () > 90 && GetShooting() > 90)
	{
		EXT_ASSERT(false);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoMakeYoung
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoMakeYoung()
{
	m_bitfields.m_Strength = 29 + RandomNumber.IntLessThan(40);
	m_bitfields.m_Pace = 29 + RandomNumber.IntLessThan(40);
	m_bitfields.m_Flair = 29 + RandomNumber.IntLessThan(40);
  	m_bitfields.m_Handling = max(GetHandling() - (GetHandling() * .2f), MINHANDLING);
  	m_bitfields.m_Tackling = max(GetTackling() - (GetTackling() * .2f), MINTACKLING);
  	m_bitfields.m_Passing = max(GetPassing  () - (GetPassing  () * .2f), MINPASSING);
  	m_bitfields.m_Shooting = max(GetShooting() - (GetShooting() * .2f), MINSHOOTING);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoMakeOld
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoMakeOld()
{
	m_bitfields.m_Pace = 24 + RandomNumber.IntLessThan(50);
  	m_bitfields.m_Handling = min(GetHandling() + (GetHandling() * .2f), MAXHANDLING);
  	m_bitfields.m_Tackling = min(GetTackling() + (GetTackling() * .2f), MINTACKLING);
  	m_bitfields.m_Passing = min(GetPassing  () + (GetPassing  () * .2f), MINPASSING);
  	m_bitfields.m_Shooting = min(GetShooting() + (GetShooting() * .2f), MINSHOOTING);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetEnergyEffect
	Access:    	public const
	Parameter: 	void
  	Returns:    const float
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
float CSoccerPlayer::GetEnergyEffect() const
{
	if (GetEnergy() < LOWENERGY)
	{
		return LOWENERGYMULTIPLIER;
	}
	return 1;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::EnergyText
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CSoccerPlayer::EnergyText()
{
	return FitnessStrID(m_bitfields.m_Energy);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoNewSeason
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoNewSeason()
{
    m_bitfields.m_Energy = (FULLYFIT - 18) + RandomNumber.IntLessThan(18);
    m_bitfields.m_Fitness = (FULLYFIT - 18) + RandomNumber.IntLessThan(18);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::AfterMatchEnergyAndFitnessUpdate
	Access:    	public
	Parameter: 	const byte HowLongPlayed
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::AfterMatchEnergyAndFitnessUpdate(const byte HowLongPlayed)
{

	byte HowLong =  (HowLongPlayed < 46 && HowLongPlayed > 20) * 1 + \
					(HowLongPlayed < 71 && HowLongPlayed > 45) * 2 + \
					(HowLongPlayed < 95 && HowLongPlayed > 69) * 3 + \
					(HowLongPlayed < 106 && HowLongPlayed > 94) * 4 + \
					(HowLongPlayed > 106) * 5;

	static const byte EnergyEffect[] =
	{
		0, 5, 10, 15, 20, 25
	};
	static const byte FitnessEffect[] =
	{
		0, 1, 2, 3, 4, 5
	};

	//No effect on Energy for goalkeeper
    if( GetPosition() != GOALKEEPER )
	{
		m_bitfields.m_Energy -= EnergyEffect[HowLong];
		m_bitfields.m_Energy = max(MINIMUMENERGY, GetEnergy());
	}
	m_bitfields.m_Fitness += FitnessEffect[HowLong];
	m_bitfields.m_Fitness = min(MAXIMUMFITNESS, GetFitness());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoInitialiseFromResource
	Access:    	public
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	m_bitfields.m_Position = (ePlayerPosition)_Resource.ReadByte();
	m_bitfields.m_BestFoot = _Resource.ReadByte();
	SetOverallSkill(_Resource.ReadByte());
	m_bitfields.m_CareerPath = WorldData().GetCareerPaths().RandomCareerPath(GetPosition());
	DoCalculateOverallSkill();
	m_Rating.OnInitialise(GetOverallSkill());
	m_Training.SetBeforeTrainingSkill(GetOverallSkill());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetBestFoot
	Access:    	public
	Parameter: 	void
	Returns:   	const eBESTFOOT
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
eBESTFOOT CSoccerPlayer::GetBestFoot()
{
	return static_cast<eBESTFOOT>(m_bitfields.m_BestFoot);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetBestFootStr
	Access:    	public
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CSoccerPlayer::GetBestFootStr()
{
	CString strPrint;
	int FootStrID[] =
	{
#ifndef NDS
		IDS_LEFT,
		IDS_RIGHT,
		IDS_BOTH,
		IDS_NOTSPECIFIED
#else
    IDS_LEFT,
    IDS_RIGHT,
    IDS_BOTH,
    IDS_NOTSPECIFIED
#endif
	};
	strPrint.LoadString(FootStrID[GetBestFoot()]);
	return strPrint;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SetPace
	Access:    	public
	Parameter: 	const byte x
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::SetPace(const byte x)
{
	EXT_ASSERT(x >= MINIMUMPACE && x <= MAXIMUMPACE);
	m_bitfields.m_Pace = x;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoAddPerformance
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoAddPerformance(const byte _Value)
{
	// Ensure is within 10 above or below overall skill
	byte Value = max(GetOverallSkill() - 10, min(_Value, GetOverallSkill() + 10));
	Value = max(min(Value, MAXPLAYSKILL), MINPLAYSKILL);
    m_Rating.DoAddPerformance(Value);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetPerformance
	Access:    	public
	Parameter: 	void
	Returns:   	sbyte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
sbyte CSoccerPlayer::GetPerformance()
{
    return m_Rating.GetPerformance();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCreateYouth
	Access:    	public
	Parameter: 	const byte _Age
	Parameter: 	CCountry & _Country
	Parameter: 	CPopularity * _Popularity
	Parameter: 	const byte _ClubDivStrength
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoCreateYouth(const byte _Age, CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength)
{
	sbyte Skill = _Popularity->GetRandomPlayerSkill(_ClubDivStrength) - (15 + RandomNumber.IntLessThan(10));
    DoCreate(_Age, DoGenerateRandomPosition(_Country), max(MINPLAYSKILL, Skill));
	m_bitfields.m_Fitness = 95;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCreatePlayer
	Access:    	public
	Parameter: 	const byte _Age
	Parameter: 	CPopularity * _Popularity
	Parameter: 	const byte _ClubDivStrength
	Parameter: 	ePlayerPosition _Position
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoCreatePlayer(const byte _Age, CPopularity* _Popularity, const byte _ClubDivStrength, ePlayerPosition _Position)
{
	sbyte Skill = _Popularity->GetRandomPlayerSkill(_ClubDivStrength);
    DoCreate(_Age, _Position, max(MINPLAYSKILL, Skill));
	m_bitfields.m_Fitness = 95;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCreate
	Access:    	public
	Parameter: 	const byte _Age
	Parameter: 	ePlayerPosition _Type
	Parameter: 	const byte _Skill
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoCreate(const byte _Age, ePlayerPosition _Type, const byte _Skill)
{
	EXT_ASSERT(_Age >= MINIMUPLAYERAGE && _Age <= MAXIMUMPLAYERAGE);		// Ensure age is reasonable
    EXT_ASSERT(_Skill >= MINPLAYSKILL && _Skill <= MAXPLAYSKILL);
    m_bitfields.m_OverallSkill = _Skill;
    m_bitfields.m_Position = _Type;
	m_bitfields.m_BestFoot = (RandomNumber.IntLessThan(10) < 5) ? 1 : 0;
	m_bitfields.m_CareerPath = WorldData().GetCareerPaths().RandomCareerPath(GetPosition());
	DoCreateSkills();
	if (_Age < 21)
	{
		DoMakeYoung();
	}
    if (_Age > 30)
	{
		DoMakeOld();
	}
	DoCalculateOverallSkill();
	m_Rating.OnInitialise(GetOverallSkill());
	m_Training.SetBeforeTrainingSkill(GetOverallSkill());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetPosition
	Access:    	public
	Parameter: 	void
	Returns:   	const ePlayerPosition
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ePlayerPosition CSoccerPlayer::GetPosition()
{
	return m_bitfields.m_Position;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoGenerateRandomSkill
	Access:    	public
  	Parameter:  const float aSkill
	Parameter: 	const byte aMinSkill
	Parameter: 	const byte aRndSkill
	Returns:   	const byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::DoGenerateRandomSkill(const float aSkill, const byte aMinSkill, const byte aRndSkill)
{
	return max(min(aSkill * aMinSkill + RandomNumber.IntLessThan(static_cast<uint>(aSkill * aRndSkill)), MAXPLAYSKILL), MINPLAYSKILL);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCreateSkills
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoCreateSkills()
{
	EXT_ASSERT(GetOverallSkill() >= MINPLAYSKILL && GetOverallSkill() <= MAXPLAYSKILL);
  	float tSkill = static_cast<float>(GetOverallSkill()) / 100;
	m_bitfields.m_Strength = 50 + tSkill * 20 + tSkill * (RandomNumber.IntLessThan(30) + 1);
	m_bitfields.m_Pace = 50 + tSkill * 20 + tSkill * (RandomNumber.IntLessThan(30) + 1);
	m_bitfields.m_Flair = 25 + tSkill * (RandomNumber.IntLessThan(60) + 1);
	m_Experience = 0;
	m_bitfields.m_StrengthPositivePoints = 0;
	m_bitfields.m_PacePositivePoints = 0;
	m_bitfields.m_PaceNegativePoints = 0;
    switch(GetPosition())
	{
        case  GOALKEEPER:
            m_bitfields.m_Handling = GetOverallSkill();
            m_bitfields.m_Tackling = DoGenerateRandomSkill(tSkill, 40, 20);
			m_bitfields.m_Passing = DoGenerateRandomSkill(tSkill, 40, 15);
			m_bitfields.m_Shooting = DoGenerateRandomSkill(tSkill, 35, 25);
			m_bitfields.m_Heading = DoGenerateRandomSkill(tSkill, 40, 20);
			break;

		case DEFENDER:
			m_bitfields.m_Handling = DoGenerateRandomSkill(tSkill, 10, 20);
            m_bitfields.m_Tackling = GetOverallSkill();
			m_bitfields.m_Passing = DoGenerateRandomSkill(tSkill, 50, 30);
			m_bitfields.m_Shooting = DoGenerateRandomSkill(tSkill, 50, 20);
			m_bitfields.m_Heading = DoGenerateRandomSkill(tSkill, 60, 20);
			break;

        case MIDFIELDER:
            m_bitfields.m_Handling = DoGenerateRandomSkill(tSkill, 10, 20);
            m_bitfields.m_Tackling = DoGenerateRandomSkill(tSkill, 50, 30);
            m_bitfields.m_Passing = GetOverallSkill();
			m_bitfields.m_Shooting = DoGenerateRandomSkill(tSkill, 50, 30);
			m_bitfields.m_Heading = DoGenerateRandomSkill(tSkill, 50, 30);
			break;

        case ATTACKER:
            m_bitfields.m_Handling = DoGenerateRandomSkill(tSkill, 10, 20);
			m_bitfields.m_Tackling = DoGenerateRandomSkill(tSkill, 50, 20);
			m_bitfields.m_Passing = DoGenerateRandomSkill(tSkill, 50, 30);
            m_bitfields.m_Shooting = GetOverallSkill();
			m_bitfields.m_Heading = DoGenerateRandomSkill(tSkill, 60, 30);
			break;

		default:
			EXT_ASSERT(false);
    }
	DoCalculateOverallSkill();
	m_Training.SetBeforeTrainingSkill(GetOverallSkill());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::HasImproved
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CSoccerPlayer::HasImproved()
{
	if (GetOverallSkill() > m_Training.GetBeforeTrainingSkill())
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::HasWorsened
	Access:    	public
	Parameter: 	void
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CSoccerPlayer::HasWorsened()
{
	if (GetOverallSkill() < m_Training.GetBeforeTrainingSkill())
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoEnsureInRange
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:	Ensure all skill are within limits
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoEnsureInRange()
{
	// If these asserts kick in then something is wrong with
	// skills, they should never get in this range
	EXT_ASSERT(GetHandling() < 250 && GetTackling() < 250 && GetPassing  () < 250 && GetShooting() < 250);
	m_bitfields.m_Handling = min(max(GetHandling(), MINPLAYSKILL), MAXPLAYSKILL);
    m_bitfields.m_Tackling = min(max(GetTackling(), MINPLAYSKILL), MAXPLAYSKILL);
	m_bitfields.m_Passing = min(max(GetPassing  (), MINPLAYSKILL), MAXPLAYSKILL);
	m_bitfields.m_Shooting = min(max(GetShooting(), MINPLAYSKILL), MAXPLAYSKILL);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCalculateOverallSkill
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoCalculateOverallSkill()
{
	DoEnsureInRange();	// Ensure all skill are within limits
    switch(GetPosition())
	{
		case  GOALKEEPER:
            m_bitfields.m_OverallSkill = min(MAXPLAYSKILL, max(MINPLAYSKILL, static_cast<byte> ( \
	            static_cast<float> (GetHandling()) * GKHANDLING + \
    	        static_cast<float> (GetTackling()) * GKTACKLING + \
        	    static_cast<float> (GetPassing  ()) * GKPASSING + \
            	static_cast<float> (GetShooting()) * GKSHOOTING + \
	            static_cast<float> (GetHeading()) * GKHEADING + \
    	        static_cast<float> (GetPace()) * GKPACE + \
        	    static_cast<float> (GetFlair()) * GKFLAIR + \
            	static_cast<float> (GetStrength()) * GKSTRENGTH + .5)));
			break;

		case DEFENDER:
            m_bitfields.m_OverallSkill = min(MAXPLAYSKILL, max(MINPLAYSKILL, static_cast<byte> ( \
  	          	static_cast<float> (GetTackling()) * DEFTACKLING + \
    	        static_cast<float> (GetPassing  ()) * DEFPASSING + \
        	    static_cast<float> (GetShooting()) * DEFSHOOTING + \
            	static_cast<float> (GetHeading()) * DEFHEADING + \
	            static_cast<float> (GetPace()) * DEFPACE + \
    	        static_cast<float> (GetFlair()) * DEFFLAIR + \
        	    static_cast<float> (GetStrength()) * DEFSTRENGTH + .5)));
			break;

        case MIDFIELDER:
            m_bitfields.m_OverallSkill = min(MAXPLAYSKILL, max(MINPLAYSKILL, static_cast<byte> ( \
            	static_cast<float> (GetTackling()) * MIDTACKLING + \
	            static_cast<float> (GetPassing  ()) * MIDPASSING + \
    	        static_cast<float> (GetShooting()) * MIDSHOOTING + \
        	    static_cast<float> (GetHeading()) * MIDHEADING + \
            	static_cast<float> (GetPace()) * MIDPACE + \
	            static_cast<float> (GetFlair()) * MIDFLAIR + \
    	        static_cast<float> (GetStrength()) * MIDSTRENGTH + .5)));
			break;

        case ATTACKER:
            m_bitfields.m_OverallSkill = min(MAXPLAYSKILL, max(MINPLAYSKILL, static_cast<byte> ( \
            	static_cast<float> (GetTackling()) * ATTTACKLING + \
	            static_cast<float> (GetPassing  ()) * ATTPASSING + \
    	        static_cast<float> (GetShooting()) * ATTSHOOTING + \
        	    static_cast<float> (GetHeading()) * ATTHEADING + \
            	static_cast<float> (GetPace()) * ATTPACE + \
	            static_cast<float> (GetFlair()) * ATTFLAIR + \
    	        static_cast<float> (GetStrength()) * ATTSTRENGTH + .5)));
			break;
		default:
			EXT_ASSERT(false);
			break;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::PosnAbbr
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CSoccerPlayer::PosnAbbr()
{
    switch(GetPosition())
	{
        case  GOALKEEPER:
            return IDS_GOALKEEPERABBR;
        case DEFENDER:
            return IDS_DEFENDERABBR;
        case MIDFIELDER:
            return IDS_MIDFIELDERABBR;
        case ATTACKER:
            return IDS_ATTACKERABBR;
		default:
			EXT_ASSERT(false);
			break;
    }
    return(0);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::PosnLongAbbr
	Access:    	public
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CSoccerPlayer::PosnLongAbbr()
{
	switch(GetPosition())
	{
	case  GOALKEEPER:
    return IDS_GOALKEEPERLONGABBR;
	case DEFENDER:
    return IDS_DEFENDERLONGABBR;
	case MIDFIELDER:
    return IDS_MIDFIELDERLONGABBR;
	case ATTACKER:
    return IDS_ATTACKERLONGABBR;
	default:
		EXT_ASSERT(false);
		break;
	}
	return(0);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::RandomPosition
	Access:    	public
	Returns:   	ePlayerPosition
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ePlayerPosition CSoccerPlayer::RandomPosition()
{
	int iType = RandomNumber.IntLessThan(100);
	if (iType < 10)
	{
		return GOALKEEPER;
	}
	if (iType < 45)
	{
		return DEFENDER;
	}
	if (iType < 75)
	{
		return MIDFIELDER;
	}
	return ATTACKER;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::PositionText
	Access:    	public
	Parameter: 	void
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CSoccerPlayer::PositionTextID()
{
    switch(GetPosition())
	{
        case  GOALKEEPER:
            return IDS_GOALKEEPER;
        case DEFENDER:
            return IDS_DEFENDER;
        case MIDFIELDER:
            return IDS_MIDFIELDER;
        case ATTACKER:
            return IDS_ATTACKER;
		default:
			EXT_ASSERT(false);
			break;
    }
    return(0);
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::PositionStr
	Access:    	public
	Parameter: 	void
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CSoccerPlayer::PositionStr()
{
	CString str;
    str.LoadString(PositionTextID());
	return str;
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoGenerateRandomContract
	Access:    	public
	Parameter: 	CContract& _ContractOffered
	Parameter: 	CCountry& rCountry
	Parameter: 	const byte Age
	Parameter: 	bool Bosman
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoGenerateRandomContract(CContract& _ContractOffered, CCountry& _Country, const byte _Age, bool _Bosman)
{
	EXT_ASSERT(_Age >= MINIMUPLAYERAGE && _Age <= MAXIMUMPLAYERAGE);		// Ensure age is reasonable
	_ContractOffered.SetValues(DoGenerateRandomSignOnFee(_Country, _Bosman), DoGenerateRandomWage(_Country), DoGenerateRandomContractLength(_Age));
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoGenerateRandomContractLength
	Access:    	public
	Parameter: 	const byte _Age
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CSoccerPlayer::DoGenerateRandomContractLength(const byte _Age)
{
	EXT_ASSERT(_Age < 40);
	ushort nContractLength = 1 + RandomNumber.IntLessThan(3);
	if (_Age > 37)
	{
		nContractLength = 1;
	}
	else if (_Age > 34)
	{
		nContractLength = 1 + RandomNumber.IntLessThan(2);
	}
	return nContractLength;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::TryAcceptContract
	Access:    	public
	Parameter: 	CContract* ContractRequired
	Parameter: 	CContract* ContractOffered
	Parameter: 	const byte FinancialRatioPercentage
	Parameter: 	bool Bosman
	Returns:   	eNewsType
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
eContractOfferResponse CSoccerPlayer::TryAcceptContract(CContract* ContractRequired, CContract* ContractOffered, const byte FinancialRatioPercentage, bool Bosman)
{
	EXT_ASSERT(ContractOffered != NULL);
	CSkillMoneyValue theSkillMoney = WorldData().GetSkillMoneyValueItem(GetOverallSkill());
  	float amount = (theSkillMoney.GetPlayerWage().GetBasicValue() / 100) * FinancialRatioPercentage;
	if (ContractOffered->GetWage() < ContractRequired->GetWage())
	{
		if (ContractOffered->GetWage() < theSkillMoney.GetPlayerWage().GetBasicValue())
		{
			return MOREWAGE;
		}
 		amount = (theSkillMoney.GetPlayerWage().GetMaximumValue() / 100) * FinancialRatioPercentage;
		if (ContractOffered->GetWage() > amount)
		{
			return LESSWAGE;
		}
	}
	if (ContractOffered->GetSignOnFee() < ContractRequired->GetSignOnFee())
	{
	 	amount = (theSkillMoney.GetBasicSigningFee() / 100) * FinancialRatioPercentage;
		if (Bosman == true)
		{
			amount *= BOSMANSIGNONFEEMULTIPLIER;
		}
		if (ContractOffered->GetSignOnFee() < amount)
		{
			return MORESIGNON;
		}
 		amount = (theSkillMoney.GetMaximumSigningFee() / 100) * FinancialRatioPercentage;
		if (ContractOffered->GetSignOnFee() > amount)
		{
			return LESSSIGNON;
	    }
	}
    return NONEINPROGRESS;
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::TryAcceptTransferContract
	Access:    	public
	Parameter: 	CContract* ContractRequired
	Parameter: 	CContract* ContractOffered
	Parameter: 	const byte FinancialRatioPercentage
	Parameter: 	bool Bosman
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CSoccerPlayer::TryAcceptTransferContract(CContract* ContractRequired, CContract* ContractOffered, const byte FinancialRatioPercentage, bool Bosman)
{
	CString str;
	eContractOfferResponse eResponse = TryAcceptContract(ContractRequired, ContractOffered, FinancialRatioPercentage, Bosman);
	if (eResponse != NONEINPROGRESS)
	{
		str.LoadString(eResponse);
        return str;
	}
    return _T("T");
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCalculateSignOnFee
	Access:    	public
	Parameter: 	CCountry& rCountry
	Parameter: 	bool Bosman
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoCalculateSignOnFee(CCountry& rCountry, bool Bosman)
{
	uint Value = (WorldData().GetSkillMoneyValueItem(GetOverallSkill()).GetBasicSigningFee() / 100) * rCountry.GetFinancialRatioPercentage();
	if (Bosman == true)
	{
		Value *= BOSMANSIGNONFEEMULTIPLIER;
	}
    return rCountry.GetCurrency().RoundedPounds(Value);
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCalculateMaximumSignOnFee
	Access:    	public
	Parameter: 	CCountry& rCountry
	Parameter: 	bool Bosman
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoCalculateMaximumSignOnFee(CCountry& rCountry, bool Bosman)
{
	uint Value = (WorldData().GetSkillMoneyValueItem(GetOverallSkill()).GetMaximumSigningFee() / 100) * rCountry.GetFinancialRatioPercentage();
	if (Bosman == true)
	{
		Value *= BOSMANSIGNONFEEMULTIPLIER;
	}
    return rCountry.GetCurrency().RoundedPounds(Value);
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCalculateMinimumTransferFee
	Access:    	public
	Parameter: 	const byte _Age
	Parameter: 	CCountry & _Country
	Parameter: 	const byte _Temper
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoCalculateMinimumTransferFee(const byte _Age, CCountry& _Country, const byte _Temper)
{
	EXT_ASSERT(_Age >= MINIMUPLAYERAGE && _Age <= MAXIMUMPLAYERAGE);		// Ensure age is reasonable
	uint Value = SuggestedTransferFee(_Age, _Temper, _Country.GetFinancialRatioPercentage());
	if (_Age > 32)
	{
		Value = (Value / 100) * 75;
	}
	else
	{
		Value = (Value / 100) * 85;
	}
    return _Country.GetCurrency().RoundedPounds(Value);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCalculateMaximumTransferFee
	Access:    	public
	Parameter: 	const byte Age
	Parameter: 	CCountry& rCountry
	Parameter: 	const byte Temper
	Returns:   	uint
	Qualifier:
	Purpose:	What is the maximum transfer fee for the player?
------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoCalculateMaximumTransferFee(const byte Age, CCountry& rCountry, const byte Temper)
{
	EXT_ASSERT(Age >= MINIMUPLAYERAGE && Age < MAXIMUMPLAYERAGE);		// Ensure age is reasonable
  	uint Value = SuggestedTransferFee(Age, Temper, rCountry.GetFinancialRatioPercentage()) * 1.25f;
    return rCountry.GetCurrency().RoundedPounds(Value + 100000);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SuggestedTransferFee
	Access:    	public
	Parameter: 	const byte Age
	Parameter: 	const byte Temper
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerPlayer::SuggestedTransferFee(const byte _Age, const byte _Temper, const byte _FinancialRatioPercentage)
{
	EXT_ASSERT(_FinancialRatioPercentage > 0);
	EXT_ASSERT(_Age >= MINIMUPLAYERAGE && _Age <= MAXIMUMPLAYERAGE);		// Ensure age is reasonable
	uint Value = WorldData().GetSkillMoneyValueItem(GetOverallSkill()).GetPlayerValue().GetMidValue() / 100;
	static const uint TemperAdjusters[] =
	{
		101, 98, 103, 97, 101, 98, 100, 99, 101, 97
	};
	static const uint PositionAdjusters[] =
	{
		75, 90, 100, 110
	};
	Value *= PositionAdjusters[GetPosition()];

	uint nAdjuster = _FinancialRatioPercentage + PositionAdjusters[GetPosition()] + TemperAdjusters[_Temper];

	Value /= 100;
	if (m_bitfields.m_Pace > 95)
	{
		Value *= 106;
		nAdjuster += 106;
	}
	else if (m_bitfields.m_Pace > 90)
	{
		Value *= 104;
		nAdjuster += 106;
	}
	else if (m_bitfields.m_Pace > 85)
	{
		Value *= 102;
		nAdjuster += 102;
	}
	else if (m_bitfields.m_Pace > 80)
	{
		Value *= 100;
		nAdjuster += 100;
	}
	else if (m_bitfields.m_Pace > 70)
	{
		Value *= 98;
		nAdjuster += 98;
	}
	else if (m_bitfields.m_Pace > 60)
	{
		Value *= 96;
		nAdjuster += 96;
	}
	else if (m_bitfields.m_Pace > 50)
	{
		Value *= 92;
		nAdjuster += 92;
	}
	else
	{
		Value *= 84;
		nAdjuster += 84;
	}

	Value /= 100;
	Value *= TemperAdjusters[_Temper];
	Value /= 100;
	Value *= _FinancialRatioPercentage;
    return Value;
 }


 /*------------------------------------------------------------------------------
 	Method:   	CSoccerPlayer::DoGenerateRandomTransferFee
 	Access:    	public
 	Parameter: 	const byte Age
 	Parameter: 	CCountry& rCountry
 	Parameter: 	byte Temper
 	Returns:   	const uint
 	Qualifier:
 	Purpose:
 ------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoGenerateRandomTransferFee(const byte _Age, CCountry& _Country, const byte _Temper)
{
	EXT_ASSERT(_Age >= MINIMUPLAYERAGE && _Age <= MAXIMUMPLAYERAGE);		// Ensure age is reasonable
	uint MinFee = DoCalculateMinimumTransferFee(_Age, _Country, _Temper);
  	CCalendar calender = WorldData().GetCurrentDate();

  	if( calender.GetWeek() > m_bitfields.m_lastUpdate  || m_stablePrice == 0 )
  	{
    	m_stablePrice = static_cast <uint>(_Country.GetCurrency().RoundedPounds(MinFee + RandomNumber.IntLessThan(DoCalculateMaximumTransferFee(_Age, _Country, _Temper) - MinFee)));
	    m_bitfields.m_lastUpdate = calender.GetWeek();
  	}
    if (m_stablePrice < 5000)
	{
		m_stablePrice = 0;
	}
	return m_stablePrice;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoGenerateRandomSignOnFee
	Access:    	public
	Parameter: 	CCountry& _Country
	Parameter: 	bool _Bosman
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoGenerateRandomSignOnFee(CCountry& _Country, bool _Bosman)
{
	uint SignOnFee = (WorldData().GetSkillMoneyValueItem(GetOverallSkill()).GetRandomSigningFee() / 100) * _Country.GetFinancialRatioPercentage();
	if (_Bosman == true)
	{
		SignOnFee *= BOSMANSIGNONFEEMULTIPLIER;
	}
    return _Country.GetCurrency().RoundedPounds(SignOnFee);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoGenerateRandomWage
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoGenerateRandomWage(CCountry& _Country)
{
    uint Wage = (static_cast<float>(WorldData().GetSkillMoneyValueItem(GetOverallSkill()).GetPlayerWage().RandomAmount()) / 100) * _Country.GetFinancialRatioPercentage();
	EXT_ASSERT(Wage > 0);
	Wage = _Country.GetCurrency().RoundedPounds(Wage);
	EXT_ASSERT(Wage > 0);
	return Wage;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::MaxValue
	Access:    	public
  	Parameter:  float Amount
	Parameter: 	const byte Age
	Parameter: 	CCountry& rCountry
	Parameter: 	const byte Temper
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CSoccerPlayer::MaxValue(float Amount, const byte Age, CCountry& rCountry, const byte Temper)
{
	EXT_ASSERT(Age >= MINIMUPLAYERAGE && Age <= MAXIMUMPLAYERAGE);		// Ensure age is reasonable
	if (DoCalculateMaximumTransferFee(Age, rCountry, Temper) < Amount)
	{
		return true;
	}
	else
	{
		return false;
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCalculateMinimumWage
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoCalculateMinimumWage(CCountry& rCountry)
{
    uint dMinimumWage = (WorldData().GetSkillMoneyValueItem(GetOverallSkill()).GetPlayerWage().GetBasicValue() / 100) * rCountry.GetFinancialRatioPercentage();
	EXT_ASSERT(dMinimumWage > 0);
	return rCountry.GetCurrency().RoundedPounds(dMinimumWage);
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoCalculateMaximumWage
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CSoccerPlayer::DoCalculateMaximumWage(CCountry& rCountry)
{
	uint dMaximumWage = (WorldData().GetSkillMoneyValueItem(GetOverallSkill()).GetPlayerWage().GetMaximumValue() / 100) * rCountry.GetFinancialRatioPercentage();
	EXT_ASSERT(dMaximumWage > 0);
    return rCountry.GetCurrency().RoundedPounds(dMaximumWage);
 }


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::FindType
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:	Determine the player's best position
------------------------------------------------------------------------------*/
byte CSoccerPlayer::FindType()
{
	byte BestVal, BestSkill;
	BestVal = GetHandling();
	BestSkill = GOALKEEPER;
	if (BestVal < GetTackling())
	{
		BestVal = GetTackling();
		BestSkill = DEFENDER;
	}
	if (BestVal < GetPassing  ())
	{
		BestVal = GetPassing  ();
		BestSkill =MIDFIELDER;
	}
	if (BestVal < GetShooting())
	{
		BestVal = GetShooting();
		BestSkill = ATTACKER;
	}
	return BestSkill;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::ReduceSkill
	Access:    	public
	Parameter: 	const byte Age
	Parameter: 	int IncreaseChance
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::ReduceSkill(const byte _Age, int _IncreaseChance)
{
	EXT_ASSERT(_IncreaseChance > 0);
    if (_Age < 21)
	{
		_IncreaseChance = _IncreaseChance << 2;
	}
    if (_Age > 20 && _Age < 28 )
	{
		_IncreaseChance = _IncreaseChance << 1;
	}
    if (_Age > 32)
	{
		_IncreaseChance = _IncreaseChance >> 1;
	}
	DoReduceActualSkill(_IncreaseChance);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoReduceActualSkill
	Access:    	public
	Parameter: 	int IncreaseChance
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoReduceActualSkill(int _IncreaseChance)
{
	EXT_ASSERT(_IncreaseChance > 0);
	if (RandomNumber.IntLessThan(_IncreaseChance) == 1)
	{
		switch(GetPosition())
		{
			case  GOALKEEPER:
				m_bitfields.m_Handling -= (GetHandling() > MINHANDLING);
				m_bitfields.m_Tackling -= ((RandomNumber.IntLessThan(2) == 1) && GetTackling() > MINTACKLING);
				m_bitfields.m_Passing -= ((RandomNumber.IntLessThan(2) == 1) && GetPassing  () > MINPASSING);
				m_bitfields.m_Shooting -= ((RandomNumber.IntLessThan(2) == 1) && GetShooting() > MINSHOOTING);
				break;

			case DEFENDER:
				m_bitfields.m_Handling -= ((RandomNumber.IntLessThan(2) == 1) && GetHandling() > MINHANDLING);
				m_bitfields.m_Tackling -= (GetTackling() > MINTACKLING);
				m_bitfields.m_Passing -= ((RandomNumber.IntLessThan(2) == 1) && GetPassing  () > MINPASSING);
				m_bitfields.m_Shooting -= ((RandomNumber.IntLessThan(2) == 1) && GetShooting() > MINSHOOTING);
				break;

			case MIDFIELDER:
				m_bitfields.m_Handling -= ((RandomNumber.IntLessThan(2) == 1) && GetHandling() > MINHANDLING);
				m_bitfields.m_Tackling -= ((RandomNumber.IntLessThan(2) == 1) && GetTackling() > MINTACKLING);
				m_bitfields.m_Passing -= (GetPassing  () > MINPASSING);
				m_bitfields.m_Shooting -= ((RandomNumber.IntLessThan(2) == 1) && GetShooting() > MINSHOOTING);
				break;

			case ATTACKER:
				m_bitfields.m_Handling -= ((RandomNumber.IntLessThan(2) == 1) && GetHandling() > MINHANDLING);
				m_bitfields.m_Tackling -= ((RandomNumber.IntLessThan(2) == 1) && GetTackling() > MINTACKLING);
				m_bitfields.m_Passing -= ((RandomNumber.IntLessThan(2) == 1) && GetPassing  () > MINPASSING);
				m_bitfields.m_Shooting -= (GetShooting() > MINSHOOTING);
				break;

			default:
				EXT_ASSERT(false);
				break;
		}
	}
	DoCalculateOverallSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::ImproveSkill
	Access:    	public
	Parameter: 	int IncreaseChance
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::ImproveSkill(int IncreaseChance)
{
	EXT_ASSERT(IncreaseChance > 0);
	if (RandomNumber.IntLessThan(IncreaseChance) == 1)
	{
		switch(GetPosition())
		{
			case  GOALKEEPER:
				m_bitfields.m_Handling += (GetHandling() < MAXHANDLING);
				m_bitfields.m_Tackling += ((RandomNumber.IntLessThan(2) == 1) && GetTackling() < MAXTACKLING);
				m_bitfields.m_Passing += ((RandomNumber.IntLessThan(2) == 1) && GetPassing  () < MAXPASSING);
				m_bitfields.m_Shooting += ((RandomNumber.IntLessThan(2) == 1) && GetShooting() < MAXSHOOTING);
				break;

			case DEFENDER:
				m_bitfields.m_Handling += ((RandomNumber.IntLessThan(2) == 1) && GetHandling() < MAXHANDLING);
				m_bitfields.m_Tackling += (GetTackling() < MAXTACKLING);
				m_bitfields.m_Passing += ((RandomNumber.IntLessThan(2) == 1) && GetPassing  () < MAXPASSING);
				m_bitfields.m_Shooting += ((RandomNumber.IntLessThan(2) == 1) && GetShooting() < MAXSHOOTING);
				break;

			case MIDFIELDER:
				m_bitfields.m_Handling += ((RandomNumber.IntLessThan(2) == 1) && GetHandling() < MAXHANDLING);
				m_bitfields.m_Tackling += ((RandomNumber.IntLessThan(2) == 1) && GetTackling() < MAXTACKLING);
				m_bitfields.m_Passing += (GetPassing  () < MAXPASSING);
				m_bitfields.m_Shooting += ((RandomNumber.IntLessThan(2) == 1) && GetShooting() < MAXSHOOTING);
				break;

			case ATTACKER:
				m_bitfields.m_Handling += ((RandomNumber.IntLessThan(2) == 1) && GetHandling() < MAXHANDLING);
				m_bitfields.m_Tackling += ((RandomNumber.IntLessThan(2) == 1) && GetTackling() < MAXTACKLING);
				m_bitfields.m_Passing += ((RandomNumber.IntLessThan(2) == 1) && GetPassing  () < MAXPASSING);
				m_bitfields.m_Shooting += (GetShooting() < MAXSHOOTING);
				break;

			default:
				EXT_ASSERT(false);
				break;
		}
	}
	DoCalculateOverallSkill();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::MakeVeryStrong
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::MakeVeryStrong()
{
    m_bitfields.m_Pace = 70 + RandomNumber.IntLessThan(30);
	m_bitfields.m_OverallSkill = 92 + RandomNumber.IntLessThan(7);
	DoCreateSkills();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetPositionSkill
	Access:    	public
	Parameter: 	void
	Returns:   	byte
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
byte CSoccerPlayer::GetPositionSkill()
{
    switch(GetPosition())
	{
        case GOALKEEPER:
            return GetHandling();
		case DEFENDER:
            return GetTackling();
        case MIDFIELDER:
            return GetPassing();
        case ATTACKER:
            return GetShooting();
		default:
			EXT_ASSERT(false);
			break;
    }
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::DoGenerateRandomPosition
	Access:    	public
	Parameter: 	CCountry& rCountry
	Returns:   	ePlayerPosition
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ePlayerPosition CSoccerPlayer::DoGenerateRandomPosition(CCountry& _Country)
{
	return(_Country.GetActiveCountry().DoFindRandomPlayer()->GetSkills().GetPosition());
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::CheckExperienceForLevelUp
	Access:    	public
	Parameter:  float CareerPathMultiplier
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoCheckExperienceForLevelUp(float _CareerPathMultiplier)
{
	int RandomValue = RandomNumber.IntLessThan(100);
	while(m_Experience >= EXPERIENCEPOINTSUP)
	{
		m_Experience -= EXPERIENCEPOINTSUP;
		switch(GetPosition())
		{
			case GOALKEEPER:
				DoIncrementHandling();
				break;

			case DEFENDER:
				DoIncrementTackling();
				break;

			case MIDFIELDER:
				DoIncrementPassing();
				break;

			case ATTACKER:
				m_bitfields.m_Shooting++;
				break;

			default:
				EXT_ASSERT(false);
				break;
		}
		// Chance of an extra skill upgrade
		if (RandomNumber.IntLessThan(3) < static_cast<uint>(_CareerPathMultiplier) )
		{
			switch(GetPosition())
			{
				case  GOALKEEPER:
					if (RandomValue < 24)
					{
						m_bitfields.m_Handling--;
					}
					else if (RandomValue < 50)
					{
						m_bitfields.m_Tackling--;
					}
					else if (RandomValue < 76)
					{
						m_bitfields.m_Passing--;
					}
					else
					{
						m_bitfields.m_Heading--;
					}
					break;

				case DEFENDER:
					if (RandomValue < 5)
					{
						m_bitfields.m_Tackling--;
					}
					else if (RandomValue < 60)
					{
						m_bitfields.m_Passing--;
					}
					else if (RandomValue < 70)
					{
						m_bitfields.m_Shooting--;
					}
					else
					{
						m_bitfields.m_Heading--;
					}
					break;

				case MIDFIELDER:
					if (RandomValue < 40)
					{
						m_bitfields.m_Tackling--;
					}
					else if (RandomValue < 45)
					{
						m_bitfields.m_Passing--;
					}
					else if (RandomValue < 70)
					{
						m_bitfields.m_Shooting--;
					}
					else
					{
						m_bitfields.m_Heading--;
					}
					break;

				case ATTACKER:
					if (RandomValue < 20)
					{
						m_bitfields.m_Tackling--;
					}
					else if (RandomValue < 55)
					{
						m_bitfields.m_Passing--;
					}
					else if (RandomValue < 64)
					{
						m_bitfields.m_Shooting--;
					}
					else
					{
						m_bitfields.m_Heading--;
					}
					break;

				default:
					EXT_ASSERT(false);
					break;
			}
		}

		if (RandomNumber.IntLessThan(3) < static_cast<uint>(_CareerPathMultiplier) && m_bitfields.m_Flair < MAXPLAYSKILL)
		{
			m_bitfields.m_Flair++;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::CheckExperienceForLevelDown
	Access:    	public
  	Parameter:  float CareerPathMultiplier
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::DoCheckExperienceForLevelDown(float _CareerPathMultiplier)
{
	int RandomValue1 = RandomNumber.IntLessThan(3);
	int RandomValue = RandomNumber.IntLessThan(100);
  	float Experience = GetExperience();

	while(m_Experience <= EXPERIENCEPOINTSDOWN)
	{
		m_Experience -= EXPERIENCEPOINTSDOWN;
		switch(GetPosition())
		{
			case  GOALKEEPER:
				m_bitfields.m_Handling--;
				break;

			case DEFENDER:
				m_bitfields.m_Tackling--;
				break;

			case MIDFIELDER:
				m_bitfields.m_Passing--;
				break;

			case ATTACKER:
				m_bitfields.m_Shooting--;
				break;

			default:
				EXT_ASSERT(false);
				break;
		}
		// Chance of an extra skill upgrade
		if( RandomValue1 < static_cast<int>(_CareerPathMultiplier) )
		{
			switch(GetPosition())
			{
				case  GOALKEEPER:
					if (RandomValue < 24)
					{
						m_bitfields.m_Handling++;
					}
					else if (RandomValue < 50)
					{
						m_bitfields.m_Tackling++;
					}
					else if (RandomValue < 76)
					{
						m_bitfields.m_Passing++;
					}
					else
					{
						m_bitfields.m_Heading++;
					}
					break;

				case DEFENDER:
					if (RandomValue < 5)
					{
						m_bitfields.m_Tackling++;
					}
					else if (RandomValue < 60)
					{
						m_bitfields.m_Passing++;
					}
					else if (RandomValue < 70)
					{
						m_bitfields.m_Shooting++;
					}
					else
					{
						m_bitfields.m_Heading++;
					}
					break;

				case MIDFIELDER:
					if (RandomValue < 40)
					{
						m_bitfields.m_Tackling++;
					}
					else if (RandomValue < 45)
					{
						m_bitfields.m_Passing++;
					}
					else if (RandomValue < 70)
					{
						m_bitfields.m_Shooting++;
					}
					else
					{
						m_bitfields.m_Heading++;
					}
					break;

				case ATTACKER:
					if (RandomValue < 20)
					{
						m_bitfields.m_Tackling++;
					}
					else if (RandomValue < 55)
					{
						m_bitfields.m_Passing++;
					}
					else if (RandomValue < 64)
					{
						m_bitfields.m_Shooting++;
					}
					else
					{
						m_bitfields.m_Heading++;
					}
					break;

				default:
					EXT_ASSERT(false);
					break;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::SetOverallSkill
	Access:    	public
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::SetOverallSkill(const byte _Value)
{
	m_bitfields.m_OverallSkill = _Value;
	DoCreateSkills();
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::GetTraining
	Access:    	public
	Parameter: 	void
	Returns:   	CSoccerPlayerTraining&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerPlayerTraining& CSoccerPlayer::GetTraining()
{
	return m_Training;
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::Write(CDataFile* _pDataFile)
{
	_pDataFile->Write(&m_Rating, sizeof(m_Rating));
	_pDataFile->Write(&m_Experience, sizeof(m_Experience));
	_pDataFile->Write(&m_Training, sizeof(m_Training));
	_pDataFile->Write(&m_bitfields, sizeof(m_bitfields));
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerPlayer::Read
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerPlayer::Read(CDataFile* _pDataFile)
{
	_pDataFile->Read(&m_Rating, sizeof(m_Rating));
	_pDataFile->Read(&m_Experience, sizeof(m_Experience));
	_pDataFile->Read(&m_Training, sizeof(m_Training));
	_pDataFile->Read(&m_bitfields, sizeof(m_bitfields));
}
