
 /*

    File :- OutOfContractPlayer.cpp

    The out of contract player class routines


	 Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "PlayerSearch.h"
#include "CupTiedPlayer.h"


const int CHANCEOUTOFCONTRACTPLAYERFINDCLUB = 40;


/*------------------------------------------------------------------------------
	Method:   	COutOfContractPlayerList::DoWeeklyTasks
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COutOfContractPlayerList::DoEndOfWeekTasks()
{
#ifdef _DEBUG
//	afxDump << " OutOfContractPlayers Start " << GetSize() << "\n";
#endif
	if (GetSize() > 0)
	{
		CPlayer* pPlayer;
		CClub* pClub;
		for (short LoopCount = GetSize() - 1; LoopCount >= 0; LoopCount--)
		{
			if (RandomNumber.IntLessThan(100) < CHANCEOUTOFCONTRACTPLAYERFINDCLUB)
			{
				pPlayer = WorldData().GetPlayer(operator[](LoopCount).GetPlayerID());
				pClub = pPlayer->DoFindSuitableNewClub(&WorldData().GetClub(operator[](LoopCount).GetClubID()));
				if (pClub != null)
				{
					CContract PlayerContract;
					pPlayer->DoGenerateRandomContract(PlayerContract, pClub->GetCountry());
					DoSignedForClub(LoopCount, PlayerContract, pClub);
#ifdef _DEBUG
//		afxDump << "Out Of Contract Signing " << pClub->GetName() << " " << pPlayer->GetName() << "\n";
#endif
				}
			}
		}
	}
#ifdef _DEBUG
//	afxDump << " OutOfContractPlayers End " << GetSize() << "\n";
#endif
}


/*------------------------------------------------------------------------------
	Method:   	COutOfContractPlayerList::DoSignedForClub
	Access:    	public 
	Parameter: 	const Int16 nListID
	Parameter: 	CContract& _PlayerContract
	Parameter: 	CClub* pClub
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COutOfContractPlayerList::DoSignedForClub(const short _ListID, CContract& _PlayerContract, CClub* _Club)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	_Club->DoBuyTransferPlayer(*WorldData().GetPlayer(operator[](_ListID).GetPlayerID()), 0, _PlayerContract, null, 0);
	CClubAndPlayerIDList::m_List.RemoveAt(_ListID);
}


/*------------------------------------------------------------------------------
	Method:   	COutOfContractPlayerList::DoBuildSuitableList
	Access:    	public 
	Parameter: 	CPlayerSearch& PlayerSearch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void COutOfContractPlayerList::DoBuildSuitableList(CPlayerSearch& PlayerSearch)
{
    CPlayer* pPlayer;
	for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
		pPlayer = WorldData().GetPlayer(operator[](LoopCount).GetPlayerID());
        if (pPlayer->ValidSearchListed(PlayerSearch) == true)
		{
			PlayerSearch.DoAddToList(operator[](LoopCount).GetPlayerID(), wNOTFOUND, 0);
		}
	}
 }
