
/*

	File:- LeagueTableSequences.cpp

	Football Director

	(c) RW Software 1994 - 2004

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


 //---------------------------------------------------------------------------------------
CLeagueTableSequences::CLeagueTableSequences()
{
	DoResetVariables();
}


 //---------------------------------------------------------------------------------------
void CLeagueTableSequences::DoResetVariables(void)
{
    m_NumberSuccessiveWins = 0;
    m_NumberSuccessiveDefeats = 0;
    m_NumberGamesSinceAWin = 0;
    m_NumberGamesSinceADefeat = 0;
}


 //---------------------------------------------------------------------------------------
void CLeagueTableSequences::DrawnGame(void)
{
	m_NumberSuccessiveWins = 0;
    m_NumberSuccessiveDefeats = 0;
    m_NumberGamesSinceAWin ++;
    m_NumberGamesSinceADefeat ++;
}


 //---------------------------------------------------------------------------------------
void CLeagueTableSequences::WonGame(void)
{
	m_NumberGamesSinceAWin = 0;
    m_NumberGamesSinceADefeat ++;
    m_NumberSuccessiveWins ++;
    m_NumberSuccessiveDefeats = 0;
}


 //---------------------------------------------------------------------------------------
void CLeagueTableSequences::LostGame(void)
{
	m_NumberGamesSinceADefeat = 0;
    m_NumberGamesSinceAWin ++;
    m_NumberSuccessiveDefeats ++;
    m_NumberSuccessiveWins = 0;
}
