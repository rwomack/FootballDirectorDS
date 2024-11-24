
/*

	RoundMoney.h: interface for the CRoundMoney class.

	(c) Rw Software 1994 - 2007

---------------------------------------------------------------
*/


#ifndef __ROUNDMONEY_H__
#define __ROUNDMONEY_H__

class CRoundMoney
{
public:
  static float     RoundValue(float _Amount);
  static int        RoundMoneyNumSteps(float _Min, float _Max);
	static uint				MoneyStepSize(const uint _Amount);
  static float     RoundMoneyIncrement(float _Num);
	static uint				RoundMoneyIncrement(uint Num);
	static uint				RoundMoneyDecrement(uint Num);
  static float     RoundMoneyDecrement(float Num);

};
#endif
