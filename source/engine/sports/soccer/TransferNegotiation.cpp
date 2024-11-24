/*

	TransferNegotiation.cpp: implementation of the CTransferNegotiation class.


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "UI.h"
#include "TransferNegotiation.h"
#include "FootballDirector.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif




/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::CTransferNegotiation
	Access:    	public
	Parameter: 	CClubTransferOffer& rTheBid
	Parameter: 	int DatesToNegotiation
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferNegotiation::CTransferNegotiation(CClubTransferOffer& rTheBid, int DatesToNegotiation)
{
    m_PlayerID = rTheBid.GetPlayer().DoFindID();
    m_SellClubID = rTheBid.GetPlayer().GetClub().DoFindID();
	m_TheOffer.DoInitialise(WorldData().GetPlayer(m_PlayerID), rTheBid.GetAmountOffered(), WorldData().GetCurrentUserClub(), rTheBid.GetSellOnPercentOffered());
    m_DatesToNegotiation = DatesToNegotiation;
	m_bPlayerAgreedTerms = false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::~CTransferNegotiation
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferNegotiation::~CTransferNegotiation()
{
}

/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::DailyTasks
	Access:    	public
	Parameter: 	CClub* pClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiation::DailyTasks(CClub* _Club)
{
	m_DatesToNegotiation--;
	if (m_DatesToNegotiation == 0)
	{
		CString str;
		CPlayer* pPlayer = WorldData().GetPlayer(GetPlayerID());
		CClub& SellClub = WorldData().GetClubByID(GetSellClubID());
		if (_Club->GetFirstTeamSquad().TotalSquadSize() == PLAYERSINSQUAD)
		{
      str.Format(IDS_NO_ROOM_SQUAD, CNewsItem::PreprocessPlayerName(pPlayer).CStr(), CNewsItem::PreprocessClubName(&SellClub).CStr());
			UserMessageBox(str);
			return;
		}
		if (_Club->GetCashBalance() < static_cast<int>(m_TheOffer.GetAmountOffered()))
		{
      str.Format(IDS_LACK_OF_FUNDS, CNewsItem::PreprocessPlayerName(pPlayer).CStr(), CNewsItem::PreprocessClubName(&SellClub).CStr());
			UserMessageBox(str);
			return;
		}

		// Check the selling club can still spare him
		if (SellClub.CanSparePlayer(pPlayer) == false)
		{
      str.Format(IDS_CANTSPAREPLAYER, CNewsItem::PreprocessClubName(&SellClub).CStr(), CNewsItem::PreprocessPlayerName(pPlayer).CStr());
			UserMessageBox(str);
			return;
		}

    str.Format(IDS_PROCEED_WITH_NEG, CNewsItem::PreprocessPlayerName(pPlayer).CStr(), CNewsItem::PreprocessClubName(&SellClub).CStr(),
										  WorldData().GetCurrentUserClub()->GetForeignStr(m_TheOffer.GetAmountOffered()));
		if  (UserMessageBox(str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
		{
			theApp.DoPlayerContractNegotiations(_Club, this);
			if (m_bPlayerAgreedTerms == true)
			{
				DoTransferPlayer();
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::SetPlayerAgreed
	Access:    	public
	Parameter: 	CContract* pContract
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiation::SetPlayerAgreed(CContract* pContract)
{
	m_bPlayerAgreedTerms = true;
	m_PlayerContract.Copy(pContract);
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::DoTransferPlayer
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiation::DoTransferPlayer()
{
	WorldData().GetCurrentUserClub()->DoBuyTransferPlayer(*WorldData().GetPlayer(GetPlayerID()),
																	  m_TheOffer.GetAmountOffered(),
																	  m_PlayerContract,
																	  &WorldData().GetClubByID(GetSellClubID()),
																	  m_TheOffer.GetSellOnPercentOffered());
	WorldData().GetClubByID(GetSellClubID()).DoSellPlayer(*WorldData().GetPlayer(GetPlayerID()),
		m_TheOffer.GetAmountOffered(),
		m_TheOffer.GetBidClub(),
		false, false);
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::GetPlayerID
	Access:    	public
	Returns:   	ushort
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CTransferNegotiation::GetPlayerID()
{
	return m_PlayerID;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::GetSellClubID
	Access:    	public
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CTransferNegotiation::GetSellClubID() const
{
	return m_SellClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::GetDatesToNegotiation
	Access:    	public
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CTransferNegotiation::GetDatesToNegotiation() const
{
	return m_DatesToNegotiation;
}


CClubTransferOffer& CTransferNegotiation::GetOffer()
{
	return m_TheOffer;
}

/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::Write
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Saves the state of the object.
------------------------------------------------------------------------------*/
void CTransferNegotiation::Write(CDataFile* _pDataFile)
{
	m_TheOffer.Write(_pDataFile);

	_pDataFile->Write(&m_PlayerContract, sizeof(m_PlayerContract));
	_pDataFile->Write(&m_SellClubID, sizeof(m_SellClubID));
	_pDataFile->Write(&m_PlayerID, sizeof(m_PlayerID));

	const byte ucDatesToNegotiation = m_DatesToNegotiation;
	const byte ucPlayerAgreedTerms = m_bPlayerAgreedTerms;

	_pDataFile->Write(&ucDatesToNegotiation, sizeof(ucDatesToNegotiation));
	_pDataFile->Write(&ucPlayerAgreedTerms, sizeof(ucPlayerAgreedTerms));
}

