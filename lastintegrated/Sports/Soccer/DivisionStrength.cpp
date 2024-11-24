
/*

	File:- DivisionStrength.cpp

	Division strength list class


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "math.h"


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionStrength::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_ManagerPointsWin = _Resource.ReadByte();
    m_ManagerPointsDraw = _Resource.ReadByte();
    m_ManagerPointsMom = _Resource.ReadByte();
    m_ManagerPointsGoodWinAway = _Resource.ReadByte();
    m_ManagerPointsGoodWinHome = _Resource.ReadByte();
    m_ManagerPointsTopWin = _Resource.ReadByte();
#ifdef MANAGERSTRESS
    m_ManagerInitialStress = _Resource.ReadByte();
#endif
    _Resource.ReadUInt8Array(&m_TicketAttendanceRatio[0], 50);
	m_SeasonShirtSponsorShipAmount = _Resource.ReadUInt32();
	m_SeasonKitSponsorShipAmount = _Resource.ReadUInt32();
	m_SeasonTVIncome = _Resource.ReadUInt32();
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::DoAdjustAttendanceForTicketPrice
	Access:    	public 
	Parameter: 	const uint _HowMany
	Parameter: 	const uint _TicketPrice
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CDivisionStrength::DoAdjustAttendanceForTicketPrice(const uint _HowMany, const uint _TicketPrice)
{
	uint AdjustedAttendance = (_HowMany * m_TicketAttendanceRatio[_TicketPrice >> 1]) / 100;
	return AdjustedAttendance;
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::GetTicketAttendanceRatio
	Access:    	public 
	Parameter: 	const uint _ticketprice
	Returns:   	double
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CDivisionStrength::GetTicketAttendanceRatio(const uint _ticketprice)
{
	EXT_ASSERT(_ticketprice < 100);
	return m_TicketAttendanceRatio[_ticketprice >> 1];
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::GetManagerPointsWin
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CDivisionStrength::GetManagerPointsWin() const
{
	return m_ManagerPointsWin;
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::GetManagerPointsDraw
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CDivisionStrength::GetManagerPointsDraw() const
{
	return m_ManagerPointsDraw;
}


#ifdef MANAGERSTRESS
/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::GetManagerInitialStress
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CDivisionStrength::GetManagerInitialStress() const
{
	return m_ManagerInitialStress;
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::GetSeasonShirtSponsorShipAmount
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CDivisionStrength::GetSeasonShirtSponsorShipAmount()
{
	return m_SeasonShirtSponsorShipAmount;
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::GetSeasonKitSponsorShipAmount
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CDivisionStrength::GetSeasonKitSponsorShipAmount()
{
	return m_SeasonKitSponsorShipAmount;
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrength::GetSeasonTVIncome
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CDivisionStrength::GetSeasonTVIncome()
{
	return m_SeasonTVIncome;
}
///////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrengthList::CDivisionStrengthList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionStrengthList::CDivisionStrengthList()
{
	CBinaryReader theResource(0, IDR_DIVISIONSTRENGTH, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	m_pDivisionStrengthList = new CDivisionStrength[GetSize()];
	for (uint n = 0; n < GetSize(); n++)
	{
		m_pDivisionStrengthList[n].DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrengthList::~CDivisionStrengthList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionStrengthList::~CDivisionStrengthList()
{
	delete[] m_pDivisionStrengthList;
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionStrengthList::GetAt
	Access:    	public 
	Parameter: 	ushort x
	Returns:   	CDivisionStrength&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDivisionStrength& CDivisionStrengthList::GetAt(ushort _ElementNumber)
{
	return static_cast<CDivisionStrength&> (m_pDivisionStrengthList[_ElementNumber]);
}
