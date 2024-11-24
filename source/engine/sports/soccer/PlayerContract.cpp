
/*

    Class:- PlayerContract


    Football Director

	(c) Rw Software 1994 - 2008

	---------------------------------------------------------------
 */

#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CContract::CContract
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CContract::CContract()
	: m_SignOnFee(0)
	, m_Wage(0)
    , m_ContractLength(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CContract::Copy
	Access:    	public 
	Parameter: 	const CContract *pContract
	Returns:   	CContract&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CContract& CContract::Copy(const CContract *pContract)
{
	m_SignOnFee = pContract->m_SignOnFee;
	m_Wage = pContract->m_Wage;
	m_ContractLength = pContract->m_ContractLength;
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CContract::operator==
	Access:    	public 
	Parameter: 	const CContract* pContract
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CContract::operator == (const CContract* pContract)
{
	if (m_SignOnFee != pContract->m_SignOnFee)
	{
		return false;
	}
	if (m_Wage != pContract->m_Wage)
	{
		return false;
	}
	if (m_ContractLength != pContract->m_ContractLength)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CContract::operator>=
	Access:    	public 
	Parameter: 	const CContract& _Contract
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CContract::operator >= (const CContract& _Contract)
{
	if (m_SignOnFee < _Contract.m_SignOnFee)
	{
		return false;
	}
	if (m_Wage < _Contract.m_Wage)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CContract::operator<
	Access:    	public 
	Parameter: 	const CContract* pPlayerContract
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CContract::operator < (const CContract* pPlayerContract)
{
	if (m_SignOnFee < pPlayerContract->m_SignOnFee)
	{
		return false;
	}
	if (m_Wage < pPlayerContract->m_Wage)
	{
		return false;
	}
	if (m_ContractLength < pPlayerContract->m_ContractLength)
	{
		return false;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CContract::GetContractEndDate
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CContract::GetContractEndDate()
{
	CContractEndTime TheDate(WorldData().GetCurrentDate());
	TheDate.DoEndOfSeason();
	for (int LoopCount = 0; LoopCount < m_ContractLength; LoopCount++)
	{
		TheDate.AddAYear();
	}
	return TheDate.LongDateStr();
}


/*------------------------------------------------------------------------------
	Method:   	CContract::IsBetterThan
	Access:    	public 
	Parameter: 	const CContract* pPlayerContract
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CContract::IsBetterThan(const CContract* pPlayerContract)
{
	if (m_SignOnFee < pPlayerContract->m_SignOnFee)
	{
		return false;
	}
	if (m_Wage < pPlayerContract->m_Wage)
	{
		return false;
	}
	if (m_ContractLength < pPlayerContract->m_ContractLength)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CContract::SetValues
	Access:    	public 
	Parameter: 	const uint a_nSignonFee
	Parameter: 	const uint a_nWage
	Parameter: 	const ushort a_nContractLength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContract::SetValues(const uint _SignonFee, const uint _Wage, const short _ContractLength)
{
	m_SignOnFee = _SignonFee;
	m_Wage = _Wage;
	SetContractLength(_ContractLength);
}


/*------------------------------------------------------------------------------
	Method:   	CContract::GetSignOnFee
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CContract::GetSignOnFee() const				
{ 
	return m_SignOnFee; 
}


/*------------------------------------------------------------------------------
	Method:   	CContract::GetWage
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CContract::GetWage() const					
{ 
	return m_Wage; 
}


/*------------------------------------------------------------------------------
	Method:   	CContract::GetContractLength
	Access:    	public const 
	Parameter: 	void
	Returns:   	short
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CContract::GetContractLength() const
{ 
	return m_ContractLength; 
}


/*------------------------------------------------------------------------------
	Method:   	CContract::SetSignOnFee
	Access:    	public 
	Parameter: 	const uint a_nSignonFee
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContract::SetSignOnFee(const uint a_nSignonFee)			
{ 
	m_SignOnFee = a_nSignonFee; 
}


/*------------------------------------------------------------------------------
	Method:   	CContract::SetWage
	Access:    	public 
	Parameter: 	const uint a_nWage
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContract::SetWage(const uint _Wage)
{ 
	EXT_ASSERT(_Wage > 0);
	m_Wage = _Wage; 
}


/*------------------------------------------------------------------------------
	Method:   	CContract::SetContractLength
	Access:    	public 
	Parameter: 	const ushort _ContractLength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContract::SetContractLength(const ushort _ContractLength)
{ 
	EXT_ASSERT(_ContractLength > 0 && _ContractLength < 8);
	m_ContractLength = _ContractLength; 
	EXT_ASSERT(m_ContractLength > 0 && m_ContractLength < 8);
}


/*------------------------------------------------------------------------------
	Method:   	CContract::DoAddContract
	Access:    	public 
	Parameter: 	const ushort a_nContractLength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CContract::DoAddContract(const short _ContractLength)
{ 
	m_ContractLength += _ContractLength; 
	EXT_ASSERT(m_ContractLength > 0 && m_ContractLength < 8);
}