/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiation::Read
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Loads the state of the object.
------------------------------------------------------------------------------*/
void CTransferNegotiation::Read(CDataFile* _pDataFile)
{
	m_TheOffer.Read(_pDataFile);

	_pDataFile->Read(&m_PlayerContract, sizeof(m_PlayerContract));
	_pDataFile->Read(&m_SellClubID, sizeof(m_SellClubID));
	_pDataFile->Read(&m_PlayerID, sizeof(m_PlayerID));

	byte ucDatesToNegotiation;
	byte ucPlayerAgreedTerms;

	_pDataFile->Read(&ucDatesToNegotiation, sizeof(ucDatesToNegotiation));
	_pDataFile->Read(&ucPlayerAgreedTerms, sizeof(ucPlayerAgreedTerms));

	m_DatesToNegotiation = ucDatesToNegotiation;
	m_bPlayerAgreedTerms = ucPlayerAgreedTerms;
}
/////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::CTransferNegotiationList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferNegotiationList::CTransferNegotiationList()
{
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::~CTransferNegotiationList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferNegotiationList::~CTransferNegotiationList()
{
    RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::DeleteAll
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiationList::DeleteAll()
{
    CTransferNegotiation* ListPtr;
	uint HowMany = m_List.GetSize();
	if (HowMany == 0)
	{
		return;		// None to delete
	}
    for (uint ListPos = 0; ListPos < HowMany; ListPos ++)
	{
	    ListPtr = GetAt(ListPos);
		if (ListPtr != NULL)
		{
			delete ListPtr;
		}
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::RemoveList
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiationList::RemoveList()
{
	DeleteAll();
    m_List.RemoveAll();
 }


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::Read
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiationList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	ushort HowMany;
    _pDataFile->Read(&HowMany, sizeof(HowMany));
	RemoveList();
	for (uint LoopCount = 0; LoopCount < HowMany; LoopCount++)
	{
		CTransferNegotiation *pNew = FD_ENGINE_NEW CTransferNegotiation();
		m_List.Add(pNew);
		pNew->Read(_pDataFile);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::Write
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiationList::Write(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile);
	ushort HowMany = m_List.GetSize();
    _pDataFile->Write(&HowMany, sizeof(HowMany));  // List size
    for (uint ListPos = 0; ListPos < HowMany; ListPos++)
	{
		GetAt(ListPos)->Write(_pDataFile);
    }
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::AddOne
	Access:    	public
	Parameter: 	CClubTransferOffer& rTheBid
	Parameter: 	int DatesToNegotiation
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiationList::AddOne(CClubTransferOffer& rTheBid, int DatesToNegotiation)
{
	m_List.Add(FD_ENGINE_NEW CTransferNegotiation(rTheBid, DatesToNegotiation));
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::GetAt
	Access:    	public
	Parameter: 	ushort _ElementNumber
	Returns:   	CTransferNegotiation*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferNegotiation* CTransferNegotiationList::GetAt(const ushort _ElementNumber)
{
	return static_cast<CTransferNegotiation*> (m_List.GetAt(_ElementNumber));
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::RemoveOne
	Access:    	public
	Parameter: 	const ushort _PlayerNum
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiationList::RemoveOne(const ushort _PlayerNum)
{
	if (m_List.GetSize() > 0)
	{
		for (uint ListPos = m_List.GetSize() - 1; ListPos > -1; ListPos--)
		{
			if (GetAt(ListPos)->GetPlayerID() == _PlayerNum)
			{
				delete GetAt(ListPos);
				m_List.RemoveAt(ListPos);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::DailyTasks
	Access:    	public
	Parameter: 	CClub* pClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiationList::DailyTasks(CClub* _Club)
{
	if (m_List.GetSize() > 0)
	{
	    for (int ListPos = m_List.GetSize() - 1; ListPos > -1; ListPos--)
		{
			GetAt(ListPos)->DailyTasks(_Club);
			if (GetAt(ListPos)->GetDatesToNegotiation() == 0)
			{
				delete GetAt(ListPos);
				m_List.RemoveAt(ListPos);
			}
		}
    }
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::DoFindNegotiationsForPlayer
	Access:    	public
	Parameter: 	CPlayer* pPlayer
	Returns:   	CTransferNegotiation*
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferNegotiation* CTransferNegotiationList::DoFindNegotiationsForPlayer(CPlayer* pPlayer)
{
	CTransferNegotiation* pTransferNegotiation = NULL;
    for (uint ListPos = 0; ListPos < m_List.GetSize(); ListPos ++)
	{
		if (WorldData().GetPlayer(GetAt(ListPos)->GetPlayerID()) == pPlayer)
		{
			pTransferNegotiation = GetAt(ListPos);
			break;
		}
    }
	return pTransferNegotiation;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::FindPlayer
	Access:    	public
	Parameter: 	ushort PlayerNumber
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferNegotiationList::FindPlayer(ushort PlayerNumber)
{
	bool bRet = false;
    for (uint ListPos = 0; ListPos < m_List.GetSize(); ListPos ++)
	{
		if (GetAt(ListPos)->GetPlayerID() == PlayerNumber)
		{
			bRet = true;
			break;
		}
    }
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::FindPlayer
	Access:    	public
	Parameter: 	CPlayer* _pPlayer
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferNegotiationList::FindPlayer(CPlayer* _pPlayer)
{
	if (DoFindNegotiationsForPlayer(_pPlayer) != NULL)
	{
		return true;
    }
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferNegotiationList::CheckIfPlayerInTransferNegotiations
	Access:    	public
	Parameter: 	ushort PlayerNum
	Parameter: 	CClub* pBiddingClub
	Parameter: 	CClub* pUserClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferNegotiationList::CheckIfPlayerInTransferNegotiations(ushort _PlayerNum, CClub* _BiddingClub, CClub* _UserClub)
{
	CString str;
    for (uint ListPos = 0; ListPos < m_List.GetSize(); ListPos ++)
	{
		if (GetAt(ListPos)->GetPlayerID() == _PlayerNum && _BiddingClub != _UserClub)
		{
      str.Format(IDS_PLAYER_SIGNED, CNewsItem::PreprocessPlayerName(WorldData().GetPlayer(_PlayerNum)).CStr(), CNewsItem::PreprocessClubName(_BiddingClub).CStr());
			UserMessageBox(str);
			RemoveOne(_PlayerNum);
			break;
		}
    }
}
