
/*

	File:- TableData.cpp

	Table Data class

	Football Director

	Copyright 1994-2009 Sports Director Ltd. All rights reserved. 

	---------------------------------------------------------------------------------------
 */


#include "stdafx.h"

#include "MatchInfo.h"
#ifndef TESTBED
#include "UI.h"
#endif


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
CLeagueTable::CLeagueTable(const ClubID _ClubID)
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
#ifndef NDS
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
	m_ClubID.id = NOCLUB;
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
	m_LeagueTableDataAfterMatch.DoNewSeason();
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
ClubID CLeagueTable::GetClubID() const
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
void CLeagueTable::SetClubID(ClubID _ClubID)
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
    _pDataFile->Read(&m_ClubID, sizeof(m_ClubID));
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
    _pDataFile->Write(&m_ClubID, sizeof(m_ClubID));
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
	RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::operator+=
	Access:    	public 
	Parameter: 	CLeagueTableList & list
	Returns:   	CLeagueTableList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTableList& CLeagueTableList::DoAdd( CLeagueTableList& _List )
{
	for (int LoopCount = 0; LoopCount < _List.GetSize(); LoopCount++)
	{
		Add(static_cast<CLeagueTable*>(_List.GetAt(LoopCount)));
	}
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::DoAddInLeaguePositionOrder
	Access:    	public 
	Parameter: 	CLeagueTableList & list
	Returns:   	CLeagueTableList&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTableList& CLeagueTableList::DoAddInLeaguePositionOrder( CLeagueTableList& _List )
{
	for (int LoopCount = 0; LoopCount < _List.GetSize(); LoopCount++)
	{
		Add(static_cast<CLeagueTable*>(_List.GetAtLeaguePosition(LoopCount)));
	}
	return *this;
}


#ifdef NDS
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
		AddNew(FD_ENGINE_NEW CLeagueTable());
	}
}
#endif


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
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
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
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
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
	Method:   	CLeagueTableList::GetAtLeaguePosition
	Access:    	public 
	Parameter: 	int _Position
	Returns:   	CLeagueTable*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CLeagueTable* CLeagueTableList::GetAtLeaguePosition(int _Position)
{
	CLeagueTable* pTableData;
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		pTableData = GetAt(LoopCount);
		if (pTableData->GetLeaguePosition() == _Position)
		{
			break;
		}
		pTableData = NULL;
	}
	EXT_ASSERT(pTableData != NULL);
	return pTableData;
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::SetClubNumbers
	Access:    	public 
	Parameter: 	CNumericArray<ushort> * _ClubIDs
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableList::SetClubIDs(CNumericArray<ClubID>* _ClubIDs)
{
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
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
	CLeagueTable* pTableData;

	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		pTableData = GetAt(LoopCount);
		EXT_ASSERT_POINTER(pTableData, CLeagueTable);
		Elements[LoopCount].Posn = LoopCount;
		Elements[LoopCount].Points = pTableData->GetPoints();
		Elements[LoopCount].GoalsDiff = pTableData->GetGoalDifference();
		Elements[LoopCount].GoalFor = pTableData->GetNumberGoalsScored();
		Elements[LoopCount].TheClub = GetAt(LoopCount)->GetClubID();
	}

	::qsort(Elements, GetSize(), sizeof(StLeagueTable), StLeagueTableSortFunc);

	// Updated the league positions
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++) 
	{
		pTableData = GetAt(Elements[LoopCount].Posn);
		EXT_ASSERT_POINTER(pTableData, CLeagueTable);
		pTableData->SetLeaguePosition(LoopCount);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CLeagueTableList::DoInitialise
	Access:    	public 
	Parameter: 	const ushort _HowMany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CLeagueTableList::DoInitialise(const ushort _HowMany)
{
	SetSize(_HowMany);
	DoCreate(_HowMany);
}
