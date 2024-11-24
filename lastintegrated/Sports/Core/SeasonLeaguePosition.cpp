
 /*

        Class:- Season League Position

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::CSeasonLeagueTableData
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSeasonLeagueTableData::CSeasonLeagueTableData()
{
	DoNewSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::DoUpdateTable
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
void CSeasonLeagueTableData::DoUpdateTable(int _OurScore, int _TheirScore, int _PointsForWin, int _PointsForDraw, bool _HomeWinPenShootout /*= false*/)
{
#ifdef FULLLEAGUETABLEHISTORY
	m_LeaguePosition[m_GamesPlayed].DoUpdateTable(_OurScore, _TheirScore, _PointsForWin, _PointsForDraw, _HomeWinPenShootout);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::SetAt
	Access:    	public 
	Parameter: 	const ushort _Position
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSeasonLeagueTableData::SetLeaguePosition(const ushort _GamesPlayed, const byte _Position)
{
#ifdef FULLLEAGUETABLEHISTORY
	EXT_ASSERT(_Position < SEASONLEAGUEMATCHSIZE);
	m_LeaguePosition[_GamesPlayed].SetLeaguePosition(_Position);
#else
	m_LeaguePosition[_GamesPlayed] = _Position;
#endif
	m_GamesPlayed = _GamesPlayed + 1;
}


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::GetAt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CSeasonLeagueTableData::GetAt(const ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < SEASONLEAGUEMATCHSIZE);
#ifdef FULLLEAGUETABLEHISTORY
	return m_LeaguePosition[_ElementNumber].GetLeaguePosition();
#else
	return m_LeaguePosition[_ElementNumber];
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::IsTrendUp
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CSeasonLeagueTableData::IsTrendUp()
{
#ifdef FULLLEAGUETABLEHISTORY
	if (m_LeaguePosition[m_GamesPlayed - 2].GetLeaguePosition() > m_LeaguePosition[m_GamesPlayed - 1].GetLeaguePosition())
#else
	if (m_LeaguePosition[m_GamesPlayed - 2] > m_LeaguePosition[m_GamesPlayed - 1])
#endif
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::IsTrendDown
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CSeasonLeagueTableData::IsTrendDown()
{
#ifdef FULLLEAGUETABLEHISTORY
	if (m_LeaguePosition[m_GamesPlayed - 2].GetLeaguePosition() < m_LeaguePosition[m_GamesPlayed - 1].GetLeaguePosition())
#else
	if (m_LeaguePosition[m_GamesPlayed - 2] < m_LeaguePosition[m_GamesPlayed - 1])
#endif
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::NewSeason
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSeasonLeagueTableData::DoNewSeason()
{
	m_GamesPlayed = 0;
	for (int LoopCount = 0; LoopCount < SEASONLEAGUEMATCHSIZE; LoopCount++)
	{
#ifdef FULLLEAGUETABLEHISTORY
		m_LeaguePosition[LoopCount].DoNewSeason();
#else
		m_LeaguePosition[LoopCount] = 0;
#endif
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSeasonLeagueTableData::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Read(&m_GamesPlayed, sizeof m_GamesPlayed);
#ifdef FULLLEAGUETABLEHISTORY
	for (int LoopCount = 0; LoopCount < SEASONLEAGUEMATCHSIZE; LoopCount++)
	{
		m_LeaguePosition[LoopCount].Read(_pDataFile);
	}
#else
	_pDataFile->Read(&m_LeaguePosition[0], sizeof m_LeaguePosition[0] * SEASONLEAGUEMATCHSIZE);
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CSeasonLeagueTableData::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSeasonLeagueTableData::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
	_pDataFile->Write(&m_GamesPlayed, sizeof m_GamesPlayed);
#ifdef FULLLEAGUETABLEHISTORY
	for (int LoopCount = 0; LoopCount < SEASONLEAGUEMATCHSIZE; LoopCount++)
	{
		m_LeaguePosition[LoopCount].Write(_pDataFile);
	}
#else
	_pDataFile->Write(&m_LeaguePosition[0], sizeof m_LeaguePosition[0] * SEASONLEAGUEMATCHSIZE);
#endif
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
