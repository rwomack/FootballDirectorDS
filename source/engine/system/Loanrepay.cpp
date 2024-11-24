/*
  ---------------------------------------------------------------------------------------

	Calculate loan repayments


	(c) Rw Software 1994 - 2007

  ---------------------------------------------------------------------------------------
*/

#include "stdafx.h"
#include "math.h"
#include "loanrepay.h"


/*------------------------------------------------------------------------------
	Method:   	CAIFinancialInterest::DoCalcRepayment
	Access:    	public
  Parameter:  const float _Rate
	Parameter: 	uint _Amount
	Parameter: 	const ushort _Years
  Parameter:  float _PaymentsPerYear
	Returns:   	const uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CAIFinancialInterest::DoCalcRepayment(const float _Rate, int _Amount, const ushort _Years, float _PaymentsPerYear /*= 52*/)
{
  float WeekAmount;
  float junk = 1 + (_Rate / 100);
	WeekAmount = exp((log(junk)) / _PaymentsPerYear);
	WeekAmount = _Amount * (WeekAmount - 1) * exp((log(junk)) * _Years);
	WeekAmount /= (exp((log(junk)) * _Years) - 1);
	return static_cast<uint>(WeekAmount);
}


/*------------------------------------------------------------------------------
	Method:   	CAIFinancialInterest::DoLoanBalance
	Access:    	public
	Parameter: 	int _Week
  Parameter:  const float _Rate
  Parameter:  float _Amount
	Parameter: 	const ushort _Years
  Parameter:  float _PaymentsPerYear
  Returns:    float
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
float CAIFinancialInterest::DoLoanBalance(int _Week, const float _Rate, float _Amount, const ushort _Years, float _PaymentsPerYear)
{
  float YearRatio = 1 + (_Rate / 100);
  float TotalRatio = exp(_Years * log(YearRatio));
  float BalRatio = exp((log(YearRatio)) * _Week / _PaymentsPerYear);
  float balance = _Amount * BalRatio - _Amount * (BalRatio - 1) * TotalRatio / (TotalRatio - 1);
	return balance;
}



/*------------------------------------------------------------------------------
	Method:   	CAIFinancialInterest::DoWeeksToRepayLoan
	Access:    	public
  Parameter:  float _WeeklyInterestRate
  Parameter:  float _LoanOwed
  Parameter:  float _LoanWeeklyRepayment
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CAIFinancialInterest::DoWeeksToRepayLoan(float _WeeklyInterestRate, float _LoanOwed, float _LoanWeeklyRepayment)
{
	int Count = 0;
    while(_LoanOwed > 0){
		_LoanOwed += _LoanOwed * _WeeklyInterestRate;		// Add the interest on
		_LoanOwed -= min(_LoanWeeklyRepayment, _LoanOwed);
		Count += (_LoanOwed > 0);
    }
	return Count;
}
