
/*

	File:- LeagueTableDetails.h

	LeagueTableDetails class


	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------------------------------
*/


#ifndef __LEAGUETABLEDETAILS_H__
#define __LEAGUETABLEDETAILS_H__


class CLeagueTableDetails
{
public:
	// Constructors
	CLeagueTableDetails();
	CLeagueTableDetails(int ClubNumber);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	void					AddAGame(int _AwayScore, int _HomeScore);
	void					DrawnGame(const int _OurScore, const int _TheirScore, const int _PointsForDraw);
	void					DoNewSeason();
	void					DoUpdateTable(int _OurScore, int _TheirScore, int _PointsForWin, int _PointsForDraw, bool _HomeWinPenShootout = false);
	int						GetGoalDifference();		
	byte 					GetLeaguePosition() const;
	byte					GetNumberGamesPlayed();				
	byte					GetNumberGamesWon();				
	byte					GetNumberGamesDrawn();				
	byte					GetNumberGamesLost();				
	byte					GetNumberGoalsScored();				
	byte					GetNumberGoalsConceded();			
	byte 					GetPoints() const;
	void					SetLeaguePosition(const int _Value);
	void					WonGame(const int _OurScore, const int _TheirScore, const int _PointsForWin);

	// Variables
private:
	byte					m_NumberGamesPlayed;
	byte					m_NumberGamesWon;
	byte					m_NumberGamesDrawn;
	byte					m_NumberGoalsScored;
	byte					m_NumberGoalsConceded;
	byte					m_LeaguePosition;
	byte					m_Points;
};
#endif