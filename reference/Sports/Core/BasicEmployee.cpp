
 /*

    Class:- BasicEmployee

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"
#include "BasicEmployee.h"



/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::CBasicEmployee
	Access:    	public 
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployeeBase::CEmployeeBase()
{
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeBase::CEmployeeBase
	Access:    	public 
	Parameter: 	const eEmployeeType _Type
	Parameter: 	CCountry & _rCountry
	Parameter: 	CPopularity & _Popularity
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployeeBase::CEmployeeBase(const eEmployeeType _Type, CCountry& _rCountry, CPopularity& _Popularity)
{
	DoGenerateRandomName(_rCountry);
	SetType(_Type);
	SetSkill(max(min(static_cast<int>(_Popularity.GetRandomEmployeeSkill()), 99), 30));
	CPerson::DoCreate(35 + RandomNumber.IntLessThan(24), static_cast<ushort>(1 + RandomNumber.IntLessThan(3)), _rCountry);
	CPerson::SetWage(DoGenerateRandomWage(_rCountry));
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::~CBasicEmployee
	Access:    	public 
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployeeBase::~CEmployeeBase()
{
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeBase::DoRegenerate
	Access:    	public 
	Parameter: 	CCountry & _Country
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeBase::DoRegenerate(CCountry& _Country)
{
	DoGenerateRandomName(_Country);
	RandomPosition();
	SetSkill(25 + RandomNumber.IntLessThan(70));
	CPerson::DoCreate(35 + RandomNumber.IntLessThan(24), static_cast<ushort>(1 + RandomNumber.IntLessThan(3)), _Country);
	CPerson::SetWage(DoGenerateRandomWage(_Country));
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeBase::DoResetVariables()
{
    m_type = COACH;
    m_Skill = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::RandomPosition
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeBase::RandomPosition()
{
    m_type = static_cast <eEmployeeType>(RandomNumber.IntLessThan(MAXEMPLOYEETYPES));
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::TypeText
	Access:    	public const 
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CEmployeeBase::TypeText() const
{
	static const uint EmployeeTexts[] =
	{
		IDS_COACH, IDS_PHYSIO, IDS_SCOUT, IDS_YOUTHCOACH
	};
	return EmployeeTexts[m_type];
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::TypeTextStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CEmployeeBase::TypeTextStr()
{
	CString str;
	str.LoadString(TypeText());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::GetSkillStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CEmployeeBase::GetSkillStr()
{
	/*int EmployeeTexts[] =
	{
		IDS_SKILLTEXT1,		// 10 - 19
		IDS_SKILLTEXT2,
		IDS_SKILLTEXT3,		// 30 - 39
		IDS_SKILLTEXT4,
		IDS_SKILLTEXT5,
		IDS_SKILLTEXT6,
		IDS_SKILLTEXT7,
		IDS_SKILLTEXT8,
		IDS_SKILLTEXT9,		// 90 - 99
	}; */
	CString str;
	str.Format(_T("%d"), GetSkill());
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::DoCalculateMinimumWage
	Access:    	public 
	Returns:   	uint - Minimum employee wage
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CEmployeeBase::DoCalculateMinimumWage()
{
    return WorldData().GetSkillMoneyValueItem(GetSkill()).GetEmployeeWage().GetBasicValue();
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeBase::GetSkill
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CEmployeeBase::GetSkill()
{
	return m_Skill;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeBase::GetType
	Access:    	public 
	Returns:   	CEmployeeBase::eEmployeeType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployeeBase::eEmployeeType CEmployeeBase::GetType()
{
	return m_type;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::DoCalculateMaximumWage
	Access:    	public 
	Returns:   	uint - Maximum employee wage
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CEmployeeBase::DoCalculateMaximumWage()
{
    return WorldData().GetSkillMoneyValueItem(GetSkill()).GetEmployeeWage().GetMaximumValue();
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::DoGenerateRandomWage
	Access:    	public 
	Parameter: 	CCountry& _Country
	Returns:   	uint - Employee wage
	Qualifier: 	
	Purpose:	
------------------------------------------------------------------------------*/
uint CEmployeeBase::DoGenerateRandomWage(CCountry& _Country)
{
    return _Country.GetCurrency().RoundedPounds(static_cast<double>(WorldData().GetSkillMoneyValueItem(GetSkill()).GetEmployeeWage().RandomAmount()));
}


/*------------------------------------------------------------------------------
	Method:   	CBasicEmployee::DoGenerateRandomContract
	Access:    	public 
	Parameter: 	CContract* _pContract
	Parameter: 	CCountry& _rCountry
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeBase::DoGenerateRandomContract(CContract* _pContract, CCountry& _rCountry)
{
	EXT_ASSERT_POINTER(_pContract, CContract);
	_pContract->SetWage(DoGenerateRandomWage(_rCountry));
	_pContract->SetContractLength(2 + RandomNumber.IntLessThan(5));
	if (CPerson::GetAge() + _pContract->GetContractLength() > 5)
	{
		_pContract->SetContractLength(5 - CPerson::GetAge());
	}
	EXT_ASSERT(_pContract->GetContractLength() > 0);
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeBase::SetSkill
	Access:    	public 
	Parameter: 	const byte _Skill
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeBase::SetSkill(const byte _Skill)
{
	m_Skill = _Skill;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeBase::SetType
	Access:    	public 
	Parameter: 	const eEmployeeType x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeBase::SetType(const eEmployeeType x)
{
	m_type = x;
}
