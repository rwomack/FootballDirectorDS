
 /*

    Class:- Employee


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "UI.h"
#include "FootballDirector.h"


	const ushort   MINAVAILEMPS = 6;
    const ushort   MAXAVAILEMPS = 10;



/*------------------------------------------------------------------------------
	Method:   	CEmployee::CEmployee
	Access:    	public 
	Parameter: 	CCountry& _rCountry
	Returns:   		
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployee::CEmployee(CCountry& _rCountry)
{
	DoRegenerate(_rCountry);
}


/*------------------------------------------------------------------------------
Method:   	CEmployee::CEmployee
Access:    	public 
Parameter: 	void
Returns:   	
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CEmployee::CEmployee()
{
	CEmployeeBase::DoResetVariables();
	CPerson::DoClearNewContract();
}

/*------------------------------------------------------------------------------
Method:   	CEmployee::~CEmployee
Access:    	public
Parameter: 	void
Returns:
Qualifier:
Purpose:	Destructor.
------------------------------------------------------------------------------*/
CEmployee::~CEmployee()
{
}

/*------------------------------------------------------------------------------
	Method:   	CEmployee::DoNewContractRequest
	Access:    	public 
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployee::DoNewContractRequest(CClub& _Club)
{
	CString str;
	if (WorldData().IsFastTest() == true)
	{
			return;
	}

	if (CPerson::GetContractEndDate().SeasonsRemaining() < 2)
	{
		switch(CPerson::GetContractRenewalStatus())
		{
			case NONEINPROGRESS:
				if (IsWillingToNegotiateNewContract(true) == true)
				{
                    str.Format(IDS_REQUESTSNEWCONTRACT, GetName(), _Club.GetName());
					DoOfferNegotiateNewContract(str);
				}
				break;
			case ACCEPTED:
			case NEGOTIATING:
			case REFUSED:
			case MORESIGNON:
			case LESSSIGNON:
			case MOREWAGE:
			case LESSWAGE:
			case LONGERCONTRACT:
			case SHORTERCONTRACT:
			case LISTED:
			case RETIRING:
				break;
			default :
				EXT_ASSERT(false);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::OfferNegotiateNewContract
	Access:    	public 
	Parameter: 	CString _Str
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployee::DoOfferNegotiateNewContract(const CString& _Str)
{
	if (WorldData().IsFastTest() == false)
	{
		theApp.OnEmployeeContractRenewalNegotiations(this, _Str);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::DoCheckIfRequestNewContract
	Access:    	public 
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployee::DoCheckIfRequestNewContract(CClub& _Club)
{
	if (WorldData().IsFastTest() == false)
	{
		CExtString str;
		if (CPerson::GetContractEndDate().SeasonsRemaining() == 1 && RandomNumber.IntLessThan(CHANCEREQUESTNEWCONTRACT) == 1)
		{
			if (IsWillingToNegotiateNewContract(false) == true &&
				CPerson::GetContractRenewalStatus() == NONEINPROGRESS)
			{
				str.Format(IDS_EMPLOYEECONTRACTNEARLYEXPIRED, CEmployeeBase::TypeTextStr(), GetName());
				DoOfferNegotiateNewContract(str);
				if (CPerson::GetContractEndDate().SeasonsRemaining() == 1)
				{
					CPerson::SetContractRenewalStatus(CLUBREFUSED);
				}
				_Club.AddToClubNews(str);
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::SackIfContractExpired
	Access:    	public 
	Parameter: 	bool _bHumanManager
	Parameter: 	CClub& _Club
	Returns:   	bool
	Qualifier: 	
	Purpose:	Offer manager chance to renew employee contract
------------------------------------------------------------------------------*/
bool CEmployee::DoSackIfContractExpired(bool _HumanManager, CClub& _Club)
{
	bool bRet = false;
	if (CPerson::GetContractEndDate().SeasonsRemaining() == 0 && WorldData().IsFastTest() == false)
	{
		bRet = true;
		if (_HumanManager == true)
		{
			CExtString str;
			if (NeedNewContract() == true)
			{
				str.Format(IDS_EMPLOYEECONTRACTWILLEXPIRETODAY, GetName());
				DoOfferNegotiateNewContract(str);
			}
			if (CPerson::GetContractEndDate().SeasonsRemaining() == 0)
			{
				str.Format(IDS_EMPLOYEECONTRACTEXPIRED, GetName(), CEmployeeBase::TypeTextStr());
				_Club.AddToClubNews(str);
			}
			else
			{
				bRet = false;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::DoEndOfSeason
	Access:    	public 
	Parameter: 	bool _bHumanManager
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployee::DoEndOfSeason(bool _bHumanManager)
{
	if (_bHumanManager == true)
	{
		CPerson::DoEndOfSeason();
	}

	// Retire ??
	if (CPerson::GetAge() > EMPLOYEERETIREMENTAGE || (CPerson::GetAge() > (EMPLOYEERETIREMENTAGE - 4) && RandomNumber.IntLessThan(10) < 3))
	{
		if (_bHumanManager == false)
		{
			// Not managed by user so just make him younger!!
			CPerson::DoGenerateDateOfBirth(35 + RandomNumber.IntLessThan(15));
		}
		else
		{
			// At a user managed club so inform the manager
			if (WorldData().IsFastTest() == false)
			{
				CString str;
				str.Format(IDS_RETIRED, GetName(), CEmployeeBase::TypeTextStr());
				UserMessageBox(str);
			}
			CPerson::SetContractRenewalStatus(RETIRING);
			CPerson::GetContractEndDate().IncreaseLength(0);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::ClubName
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CEmployee::ClubName()
{
	if (WorldData().GetClubList().GetEmployeeClub(this) != null)
	{
		return GetClub().GetName();
	}
	else
	{
		return TEXT("-");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::GotNewContract
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CEmployee::GotNewContract()
{
    if (CPerson::GetContractRenewalStatus() == ACCEPTED)
	{
        return true;
    }
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::DoGenerateRandomContract
	Access:    	public 
	Parameter: 	CContract& _ContractOffered
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployee::DoGenerateRandomContract(CContract& _ContractOffered, CClub& _Club)
{
	_ContractOffered.SetWage(CEmployeeBase::DoGenerateRandomWage(_Club.GetCountry()));
	_ContractOffered.SetContractLength(1 + RandomNumber.IntLessThan(5));
	if ((GetAge() + _ContractOffered.GetContractLength()) > EMPLOYEERETIREMENTAGE)
	{
		_ContractOffered.SetContractLength(EMPLOYEERETIREMENTAGE - GetAge());
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::DoEndOfWeek
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployee::DoWeeklyTasks()
{
	CPerson::DoWeeklyTasks();
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::GetClub
	Access:    	public 
	Parameter: 	void
	Returns:   	CClub&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CClub& CEmployee::GetClub()
{
    CClub* pClub = static_cast<CClub*> (WorldData().GetClubList().GetEmployeeClub(this));
	if (pClub == null)
	{
		EXT_ASSERT_POINTER(WorldData().GetCurrentUserClub(), CClub);
		pClub = WorldData().GetCurrentUserClub();
	}
	return *pClub;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::IsWillingToNegotiateNewContract
	Access:    	public 
	Parameter: 	bool _bMessages
	Returns:   	bool
	Qualifier: 	
	Purpose:	Decide whether to retire or leave at end of contract
------------------------------------------------------------------------------*/
bool CEmployee::IsWillingToNegotiateNewContract(bool _bMessages)
{
	CString str;

	// See if he wants to retire
	if (CPerson::GetAge() > (EMPLOYEERETIREMENTAGE - 6) && RandomNumber.IntLessThan(EMPLOYEERETIREMENTAGE - CPerson::GetAge()) == 1) 
	{
		CPerson::SetContractRenewalStatus(RETIRING);
		if (_bMessages == true)
		{
            str.Format(IDS_RETIREATENDOFCONTRACT, GetName());
			UserMessageBox(str);
		}
		return false;
	}

	// See if he wishes to move anyway
	if (RandomNumber.IntLessThan(CHANCEREFUSECONTRACT) == 1)
	{
		CPerson::SetContractRenewalStatus(REFUSED);
		if (_bMessages == true)
		{
            str.Format(IDS_LEAVEATENDOFCONTRACT, GetName());
			UserMessageBox(str);
		}
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::IsUnemployed
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CEmployee::IsUnemployed()
{
    if (CPerson::GetContractEndDate().SeasonsRemaining() == 0)
	{
        return true;
	}
    else
	{
        return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::NeedNewContract
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CEmployee::NeedNewContract()
{
    if (CPerson::GetContractEndDate().SeasonsRemaining() > 1)
	{
		// Only if got less than a season to run
        return false;
	}
    switch(CPerson::GetContractRenewalStatus())
	{
		case NONEINPROGRESS:
        case NEGOTIATING:
        case MORESIGNON:
        case LESSSIGNON:
        case MOREWAGE:
		case LESSWAGE:
        case LONGERCONTRACT:
        case SHORTERCONTRACT:
			return true;

		case REFUSED:
        case ACCEPTED:
		case RETIRING:
		case LISTED:
	        return false;

		default :
			EXT_ASSERT(false);
			return true;
    }
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::Retire
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CEmployee::Retire()
{
    if (CPerson::GetAge() >= EMPLOYEERETIREMENTAGE)
	{
        return true;
	}
    else
	{
        return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::szContractResponse
	Access:    	public 
	Parameter: 	eNewsType _eResponse
	Parameter: 	CContract& _ContractOffered
	Parameter: 	CClub& _Club
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CEmployee::szContractResponse(eContractOfferResponse _eResponse, CContract& _ContractOffered, CClub& _Club)
{
	CString szStr;
	switch(_eResponse)
	{
		case ACCEPTED:
			CPerson::DoUpdateContractOffered(_ContractOffered);
			if (IsUnemployed() == true)
			{
				szStr.Format(IDS_EMP_CONTRACTACCEPTED, GetName(), _Club.GetName(), _ContractOffered.GetContractEndDate());
			}
			else
			{
				// Contract renewal
				szStr.Format(IDS_EMP_CONTRACTRENEWALACCEPTED, GetName(), _Club.GetName(), _ContractOffered.GetContractEndDate());
			}
			break;
        case MORESIGNON:
			break;
        case LESSSIGNON:
			break;
        case MOREWAGE:
			szStr.Format(IDS_WAGETOOLOW, GetName());
			break;
		case LESSWAGE:
			szStr.Format(IDS_WAGETOOHIGH, GetName());
			break;
        case LONGERCONTRACT:
			szStr.Format(IDS_CONTRACTTOOSHORT, GetName());
			break;
        case SHORTERCONTRACT:
			szStr.Format(IDS_CONTRACTTOOLONG, GetName());
			break;
		case LISTED:
		case RETIRING:
		case NONEINPROGRESS:
        case NEGOTIATING:
        case REFUSED:
			szStr.Format(IDS_CONTRACTREFUSED, GetName());
			break;
		default :
			EXT_ASSERT(false);		// Unhandled result
	}
    return szStr;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployee::TryAcceptContract
	Access:    	public 
	Parameter: 	CContract& _ContractRequired
	Parameter: 	CClub& _Club
	Parameter: 	CContract& _ContractOffered
	Returns:   	eNewsType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eContractOfferResponse CEmployee::TryAcceptContract(CContract& _ContractRequired, CClub& _Club, CContract& _ContractOffered)
{
	// Top employees won't go to low clubs
    if (CEmployeeBase::GetSkill() > 87 && _Club.IsGoodClub() == false)
	{
		return REFUSED;
    }

	// Accepts if demands are met
	if (_ContractOffered >= _ContractRequired)
	{
		return ACCEPTED;
	}

	CCountry& rCountry = _Club.GetCountry();
	CString Str;
	uint MinmWage = (DoCalculateMinimumWage() / 100) * rCountry.GetFinancialRatioPercentage();
	// If offered less than the minimum for skill
	if (_ContractOffered.GetWage() < _ContractRequired.GetWage())
	{
		if (MinmWage > _ContractOffered.GetWage())
		{
			_ContractRequired.SetWage(rCountry.GetCurrency().RoundedPounds(MinmWage + (_ContractRequired.GetWage() - MinmWage) / 2));
			return MOREWAGE;
		}

		// You'll have to offer more than this
		uint NewRequiredWage = rCountry.GetCurrency().RoundedPounds(MinmWage + (_ContractRequired.GetWage() - MinmWage) / 2);
		if (_ContractOffered.GetWage() < _ContractRequired.GetWage())
		{
			_ContractRequired.SetWage(NewRequiredWage);
			if (_ContractOffered.GetWage() < NewRequiredWage)
			{
				return MOREWAGE;
			}
		}

		uint MaxmWage = (DoCalculateMaximumWage() / 100) * rCountry.GetFinancialRatioPercentage();
		if (MaxmWage < _ContractOffered.GetWage())
		{
			_ContractRequired.SetWage(MaxmWage);
			return LESSWAGE;
		}
	}

	if (_ContractOffered.GetContractLength() < _ContractRequired.GetContractLength())
	{
		if (_ContractOffered.GetContractLength() > (int)RandomNumber.IntLessThan(50) || _ContractOffered.GetContractLength() < 1)
		{
			_ContractRequired.SetContractLength(_ContractRequired.GetContractLength() - RandomNumber.IntLessThan(_ContractRequired.GetContractLength()));
			return LONGERCONTRACT;
		}
	}

	if (_ContractOffered.GetContractLength() > 9)
	{
		_ContractRequired.SetContractLength(_ContractRequired.GetContractLength() - RandomNumber.IntLessThan(_ContractRequired.GetContractLength()));
		return SHORTERCONTRACT;
	}

    // Employee has accepted
	return ACCEPTED;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::~CEmployeeList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CEmployeeList::~CEmployeeList()
{
	RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::DoEndOfSeason
	Access:    	public 
	Parameter: 	bool HumanManager
	Parameter: 	CClub* _pClub
	Returns:   	void
	Qualifier: 	
	Purpose:	Decrease the  employees contracts
------------------------------------------------------------------------------*/
void CEmployeeList::DoEndOfSeason(bool _HumanManager, CClub* _Club /*= null*/)
{
	ushort ListPos;
    CEmployee* pEmployee;
    CCountry* pCountry = null;
	if (_Club != null)
	{
		pCountry = &_Club->GetCountry();
	}
    for (ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        GetAt(ListPos)->DoEndOfSeason(_HumanManager);
	}

	// Remove any employees who have retired
    for (ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        pEmployee = GetAt(ListPos);
		bool bWasAddedToCountryEmployees = false;
		if (_Club != null)
		{
			if (pEmployee->GetContractRenewalStatus() == REFUSED &&
				pEmployee->GetContractEndDate().SeasonsRemaining() == 0)
			{
				pCountry->GetActiveCountry().AddToAvailableEmployees(pEmployee);
				pEmployee->SetContractRenewalStatus(RETIRING);
				bWasAddedToCountryEmployees = true;
			}
		}
        if (pEmployee->GetContractRenewalStatus() == RETIRING && pEmployee->GetContractEndDate().SeasonsRemaining() == 0)
		{
			if (bWasAddedToCountryEmployees == false)
			{
				DeleteOne(ListPos);
			}
			else
			{			
				RemoveAt(ListPos);
			}
			ListPos --;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::SackIfContractExpired
	Access:    	public 
	Parameter: 	bool _bHumanManager
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeList::SackIfContractExpired(bool _bHumanManager, CClub& _Club)
{
    CEmployee* pEmployee;
    for (int ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        pEmployee = GetAt(ListPos);
		if (pEmployee->DoSackIfContractExpired(_bHumanManager, _Club) == true)
		{
			DoMoveToAvailableEmployees(pEmployee, _Club);
			ListPos --;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::MoveToAvailableEmployees
	Access:    	public 
	Parameter: 	CEmployee* _pEmployee
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeList::DoMoveToAvailableEmployees(CEmployee* _Employee, CClub& _Club)
{
    _Club.GetActiveCountry().AddToAvailableEmployees(_Employee);
	Remove(_Employee);
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeList::Write(CDataFile* _pDataFile)
{
    CEmployee* pEmployee;
	byte nCount = GetSize();
    _pDataFile->Write(&nCount, sizeof nCount);
    for (uint ListPos = 0; ListPos < nCount; ListPos ++)
	{
        pEmployee = GetAt(ListPos);
		const unsigned char ucHasEmployee = (pEmployee != NULL) ? 1 : 0;
		_pDataFile->Write(&ucHasEmployee, sizeof(ucHasEmployee));
        if (pEmployee != null)
		{
			pEmployee->Write(_pDataFile);
        }
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeList::Read(CDataFile* _pDataFile)
{
    CEmployee* pNewEmployee;
    RemoveList();           // Remove all existing Employees
	byte nCount;
    _pDataFile->Read(&nCount, sizeof nCount);
    EXT_ASSERT(nCount <= EMPLOYEESINSQUAD);
    for (uint ListPos = 0; ListPos < nCount; ListPos ++)
	{
		unsigned char ucHasEmployee;
        _pDataFile->Read(&ucHasEmployee, sizeof(ucHasEmployee));

        if (ucHasEmployee == 1)
		{
			pNewEmployee = new CEmployee;
			SetAtGrow(ListPos, pNewEmployee);
			pNewEmployee->Read(_pDataFile);
        }
    }
 }


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::DoCalculateWageBill
	Access:    	public 
	Parameter: 	void
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CEmployeeList::DoCalculateWageBill()
{
	int iRet = 0;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount ++)
	{
        iRet += static_cast<CEmployee*>(GetAt(LoopCount))->GetWage();
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::DoNewContractRequests
	Access:    	public 
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeList::DoNewContractRequests(CClub& _Club)
{
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount ++)
	{
		if (RandomNumber.IntLessThan(CHANCENEWCONTRACTREQUEST) == 1)
		{
            GetAt(LoopCount)->DoNewContractRequest(_Club);
            break;  // Only one a turn
        }
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::DoRequestNewContracts
	Access:    	public 
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeList::DoRequestNewContracts(CClub& _Club)
{
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        GetAt(LoopCount)->DoCheckIfRequestNewContract(_Club);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::RandomElement
	Access:    	public 
	Parameter: 	const byte _Type
	Parameter: 	const byte _MinimumSkill
	Parameter: 	const byte _MaximumSkill
	Returns:   	CEmployee* - null pointer if none suitable are found
	Qualifier: 	
	Purpose:	Find an employee of the specified type with skill in the required range
------------------------------------------------------------------------------*/
CEmployee* CEmployeeList::RandomElement(const byte _Type, const byte _MinimumSkill, const byte _MaximumSkill)
{
	CEmployee* pEmployee;
	CEmployee* pRetEmployee = null;
    for (int LoopCount = 0 ; LoopCount < GetSize(); LoopCount++)
	{
        pEmployee = GetAt(LoopCount);
        if (pEmployee != null && pEmployee->GetType() == _Type && \
			pEmployee->GetSkill() >= _MinimumSkill && pEmployee->GetSkill() <= _MaximumSkill)
		{
			if (RandomNumber.IntLessThan(10) < 6)
			{
				pRetEmployee = pEmployee;
			}
		}
	}
	return pRetEmployee;
}


/*------------------------------------------------------------------------------
	Method:   	CEmployeeList::DoEndOfWeek
	Access:    	public 
	Parameter: 	CCountry& _rCountry
	Parameter: 	bool _bAvailableEmployees
	Returns:   	void
	Qualifier: 	
	Purpose:	End Of Week for Available employee lists
				If employee has been available for a certain length of time then replace him
------------------------------------------------------------------------------*/
void CEmployeeList::DoWeeklyTasks(CCountry& _Country, bool _AvailableEmployees /*= false*/)
{
	if (_AvailableEmployees == true)
	{
		CEmployee* pEmployee;
		for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
		{
			pEmployee = GetAt(LoopCount);
			if (pEmployee != null)
			{
				pEmployee->DoWeeklyTasks();
   				if (pEmployee->GetWeeksHere() > MAXAVAILWEEKS || RandomNumber.IntLessThan(6) == 3)
				{
					pEmployee->DoRegenerate(_Country);
				}
			}
		}

	    // Create some more employees if not many available
		if (GetSize() < MINAVAILEMPS)
		{
    		int MaxCreate = RandomNumber.IntLessThan(MAXAVAILEMPS - GetSize());
			for (int LoopCount = 0; LoopCount < MaxCreate; LoopCount++)
			{
				Add(new CEmployee(_Country));
			}
  		}
	}
}
