
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
        CClub *pPreviousClub = &WorldData().GetClubByID(operator[](LoopCount).GetClubID());
        pClub = pPlayer->DoFindSuitableNewClub( pPreviousClub );
//        lLogFmt("Player: %s\n", pPlayer->GetName());
//        if( pPreviousClub ) lLogFmt("PreviousClub: %s\n", pPreviousClub->GetName());
        //if( pClub ) lLogFmt("NewClub: %s\n", pClub->GetName());
				if (pClub != NULL)
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
	Parameter: 	const short _ListID
	Parameter: 	CContract & _PlayerContract
	Parameter: 	CClub * _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void COutOfContractPlayerList::DoSignedForClub(const short _ListID, CContract& _PlayerContract, CClub* _Club)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	CPlayer& thePlayer = *WorldData().GetPlayer(operator[](_ListID).GetPlayerID());
	_Club->DoBuyTransferPlayer(thePlayer, 0, _PlayerContract, NULL, 0);

	 // Add to news
	CExtString str;
  str.Format(IDS_PLAYER_SIGNED, CNewsItem::PreprocessPlayerName(&thePlayer).CStr(),
                                       CNewsItem::PreprocessClubName(_Club).CStr());
  _Club->AddToNewsList(str);

	CClubAndPlayerID *pRemoved = CClubAndPlayerIDList::m_List.GetAt(_ListID);
	CClubAndPlayerIDList::m_List.RemoveAt(_ListID);
	delete pRemoved;
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
			int PlayerValue = pPlayer->GetBosmanValuation(operator[](LoopCount).GetClubID());
			PlayerSearch.DoAddToList(operator[](LoopCount).GetPlayerID(), operator[](LoopCount).GetClubID(), PlayerValue);
		}
	}
 }
