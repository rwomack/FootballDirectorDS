
/*

	SkillMoneyValues Class
	

  	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */


#if !defined(AFX_SKILLMONEYVALUES_H__B162CE21_48C7_11D2_B220_0080C883C4EB__INCLUDED_)
#define AFX_SKILLMONEYVALUES_H__B162CE21_48C7_11D2_B220_0080C883C4EB__INCLUDED_


class CSkillMoneyValue 
{
public:
    // Methods
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	CRangedValue<uint>&		GetPlayerValue();
	CRangedValue<uint>&		GetPlayerWage();
	CRangedValue<uint>&		GetEmployeeWage();
	uint					GetBasicSigningFee();
	uint					GetMaximumSigningFee();
	uint					GetRandomSigningFee();

	// Variables
private:
    CRangedUInt				m_PlayerValue;
    CRangedUInt				m_PlayerWage;
    CRangedUInt				m_EmployeeWage;
};


//----------------------------------------------------------------------------
class CSkillMoneyValueList : public CExtArray 
{
public:
    CSkillMoneyValueList();
    ~CSkillMoneyValueList();

    // Methods
    CSkillMoneyValue&       operator [](ushort x);

	// Variables
private:
	CSkillMoneyValue*		m_pList;
};
#endif // !defined(AFX_SKILLMONEYVALUES_H__B162CE21_48C7_11D2_B220_0080C883C4EB__INCLUDED_)
