
/*

	File:- TableData.cpp

	Table Data class

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "MatchInfo.h"
#ifndef TESTBED
#include "UI.h"
#endif

//---------------------------------------------------------------------------------------
CLeagueTableDetails::CLeagueTableDetails()
{
	DoNewSeason();
}

/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::DoNewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableDetails::DoNewSeason()
{
	m_LeaguePosition = 0;
	m_NumberGamesPlayed = m_NumberGamesWon = 0;
    m_NumberGamesDrawn = 0;
    m_NumberGoalsScored = m_NumberGoalsConceded = 0;
	m_Points = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::DoUpdateTable
	Access:    	public 
	Parameter: 	int _OurScore
	Parameter: 	int _TheirScore
	Parameter: 	int _PointsForWin
	Parameter: 	int _PointsForDraw
	Parameter: 	bool _HomeWinPenShootout
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableDetails::DoUpdateTable(int _OurScore, int _TheirScore, int _PointsForWin, int _PointsForDraw, bool _HomeWinPenShootout /*= false*/)
{
	if (_OurScore == _TheirScore && _PointsForDraw > 0)
	{
		// draw
		DrawnGame(_OurScore, _TheirScore, _PointsForDraw);
	}

	if (_OurScore > _TheirScore || (_HomeWinPenShootout == true && _PointsForDraw == 0))
	{
		// win
		WonGame(_OurScore, _TheirScore, _PointsForWin);
	}

	if (_OurScore < _TheirScore || (_HomeWinPenShootout == false && _PointsForDraw == 0))
	{
		// lose
		AddAGame(_OurScore, _TheirScore);
	}
}



