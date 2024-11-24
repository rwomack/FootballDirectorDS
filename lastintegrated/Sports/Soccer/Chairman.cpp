
 /*

    Class:- Chairman

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "IndexedIntArray.h"
#include "Chairman.h"



/*------------------------------------------------------------------------------
	Method:   	CChairman::CChairman
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CChairman::CChairman()
{
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CChairman::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CChairman::DoResetVariables()
{
	m_SeasonTargetPosition = 0xff;
	m_TargetPointsForMonth = 0xff;
	m_PointsThisMonth = 0xff;
}


 //---------------------------------------------------------------------------
/*void CChairman::DoMonthlyReport(CDivision& aDivision, ushort iClubID)
{
	if (m_TargetPointsForMonth != 0xff)
	{
		CExtString szStr;
		szStr.LoadString(IDS_MONTHLYTARGETMET);
		if (m_TargetPointsForMonth < m_PointsThisMonth)
		{
			szStr.LoadString(IDS_MONTHLYTARGETBEATEN);
		}
		if (m_TargetPointsForMonth > m_PointsThisMonth)
		{
			szStr.LoadString(IDS_MONTHLYTARGETNOTMET);
		}
		WorldData().getClub(iClubID).AddToClubNews(szStr);
	}
	m_TargetPointsForMonth = 0xff;
	m_PointsThisMonth = 0;

	CPointerArray MatchesThisMonth;
	pDivision->ClubMatchListForCurrentMonth(iClubID, MatchesThisMonth);
	if (MatchesThisMonth.GetSize() > 0)
	{
		m_TargetPointsForMonth = 0;
		for (uint LoopCount = 0; LoopCount < MatchesThisMonth.GetSize(); LoopCount++)
		{
			if (WorldData().getClub(iClubID).pTableData()->GetNumberGamesPlayed() < 4)
			{
			}
			else
			{
			}
		//m_TargetPointsForMonth = pDivision->
		}
		m_TargetPointsForMonth = MatchesThisMonth.GetSize();
		MatchesThisMonth.DeleteAll();
		//szStr.Format(IDS_MONTHLYTARGET, m_TargetPointsForMonth);
		//WorldData().getClub(iClubID).AddToClubNews(szStr);
	}
}*/


/*------------------------------------------------------------------------------
	Method:   	CChairman::DoManagerSacked
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CChairman::DoManagerSacked()
{
}


/*------------------------------------------------------------------------------
	Method:   	CChairman::DoNewSeason
	Access:    	public 
	Parameter: 	CDivision& aDivision
	Parameter: 	const ushort iClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CChairman::DoNewSeason(CDivision& _Division, CClub& _Club)
{
	CIndexedIntArray TotalSkillList;
	for (int LoopCount = 0; LoopCount < _Division.GetNumberOfClubsInDivision(); LoopCount++)
	{
		CClub& theClub = _Division.GetClub(LoopCount);
		if (theClub.IsUserControlled() == false)
		{
			theClub.GetFirstTeamSquad().DoSelectNonUserControlledTeam(3, theClub);
			TotalSkillList.AddNew(theClub.DoFindID(), theClub.GetFirstTeamSquad().DoCalculateAverageTeamPlayerSkill());
		}
		else
		{
			CPlayerSquad PlayerSquad(theClub.GetFirstTeamSquad());
			PlayerSquad.DoSelectTeam(3, theClub);
			TotalSkillList.AddNew(theClub.DoFindID(), PlayerSquad.DoCalculateAverageTeamPlayerSkill());
		}
	}
	TotalSkillList.SortHigh();

	m_SeasonTargetPosition = TotalSkillList.IndexPosition(_Club.DoFindID());
	m_TargetPointsForMonth = 0xff;
	m_PointsThisMonth = 0;

	CExtString szStr;
	if (m_SeasonTargetPosition < 4)
	{
		szStr.LoadString(IDS_AIMFORTITLE);
	}
	else if (m_SeasonTargetPosition < 6)
	{
		if (_Division.GetNumberClubsPromoted() > 1)
		{
			szStr.LoadString(IDS_AIMFORPROMOTION);
		}
		else if (_Division.IsTopDivision() == true)
		{
			szStr.LoadString(IDS_AIMFOREUROPE);
		}
	}
	else if (m_SeasonTargetPosition < _Division.GetNumberOfClubsTopPlayOff () + 2 + _Division.GetNumberClubsPromoted())
	{
		szStr.LoadString(IDS_AIMFORPLAYOFF);
	}
	else if (m_SeasonTargetPosition < _Division.GetNumberOfClubsTopPlayOff () + 2 + _Division.GetNumberClubsPromoted())
	{
		szStr.LoadString(IDS_AVOIDRELEGATION);
	}
	else
	{
		szStr.LoadString(IDS_AIMFORMIDTABLE);
	}
	_Club.AddToClubNews(szStr);
}
