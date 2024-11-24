/*
  ---------------------------------------------------------------------------------------

	Calculate loan repayments


	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "math.h"
#include "loanrepay.h"


/*------------------------------------------------------------------------------
	Method:   	CAIFinancialInterest::DoCalcRepayment
	Access:    	public 
	Parameter: 	const double _Rate
	Parameter: 	uint _Amount
	Parameter: 	const ushort _Years
	Parameter: 	double _PaymentsPerYear
	Returns:   	const uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CAIFinancialInterest::DoCalcRepayment(const double _Rate, int _Amount, const ushort _Years, double _PaymentsPerYear /*= 52*/)
{
	double WeekAmount;
	double junk = 1 + (_Rate / 100);
	WeekAmount = exp((log(junk)) / _PaymentsPerYear);
	WeekAmount = _Amount * (WeekAmount - 1) * exp((log(junk)) * _Years);
	WeekAmount /= (exp((log(junk)) * _Years) - 1);
	return static_cast<uint>(WeekAmount);
}


/*------------------------------------------------------------------------------
	Method:   	CAIFinancialInterest::DoLoanBalance
	Access:    	public 
	Parameter: 	int _Week
	Parameter: 	const double _Rate
	Parameter: 	double _Amount
	Parameter: 	const ushort _Years
	Parameter: 	double _PaymentsPerYear
	Returns:   	double
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
double CAIFinancialInterest::DoLoanBalance(int _Week, const double _Rate, double _Amount, const ushort _Years, double _PaymentsPerYear)
{
	double YearRatio = 1 + (_Rate / 100);
	double TotalRatio = exp(_Years * log(YearRatio));
	double BalRatio = exp((log(YearRatio)) * _Week / _PaymentsPerYear);
	double balance = _Amount * BalRatio - _Amount * (BalRatio - 1) * TotalRatio / (TotalRatio - 1);
	return balance;
}



/*------------------------------------------------------------------------------
	Method:   	CAIFinancialInterest::DoWeeksToRepayLoan
	Access:    	public 
	Parameter: 	double _WeeklyInterestRate
	Parameter: 	double _LoanOwed
	Parameter: 	double _LoanWeeklyRepayment
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CAIFinancialInterest::DoWeeksToRepayLoan(double _WeeklyInterestRate, double _LoanOwed, double _LoanWeeklyRepayment)
{
	int Count = 0;
    while(_LoanOwed > 0){
		_LoanOwed += _LoanOwed * _WeeklyInterestRate;		// Add the interest on
		_LoanOwed -= min(_LoanWeeklyRepayment, _LoanOwed);
		Count += (_LoanOwed > 0);
    }
	return Count;
}
