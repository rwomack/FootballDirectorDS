
/*

	SoccerDefines.h


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#ifndef __SOCCERDEFINES_H__
#define __SOCCERDEFINES_H__


	enum eCupEventType
	{
		DRAWDATE,
		MATCHDATE,
		REPLAYDATE
	};

	enum eMatchDayEffect
	{
		NODAYTYPE,
		GOODDAY,
		BADDAY
	};

	const byte				MAXBYTEVAL = 0xff;

	const uint				MAXHUMANMANAGERS = 1;

	const uint				HALFTIME = 45;
	const uint				FULLTIME = 90;
	const uint				HALFTIMEEXTRATIME = 105;
	const uint				FULLTIMEEXTRATIME = 120;
    
	const uint				CHANCEREFUSECONTRACT = 40;							// May refuse to sign a new contract
	const uint				CHANCEREQUESTNEWCONTRACT = 5;
    const uint				CHANCENEWCONTRACTREQUEST = 30;
	const uint				CHANCESELLUNLISTEDPLAYER = 10;
	const uint				ATTEMPTSFINDNONUSERCLUBINDIVISION = 50;
	
	const uint				SELLATTEMPTS =  4;					// Number of attempts a managed club has to receive a bid for a player from a non managed club
    const uint				MAXIMUMMATCHLENGTH = 130;

	const ushort			NOPLAYER = 0xffff;					// the squad index is set to this player if no player
	const ushort			NOMANAGER =	0xffff;					// the manager number is set to this if no manager
    const ushort			NOCLUB = 0xffff;
    const int				RENEWMANAGER =	-2;
	const ushort			NUMTRANSFERWINDOWS = 4;

	const uint				MAXSUBS = 5;
    const uint				EMPLOYEESINSQUAD = 6;
    const uint				MAXAVAILWEEKS = 5;

    const byte				MINPLAYSKILL = 10;				// Player skill cannot go below this
    const byte				MAXPLAYSKILL = 99;				// Player skill cannot go above this

    const uint				MINTEMPER = 0;
    const uint				MAXTEMPER = 99;

    const uint				EMPLOYEERETIREMENTAGE = 65;
    const uint				YOUTHWEEKLYWAGE = 125;

	enum eListedType
	{
		PLYLISTED,
		PLYOUTOFCONTRACT,
		PLYAVAILFORLOAN,
		PLYUNLISTED
	};

	enum eBESTFOOT
	{
		LEFTFOOT,
		RIGHTFOOT
	};

	// Person Types
	enum ePersonType
	{
		PLAYER,
		EMPLOYEE,
		AVAILEMPLOYEE,
		YOUTH,
		AVAILYOUTH,
		REFEREE,
		MANAGER
	};

    enum  eWhichTeam
	{
		HOME,
		AWAY,
		NOMATCH
	};

	const uint 			NOGAMESPLAYED = 0xffffffff;

    enum eDisciplineEvent
	{
		BOOKPLAYER,
		SENDPLAYEROFF
	};

	enum ePlayerPerformanceEvent
	{
		PERFORMANCEBOOKED,
		PERFORMANCESENTOFF,
	};
	
	// The type of match
	enum eMatchCompetitionType
	{
		LEAGUEMATCH,
		CUPMATCH,
		REPLAYMATCH,
		FEDERATIONCUPMATCH,
		CUPMATCHNEUTRALVENUE,
		REPLAYMATCHNEUTRALVENUE,
		FEDERATIONCUPMATCHNEUTRALVENUE,
		FRIENDLYMATCH,
		NOTAMATCH
	};

	enum eClubResultsType
	{
		MATCHES_NONE,
		MATCHES_ALL,
		MATCHES_PLAYED,
		MATCHES_WON,
		MATCHES_DRAWN,
		MATCHES_LOST,
		MATCHES_SCORED,
		MATCHES_CONCEDED,
		MATCHES_HOME_PLAYED,
		MATCHES_HOME_WON,
		MATCHES_HOME_DRAWN,
		MATCHES_HOME_LOST,
		MATCHES_HOME_SCORED,
		MATCHES_HOME_CONCEDED,
		MATCHES_AWAY_PLAYED,
		MATCHES_AWAY_WON,
		MATCHES_AWAY_DRAWN,
		MATCHES_AWAY_LOST,
		MATCHES_AWAY_SCORED,
		MATCHES_AWAY_CONCEDED,
		MATCHES_FORM_PLAYED,
		MATCHES_FORM_WON,
		MATCHES_FORM_DRAWN,
		MATCHES_FORM_LOST,
		MATCHES_FORM_SCORED,
		MATCHES_FORM_CONCEDED
	};

	// Sound Effect IDs
	enum eSOUNDFX
	{
		SOUND_BOO,
		SOUND_CHEER,
		SOUND_CROWDLRG,
		SOUND_CROWDMED,
		SOUND_CROWDSML,
		SOUND_FULLWHST,
		SOUND_GOALCHEE,
		SOUND_MISS,
		SOUND_ROAR,
		SOUND_WHISTLE,
		SOUND_BEEP,
		SOUND_MAX
	};

    const uint				LOWPOPULARITY = 12;
#endif