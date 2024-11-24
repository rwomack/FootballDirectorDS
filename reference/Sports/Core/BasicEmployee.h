

 /*
	C++ Classes

    BasicEmployee.h: interface for the CBasicEmployee class.


	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
*/

#if !defined(AFX_BASICEMPLOYEE_H__95801F02_2F3C_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_BASICEMPLOYEE_H__95801F02_2F3C_11D1_A3F5_00001C303EC4__INCLUDED_


class CEmployeeBase : public CPerson
{

public:
    // Employee types
	enum eEmployeeType
	{
        COACH,
        PHYSIO,
        //SCOUT,
		//YOUTHCOACH,
		//ASSISTANTMANAGER,
		MAXEMPLOYEETYPES
	};

    CEmployeeBase();
	CEmployeeBase(const eEmployeeType Type, CCountry& rCountry, CPopularity& aPopularity);
    virtual ~CEmployeeBase();

    // Data Access Methods
	eEmployeeType			GetType();
    byte					GetSkill();
	void					SetType(const eEmployeeType x);
	void					SetSkill(const byte _Skill);

	// Methods
    uint					DoCalculateMaximumWage();
	uint					DoCalculateMinimumWage();
	void					DoGenerateRandomContract(CContract* _pContractRequired, CCountry& _rCountry);
	uint					DoGenerateRandomWage(CCountry& _rCountry);
	void					DoRegenerate(CCountry& _Country);
	void                    DoResetVariables();
    void					RandomPosition();
    CString		            GetSkillStr();
    uint					TypeText() const;
    CString		            TypeTextStr();

	// Variables
private:
    eEmployeeType			m_type;
    byte					m_Skill;
};

#endif // !defined(AFX_BASICEMPLOYEE_H__95801F02_2F3C_11D1_A3F5_00001C303EC4__INCLUDED_)
