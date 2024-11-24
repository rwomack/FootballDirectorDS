
/*

	File:- CPopularity.h

	Popularity class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
 */


#ifndef __POPULAR_H__
#define __POPULAR_H__


	static const int		MAXDIVISIONSTRENGTHS = 4;

	
class CPopularity 
{
	// Variables
private:
	int						m_MaximumAttendance;
	int						m_BaseAttendance;
	byte					m_RelegationDecrease;
	byte					m_PromotionIncrease;
	uint					m_AnnualMaximumLoanAmount;
	CRangedUInt				m_rangeManagerWeeklyWage;
	CRangedByte				m_rangeTicketPrice;
	CRangedUInt				m_rangeInitialClubBalance;
    CRangedUInt				m_rangeWeeklyGeneralWage;
	int						m_BasicExpenses;
    CRangedUInt				m_rangeRunningCosts;
    int						m_PlayerPurchaseThreshold;            // computer managed club will try to buy players if got this much money
	int						m_MaximumPlayerPurchaseValue;		  // computer managed club will try to buy players below this value
	int						m_MinimumClubBalance;
    CRangedUInt				m_rangeWeeklySponsorship;
    CRangedUInt				m_rangeWeeklyTvRevenue;
	uint					m_WeeklyClubMerchandise;
	byte					m_MaxSkill;
    byte					m_GoodSkill;
    byte					m_SackSkill;
    byte					m_GoodPlayer;					/* When players skill is above this then slow down improvement rate!
														   When players skill is above this then if he is available to play but
														   is not selected then his skill will go down */
	byte					m_NumStarPlayers[MAXDIVISIONSTRENGTHS];
	byte					m_StarPlayerMinSkill[MAXDIVISIONSTRENGTHS];
	byte					m_StarPlayerRndSkill;
	byte					m_PlayerMinSkill[MAXDIVISIONSTRENGTHS];
	byte					m_PlayerRndSkill;
	CRangedByte				m_rangeEmployeeSkill;
	byte					m_MinimumManagerRatingToManageClub;

#ifdef ACTIVESTADIUMPLANS
	CTicketPrice			m_ticketPriceBand1;
	CTicketPrice			m_ticketPriceBand2;
	CTicketPrice			m_ticketPriceBand3;
	double					m_BoxPrice;
#endif

public:
	uint					GetManagerAnnualWageOffer(const byte _FinancialRatioPercentage);
	byte					GetRandomEmployeeSkill();
	int						GetMinimumClubBalance(byte _FinancialRatioPercentage);								
	byte					GetRandomPlayerSkill(int _DivisionStrength);
 	byte 					RandomStarPlayerSkill(int _DivisionStrength);
 	byte 					MinimumStarPlayerSkill(int _DivisionStrength);
	byte					GetMaximumStarPlayerSkill(int _DivisionStrength);
 	byte 					HowManyStarPlayers(int _DivisionStrength);
	byte					GetMinimumPlayerSkill(int _DivisionStrength);
    byte					GetMaximumPlayerSkill(int DivisionStrength);
	uint					GetRunningCosts(const byte _FinancialRatioPercentage);
	uint GetLoanAmount() const;
    const uint				RandomManagerWeeklyWage(const byte FinancialRatioPercentage);      					
	int						GetStartingClubBalance(byte _FinancialRatioPercentage);
	uint 					GetGeneralWage(const byte _FinancialRatioPercentage);
	uint 					GetMaximumAttendance() const;          					
	uint 					GetBaseAttendance();            					
	uint 					GetBasicExpenses(const byte _FinancialRatioPercentage);        
    const byte				GetGoodSkill() const;										
    const byte				GetSackSkill() const;										
	const double			GetWageAdjustmentRatio() const;
    const byte				GetGoodPlayer() const;									
	uint					GetPlayerPurchaseThreshold(const byte _FinancialRatioPercentage); 
    byte					GetMaximumPlayerSkill() const;								
	ushort					GetRelegationDecrease() const;						
	ushort					GetPromotionIncrease() const;								
    const bool              IsGoodClub();
	bool 					IsPlayerSkillInRange(const byte _Skill, int _DivisionStrength);
	bool 					IsPlayerSkillInRangeNonUserBid(const byte _Skill, int _DivisionStrength);
	uint 					GetWeeklyMerchandiseIncome(const byte _FinancialRatioPercentage);			
	uint 					GetMaximumPlayerPurchaseValue(const byte _FinancialRatioPercentage) const;
	uint 					GetTicketPrice();
	byte 					GetMinimumEmployeeSkill();
	byte 					GetMaximumEmployeeSkill();					
	byte 					GetMinimumManagerRatingToManageClub() const;
	const bool				IsUserGoodEnoughToManageClub(const byte ManagerRating);
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	uint					GetWeeklyTvRevenue(const byte _FinancialRatioPercentage);			
	uint					GetWeeklySponsorship(const byte _FinancialRatioPercentage);
#ifdef ACTIVESTADIUMPLANS
	const double			GetBoxPrice() const;
	CTicketPrice&			GetPriceBand(const ushort nIndex);
#endif
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CPopularityList : public CExtArray 

{
public:
   CPopularityList();             // Constructor
   ~CPopularityList();			  // Destructor

    // Methods
	byte					DoCalculateID(const uint nAverageAttendance);
    CPopularity&            operator [](const ushort IndVal);

	// Variables
private:
	CPopularity*			m_pList;
};
#endif
