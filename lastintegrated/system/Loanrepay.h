

/*

	Financial Loan class

	Football Director

	(c) RW Software 1994 - 2008

 ---------------------------------------------------------------
 */


#ifndef __FINANCIALINTEREST_H__
#define __FINANCIALINTEREST_H__
class CAIFinancialInterest
{
public:
	double					DoLoanBalance(int _Week, const double _Rate, double _Amount, const ushort _Years, double _PaymentsPerYear);
	int						DoCalcRepayment(const double _Rate, int _Amount, const ushort _Years, double _PaymentsPerYear = 52);
	int						DoWeeksToRepayLoan(double _WeeklyInterestRate, double _LoanOwed, double _LoanWeeklyRepayment);
};
#endif