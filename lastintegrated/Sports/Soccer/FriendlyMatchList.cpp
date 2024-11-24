

 /*

    Class:- CFriendlyMatchList


	Football Director

	(c) RW Software 1994 - 2001

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"

#include "FriendlyMatchList.h"
#include "MatchInfo.h"
#include "Stadium.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// Constructor
//---------------------------------------------------------------------------------------
CFriendlyMatchList::CFriendlyMatchList()
{
}


// Destructor
//---------------------------------------------------------------------------------------
CFriendlyMatchList::~CFriendlyMatchList()
{
}


 //-----------------------------------------------------------------------------
void CFriendlyMatchList::Read(FILE* File)
{
	ASSERT (File != NULL);			// File missing
    m_WeekNumbers.Read(File);
	CFixtureList::Read(File);
}


//-----------------------------------------------------------------------------
void CFriendlyMatchList::Write(FILE* File)
{
	ASSERT (File != NULL);			// File missing
    m_WeekNumbers.Write(File);
	CFixtureList::Write(File);
}


//-----------------------------------------------------------------------------
CExtString CFriendlyMatchList::ResultStr(const uint16 ClubID, uint8 WeekNumber)
{
	CExtString str;
    CFixture* pFixture;
    for (uint32 n = 0; n < GetSize(); n++)
	{
		if (m_WeekNumbers.GetAt(n) == WeekNumber)
		{
            pFixture = GetAt(n);
			if (pFixture->IsClubInvolved(ClubID) == true)
			{
				str = pFixture->LatestScoreStr(FULLTIMEEXTRATIME, false);
				break;
			}
		}
	}
	return str;
}


//-----------------------------------------------------------------------------
/*bool CFriendlyMatchList::PlayMatches(const bool bShowSummary)
{
	bool bRet = false;
	uint8 WeekNum = WorldData().GetMatchNumber();
    CFixture* pFixture;
	CMatchVars* MatchVars;
    for (uint32 n = 0; n < GetSize(); n++)
	{
		if (m_WeekNumbers.GetAt(n) == WeekNum)
		{
            pFixture = GetAt(n);
            MatchVars = &(pFixture->getMatchVars());
			pFixture->SetClubPtrs();

			//CMatchPlay MatchPlay(pFixture, NULL, NULL);
            if (pFixture->IsUserMatch() == true && WorldData().DisplayMatchIntro(&MatchPlay, bShowSummary) == true)
			{
				// Add match to matches view list
				WorldData().GetMatchPlay().DoMatch(pFixture, MatchVars->getHomeMatchVars().getClub().pStadium()->GetName());
				bRet = true;
			}
			else
			{
				// Calculate the match result
				pFixture->PlayMatch(WeekNum);
				pFixture->DoShowSummary(bShowSummary, NULL, TEXT(""));
			}
		}
	}
	return bRet;
}*/


 //-----------------------------------------------------------------------------
void CFriendlyMatchList::AddOne(const uint16 HomeTeam, const uint16 AwayTeam, uint8 WeekNumber)
{
	m_WeekNumbers.Add(WeekNumber);
	CMatchInfo Match(FRIENDLYMATCH, 5, 5);
	Match.InitialiseClubs(HomeTeam, AwayTeam);
	Match.SetGroundSize(0);
	Add(new CFixture(Match));

 }


 //---------------------------------------------------------------------------------------
void CFriendlyMatchList::DeleteClubFixtures(const uint16 ClubNumber)
{
    for (uint32 ListPos = GetUpperBound(); ListPos > -1; ListPos--)
	{
        if (GetAt(ListPos)->IsClubInvolved(ClubNumber) == true)
		{
            delete GetAt(ListPos);
			RemoveAt(ListPos);
			m_WeekNumbers.RemoveAt(ListPos);
		}
	}
}


