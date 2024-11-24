
/*

	File:- TableData.h

	TableData class


	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------------------------------
*/


#ifndef __LEAGUETABLE_H__
#define __LEAGUETABLE_H__


	struct StLeagueTable
	{
		ushort Posn;
		uint Points;
		uint GoalFor;
		signed int GoalsDiff;
	};

	class CGrid;
	class CMatchInfo;


class CLeagueTable 
{ 
public:
	// Constructors
	CLeagueTable();
	CLeagueTable(const ushort ClubNumber);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Methods
	void					DoAwayUpdateTable(int _HomeScore, int _AwayScore, int _PointsForWin, int _PointsForDraw, bool _HomeWinPenShootout = false);
#ifndef TESTBED
	void					DoBuildLeagueTableRow(const ushort _Row, CGrid& _Grid);
#endif
	void					DoHomeUpdateTable(int _HomeScore, int _AwayScore, int _PointsForWin, int _PointsForDraw, bool _HomeWinPenShootout = false);
    void					DoResetVariables();
    void					DoNewSeason();
	CLeagueTableDetails*	GetAwayGames();
	ushort					GetClubID() const;
	int						GetGoalDifference();
	CLeagueTableDetails*	GetHomeGames();
	byte					GetLeaguePosition() const;
	CSeasonLeagueTableData* GetLeaguePositionAfterMatch();
	byte					GetNumberGamesWon();
	byte 					GetNumberGamesDrawn();
	byte 					GetNumberGamesLost();
	byte 					GetNumberGoalsScored();
	byte 					GetNumberGoalsConceded();
	byte 					GetNumberGamesPlayed();
	byte 					GetPoints() const;
	void					SetClubID(ushort _ClubID);
	void					SetLeaguePosition(const int _Value);

	// Variables
private:
	ushort					m_ClubID;			// Not currently used in club
	CLeagueTableDetails		m_HomeGames;
	CLeagueTableDetails		m_AwayGames;
	CSeasonLeagueTableData	m_LeagueTableDataAfterMatch;     // Gives league position after every league game played this season

#ifdef STOREGAMESEQUENCERECORDS
	CLeagueTableSequences	m_Sequences;
	CLeagueTableSequences	m_HomeSequences;
	CLeagueTableSequences	m_AwaySequences;
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CLeagueTableList : public CPointerArray<CLeagueTable>
{
public:
	// Constructor
	CLeagueTableList()		{};
	~CLeagueTableList();

	//void					AddPercentagePointsFromDivision(byte PercentagePointsFromDivision);
	CLeagueTableList&		DoAdd(CLeagueTableList& list);
	void					DoCreate(int _Count);
	void					DoNewSeason(void);
	void					DoSort(void);
	void					DoUpdate(CMatchInfo& _MatchInfo);
	void					SetClubIDs(CNumericArray<ushort>* _ClubIDs);

	// Variables
private:

};
#endif
