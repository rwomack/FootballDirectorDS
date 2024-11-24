
/*

    Match Events Stats Class -

	Football Director

	(c) Rw Software 1994 - 2008

    ---------------------------------------------------------------
*/


#ifndef __MATCHEVENTSSTATS_H__
#define __MATCHEVENTSSTATS_H__

#include <SoccerDefines.h>

class	CMatchEventsStats
{
// Variables
private:
	static const int PERCENTAGEOFPLAYBITS = 7;
	static const int SHOTSONTARGETBITS = 4;
	static const int SHOTSOFFTARGETBITS = 4;
	static const int FREEKICKSBITS = 4;
	static const int OFFSIDESBITS = 4;
	static const int FOULSBITS = 4;
	static const int CORNERSBITS = 4;

	CMatchTeamSkills		m_TeamSkills;
    uint					m_PercentageOfPlay:PERCENTAGEOFPLAYBITS;
    uint					m_ShotsOnTarget:SHOTSONTARGETBITS;
    uint					m_ShotsOffTarget:SHOTSOFFTARGETBITS;
	uint					m_FreeKicks:FREEKICKSBITS;
	uint					m_OffSides:OFFSIDESBITS;
	uint					m_Fouls:FOULSBITS;
	uint					m_Corners:CORNERSBITS;
    ClubID					m_ClubID;			// Club number of team
#ifdef STOREMATCHINJURIES
	byte					m_Injuries;
#endif
#ifdef STOREMATCHPOSESSION
    float                  m_Posession;
#endif

public:
	CMatchEventsStats();

	void					CopyStats(CMatchEventsStats& pStats);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

// Methods
	CMatchTeamSkills&		GetTeamSkills();
    ClubID					GetClubID() const;
    sbyte					GetShotsOnTarget();
    sbyte					GetShotsOffTarget();
    byte					GetOffsides();
    byte					GetFouls();
    byte					GetCorners();
    byte					GetFreeKicks();
    byte					GetPercentageOfPlay() const;
    void					DoIncrementCorners();
    void					DoIncrementFreeKicks();		
    void					DoIncrementOffsides();
	void					DoIncrementFouls();
    void					DoIncrementShotsOnTarget();
    void					DoIncrementShotsOffTarget();
	void					DoAddShotsOnTarget(const byte _Value);
	void 					SetShotsOnTarget(const byte _Value);
	void 					SetShotsOffTarget(const byte _Value);
	void 					SetCorners(const byte _Value);
	void 					SetFreeKicks(const byte _Value);
	void 					SetOffsides(const byte _Value);
	void					SetFouls(const byte _Value);
    void					SetPercentageOfPlay(const byte _Value);
	void					SetClubID(const ClubID _ClubID);
	void					DoResetVariables();
	byte					Performance();

#ifdef STOREMATCHPOSESSION
	void				DoIncrementPosession()		          	{ m_Posession++; }
	void				DoDecrementPosession()		          	{ m_Posession--; }
#endif
#ifdef STOREMATCHINJURIES
    void				SetInjuries(const byte x)	   				{ m_Injuries = x; }
#endif
};
#endif
