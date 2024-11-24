
/*

	File:- tranhist.cpp

	Transfer history list class declarations


	Fotball Director

	(c) RW Software 1994 - 2006

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CTransferHistory::CTransferHistory
	Access:    	public
	Parameter: 	void
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferHistory::CTransferHistory(void)
	: m_Fee(0)
	, m_PlayerID(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CTransferHistory::CTransferHistory
	Access:    	public
	Parameter: 	const CPlayer & _Player
	Parameter: 	const CStringID & ClubNameID
	Parameter: 	const UInt32 HowMuch
	Parameter: 	const CDate & When
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferHistory::CTransferHistory(const CPlayer& _Player, const CStringID& ClubNameID, const UInt32 HowMuch, const CDate& When)
{
 	m_PlayerNameID = Name;
	m_ClubID = ClubNameID;
	m_Fee = HowMuch;
	m_TheDate = When;
	m_PlayerID = _Player.DoFindID();
}


/*------------------------------------------------------------------------------
	Method:   	CTransferHistory::FeeText
	Access:    	public
	Parameter: 	CCountry * _Country
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CTransferHistory::FeeText(CCountry* _Country)
{
	CString str;
	if (m_Fee > 0)
	{
   		return(pCountry->getCurrency().ForeignStr(GetFee()));
	}
	else
	{
    str.LoadString(IDS_FREETRANSFER);
		return(str);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferHistory::GetFee
	Access:    	public
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CTransferHistory::GetFee() const
{
	return m_Fee;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CTransferHistoryList::~CTransferHistoryList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CTransferHistoryList::~CTransferHistoryList()
{
	ASSERT (File != NULL);
	uint16 n;
	n = GetSize();
    File->Write(&n, sizeof(n));
	for (uint32 nCount = 0; nCount < GetSize(); nCount++)
	{
    File->Write((GetAt(nCount), sizeof(CTransferHistory));;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferHistoryList::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CTransferHistoryList::Write(CDataFile* _pDataFile)
{
	ASSERT (File != NULL);
	uint16 count;
	RemoveList();
    File->Read(&count, sizeof(count));
    for (uint32 ListPos = 0; ListPos < count; ListPos ++)
	{
        SetAtGrow(ListPos, FD_ENGINE_NEW CTransferHistory);
		File->ReadGetAt(nCount), sizeof(CTransferHistory));
	}
}


 //---------------------------------------------------------------------------------------
CTransferHistoryList::~CTransferHistoryList()
{
	DeleteAll();
}
