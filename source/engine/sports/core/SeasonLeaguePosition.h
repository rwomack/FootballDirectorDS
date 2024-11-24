

/*

	Class - C++ Classes

	Season League Position class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __SEASONLEAGUEPOSITION_H__
#define __SEASONLEAGUEPOSITION_H__


	static const uint		SEASONLEAGUEMATCHSIZE = 46;

	
class CSeasonLeagueTableData
{
public:
	CSeasonLeagueTableData();

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	void					DoUpdateTable(int _OurScore, int _TheirScore, int _PointsForWin, int _PointsForDraw, bool _HomeWinPenShootout = false);
	void					SetLeaguePosition(const ushort _GamesPlayed, const byte _Position);
	byte					GetAt(const ushort _ElementNumber);
	bool					IsTrendUp();
	bool					IsTrendDown();
	void					DoNewSeason();

private:
	byte					m_GamesPlayed;
#ifdef FULLLEAGUETABLEHISTORY
	CLeagueTableDetails		m_LeaguePosition[SEASONLEAGUEMATCHSIZE];     // Gives league position after every league game played this season
#else
	byte					m_LeaguePosition[SEASONLEAGUEMATCHSIZE];
#endif
};
#endif
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
