
 /*

    Class:- TransferListManager

	Football Director

	(c) Sports Director Ltd 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#ifdef L_PLATFORM_NDS
#include <nitro/code32.h>
#include <nitro/itcm_begin.h>
#endif

#include "stdafx.h"
#include <fdmemory.h>
#include <PlayerLoanManager.h>

/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::CPlayerLoanManager
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerLoanManager::CPlayerLoanManager()
{
}

/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::~CPlayerLoanManager
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerLoanManager::~CPlayerLoanManager()
{
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::Reset
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoanManager::Reset()
{
	m_records.RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::FindRecord
	Access:    	private 
	Parameter: 	const ushort usPlayerID
	Returns:   	CPlayerLoanManager::StPlayerLoanInfo*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPlayerLoanManager::StPlayerLoanInfo* CPlayerLoanManager::FindRecord(const ushort usPlayerID)
{
	for (int i = 0; i < m_records.GetSize(); i++)
	{
		if (m_records[i].m_usPlayerID == usPlayerID)
		{
			return &m_records[i];
		}
	}

	return NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::FindRecord
	Access:    	private 
	Parameter: 	const ushort usPlayerID
	Returns:   	const CPlayerLoanManager::StPlayerLoanInfo*
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
const CPlayerLoanManager::StPlayerLoanInfo* CPlayerLoanManager::FindRecord(const ushort usPlayerID) const
{
	for (int i = 0; i < m_records.GetSize(); i++)
	{
		if (m_records[i].m_usPlayerID == usPlayerID)
		{
			return &m_records[i];
		}
	}

	return NULL;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::SetWeeksOnLoan
	Access:    	public 
	Parameter: 	const byte _WeeksOnLoan
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoanManager::SetWeeksOnLoan(const ushort usPlayerID, const byte _WeeksOnLoan)
{ 
	StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		pRecord->m_WeeksOnLoan = _WeeksOnLoan; 
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::SetClubOnLoanFrom
	Access:    	public 
	Parameter: 	const ushort _ClubOnLoanFromID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoanManager::SetClubOnLoanFrom(const ushort usPlayerID, const ClubID _ClubOnLoanFromID)
{ 
	StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		pRecord->m_ClubOnLoanFromID = _ClubOnLoanFromID;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::DoGoingOnLoan
	Access:    	public 
	Parameter: 	const ushort _ClubOnLoanFromID
	Parameter: 	const byte _WeeksOnLoan
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoanManager::DoGoingOnLoan(const ushort usPlayerID, const ClubID _ClubOnLoanFromID, const byte _WeeksOnLoan)
{
	StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);

	if (pRecord != NULL)
	{
		pRecord->m_WeeksOnLoan = _WeeksOnLoan;
		pRecord->m_ClubOnLoanFromID = _ClubOnLoanFromID;
	}
	else
	{
		StPlayerLoanInfo cNewRecord;
		cNewRecord.m_usPlayerID = usPlayerID;
		cNewRecord.m_ClubOnLoanFromID = _ClubOnLoanFromID;
		cNewRecord.m_WeeksOnLoan = _WeeksOnLoan;

		m_records.Add(cNewRecord);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::DoWeeklyUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoanManager::DoWeeklyUpdate()
{
	for (int i = 0; i < m_records.GetSize(); i++)
	{
    	if (m_records[i].m_WeeksOnLoan != MAXLOANPLAYERWEEKS)
		{
			if (m_records[i].m_WeeksOnLoan > 0)
			{
      			m_records[i].m_WeeksOnLoan--;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::IsOnLoan
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerLoanManager::IsOnLoan(const ushort usPlayerID) const
{
	const StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);

	if (pRecord != NULL)
		return (pRecord->m_WeeksOnLoan > 0);
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::IsLoanedUntilEndOfSeason
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerLoanManager::IsLoanedUntilEndOfSeason(const ushort usPlayerID) const
{
	const StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		return (pRecord->m_WeeksOnLoan == MAXLOANPLAYERWEEKS);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoanManager::DoInitialise(const ushort usPlayerID)
{
	StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		m_records.RemoveElement(*pRecord);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::IsLoanedOut
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerLoanManager::IsLoanedOut(const ushort usPlayerID, const ClubID _ClubID) const
{
	const StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		return (pRecord->m_ClubOnLoanFromID == _ClubID);
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::GetClubOnLoanFrom
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CPlayerLoanManager::GetClubOnLoanFrom(const ushort usPlayerID) const
{ 
	const StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		return pRecord->m_ClubOnLoanFromID;
	}
	return ClubID(NOCLUB);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::GetWeeksOnLoan
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerLoanManager::GetWeeksOnLoan(const ushort usPlayerID) const
{
	const StPlayerLoanInfo *pRecord = FindRecord(usPlayerID);
	if (pRecord != NULL)
	{
		return pRecord->m_WeeksOnLoan;
	}
	return 0;
}

#ifdef L_PLATFORM_NDS
#include <nitro/itcm_end.h>
#endif


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::Write
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoanManager::Write(CDataFile* _pDataFile)
{
	ushort usNumRecords = m_records.GetSize();
	_pDataFile->Write(&usNumRecords, sizeof(usNumRecords));
	for (int i = 0; i < usNumRecords; i++)
	{
		StPlayerLoanInfo &cRecord = m_records[i];
		_pDataFile->Write(&cRecord.m_usPlayerID, sizeof(cRecord.m_usPlayerID));
		_pDataFile->Write(&cRecord.m_ClubOnLoanFromID, sizeof(cRecord.m_ClubOnLoanFromID));
		_pDataFile->Write(&cRecord.m_WeeksOnLoan, sizeof(cRecord.m_WeeksOnLoan));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerLoanManager::Read
	Access:    	public 
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerLoanManager::Read(CDataFile* _pDataFile)
{
	ushort usNumRecords;
	_pDataFile->Read(&usNumRecords, sizeof(usNumRecords));
	m_records.SetSize(usNumRecords, StPlayerLoanInfo());
	for (int i = 0; i < usNumRecords; i++)
	{
		StPlayerLoanInfo &cRecord = m_records[i];
		_pDataFile->Read(&cRecord.m_usPlayerID, sizeof(cRecord.m_usPlayerID));
		_pDataFile->Read(&cRecord.m_ClubOnLoanFromID, sizeof(cRecord.m_ClubOnLoanFromID));
		_pDataFile->Read(&cRecord.m_WeeksOnLoan, sizeof(cRecord.m_WeeksOnLoan));
	}
}
