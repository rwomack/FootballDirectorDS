
 /*

        Class:- RefusedDealList

	Football Director

	(c) Rw Software 1994 - 2008

	---------------------------------------------------------------
 */

#include "stdafx.h"

	const int				CHANCETAKEOFFREFUSEDLIST = 5;


/*------------------------------------------------------------------------------
	Method:   	CRefusedDealList::DoAddLoan
	Access:    	public 
	Parameter: 	const ushort _ID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRefusedDealList::DoAddLoan(const ushort _ID)
{
	m_RefusedPlayerLoans.Add(_ID);
}


/*------------------------------------------------------------------------------
	Method:   	CRefusedDealList::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRefusedDealList::Read(CDataFile* _pDataFile)
{
	m_RefusedPlayerTransfers.Read(_pDataFile);
	m_RefusedPlayerLoans.Read(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CRefusedDealList::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRefusedDealList::Write(CDataFile* _pDataFile)
{
	m_RefusedPlayerTransfers.Write(_pDataFile);
	m_RefusedPlayerLoans.Write(_pDataFile);
}


/*------------------------------------------------------------------------------
	Method:   	CRefusedDealList::DoAddTransfer
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRefusedDealList::DoAddTransfer(const ushort _PlayerID)
{
	m_RefusedPlayerTransfers.Add(_PlayerID);
}


/*------------------------------------------------------------------------------
	Method:   	CRefusedDealList::DoEndOfWeekTasks
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRefusedDealList::DoEndOfWeekTasks()
{
    int num = 0;
	bool DeletedOne;

	// Remove  players from the refuse to negotiate a transfer deal with the club
	while (num < m_RefusedPlayerTransfers.GetSize())
	{
		DeletedOne = true;      // Go round inner loop
		while (DeletedOne == true && num < m_RefusedPlayerTransfers.GetSize())
		{
			DeletedOne = false;
			if (RandomNumber.IntLessThan(CHANCETAKEOFFREFUSEDLIST) == 1)
			{
				m_RefusedPlayerTransfers.RemoveAt(num);
				DeletedOne = true;
			}
			num++;
		}
	}

	// Remove  players from the refuse to negotiate a loan deal with the club
	num = 0;
	while (num < m_RefusedPlayerLoans.GetSize())
	{
		DeletedOne = true;      // Go round inner loop
		while (DeletedOne != true && num < m_RefusedPlayerLoans.GetSize())
		{
			DeletedOne = false;
			if (RandomNumber.IntLessThan(CHANCETAKEOFFREFUSEDLIST) == 1)
			{
				m_RefusedPlayerLoans.RemoveAt(num);
				DeletedOne = true;
			}
		}
		num++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CRefusedDealList::DoCheckIfPlayerRefusedTransfer
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CRefusedDealList::DoCheckIfPlayerRefusedTransfer(const ushort _PlayerID)
{
	bool bRet = false;
	if (m_RefusedPlayerTransfers.Find(_PlayerID) != wNOTFOUND)
	{
		bRet = true;
	}
	return bRet;
}


 //----------------------------------------------------------------------------
bool CRefusedDealList::DoCheckIfPlayerRefusedLoan(const ushort PlayerNum)
{
	bool bRet = false;
	if (m_RefusedPlayerLoans.Find(PlayerNum) != wNOTFOUND)
	{
		bRet = true;
	}
	return bRet;
 }
