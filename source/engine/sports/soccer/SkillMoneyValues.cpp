
 /*

    SkillMoneyValues.cpp: implementation of the CSkillMoneyValues class.


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValue::GetPlayerValue
	Access:    	public 
	Returns:   	CRangedValue<uint>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRangedValue<uint>& CSkillMoneyValue::GetPlayerValue() 
{ 
	return m_PlayerValue; 
}


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValue::GetPlayerWage
	Access:    	public 
	Returns:   	CRangedValue<uint>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRangedValue<uint>& CSkillMoneyValue::GetPlayerWage() 
{ 
	return m_PlayerWage; 
}	


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValue::GetEmployeeWage
	Access:    	public 
	Returns:   	CRangedValue<uint>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRangedValue<uint>& CSkillMoneyValue::GetEmployeeWage() 
{ 
	return m_EmployeeWage; 
}	


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValue::GetBasicSigningFee
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CSkillMoneyValue::GetBasicSigningFee()
{
	return m_PlayerValue.GetBasicValue() / 10;
}


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValue::GetMaximumSigningFee
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CSkillMoneyValue::GetMaximumSigningFee()
{
	return m_PlayerValue.GetMaximumValue() / 10;
}


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValue::GetRandomSigningFee
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CSkillMoneyValue::GetRandomSigningFee()
{
	return m_PlayerValue.RandomAmount() / 10;
}


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValue::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSkillMoneyValue::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_PlayerValue.ReadResource(_Resource);
	m_PlayerValue.SetBasicValue(m_PlayerValue.GetBasicValue() * 2); // Increase player values
	m_PlayerWage.ReadResource(_Resource);
	m_PlayerWage.SetBasicValue(m_PlayerWage.GetBasicValue() * 8); // Increase player wages
	m_EmployeeWage.ReadResource(_Resource);
	m_EmployeeWage.SetBasicValue(m_EmployeeWage.GetBasicValue() * 4); // Increase employee wages
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValueList::CSkillMoneyValueList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSkillMoneyValueList::CSkillMoneyValueList()
{
	CBinaryReader theResource(0, IDR_SKILLMONEYVALUE, TEXT("FDDATA"));
	CExtArray::ReadSizeFromResource(theResource);
	EXT_ASSERT(CExtArray::GetSize() > 0);

	m_pList = FD_ENGINE_NEW CSkillMoneyValue[CExtArray::GetSize()];
    for (uint ListPos = 0; ListPos < CExtArray::GetSize(); ListPos ++)
	{
		m_pList[ListPos].DoInitialiseFromResource(theResource);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValueList::~CSkillMoneyValueList
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSkillMoneyValueList::~CSkillMoneyValueList()
{
	delete[] m_pList;
}


/*------------------------------------------------------------------------------
	Method:   	CSkillMoneyValueList::operator[]
	Access:    	public 
	Parameter: 	ushort x
	Returns:   	CSkillMoneyValue&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CSkillMoneyValue& CSkillMoneyValueList::operator[](ushort x)
{
    EXT_ASSERT(x < GetSize());
	return static_cast<CSkillMoneyValue&> (m_pList[x]);
}

