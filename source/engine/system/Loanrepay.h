

/*

	Financial Loan class

	Football Director

	(c) RW Software 1994 - 2007

 ---------------------------------------------------------------
 */


#ifndef __FINANCIALINTEREST_H__
#define __FINANCIALINTEREST_H__
class CAIFinancialInterest
{
public:
  float          DoLoanBalance(int _Week, const float _Rate, float _Amount, const ushort _Years, float _PaymentsPerYear);
  int           DoCalcRepayment(const float _Rate, int _Amount, const ushort _Years, float _PaymentsPerYear = 52);
  int           DoWeeksToRepayLoan(float _WeeklyInterestRate, float _LoanOwed, float _LoanWeeklyRepayment);
};
#endif
