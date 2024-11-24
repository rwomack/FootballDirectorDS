


/*
    Class - C++ Employee Classes

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */



#ifndef __EMPLOYEE_H__
#define __EMPLOYEE_H__


#include "BasicEmployee.h"
#include "PlayerContract.h"


class CEmployee : public CEmployeeBase 
{
public:
    CEmployee(CCountry& rCountry);
    CEmployee(CEmployee* pEmployee);
	CEmployee(const eEmployeeType _Type, CCountry& _rCountry, CPopularity& _Popularity)
		: CEmployeeBase(_Type, _rCountry, _Popularity){};
    CEmployee();

	virtual ~CEmployee();
	// Other methods
	CClub&  GetClub();
	CString					ClubName();
    void				    DoCheckIfRequestNewContract(CClub& _Club);
    void					DoWeeklyTasks();
	void					DoEndOfSeason(const bool _bHumanManager);    
	void					DoGenerateRandomContract(CContract& _ContractOffered, CClub& _Club);
    void					DoNewContractRequest(CClub& _Club);
	void					DoOfferNegotiateNewContract(const CString &_Str);
	bool					DoSackIfContractExpired(bool _HumanManager, CClub& _Club);
	bool					GotNewContract();
    bool					IsUnemployed();
	bool					NeedNewContract();
    bool					Retire();
	CString					szContractResponse(eContractOfferResponse _eResponse, CContract& _ContractOffered, CClub& _Club);
	eContractOfferResponse	TryAcceptContract(CContract& _ContractRequired, CClub& _Club, CContract& _ContractOffered);
	bool					IsWillingToNegotiateNewContract(bool _bMessages);
};
///////////////////////////////////////////////////////////////////////////////////////////////


//----------------------------------------------------------------------------
class CEmployeeList : public CPointerArray<CEmployee>
{
public:
    CEmployeeList() {};					// Constructor
    ~CEmployeeList();					// Destructor

    // IO Methods
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

// Other methods
	void					DoWeeklyTasks(CCountry& _Country, bool _AvailableEmployees = false);
	void					DoEndOfSeason(bool _HumanManager, CClub* _Club = null);
	void					DoMoveToAvailableEmployees(CEmployee* _Employee, CClub& _Club);
    void					DoNewContractRequests(CClub& _Club);
	CEmployee*				RandomElement(const byte _Type, const byte _MinimumSkill, const byte _MaximumSkill);
    void					DoRequestNewContracts(CClub& _Club);
	void					SackIfContractExpired(bool _bHumanManager, CClub& _Club);
	int						DoCalculateWageBill();
};

#endif
