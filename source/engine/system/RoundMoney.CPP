
/*


	(c) Rw Software 1994 - 2007

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "RoundMoney.h"
#include "math.h"


/*------------------------------------------------------------------------------
	Method:   	CRoundMoney::RoundValue
	Access:    	public
  Parameter:  float _Amount
  Returns:    float
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
float CRoundMoney::RoundValue(float _Amount)
{
  float RetAmount = 0;
    if (_Amount < 0)
	{
		RetAmount = 0;
	}
    else if (_Amount < 20)
	{
        RetAmount = static_cast<int> (floor(_Amount + 0.5));
	}
    else
	{
		uint Step = MoneyStepSize(_Amount);
		RetAmount = (uint)((_Amount + Step / 2) / Step) * Step;
	}
	return 	RetAmount;
}


/*------------------------------------------------------------------------------
	Method:   	CRoundMoney::RoundMoneyNumSteps
	Access:    	public
  Parameter:  float _Min
  Parameter:  float _Max
	Returns:   	int
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
int CRoundMoney::RoundMoneyNumSteps(float _Min, float _Max)
{
  FDASSERT (_Min < _Max);
    int iRet = 0;
    _Min = RoundValue(_Min);
    _Max = RoundValue(_Max);

    while (_Min < _Max)
	{
        _Min += MoneyStepSize(_Min);
        iRet ++;
	}
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CRoundMoney::MoneyStepSize
	Access:    	public
	Parameter: 	const uint _Amount
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CRoundMoney::MoneyStepSize(const uint _Amount)
{
	uint Step = 1;
    if (_Amount >= 20 && _Amount < 100)
	{
        Step = 5;
    }
    else if (_Amount >= 100 && _Amount < 250)
	{
        Step = 10;
    }
    else if (_Amount >= 250 && _Amount < 500)
	{
        Step = 25;
    }
    else if (_Amount >= 500 && _Amount < 1000)
	{
        Step = 50;
    }
    else if (_Amount >= 1000 && _Amount < 10000)
	{
        Step = 100;
    }
    else if (_Amount >= 10000 && _Amount < 100000)
	{
        Step = 500;
    }
    else if (_Amount >= 100000 && _Amount < 1000000)
	{
        Step = 5000;
    }
    else if (_Amount >= 1000000 && _Amount < 5000000)
	{
        Step = 50000;
    }
	else if (_Amount >= 5000000 && _Amount < 10000000)
	{
        Step = 100000;
    }
	else if (_Amount >= 10000000)
	{
        Step = 250000;
    }
	return Step;
}


/*------------------------------------------------------------------------------
	Method:   	CRoundMoney::RoundMoneyIncrement
	Access:    	public
  Parameter:  float _Num
  Returns:    float
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
float CRoundMoney::RoundMoneyIncrement(float _Num)
{
  float dRet = RoundValue(_Num);
	if (dRet > _Num)
	{
		return dRet;
	}

	_Num = dRet + MoneyStepSize(_Num);
    return _Num;
}


/*------------------------------------------------------------------------------
	Method:   	CRoundMoney::RoundMoneyIncrement
	Access:    	public
	Parameter: 	uint _Num
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CRoundMoney::RoundMoneyIncrement(uint _Num)
{
  uint dRet = RoundValue(static_cast<float>(_Num));
	if (dRet > _Num)
	{
		return dRet;
	}

	_Num = dRet + MoneyStepSize(_Num);
    return _Num;
}


/*------------------------------------------------------------------------------
	Method:   	CRoundMoney::RoundMoneyDecrement
	Access:    	public
	Parameter: 	uint _Num
	Returns:   	uint
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
uint CRoundMoney::RoundMoneyDecrement(uint _Num)
{
  float dRet = RoundValue(static_cast<float>(_Num));
	if (dRet < _Num || _Num == 0)
	{
		return dRet;
	}

	_Num = dRet - MoneyStepSize(_Num);
    return _Num;
}


/*------------------------------------------------------------------------------
	Method:   	CRoundMoney::RoundMoneyDecrement
	Access:    	public
  Parameter:  float _Num
  Returns:    float
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
float CRoundMoney::RoundMoneyDecrement(float _Num)
{
  float dRet = RoundValue(_Num);
	if (dRet < _Num || _Num == 0)
	{
		return dRet;
	}

	_Num = dRet - MoneyStepSize(_Num);
    return _Num;
}
