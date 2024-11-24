
 /*

    Class:- CFinancialLoan

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"

#include "FinancialLoan.h"


	const uint			MINLOANAMOUNT = 10000;


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::DoResetVariables
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialLoan::DoResetVariables()
{
    m_LoanOwed = 0;
    m_LoanWeeklyRepayment = 0;
    m_LoanWeeksToGo = 0;
	m_AlreadyBorrowedToday = false;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::WeeksToRepayLoan
	Access:    	public 
	Parameter: 	CCountry& _rCountry
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CFinancialLoan::WeeksToRepayLoan(CCountry& _rCountry)
{
	ushort Count = 0;
    int LoanOwed = m_LoanOwed;
	int LoanWeeklyRepayment = GetLoanRepayment();
    while(LoanOwed > 0)
	{
		LoanOwed += (int)(LoanOwed * _rCountry.GetLoanInterestWeeklyMultiplier());		// Add the interest on
		LoanOwed -= min(LoanWeeklyRepayment, LoanOwed);
		Count += (LoanOwed > 0);
    }
	return Count;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::GetRecommendedLoanBorrow
	Access:    	public 
	Parameter: 	CPopularity* _pPopularity
	Parameter: 	const ushort _nHowLong
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CFinancialLoan::GetRecommendedLoanBorrow(CPopularity* _pPopularity, const ushort _nHowLong )
{
	int dRet = _pPopularity->GetLoanAmount() * _nHowLong;
	dRet -= m_LoanOwed;
	if (dRet < 100)
	{
		dRet = 0;
	}
	return dRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::DoApplyLoan
	Access:    	public 
	Parameter: 	uint HowMuch
	Parameter: 	CCountry& rCountry
	Parameter: 	CPopularity* pPopularity
	Parameter: 	const ushort HowLong
	Returns:   	bool
	Qualifier: 	
	Purpose:	Give a loan if request is ok
------------------------------------------------------------------------------*/
bool CFinancialLoan::DoApplyLoan(uint _HowMuch, CCountry& _rCountry, CPopularity* _pPopularity, const ushort _HowLong )
{
	_HowMuch = _rCountry.GetCurrency().ConvToPounds(_HowMuch);
	if (_HowMuch < MINLOANAMOUNT)
	{
		UserMessageBox(IDS_LOANTOOSMALL);
		return false;
	}

    if (m_AlreadyBorrowedToday == true || (_HowMuch + m_LoanOwed) > _pPopularity->GetLoanAmount() * _HowLong)
	{
		UserMessageBox(IDS_LOANREFUSED);
		return false;
	}
	else
	{
		CString str;
		str.Format(IDS_LOANACCEPTED, _rCountry.GetCurrency().GetForeignStr(_HowMuch), _rCountry.GetActiveCountry().GetLoanRate(), \
									  _rCountry.GetCurrency().GetForeignStr(
										DoCalcRepayment(_rCountry.GetActiveCountry().GetLoanRate(), (uint)_HowMuch, _HowLong, 52)),
									  _HowLong);

		if (UserMessageBox(str, MB_YESNO + MB_ICONEXCLAMATION + MB_DEFBUTTON1) == IDYES)
		{
			m_AlreadyBorrowedToday = true;
			DoAcceptLoan(_HowMuch, _rCountry, _HowLong);
            return true;
		}
		else
		{
			return false;
		}
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::GetLoanRepayment
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CFinancialLoan::GetLoanRepayment()
{
	if (m_LoanOwed < m_LoanWeeklyRepayment)
	{
		m_LoanWeeklyRepayment = m_LoanOwed;
	}
	return m_LoanWeeklyRepayment;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::DoAcceptLoan
	Access:    	public 
	Parameter: 	const int _HowMuch
	Parameter: 	CCountry& _rCountry
	Parameter: 	const ushort _nHowLong
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialLoan::DoAcceptLoan(const int _HowMuch, CCountry& _Country, const ushort _HowLong /*= DEFAULTLOANPERIOD*/)
{
	m_LoanOwed += _HowMuch;
	m_LoanWeeklyRepayment = DoCalcRepayment(_Country.GetActiveCountry().GetLoanRate(), m_LoanOwed, _HowLong);
	m_LoanWeeksToGo = 52 * _HowLong;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::LoanOwedStr
	Access:    	public 
	Parameter: 	CCountry& _rCountry
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CFinancialLoan::LoanOwedStr(CCountry& _rCountry)
{
	return _rCountry.GetCurrency().GetForeignStr(GetLoanOwed());
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::LoanRepaymentStr
	Access:    	public 
	Parameter: 	CCountry& _rCountry
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CFinancialLoan::LoanRepaymentStr(CCountry& _rCountry)
{
	return _rCountry.GetCurrency().GetForeignStr(GetLoanRepayment());
}



/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::ForeignLoanOwed
	Access:    	public 
	Parameter: 	CCountry& _rCountry
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CFinancialLoan::ForeignLoanOwed(CCountry& _rCountry)
{
	return _rCountry.GetCurrency().ConvToForeign(m_LoanOwed);
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::DoWeeklyPayment
	Access:    	public 
	Parameter: 	CCountry& _rCountry
	Returns:   	uint
	Qualifier: 	
	Purpose:	Repay loan if got one
------------------------------------------------------------------------------*/
uint CFinancialLoan::DoWeeklyPayment(CCountry& _rCountry)
{
	int dRet = 0;
    if (m_LoanOwed > 0)
	{
    	m_LoanWeeksToGo--;
		m_LoanOwed += (m_LoanOwed * _rCountry.GetLoanInterestWeeklyMultiplier());		// Add the interest on
		dRet = min(GetLoanRepayment(), m_LoanOwed);
		m_LoanOwed -= dRet;
	}
	else
	{
		m_LoanWeeksToGo = 0;
		m_LoanWeeklyRepayment = 0;
    }
	return dRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::DoRepayLoan
	Access:    	public 
	Parameter: 	const uint _HowMuch
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CFinancialLoan::DoRepayLoan(const uint _HowMuch)
{
	uint dRet = min(m_LoanOwed, _HowMuch);
	m_LoanOwed -= dRet;
	if (m_LoanOwed < 1)
	{
		DoResetVariables();
	}
	return dRet;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::GetLoanOwed
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CFinancialLoan::GetLoanOwed() const
{
	return m_LoanOwed;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::SetLoanOwed
	Access:    	public 
	Parameter: 	const uint _Amount
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialLoan::SetLoanOwed(const uint _Amount)
{
	EXT_ASSERT(_Amount > 0); 
	m_LoanOwed = _Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::SetLoanRepayment
	Access:    	public 
	Parameter: 	const uint _Repayment
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialLoan::SetLoanRepayment(const uint _Repayment)
{
	EXT_ASSERT(_Repayment > 0); 
	m_LoanWeeklyRepayment = _Repayment;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::SetLoanWeeksToGo
	Access:    	public 
	Parameter: 	const ushort _WeeksToGo
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialLoan::SetLoanWeeksToGo(const ushort _WeeksToGo)
{
	EXT_ASSERT(_WeeksToGo > 0); 
	m_LoanWeeksToGo = _WeeksToGo;
}


/*------------------------------------------------------------------------------
	Method:   	CFinancialLoan::SetAlreadyBorrowedToday
	Access:    	public 
	Parameter: 	const bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CFinancialLoan::SetAlreadyBorrowedToday(const bool _Value)
{
	m_AlreadyBorrowedToday = _Value;
}
