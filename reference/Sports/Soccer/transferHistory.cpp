
/*

	File:- tranhist.cpp

	Transfer history list class declarations


	Fotball Director

	(c) RW Software 1994 - 2008

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
   		return(_Country->GetCurrency().GetForeignStr(GetFee()));
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
	DeleteAll();
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
	ASSERT (_pDataFile != NULL);
	ushort HowMany;
	HowMany = GetSize();
    _pDataFile->Write(&HowMany, sizeof(HowMany));
	for (uint nCount = 0; nCount < GetSize(); nCount++)
	{
    _pDataFile->Write((GetAt(nCount), sizeof(CTransferHistory));;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferHistoryList::Read
	Access:    	public 
	Parameter: 	FILE * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTransferHistoryList::Read(FILE* _pDataFile)
{
	ASSERT (_pDataFile != NULL);
	ushort HowMany;
	RemoveList();
    _pDataFile->Read(&HowMany, sizeof(HowMany));
    for (uint ListPos = 0; ListPos < HowMany; ListPos ++)
	{
        SetAtGrow(ListPos, new CTransferHistory);
		_pDataFile->ReadGetAt(nCount), sizeof(CTransferHistory));
	}
}
