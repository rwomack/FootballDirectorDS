
 /*

        Class:- Accounts


		Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::CAccountsItem
	Access:    	public
	Parameter: 	void
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CAccountsItem::CAccountsItem()
	: m_WeeklyTotal(0)
    , m_AnnualTotal(0)
    , m_IsIncome(false)
{
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::GetAmount
	Access:    	public
	Parameter: 	const eFINANCIALPERIOD _WhichPeriod
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CAccountsItem::GetAmount(const eFINANCIALPERIOD _WhichPeriod)
{
	switch(_WhichPeriod)
	{
		case ANNUAL_FINANCE:
			return GetAnnual();
			break;
		case WEEKLY_FINANCE:
			return GetWeekly();
			break;
		default:
			EXT_ASSERT(false);
			break;
	}
	return 0;
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::SetIsIncome
	Access:    	public
	Parameter: 	const bool _IsIncome
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItem::SetIsIncome(const bool _IsIncome)
{
    m_IsIncome = _IsIncome;
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::GetIsIncome
	Access:    	public const
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CAccountsItem::GetIsIncome() const
{
    return m_IsIncome;
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::WeeklyClearDown
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItem::DoWeeklyClearDown()
{
    m_WeeklyTotal = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::AnnualClearDown
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItem::DoAnnualClearDown()
{
    m_AnnualTotal = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::GetWeekly
	Access:    	public const
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CAccountsItem::GetWeekly() const
{
    return m_WeeklyTotal;
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::GetAnnual
	Access:    	public const
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CAccountsItem::GetAnnual() const
{
    return m_AnnualTotal;
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::Minus
	Access:    	public
	Parameter: 	const uint _HowMuch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItem::Minus(const uint _HowMuch)
{
    m_WeeklyTotal -= _HowMuch;
    m_AnnualTotal -= _HowMuch;
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItem::Plus
	Access:    	public
	Parameter: 	const uint _HowMuch
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItem::Plus(const uint _HowMuch)
{
    m_WeeklyTotal += _HowMuch;
    m_AnnualTotal += _HowMuch;
}

///////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_FDMEMORY_BINALLOC(CAccountsItemList);

/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::CAccountsItemList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CAccountsItemList::CAccountsItemList()
{
	m_List[TELEVISIONINCOME].SetIsIncome(true);
	m_List[TRANSFERINCOME].SetIsIncome(true);
	m_List[GATERECIEPTS].SetIsIncome(true);
	m_List[MERCHANDISEINCOME].SetIsIncome(true);
	m_List[SPONSORSHIPINCOME].SetIsIncome(true);
	m_List[BANKINTEREST].SetIsIncome(true);
	m_List[LOANRECEIVED].SetIsIncome(true);
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::~CAccountsItemList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CAccountsItemList::~CAccountsItemList()
{
}



/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::Write
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItemList::Write(CDataFile* _pDataFile)
{
    _pDataFile->Write(&m_List[0], sizeof(CAccountsItem) * TOTALACCOUNTSITEMS);
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::Read
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItemList::Read(CDataFile* _pDataFile)
{
    _pDataFile->Read(&m_List[0], sizeof(CAccountsItem) * TOTALACCOUNTSITEMS);
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::AnnualClearDown
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItemList::DoAnnualClearDown()
{
    for (int LoopCount = 0; LoopCount < TOTALACCOUNTSITEMS; LoopCount++)
	{
        m_List[LoopCount].DoAnnualClearDown();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::WeeklyClearDown
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItemList::DoWeeklyClearDown()
{
    for (int LoopCount = 0; LoopCount < TOTALACCOUNTSITEMS; LoopCount++)
	{
        m_List[LoopCount].DoWeeklyClearDown();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::GetAt
	Access:    	public
	Parameter: 	const ushort _ElementNumber
	Returns:   	CAccountsItem&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CAccountsItem& CAccountsItemList::GetAt(const ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < TOTALACCOUNTSITEMS);
	return m_List[_ElementNumber];
}


#ifdef ACCOUNTS_PIE_CHARTS
/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::BuildWeeklyIncomePieChart
	Access:    	public
	Parameter: 	uint* _TheList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItemList::BuildWeeklyIncomePieChart(uint* _TheList)
{
	EXT_ASSERT(_TheList);
	CAccountsItem* pAccountsItem = m_pList;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (pAccountsItem && pAccountsItem->GetIsIncome() == true)
		{
			*_TheList = pAccountsItem->GetWeekly();
			_TheList++;
		}
        pAccountsItem++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::BuildWeeklyExpensePieChart
	Access:    	public
	Parameter: 	uint* _TheList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItemList::BuildWeeklyExpensePieChart(uint* _TheList)
{
	EXT_ASSERT(_TheList);
	CAccountsItem* pAccountsItem = m_pList;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (pAccountsItem && pAccountsItem->GetIsIncome() == false)
		{
			*_TheList = pAccountsItem->GetWeekly();
			_TheList++;
		}
        pAccountsItem++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::BuildAnnualIncomePieChart
	Access:    	public
	Parameter: 	uint* _TheList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItemList::BuildAnnualIncomePieChart(uint* _TheList)
{
	EXT_ASSERT(_TheList);
	CAccountsItem* pAccountsItem = m_pList;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (pAccountsItem && pAccountsItem->GetIsIncome() == true)
		{
			*_TheList = pAccountsItem->GetAnnual();
			_TheList++;
		}
        pAccountsItem++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CAccountsItemList::BuildAnnualExpensePieChart
	Access:    	public
	Parameter: 	uint* _TheList
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CAccountsItemList::BuildAnnualExpensePieChart(uint* _TheList)
{
	EXT_ASSERT(_TheList);
	CAccountsItem* pAccountsItem = m_pList;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
		if (pAccountsItem && pAccountsItem->GetIsIncome() == false)
		{
			*_TheList = pAccountsItem->GetAnnual();
			_TheList++;
		}
	}
}
#endif
