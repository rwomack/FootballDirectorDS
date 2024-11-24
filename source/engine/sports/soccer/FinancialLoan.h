

/*

	Financial Loan class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __FINANCIALLOAN_H__
#define __FINANCIALLOAN_H__

class CUser;

const ushort DEFAULTLOANPERIOD = 3;

class CFinancialLoan : private CAIFinancialInterest
{
public:
	CFinancialLoan() {};

	uint					GetLoanOwed() const;
	uint					GetLoanRepayment();
	void					SetLoanOwed(const uint _Amount);
	void					SetLoanRepayment(const uint _Repayment);
	void					SetLoanWeeksToGo(const ushort _WeeksToGo);
	ushort					WeeksToRepayLoan(CCountry& _rCountry);
	uint					DoRepayLoan(const uint _HowMuch);
    void                    DoResetVariables();
	void					DoAcceptLoan(const int _HowMuch, CCountry& _Country, const ushort _HowLong = DEFAULTLOANPERIOD);
    bool					DoApplyForLoan();
	bool					DoApplyLoan(uint _HowMuch, CCountry& _rCountry, CPopularity* _pPopularity, const ushort _HowLong = DEFAULTLOANPERIOD);
	uint					DoWeeklyPayment(CCountry& _rCountry);
	uint					ForeignLoanOwed(CCountry& _rCountry);
	CString					LoanOwedStr(CCountry& _rCountry);
	CString					LoanRepaymentStr(CCountry& _rCountry);
	uint					GetRecommendedLoanBorrow(CPopularity* _pPopularity, const ushort _nHowLong = DEFAULTLOANPERIOD);
	void					SetAlreadyBorrowedToday(const bool _Value);

protected:
	uint					m_LoanOwed;
    uint					m_LoanWeeklyRepayment;
    ushort					m_LoanWeeksToGo;
	bool					m_AlreadyBorrowedToday;
};	
#endif