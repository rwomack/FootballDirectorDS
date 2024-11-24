
/*

	File:- popular.cpp

    Popularity list class


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "Popular.h"


/*------------------------------------------------------------------------------
	Method:   	CPopularity::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPopularity::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_AnnualMaximumLoanAmount = _Resource.ReadUInt32();
	m_rangeManagerWeeklyWage.ReadResource(_Resource);
//	m_WageAdjustmentRatio = _Resource.ReadByte();
    m_rangeTicketPrice.ReadResource(_Resource);
    m_rangeInitialClubBalance.ReadResource(_Resource);
    m_rangeWeeklyGeneralWage.ReadResource(_Resource);
	m_BaseAttendance = _Resource.ReadUInt32();
    m_MaximumAttendance = _Resource.ReadUInt32();
    m_BasicExpenses = _Resource.ReadUInt32();
    m_rangeRunningCosts.ReadResource(_Resource);

    m_MaxSkill = _Resource.ReadByte();
	EXT_ASSERT(m_MaxSkill > 0);
    m_GoodSkill = _Resource.ReadByte();
	EXT_ASSERT(m_GoodSkill > 0);
    m_SackSkill = _Resource.ReadByte();
	EXT_ASSERT(m_SackSkill > 0);
    m_GoodPlayer = _Resource.ReadByte();
	EXT_ASSERT(m_GoodPlayer > 0);
    m_PlayerPurchaseThreshold = _Resource.ReadUInt32();
	EXT_ASSERT(m_PlayerPurchaseThreshold > 0);
    m_MaximumPlayerPurchaseValue = _Resource.ReadUInt32();
	EXT_ASSERT(m_MaximumPlayerPurchaseValue > 0);

    m_RelegationDecrease = _Resource.ReadByte();
    m_PromotionIncrease = _Resource.ReadByte();
    m_rangeEmployeeSkill.ReadResource(_Resource);
    m_MinimumClubBalance = _Resource.ReadUInt32();

	_Resource.ReadUInt8Array(&m_NumStarPlayers[0], MAXDIVISIONSTRENGTHS);
	_Resource.ReadUInt8Array(&m_StarPlayerMinSkill[0], MAXDIVISIONSTRENGTHS);
	_Resource.ReadUInt8Array(&m_PlayerMinSkill[0], MAXDIVISIONSTRENGTHS);
	m_StarPlayerRndSkill = _Resource.ReadByte();
	m_PlayerRndSkill = _Resource.ReadByte();
	m_OldPlayerAdjustment = _Resource.ReadByte();

    m_rangeWeeklySponsorship.ReadResource(_Resource);
    m_rangeWeeklyTvRevenue.ReadResource(_Resource);
    m_WeeklyClubMerchandise = _Resource.ReadUInt32();
	m_MinimumManagerRatingToManageClub = _Resource.ReadByte();
	m_GoodMatchDayChance = _Resource.ReadByte();
	m_BadMatchDayChance = _Resource.ReadByte();
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMinimumClubBalance
	Access:    	public 
	Parameter: 	byte FinancialRatioPercentage
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPopularity::GetMinimumClubBalance(byte _FinancialRatioPercentage)
{
	EXT_ASSERT(_FinancialRatioPercentage > 0 && _FinancialRatioPercentage < 101);
    return (m_MinimumClubBalance / 100) * _FinancialRatioPercentage;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::WeeklyTvRevenue
	Access:    	public 
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetWeeklyTvRevenue(const byte _FinancialRatioPercentage)
{
	EXT_ASSERT(_FinancialRatioPercentage > 0 && _FinancialRatioPercentage < 101);
	return (m_rangeWeeklyTvRevenue.RandomAmount() / 100) * _FinancialRatioPercentage;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::WeeklySponsorship
	Access:    	public 
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetWeeklySponsorship(const byte _FinancialRatioPercentage)
{
	EXT_ASSERT(_FinancialRatioPercentage > 0 && _FinancialRatioPercentage < 101);
	return (m_rangeWeeklySponsorship.RandomAmount() / 100) * _FinancialRatioPercentage;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::RunningCosts
	Access:    	public 
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetRunningCosts(const byte _FinancialRatioPercentage)
{
	EXT_ASSERT(_FinancialRatioPercentage > 0 && _FinancialRatioPercentage < 101);
	return (m_rangeRunningCosts.RandomAmount() / 100) * _FinancialRatioPercentage;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::IsUserGoodEnoughToManageClub
	Access:    	public 
	Parameter: 	const byte ManagerRating
	Returns:   	const bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPopularity::IsUserGoodEnoughToManageClub(const byte _ManagerRating)
{
	return (m_MinimumManagerRatingToManageClub <=  _ManagerRating);
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMaxAttendance
	Access:    	public const 
	Parameter: 	void
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetMaximumAttendance() const
{ 
	return m_MaximumAttendance; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetBaseAttendance
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetBaseAttendance()
{ 
	return m_BaseAttendance; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetBasicExpenses
	Access:    	public 
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetBasicExpenses(const byte _FinancialRatioPercentage)
{ 
	EXT_ASSERT(_FinancialRatioPercentage > 0 && _FinancialRatioPercentage < 101);
	return (m_BasicExpenses / 100) * _FinancialRatioPercentage; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetGoodSkill
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetGoodSkill() const
{ 
	return m_GoodSkill; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetSackSkill
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetSackSkill() const
{ 
	return m_SackSkill; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetGoodPlayer
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetGoodPlayer() const
{ 
	return m_GoodPlayer; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetPlayerPurchaseThreshold
	Access:    	public 
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetPlayerPurchaseThreshold(const byte _FinancialRatioPercentage)
{ 
	EXT_ASSERT(_FinancialRatioPercentage > 0 && _FinancialRatioPercentage < 101);
	return (m_PlayerPurchaseThreshold / 100) * _FinancialRatioPercentage; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMaximumPlayerSkill
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetMaximumPlayerSkill() const								
{ 
	return m_MaxSkill; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetRelegationDecrease
	Access:    	public const 
	Parameter: 	void
	Returns:   	const ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPopularity::GetRelegationDecrease() const
{ 
	return m_RelegationDecrease; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetPromotionIncrease
	Access:    	public const 
	Parameter: 	void
	Returns:   	const ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CPopularity::GetPromotionIncrease() const
{ 
	return m_PromotionIncrease; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetLoanAmount
	Access:    	public const 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetLoanAmount() const
{ 
	return m_AnnualMaximumLoanAmount; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetWeeklyMerchandiseIncome
	Access:    	public 
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetWeeklyMerchandiseIncome(const byte _FinancialRatioPercentage)
{ 
	EXT_ASSERT(_FinancialRatioPercentage > 0 && _FinancialRatioPercentage < 101);
	return (m_WeeklyClubMerchandise / 100) * _FinancialRatioPercentage; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMaximumPlayerPurchaseValue
	Access:    	public const 
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	const uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetMaximumPlayerPurchaseValue(const byte _FinancialRatioPercentage) const
{ 
	return (m_MaximumPlayerPurchaseValue / 100) * _FinancialRatioPercentage; 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMinimumEmployeeSkill
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetMinimumEmployeeSkill()
{ 
	return m_rangeEmployeeSkill.GetBasicValue(); 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMaximumEmployeeSkill
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetMaximumEmployeeSkill()
{ 
	return m_rangeEmployeeSkill.GetMaximumValue(); 
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMinimumManagerRatingToManageClub
	Access:    	public const 
	Parameter: 	void
	Returns:   	const byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetMinimumManagerRatingToManageClub() const
{ 
	return m_MinimumManagerRatingToManageClub; 
}


#ifdef ACTIVESTADIUMPLANS
/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetBoxPrice
	Access:    	public const 
	Parameter: 	void
  	Returns:    float
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
float CPopularity::GetBoxPrice() const
{
	return m_BoxPrice;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetPriceBand
	Access:    	public 
	Parameter: 	const ushort nIndex
	Returns:   	CTicketPrice&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTicketPrice& CPopularity::GetPriceBand(const ushort nIndex)
{
	switch(nIndex) 
	{
		case 0:
			return m_ticketPriceBand1;
		case 1:
			return m_ticketPriceBand2; 
		case 2:
			return m_ticketPriceBand3;
		default:
			EXT_ASSERT(false);						// Invalid index
	}
	return m_ticketPriceBand1;			// Default to first one
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CPopularity::IsPlayerSkillInRange
	Access:    	public 
	Parameter: 	const byte _Skill
	Parameter: 	int _DivisionStrength
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPopularity::IsPlayerSkillInRange(const byte _Skill, int _DivisionStrength)
{
	if (_Skill >= (GetMinimumPlayerSkill(_DivisionStrength) - 10) && _Skill <= (GetMaximumStarPlayerSkill(_DivisionStrength)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::IsPlayerSkillInRangeNonUserBid
	Access:    	public 
	Parameter: 	const byte Skill
	Parameter: 	int DivisionStrength
	Returns:   	const bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPopularity::IsPlayerSkillInRangeNonUserBid(const byte _Skill, const byte _Age, int _DivisionStrength)
{
// 	g_DebugStr.Format(_T("Player skill %d, min skill %d, max skill %d"), _Skill, GetMinimumPlayerSkill(_DivisionStrength), GetMaximumStarPlayerSkill(_DivisionStrength));
// 	g_DebugStr.OutputDebug();
	if (_Skill >= GetMinimumPlayerSkill(_DivisionStrength) && _Skill <= GetMaximumStarPlayerSkill(_DivisionStrength))
	{
		if (_Age > 28 && _Skill < GetMinimumPlayerSkill(_DivisionStrength) + m_OldPlayerAdjustment)
		{
			return false;
		}
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetTicketPrice
	Access:    	public 
	Parameter: 	void
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetTicketPrice()
{
	return m_rangeTicketPrice.RandomAmount();
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetStartingClubBalance
	Access:    	public 
	Parameter: 	const byte FinancialRatioPercentage
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CPopularity::GetStartingClubBalance(byte _FinancialRatioPercentage)
{
    return (m_rangeInitialClubBalance.RandomAmount() / 100) * _FinancialRatioPercentage;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::RandomEmployeeSkill
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetRandomEmployeeSkill()
{
	return m_rangeEmployeeSkill.RandomAmount();
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::RandomPlayerSkill
	Access:    	public 
	Parameter: 	int DivisionStrength
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetRandomPlayerSkill(int _DivisionStrength)
{
	return m_PlayerMinSkill[_DivisionStrength] + RandomNumber.IntLessThan(m_PlayerRndSkill);
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMinimumPlayerSkill
	Access:    	public 
	Parameter: 	int DivisionStrength
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetMinimumPlayerSkill(int _DivisionStrength)
{
	return m_PlayerMinSkill[_DivisionStrength];
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::MaximumPlayerSkill
	Access:    	public 
	Parameter: 	int DivisionStrength
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetMaximumPlayerSkill(int DivisionStrength)
{
	return m_PlayerMinSkill[DivisionStrength] + m_PlayerRndSkill;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetMaximumStarPlayerSkill
	Access:    	public 
	Parameter: 	int DivisionStrength
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::GetMaximumStarPlayerSkill(int _DivisionStrength)
{
	return m_StarPlayerMinSkill[_DivisionStrength] + m_StarPlayerRndSkill + 5;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::RandomStarPlayerSkill
	Access:    	public 
	Parameter: 	int DivisionStrength
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::RandomStarPlayerSkill(int _DivisionStrength)
{
	return MinimumStarPlayerSkill(_DivisionStrength) + m_StarPlayerRndSkill;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::MinimumStarPlayerSkill
	Access:    	public 
	Parameter: 	int DivisionStrength
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::MinimumStarPlayerSkill(int _DivisionStrength)
{
	return m_StarPlayerMinSkill[_DivisionStrength];
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::HowManyStarPlayers
	Access:    	public 
	Parameter: 	int DivisionStrength
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPopularity::HowManyStarPlayers(int _DivisionStrength)
{
	return m_NumStarPlayers[_DivisionStrength];
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetGeneralWage
	Access:    	public 
	Parameter: 	const byte _FinancialRatioPercentage
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetGeneralWage(const byte _FinancialRatioPercentage)
{
	EXT_ASSERT(_FinancialRatioPercentage > 0 && _FinancialRatioPercentage < 101);
    return (m_rangeWeeklyGeneralWage.RandomAmount() / 100) * _FinancialRatioPercentage;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::RandomManagerWeeklyWage
	Access:    	public 
	Parameter: 	const byte _FinancialRatioPercentage
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::RandomManagerWeeklyWage(const byte _FinancialRatioPercentage)
{
    return (m_rangeManagerWeeklyWage.RandomAmount() / 100) * _FinancialRatioPercentage;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::GetManagerAnnualWageOffer
	Access:    	public 
	Parameter: 	const byte _FinancialRatioPercentage
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CPopularity::GetManagerAnnualWageOffer(const byte _FinancialRatioPercentage)
{
    return ((m_rangeManagerWeeklyWage.RandomAmount() / 100) * _FinancialRatioPercentage) * 52;
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::IsGoodClub
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPopularity::IsGoodClub()
{
	if (GetMaximumAttendance() > 25000)
	{
		return true;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPopularity::RandomMatchDayType
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eMatchDayEffect CPopularity::RandomMatchDayType()
{
	if (RandomNumber.IntLessThan(m_BadMatchDayChance) == 2)
	{
		return BADDAY;
	}
	if (RandomNumber.IntLessThan(m_GoodMatchDayChance) == 2)
	{
		return GOODDAY;
	}
	return NODAYTYPE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CPopularityList::CPopularityList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPopularityList::CPopularityList()
{
	CBinaryReader theResource(0, IDR_POPULARITY, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	m_pList = FD_ENGINE_NEW CPopularity[GetSize()];
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
	{
		m_pList[ListPos].DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPopularityList::~CPopularityList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPopularityList::~CPopularityList()
{
	delete[] m_pList;
 }


/*------------------------------------------------------------------------------
	Method:   	CPopularityList::operator[]
	Access:    	public 
	Parameter: 	const ushort IndVal
	Returns:   	CPopularity&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPopularity& CPopularityList::operator [](const ushort IndVal)
{
    EXT_ASSERT(IndVal < CExtArray::GetSize());
	return *(static_cast<CPopularity*> (&m_pList[IndVal]));
}


/*------------------------------------------------------------------------------
	Method:   	CPopularityList::DoCalculateID
	Access:    	public 
	Parameter: 	const uint _AverageAttendance
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CPopularityList::DoCalculateID(const uint _AverageAttendance)
{
	EXT_ASSERT(_AverageAttendance < m_pList[0].GetMaximumAttendance());
	byte bRet = 0;
    for (short nListPos = CExtArray::GetUpperBound(); nListPos >= 0; nListPos--)
	{
		if (_AverageAttendance < m_pList[nListPos].GetMaximumAttendance())
		{
			bRet = nListPos;
			break;
		}
	}
	return bRet;
}
