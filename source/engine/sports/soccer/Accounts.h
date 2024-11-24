
 /*

	Accounts class


	Football Director

	(c) Rw Software 1994 - 2008

	---------------------------------------------------------------
 */


 #ifndef __ACCOUNTS_H__
 #define __ACCOUNTS_H__

enum eFINANCIALPERIOD
{
	WEEKLY_FINANCE,
	ANNUAL_FINANCE
};


class CAccountsItem
{
public:
	CAccountsItem();

    // Data Access Methods
	uint					GetAmount(const eFINANCIALPERIOD _WhichPeriod);
    uint					GetAnnual() const;
    uint					GetWeekly() const;
	bool					GetIsIncome() const;
	void					SetIsIncome(const bool _IsIncome);

    // Methods
	void					DoAnnualClearDown();
	void					Minus(const uint _HowMuch);
	void					Plus(const uint _HowMuch);
	void					DoWeeklyClearDown();

    // Variables
private:
	bool					m_IsIncome;
    uint					m_WeeklyTotal;
    uint					m_AnnualTotal;
 };


//----------------------------------------------------------------------------
class CAccountsItemList
{
  DECLARE_FDMEMORY_BINALLOC(CAccountsItemList);
public:

    enum eAccountItemType
	{
        TELEVISIONINCOME,
        LOANPAID,
        TRANSFERINCOME,
        TRANSFEREXPENDITURE,
        GENERALEXPENDITURE,
        GATERECIEPTS,
        RUNNINGCOSTS,
        SECURITY,
        GENERALWAGE,
        MERCHANDISEINCOME,
        SPONSORSHIPINCOME,
        MANAGERSWAGES,
        YOUTHWAGES,
        EMPLOYEEWAGES,
        PLAYERWAGES,
#ifdef ALLOWSTADIUMBUILDING
		BUILDINGWORK,
#endif
		BANKINTEREST,
		LOANRECEIVED,
        TOTALACCOUNTSITEMS
    };

	CAccountsItemList();   // Constructor
	~CAccountsItemList();  // Destructor

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoAnnualClearDown();
	CAccountsItem&			GetAt(const ushort _ElementNumber);
	void					DoWeeklyClearDown();

#ifdef ACCOUNTS_PIE_CHARTS
	void					BuildWeeklyIncomePieChart(uint* _TheList);
	void					BuildWeeklyExpensePieChart(uint* _TheList);
	void					BuildAnnualIncomePieChart(uint* _TheList);
	void					BuildAnnualExpensePieChart(uint* _TheList);
#endif
private:
	CAccountsItem			m_List[TOTALACCOUNTSITEMS];
};
#endif




















































