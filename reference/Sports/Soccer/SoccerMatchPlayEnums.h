
/*

    File:- SoccerMatchPlayEnums.h


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __SOCCERMATCHPLAYENUMS_H__
#define __SOCCERMATCHPLAYENUMS_H__
// Ball position display
	enum
	{
		BALL_WIDTH = 16,
		PITCHX1 = 3,
		PITCHX2 = 9,
		MAXPITCHX = 12,
		MAXPITCHY = 8,
		PITCHCENTREX = 6,
		PITCHCENTREY = 4,
		PITCHGOALY = 4,

		PITCH_WIDTH = 226,
		PITCH_HEIGHT = 112,
		PITCH_STEPX = (PITCH_WIDTH - BALL_WIDTH) / MAXPITCHX,
		PITCH_STEPY = (PITCH_HEIGHT - BALL_WIDTH) / MAXPITCHY,
	};


// Event likelyhood percent chances
	enum eFreeKickChance
	{
		FREEKICKINDIRECT = 35,
		FREEKICKDEFLECTED = 15,
		FREEKICKCLEARED = 15,
		FREEKICKOFFTARGET = 10,
		FREEKICKONTARGET = 5,
		FREEKICKGOAL = 20
	};

	enum eFreeKickType
	{
		FREEKICKTYPEINDIRECT,
		FREEKICKTYPEDEFLECTED,
	    FREEKICKTYPECLEARED,
		FREEKICKTYPEOFFTARGET,
		FREEKICKTYPEONTARGET,
		FREEKICKTYPEGOAL
	};

	enum eCornerChance
	{
		CORNERDEFLECTED = 15,
		CORNERCLEARED = 30,
		CORNEROFFTARGET = 8,
		CORNERONTARGET = 7,
		CORNERGOAL = 40
	};

	enum eCornerType
	{
		CORNERTYPEDEFLECTED,
		CORNERTYPECLEARED,
		CORNERTYPEOFFTARGET,
		CORNERTYPEONTARGET,
		CORNERTYPEGOAL
	};

	enum eMatchEventPercentChance
	{
		HEADERCHANCE = 8,						// 1 in 10 >= then shot was a header
		CHANCEDISALLOWEDGOAL = 30,
		CHANCEOWNGOAL = 60,						// 1 goal in this value is likely to be an own goal
		CHANCEBOOKINGPENALTY = 15,				// 1 booking in this value is likely to result in a penalty
		CHANCESENDINGOFFPENALTY = 5,			// 1 sending off in this value is likely to result in a penalty
		CHANCESENDINGOFF = 4,					// 1 in nn chance of sending off this value
		SHOTADVANTAGE = 20,
		MAXPENALTYSHOOTOUTSHOTS = 5,
		CHANCEINJUREPLAYER = 4,
		CHANCEGOALKEEPERINJURED = 60,			// 1 in nn chance of selecting goalkeeper when selecting player to injure
		CHANCEMIDFIELDEROFFSIDE = 8,				// 1 in nn chance of a midfielder being caught offside instead of attacker
		CHANCESENDOFF = 90,
		CHANCEBOOK = 90
	};


#endif