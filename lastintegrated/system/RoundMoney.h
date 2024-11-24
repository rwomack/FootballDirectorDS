
/*

	RoundMoney.h: interface for the CRoundMoney class.

	(c) Rw Software 1994 - 2008


  ---------------------------------------------------------------
*/


#ifndef __ROUNDMONEY_H__
#define __ROUNDMONEY_H__

class CRoundMoney
{
public:
	static double			RoundValue(double _Amount);
	static uint				RoundMoneyNumSteps(double _Min, double _Max);
	static uint				MoneyStepSize(const uint _Amount);
	static uint				RoundMoneyIncrement(uint Num);
	static uint				RoundMoneyDecrement(uint Num);
	static double			RoundMoneyDecrement(double Num);

};
#endif	