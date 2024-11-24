
/*

	ManagerContractOffer.cpp

	Football Director

	(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CManagerContractOffer::CManagerContractOffer
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManagerContractOffer::CManagerContractOffer()
{
}


/*------------------------------------------------------------------------------
	Method:   	CManagerContractOffer::~CManagerContractOffer
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CManagerContractOffer::~CManagerContractOffer()
{
}


/*------------------------------------------------------------------------------
	Method:   	CManagerContractOffer::SetDetails
	Access:    	public 
	Parameter: 	const uint _Amount
	Parameter: 	const ushort _Years
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerContractOffer::SetDetails(const uint _Amount, const ushort _Years)
{
	SetWageOffer(_Amount);
	SetContractOffer(_Years);
}


/*------------------------------------------------------------------------------
	Method:   	CManagerContractOffer::SetWageOffer
	Access:    	public 
	Parameter: 	const uint _Amount
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerContractOffer::SetWageOffer(const uint _Amount)
{
	m_WageOffer = _Amount;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerContractOffer::SetContractOffer
	Access:    	public 
	Parameter: 	const ushort _Years
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CManagerContractOffer::SetContractOffer(const ushort _Years)
{
	m_ContractOffer = _Years;
}


/*------------------------------------------------------------------------------
	Method:   	CManagerContractOffer::GetWageOffer
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CManagerContractOffer::GetWageOffer() const 						
{ 
	return m_WageOffer; 
}


/*------------------------------------------------------------------------------
	Method:   	CManagerContractOffer::GetContractOffer
	Access:    	public const 
	Parameter: 	void
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ushort CManagerContractOffer::GetContractOffer() const					
{ 
	return m_ContractOffer; 
}



/*------------------------------------------------------------------------------
Method:   	CManagerContractOffer::DoResetVariables
Access:    	public 
Parameter: 	void
Returns:   	void
Qualifier: 	
Purpose:	Clear the manager's offered contract details
------------------------------------------------------------------------------*/
void CManagerContractOffer::DoResetVariables()
{
	m_WageOffer = 0;
	m_ContractOffer = 0;
}


/*------------------------------------------------------------------------------
Method:   	CManagerContractOffer::DoCalculateOffer
Access:    	public 
Parameter: 	const uint _WageOffer
Parameter: 	const ushort _ContractOffer
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CManagerContractOffer::DoCalculateOffer(const uint _WageOffer, const ushort _ContractOffer)
{
	if (GetWageOffer() == 0)
	{
		m_WageOffer = _WageOffer;
		m_ContractOffer = _ContractOffer;
	}
	else
	{
		m_WageOffer /= 100;
		m_WageOffer *= 102 + RandomNumber.IntLessThan(3);
		// Shorten length of contract?
		if (RandomNumber.IntLessThan(2) == 1 && m_ContractOffer > 1)
		{
			m_ContractOffer--;
		}
	}
}
