
/*
	File:- TransferDeadlineSystem.cpp


	Football Director

	(c) Sports Director Ltd 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


const int MAX_DEADLINE_WINDOWS = 4;

const byte LOAN_ONLY = 2;
const byte BUY_ONLY = LOAN_ONLY + 1;


CTransferDeadlineItem::CTransferDeadlineItem(void) 
{
	m_OpenWeekNumber = TOTALMATCHES;
	m_CloseWeekNumber = TOTALMATCHES;
};

/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineItem::GetType
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTransferDeadlineItem::GetType()
{
	return m_Type;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineItem::OpensToday
	Access:    	public 
	Parameter: 	const byte _WeekNumber
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferDeadlineItem::OpensToday(const byte _WeekNumber)
{
	EXT_ASSERT(m_OpenWeekNumber != TOTALMATCHES);
	if (m_OpenWeekNumber == _WeekNumber)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineItem::IsOpen
	Access:    	public 
	Parameter: 	const byte _WeekNumber
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferDeadlineItem::IsOpen(const byte _WeekNumber)
{
	EXT_ASSERT(m_OpenWeekNumber != TOTALMATCHES);
	bool bRet = false;
	if (m_OpenWeekNumber < m_CloseWeekNumber)
	{
		if (_WeekNumber >= m_OpenWeekNumber && _WeekNumber < m_CloseWeekNumber)
		{
			bRet = true;
		}
	}
	else
	{
		// Window goes past year end
		if (_WeekNumber >= m_OpenWeekNumber || _WeekNumber < m_CloseWeekNumber)
		{
			bRet = true;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineItem::ClosesToday
	Access:    	public 
	Parameter: 	const byte _WeekNumber
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferDeadlineItem::ClosesToday(const byte _WeekNumber)
{
	EXT_ASSERT(m_CloseWeekNumber != TOTALMATCHES);
	if (m_CloseWeekNumber == _WeekNumber)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineItem::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTransferDeadlineItem::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	m_Type = _Resource.ReadByte();
	m_OpenWeekNumber = _Resource.ReadByte();
	m_CloseWeekNumber = _Resource.ReadByte();
// 	DEBUG_OUTPUT(("Window Type %d, Opens %d, Closes %d"), (m_Type, m_OpenWeekNumber, m_CloseWeekNumber));
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadline::DoAnnouncements
	Access:    	public 
	Parameter: 	CActiveCountry & _ActiveCountry
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTransferDeadline::DoAnnouncements(CActiveCountry& _ActiveCountry)
{
	byte CurrentWeekNumber = WorldData().GetCurrentDate().GetMatchDay();
	byte NextWeekNumber = CurrentWeekNumber + 2;
	int CurrentNewsItem = NOTFOUND;
	for (uint ListPos = 0; ListPos < MAX_DEADLINE_WINDOWS; ListPos++)
	{
 		if (WorldData().GetTransferDeadlineSystem().GetItem(ListPos).OpensToday(CurrentWeekNumber) == true)
 		{
			if (WorldData().GetTransferDeadlineSystem().GetItem(ListPos).GetType() != LOAN_ONLY)
			{
				CurrentNewsItem = IDS_TRANSFERWINDOWOPEN;
			}
			else
			{
				CurrentNewsItem = IDS_TRANSFERWINDOWLOANOPEN;
			}
		}
		else if (WorldData().GetTransferDeadlineSystem().GetItem(ListPos).OpensToday(NextWeekNumber) == true)
		{
			if (WorldData().GetTransferDeadlineSystem().GetItem(ListPos).GetType() != LOAN_ONLY)
			{
				CurrentNewsItem = IDS_TRANSFERWINDOWOPENSOON;
			}
			else
			{
				CurrentNewsItem = IDS_TRANSFERWINDOWLOANOPENSOON;
			}
		}
		else if (WorldData().GetTransferDeadlineSystem().GetItem(ListPos).ClosesToday(CurrentWeekNumber) == true)
		{
			if (WorldData().GetTransferDeadlineSystem().GetItem(ListPos).GetType() != LOAN_ONLY)
			{
				CurrentNewsItem = IDS_TRANSFERWINDOWCLOSED;
			}
			else
			{
				CurrentNewsItem = IDS_TRANSFERDEADLINELOANCLOSED;
			}
		}
		else if (WorldData().GetTransferDeadlineSystem().GetItem(ListPos).ClosesToday(NextWeekNumber) == true)
		{
			if (WorldData().GetTransferDeadlineSystem().GetItem(ListPos).GetType() != LOAN_ONLY)
			{
				CurrentNewsItem = IDS_TRANSFERWINDOWCLOSELOOMING;
			}
			else
			{
				CurrentNewsItem = IDS_TRANSFERWINDOWCLOSELOANLOOMING;
			}
		}
		if (CurrentNewsItem != NOTFOUND)
		{
			_ActiveCountry.AddToNewsLists(CurrentNewsItem, NEWSTRANSFER);
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadline::CanBuyPlayer
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferDeadline::CanBuyPlayer()
{
	byte CurrentWeekNumber = WorldData().GetCurrentDate().GetMatchDay();
	bool bRet = false;
	CTransferDeadlineSystem& theDeadlineSystem = WorldData().GetTransferDeadlineSystem();
	for (uint ListPos = 0; ListPos < MAX_DEADLINE_WINDOWS; ListPos++)
	{
		if (m_TransferDeadlineItemID[ListPos] != wNOTFOUND && 
			theDeadlineSystem.GetItem(m_TransferDeadlineItemID[ListPos]).IsOpen(CurrentWeekNumber) == true && 
			theDeadlineSystem.GetItem(m_TransferDeadlineItemID[ListPos]).GetType() != LOAN_ONLY)
 		{
 			bRet = true;
 			break;
 		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadline::CanLoanPlayer
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTransferDeadline::CanLoanPlayer()
{
	byte CurrentWeekNumber = WorldData().GetCurrentDate().GetMatchDay();
	bool bRet = false;
	CTransferDeadlineSystem& theDeadlineSystem = WorldData().GetTransferDeadlineSystem();
	for (uint ListPos = 0; ListPos < MAX_DEADLINE_WINDOWS; ListPos++)
	{
		if (m_TransferDeadlineItemID[ListPos] != wNOTFOUND && 
			theDeadlineSystem.GetItem(m_TransferDeadlineItemID[ListPos]).IsOpen(CurrentWeekNumber) == true && 
			theDeadlineSystem.GetItem(m_TransferDeadlineItemID[ListPos]).GetType() != BUY_ONLY)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}

/*------------------------------------------------------------------------------
	Method:   	CTransferDeadline::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryReader _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTransferDeadline::DoInitialiseFromResource(CBinaryReader& _Resource)
{
	for (uint ListPos = 0; ListPos < MAX_DEADLINE_WINDOWS; ListPos++)
	{
		m_TransferDeadlineItemID[ListPos] = _Resource.ReadUInt16();
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineSystem::~CTransferDeadlineSystem
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTransferDeadlineSystem::~CTransferDeadlineSystem()
{
	delete []m_TransferDeadlineList;
	delete []m_TransferDeadlineItemList;
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineSystem::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTransferDeadlineSystem::DoInitialise()
{
	CBinaryReader theResource(0, IDR_TRANSFERDEADLINESYSTEM, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);
	m_TransferDeadlineList = new CTransferDeadline[CExtArray::GetSize()];
	for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos++)
	{
		m_TransferDeadlineList[ListPos].DoInitialiseFromResource(theResource);
	}

	m_HowManyItems = theResource.ReadUInt16();
	EXT_ASSERT(m_HowManyItems > 0);
	m_TransferDeadlineItemList = new CTransferDeadlineItem[m_HowManyItems];
	for (uint ListPos = 0; ListPos < m_HowManyItems; ListPos++)
	{
		m_TransferDeadlineItemList[ListPos].DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineSystem::GetAt
	Access:    	public 
	Parameter: 	ushort _ElementNumber
	Returns:   	CTransferDeadline&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTransferDeadline& CTransferDeadlineSystem::GetAt(ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < CExtArray::GetSize());
	return static_cast<CTransferDeadline&>(m_TransferDeadlineList[_ElementNumber]);
}


/*------------------------------------------------------------------------------
	Method:   	CTransferDeadlineSystem::GetItem
	Access:    	public 
	Parameter: 	ushort _ElementNumber
	Returns:   	CTransferDeadlineItem&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTransferDeadlineItem& CTransferDeadlineSystem::GetItem(ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < m_HowManyItems);
	return static_cast<CTransferDeadlineItem&>(m_TransferDeadlineItemList[_ElementNumber]);
}