/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::WonGame
	Access:    	public 
	Parameter: 	const int _HomeScore
	Parameter: 	const int _AwayScore
	Parameter: 	const int _PointsForWin
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableDetails::WonGame(const int _OurScore, const int _TheirScore, const int _PointsForWin)
{
	m_NumberGamesWon++;
	m_Points += _PointsForWin;
	AddAGame(_OurScore, _TheirScore);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::DrawnGame
	Access:    	public 
	Parameter: 	const int _HomeScore
	Parameter: 	const int _AwayScore
	Parameter: 	const int _PointsForDraw
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableDetails::DrawnGame(const int _OurScore, const int _TheirScore, const int _PointsForDraw)
{
	m_NumberGamesDrawn++;
	m_Points += _PointsForDraw;
	AddAGame(_OurScore, _TheirScore);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::AddAGame
	Access:    	public 
	Parameter: 	int _OurGoals
	Parameter: 	int _TheirGoals
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableDetails::AddAGame(int _OurGoals, int _TheirGoals)
{
	m_NumberGoalsScored += _OurGoals;
	m_NumberGoalsConceded += _TheirGoals;
	m_NumberGamesPlayed++;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetLeaguePosition
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTableDetails::GetLeaguePosition() const
{
	return m_LeaguePosition;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::SetLeaguePosition
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableDetails::SetLeaguePosition(const int _Value)
{
	EXT_ASSERT(_Value < 30); // League position seems high
	m_LeaguePosition = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetPoints
	Access:    	public 
	Parameter: 	uint _Win
	Parameter: 	uint _Draw
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTableDetails::GetPoints() const
{
	return m_Points;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetGoalDifference
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CLeagueTableDetails::GetGoalDifference()
{
	return GetNumberGoalsScored() - GetNumberGoalsConceded();
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetNumberGamesPlayed
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTableDetails::GetNumberGamesPlayed()
{
	return m_NumberGamesPlayed;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetNumberGamesWon
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTableDetails::GetNumberGamesWon()
{
	return m_NumberGamesWon;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetNumberGamesDrawn
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTableDetails::GetNumberGamesDrawn()
{
	return m_NumberGamesDrawn;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetNumberGamesLost
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTableDetails::GetNumberGamesLost()
{
	return m_NumberGamesPlayed - (m_NumberGamesWon + m_NumberGamesDrawn);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetNumberGoalsScored
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTableDetails::GetNumberGoalsScored()
{
	return m_NumberGoalsScored;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::GetNumberGoalsConceded
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTableDetails::GetNumberGoalsConceded()
{
	return m_NumberGoalsConceded;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableDetails::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Read(&m_NumberGamesPlayed, sizeof m_NumberGamesPlayed);
	_pDataFile->Read(&m_NumberGamesWon, sizeof m_NumberGamesWon);
	_pDataFile->Read(&m_NumberGamesDrawn, sizeof m_NumberGamesDrawn);
	_pDataFile->Read(&m_NumberGoalsScored, sizeof m_NumberGoalsScored);
	_pDataFile->Read(&m_NumberGoalsConceded, sizeof m_NumberGoalsConceded);
	_pDataFile->Read(&m_LeaguePosition, sizeof m_LeaguePosition);
	_pDataFile->Read(&m_Points, sizeof m_Points);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableDetails::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableDetails::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Write(&m_NumberGamesPlayed, sizeof m_NumberGamesPlayed);
	_pDataFile->Write(&m_NumberGamesWon, sizeof m_NumberGamesWon);
	_pDataFile->Write(&m_NumberGamesDrawn, sizeof m_NumberGamesDrawn);
	_pDataFile->Write(&m_NumberGoalsScored, sizeof m_NumberGoalsScored);
	_pDataFile->Write(&m_NumberGoalsConceded, sizeof m_NumberGoalsConceded);
	_pDataFile->Write(&m_LeaguePosition, sizeof m_LeaguePosition);
	_pDataFile->Write(&m_Points, sizeof m_Points);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::CLeagueTable
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTable::CLeagueTable()
{
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::CLeagueTable
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTable::CLeagueTable(const ushort _ClubID)
{
	DoResetVariables();
	m_ClubID = _ClubID;
};


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::DoBuildLeagueTableRow
	Access:    	public 
	Parameter: 	const ushort _Row
	Parameter: 	CGrid & _Grid
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
#ifndef TESTBED
void CLeagueTable::DoBuildLeagueTableRow(const ushort _Row, CGrid& _Grid)
{
	_Grid.SetItemNum(_Row, 1, GetNumberGamesPlayed());
	_Grid.SetItemNum(_Row, 2, GetNumberGamesWon());
	_Grid.SetItemNum(_Row, 3, GetNumberGamesDrawn());
	_Grid.SetItemNum(_Row, 4, GetNumberGamesLost());
	_Grid.SetItemNum(_Row, 5, GetNumberGoalsScored());
	_Grid.SetItemNum(_Row, 6, GetNumberGoalsConceded());
	_Grid.SetItemNum(_Row, 7, GetPoints());
}
#endif

/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTable::DoResetVariables()
{
	m_ClubID = NOCLUB;
    DoNewSeason();
#ifdef STOREGAMESEQUENCERECORDS
	m_Sequences.DoResetVariables();
	m_HomeSequences.DoResetVariables();
	m_AwaySequences.DoResetVariables();
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::DoNewSeason
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTable::DoNewSeason()
{
	m_HomeGames.DoNewSeason();
	m_AwayGames.DoNewSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::DoHomeUpdateTable
	Access:    	public 
	Parameter: 	int _HomeScore
	Parameter: 	int _AwayScore
	Parameter: 	int _PointsForWin
	Parameter: 	int _PointsForDraw
	Parameter: 	bool _HomeWinPenShootout
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTable::DoHomeUpdateTable(int _HomeScore, int _AwayScore, int _PointsForWin, int _PointsForDraw, bool _HomeWinPenShootout /*= false*/)
{
	if (_HomeScore == _AwayScore && _PointsForDraw > 0)
	{
    	// draw
		m_HomeGames.DrawnGame(_HomeScore, _AwayScore, _PointsForDraw);
#ifdef STOREGAMESEQUENCERECORDS
		m_Sequences.DrawnGame();
		m_HomeSequences.DrawnGame();
#endif
     }

   	if (_HomeScore > _AwayScore || (_HomeWinPenShootout == true && _PointsForDraw == 0))
	{
    	// home win
		m_HomeGames.WonGame(_HomeScore, _AwayScore, _PointsForWin);
#ifdef STOREGAMESEQUENCERECORDS
		m_Sequences.WonGame();
		m_HomeSequences.WonGame();
#endif
     }

     if (_HomeScore < _AwayScore || (_HomeWinPenShootout == false && _PointsForDraw == 0))
	 {
     	// away win
		m_HomeGames.AddAGame(_HomeScore, _AwayScore);
#ifdef STOREGAMESEQUENCERECORDS
		m_Sequences.LostGame();
		m_HomeSequences.LostGame();
#endif
     }
	 m_LeagueTableDataAfterMatch.DoUpdateTable(_HomeScore, _AwayScore, _PointsForWin, _PointsForDraw, _HomeWinPenShootout);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::DoAwayUpdateTable
	Access:    	public 
	Parameter: 	int _HomeScore
	Parameter: 	int _AwayScore
	Parameter: 	int _PointsForWin
	Parameter: 	int _PointsForDraw
	Parameter: 	bool _HomeWinPenShootout
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTable::DoAwayUpdateTable(int _HomeScore, int _AwayScore, int _PointsForWin, int _PointsForDraw, bool _HomeWinPenShootout /*= false*/)
{
	if (_HomeScore == _AwayScore && _PointsForDraw > 0)
	{
     	// draw
		m_AwayGames.DrawnGame(_AwayScore, _HomeScore, _PointsForDraw);
#ifdef STOREGAMESEQUENCERECORDS
		m_Sequences.DrawnGame();
		m_AwaySequences.DrawnGame();
#endif
    }

    if (_HomeScore > _AwayScore || (_HomeWinPenShootout == true && _PointsForDraw == 0))
	{
     	// home win
		m_AwayGames.AddAGame(_AwayScore, _HomeScore);
#ifdef STOREGAMESEQUENCERECORDS
		m_Sequences.LostGame();
		m_AwaySequences.LostGame();
#endif
    }

    if (_HomeScore < _AwayScore || (_HomeWinPenShootout == false && _PointsForDraw == 0))
	{
      	// away win
        m_AwayGames.WonGame(_AwayScore, _HomeScore, _PointsForWin);
#ifdef STOREGAMESEQUENCERECORDS
		m_Sequences.WonGame();
		m_AwaySequences.WonGame();
#endif
    }
	m_LeagueTableDataAfterMatch.DoUpdateTable(_HomeScore, _AwayScore, _PointsForWin, _PointsForDraw, _HomeWinPenShootout);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetHomeGames
	Access:    	public 
	Returns:   	CLeagueTableDetails*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTableDetails* CLeagueTable::GetHomeGames()
{ 
	return &m_HomeGames; 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetAwayGames
	Access:    	public 
	Returns:   	CLeagueTableDetails*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTableDetails* CLeagueTable::GetAwayGames()
{ 
	return &m_AwayGames; 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetClubID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CLeagueTable::GetClubID() const
{ 
	return m_ClubID; 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetNumberGamesWon
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTable::GetNumberGamesWon()
{ 
	return m_HomeGames.GetNumberGamesWon() + m_AwayGames.GetNumberGamesWon(); 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetNumberGamesDrawn
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTable::GetNumberGamesDrawn()
{ 
	return m_HomeGames.GetNumberGamesDrawn() + m_AwayGames.GetNumberGamesDrawn(); 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetNumberGamesLost
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTable::GetNumberGamesLost()
{ 
	return m_HomeGames.GetNumberGamesLost() + m_AwayGames.GetNumberGamesLost(); 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetNumberGoalsScored
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTable::GetNumberGoalsScored()
{ 
	return m_HomeGames.GetNumberGoalsScored() + m_AwayGames.GetNumberGoalsScored(); 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetNumberGoalsConceded
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTable::GetNumberGoalsConceded()
{ 
	return m_HomeGames.GetNumberGoalsConceded() + m_AwayGames.GetNumberGoalsConceded(); 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetNumberGamesPlayed
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTable::GetNumberGamesPlayed()
{ 
	return m_HomeGames.GetNumberGamesPlayed() + m_AwayGames.GetNumberGamesPlayed(); 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetTablePosition
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTable::GetLeaguePosition() const
{ 
	return m_HomeGames.GetLeaguePosition(); 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetPoints
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CLeagueTable::GetPoints() const
{ 
	return m_AwayGames.GetPoints() + m_HomeGames.GetPoints(); 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetGoalDifference
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CLeagueTable::GetGoalDifference()
{
	return m_HomeGames.GetGoalDifference() + m_AwayGames.GetGoalDifference ();
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::GetLeaguePositionAfterMatch
	Access:    	public 
	Returns:   	CSeasonLeagueTableData*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSeasonLeagueTableData* CLeagueTable::GetLeaguePositionAfterMatch()
{
	return  &m_LeagueTableDataAfterMatch;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::SetClubID
	Access:    	public 
	Parameter: 	ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTable::SetClubID(ushort _ClubID)
{ 
	m_ClubID = _ClubID; 
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::SetLeaguePosition
	Access:    	public 
	Parameter: 	const int _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTable::SetLeaguePosition(const int _Value)
{
	EXT_ASSERT(_Value < 30); // League position seems high
	m_HomeGames.SetLeaguePosition(_Value);
	m_AwayGames.SetLeaguePosition(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTable::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Read(&m_ClubID, sizeof m_ClubID);
	m_HomeGames.Read(_pDataFile);
	m_AwayGames.Read(_pDataFile);
	m_LeagueTableDataAfterMatch.Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTable::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTable::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Write(&m_ClubID, sizeof m_ClubID);
	m_HomeGames.Write(_pDataFile);
	m_AwayGames.Write(_pDataFile);
	m_LeagueTableDataAfterMatch.Write(_pDataFile);
}
/////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::~CLeagueTableList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTableList::~CLeagueTableList()		
{
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::operator+=
	Access:    	public 
	Parameter: 	CLeagueTableList & list
	Returns:   	CLeagueTableList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTableList& CLeagueTableList::DoAdd(CLeagueTableList& list)
{
	for (int LoopCount = 0; LoopCount < list.GetSize(); LoopCount++)
	{
		Add(static_cast<CLeagueTable*>(list.GetAt(LoopCount)));
	}
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::DoCreate
	Access:    	public 
	Parameter: 	int _Count
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableList::DoCreate(int _Count)
{
	for (int LoopCount = 0; LoopCount < _Count; LoopCount++)
	{
		AddNew(new CLeagueTable());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::DoNewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableList::DoNewSeason(void)
{
	for (int LoopCount = 0; LoopCount < GetSize (); LoopCount++)
	{
		GetAt(LoopCount)->DoNewSeason();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::DoUpdate
	Access:    	public 
	Parameter: 	CMatchInfo & _MatchInfo
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableList::DoUpdate(CMatchInfo& _MatchInfo)
{
	CLeagueTable* pTableData;
	for (int LoopCount = 0; LoopCount < GetSize (); LoopCount++)
	{
		pTableData = GetAt(LoopCount);
		EXT_ASSERT_POINTER(pTableData, CLeagueTable);
		// If the match has not been played yet then attendance will be 0
		if (_MatchInfo.GetFixture()->HasMatchBeenPlayed() == true) 
		{
			if (pTableData->GetClubID() == (_MatchInfo.GetHomeClubID())) 
			{
				pTableData->DoHomeUpdateTable(_MatchInfo.GetFixture()->GetNumberGoals(HOME), _MatchInfo.GetFixture()->GetNumberGoals(AWAY), 3, 1);
			}
			if (pTableData->GetClubID() == _MatchInfo.GetAwayClubID()) 
			{
				pTableData->DoAwayUpdateTable(_MatchInfo.GetFixture()->GetNumberGoals(HOME), _MatchInfo.GetFixture()->GetNumberGoals(AWAY), 3, 1);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::SetClubNumbers
	Access:    	public 
	Parameter: 	CNumericArray<ushort> * _ClubIDs
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableList::SetClubIDs(CNumericArray<ushort>* _ClubIDs)
{
	EXT_ASSERT(_ClubIDs->GetSize () == GetSize ());
	for (int LoopCount = 0; LoopCount < GetSize (); LoopCount++)
	{
		GetAt (LoopCount)->SetClubID(_ClubIDs->GetAt(LoopCount));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::DoSort
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableList::DoSort(void)
{
	StLeagueTable Elements[26];
	StLeagueTable Element;
	int ra = 0;
	CLeagueTable* pTableData;

	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		pTableData = GetAt(LoopCount);
		EXT_ASSERT_POINTER(pTableData, CLeagueTable);
		Elements[LoopCount].Posn = LoopCount;
		Elements[LoopCount].Points = pTableData->GetPoints();
		Elements[LoopCount].GoalsDiff = pTableData->GetGoalDifference();
		Elements[LoopCount].GoalFor = pTableData->GetNumberGoalsScored();
	}

	// sort into order, points first
	// if points equal then best goal difference
	// if goal difference equal then highest goals scored
	for (int num = 1; num < GetSize (); num ++)
	{
		ra = num - 1;
		Element = Elements[num];

		do 
		{
			if (Elements [ra].Points > Element.Points) 
			{
				break;
			}

			if (Elements[ra].Points == Element.Points && Elements[ra].GoalsDiff > Element.GoalsDiff) 
			{
				break;
			}

			if (Elements[ra].Points == Element.Points && 
				Elements[ra].GoalsDiff == Element.GoalsDiff && 
				Elements[ra].GoalFor > Element.GoalFor) 
			{
				break;
			}

			Elements[ra + 1] = Elements[ra];
			Swap(ra, ra + 1);
			ra--;
		} 
		while(ra >= 0);

		Elements[ra + 1] = Element;
	}

	// Updated the league positions
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++) 
	{
		pTableData = GetAt(LoopCount);
		EXT_ASSERT_POINTER(pTableData, CLeagueTable);
		pTableData->SetLeaguePosition(LoopCount);
	}
}
