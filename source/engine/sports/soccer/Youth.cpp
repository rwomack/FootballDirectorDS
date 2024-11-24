
 /*

        Class:- youth


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "UI.h"
#include "FootballDirector.h"


    const uint  MINAVAILYOUTHS = 8;
    const uint  MAXAVAILYOUTHS = 16;
	const int	YOUTHSINSQUAD = 12;


/*------------------------------------------------------------------------------
	Method:   	CYouth::CYouth
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CYouth::CYouth()
{
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::CYouth
	Access:    	public
	Parameter: 	CCountry& _Country
	Parameter: 	CPopularity* _Popularity
	Parameter: 	const byte _ClubDivStrength
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CYouth::CYouth(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength)
{
	DoRegenerate(_Country, _Popularity, _ClubDivStrength);
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::DoRegenerate
	Access:    	public
	Parameter: 	CCountry& _rCountry
	Parameter: 	CPopularity* _pPopularity
	Parameter: 	const byte _ClubDivStrength
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouth::DoRegenerate(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength)
{
	CPerson::DoCreateYouth(_Country);
	m_Skills.DoCreateYouth(CPerson::GetAge(), _Country, _Popularity, _ClubDivStrength);
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::DoCheckIfRequestNewContract
	Access:    	public
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouth::DoCheckIfRequestNewContract(CClub& _Club)
{
	if (_Club.GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD && CPerson::GetContractRenewalStatus() == NONEINPROGRESS)
	{
		if (GetAge() == 18 && RandomNumber.IntLessThan(CHANCEREQUESTNEWCONTRACT) == 1 && WorldData().IsFastTest() == false)
		{
			WorldData().SetCurrentYouth(this);
			CExtString str;
        str.Format(IDS_YOUTHCONTRACTNEARLYEXPIRED, GetName());
			_Club.AddToClubNews(str);
			theApp.OnPromoteYouth(str);
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::szContractResponse
	Access:    	public
	Parameter: 	eNewsType _eResponse
	Parameter: 	CContract* _ContractOffered
	Returns:   	CString
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CString CYouth::GetContractResponseStr(eContractOfferResponse _eResponse, CContract* _ContractOffered)
{
	CString strMessage;
	switch(_eResponse)
	{
		case ACCEPTED:
      strMessage.Format(IDS_CONTRACTACCEPTED, GetName(), GetClub().GetName());
			break;
        case MORESIGNON:
      strMessage.Format(IDS_SIGNONFEETOOLOW, GetName());
			break;
        case LESSSIGNON:
      strMessage.Format(IDS_SIGNONFEETOOHIGH, GetName());
			break;
        case MOREWAGE:
      strMessage.Format(IDS_WAGETOOLOW, GetName());
			break;
		case LESSWAGE:
      strMessage.Format(IDS_WAGETOOHIGH, GetName());
			break;
        case LONGERCONTRACT:
      strMessage.Format(IDS_CONTRACTTOOSHORT, GetName());
			break;
        case SHORTERCONTRACT:
      strMessage.Format(IDS_CONTRACTTOOLONG, GetName());
			break;
		case LISTED:
		case RETIRING:
		case NONEINPROGRESS:
        case NEGOTIATING:
        case REFUSED:
		default:
			EXT_ASSERT(false);		// Unhandled result
	}
	EXT_ASSERT(strMessage.IsEmpty() == false);
    return strMessage;
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::GetSkills
	Access:    	public
	Parameter: 	void
	Returns:   	CSoccerPlayer&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CSoccerPlayer& CYouth::GetSkills()
{
	return m_Skills;
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::TryAcceptContract
	Access:    	public
	Parameter: 	CContract* _ContractRequired
	Parameter: 	CCountry& _rCountry
	Parameter: 	CContract* _ContractOffered
	Returns:   	eNewsType
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
eContractOfferResponse CYouth::TryAcceptContract(CContract* _ContractRequired, CCountry& _rCountry, CContract* _ContractOffered)
{
	CString strMessage;
	// Ok if all requirements are exactly right
	if (*_ContractOffered >= *_ContractRequired)
	{
		return ACCEPTED;
	}

	uint MinmSignOnFee = m_Skills.DoCalculateSignOnFee(_rCountry, false);
	_ContractRequired->SetSignOnFee(MinmSignOnFee + (_ContractRequired->GetSignOnFee() - MinmSignOnFee) / 2);
    if (_ContractOffered->GetSignOnFee() < MinmSignOnFee && \
		_ContractOffered->GetSignOnFee() < _ContractRequired->GetSignOnFee())
	{
		return MORESIGNON;
    }
	// Now check not offering far too much
	MinmSignOnFee = m_Skills.DoCalculateMaximumSignOnFee(_rCountry, false);
    if (_ContractOffered->GetSignOnFee() > MinmSignOnFee)
	{
		return LESSSIGNON;
    }

	if (_ContractOffered->GetContractLength() > 9)
	{
		_ContractRequired->SetContractLength(_ContractOffered->GetContractLength() - RandomNumber.IntLessThan(_ContractRequired->GetContractLength()));
		return SHORTERCONTRACT;
    }

	uint MinmWage = m_Skills.DoCalculateMinimumWage(_rCountry);
    if (MinmWage > _ContractOffered->GetWage())
	{
		_ContractRequired->SetWage(MinmWage + (_ContractRequired->GetWage() - MinmWage) / 2);
		return MOREWAGE;
    }

    if (_ContractRequired->GetWage() < _ContractOffered->GetWage() && RandomNumber.IntLessThan(10) < 7)
	{
		_ContractRequired->SetWage(MinmWage + (_ContractRequired->GetWage() - MinmWage) / 2);
		return LESSWAGE;
	}

	// Ensure not offering a silly wage
	uint MaxmWage = m_Skills.DoCalculateMaximumWage(_rCountry);
    if (MaxmWage < _ContractOffered->GetWage())
	{
		_ContractRequired->SetWage(MaxmWage);
		return LESSWAGE;
    }

	if (_ContractRequired->GetContractLength() != _ContractOffered->GetContractLength())
	{
		if (_ContractOffered->GetContractLength() > (short)RandomNumber.IntLessThan(50) || _ContractOffered->GetContractLength() < 1)
		{
			_ContractOffered->DoAddContract(RandomNumber.IntLessThan(_ContractOffered->GetContractLength()));
			return LONGERCONTRACT;
		}
	}
	return ACCEPTED;
 }


/*------------------------------------------------------------------------------
	Method:   	CYouth::DoGenerateRandomContract
	Access:    	public
	Parameter: 	CContract& _ContractOffered
	Parameter: 	CCountry& _rCountry
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouth::DoGenerateRandomContract(CContract& _ContractOffered, CCountry& _rCountry)
{
	m_Skills.DoGenerateRandomContract(_ContractOffered, _rCountry, CPerson::GetAge(), false);
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::DoEndOfSeason
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouth::DoEndOfSeason()
{
    CPerson::DoEndOfSeason();
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::DoWeeklyTasks
	Access:    	public
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouth::DoWeeklyTasks(CClub& _Club)
{
#ifdef YOUTHTRAINING
    m_Skills.TrainYouth(CPerson::GetAge(), _Club);
#endif
	DoWeeklyTasks();
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::DoWeeklyTasks
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouth::DoWeeklyTasks()
{
	CPerson::DoWeeklyTasks();
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::DoResetVariables
	Access:    	public
	Parameter: 	void
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouth::DoResetVariables()
{
	CPerson::DoResetVariables();
    m_Skills.DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::GetClub
	Access:    	public
	Parameter: 	void
	Returns:   	CClub&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClub& CYouth::GetClub()
{
    return WorldData().GetClubList().GetYouthClub(this);
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::Write
	Access:    	public
	Parameter: 	CDataFile * _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:	Serializes the CYouth.
------------------------------------------------------------------------------*/
void CYouth::Write(CDataFile* _pDataFile)
{
	CPerson::Write(_pDataFile);
	_pDataFile->Write(&m_Skills, sizeof(m_Skills));
}


/*------------------------------------------------------------------------------
	Method:   	CYouth::Read
	Access:    	public
	Parameter: 	CDataFile *_pDataFile
	Returns:   	void
	Qualifier:
	Purpose:	Deserializes the CYouth.
------------------------------------------------------------------------------*/
void CYouth::Read(CDataFile* _pDataFile)
{
	CPerson::Read(_pDataFile);
	_pDataFile->Read(&m_Skills, sizeof (m_Skills));
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CYouthList::~CYouthList
	Access:    	public
	Returns:
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CYouthList::~CYouthList()
{
	RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::DoEndOfSeason
	Access:    	public
	Parameter: 	CClub* _pClub
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::DoEndOfSeason(CClub* _Club)
{
	if (GetSize() > 0)
	{
		CExtString str;
		CYouth* pYouth;
		for (int LoopCount = GetUpperBound(); LoopCount > 0; LoopCount--)
		{
			pYouth = GetAt(LoopCount);
			EXT_ASSERT_POINTER(pYouth, CYouth);
			pYouth->DoEndOfSeason();
			if (pYouth->GetAge() >= 18 && _Club != null && _Club->IsUserControlled() == true)
			{
				if (_Club->GetFirstTeamSquad().TotalSquadSize() < PLAYERSINSQUAD /*&& pYouth->GetContractRenewalStatus() == NONEINPROGRESS*/)
				{
					WorldData().SetCurrentYouth(pYouth);
          str.Format(IDS_YOUTHCONTRACTEXPIRESTODAY, pYouth->GetName());
					_Club->AddToClubNews(str);
					theApp.OnPromoteYouth(str);
				}
				// Check if he was promoted to the first team squad
				if (LoopCount > (int)GetUpperBound() || pYouth == GetAt(LoopCount))
				{
					// No, so remove him
          str.Format(IDS_PLAYERCONTRACTEXPIREDLEFTCLUB, pYouth->GetName());
					_Club->AddToClubNews(str);
					RemoveAt(LoopCount);
					LoopCount--;
				}
			}
		}
	}
//TODO  theApp.GetMainWnd()->UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::DoWeeklyTasks
	Access:    	public
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::DoWeeklyTasks(CClub& _Club)
{
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        GetAt(LoopCount)->DoWeeklyTasks(_Club);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::DoWeeklyTasks
	Access:    	public
	Parameter: 	CCountry& _rCountry
	Parameter: 	CPopularity* _pPopularity
	Parameter: 	const byte _ClubDivStrength
	Returns:   	void
	Qualifier:
	Purpose:	End Of Week for Available youth lists
				If youth has been available for a certain length of time then
				replace him with a new one
------------------------------------------------------------------------------*/
void CYouthList::DoWeeklyTasks(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength)
{
    CYouth* pYouth;
    int MaxCreate;
	int LoopCount;
    for (LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        pYouth = GetAt(LoopCount);
		EXT_ASSERT_POINTER(pYouth, CYouth);
        if (pYouth->GetWeeksHere() > MAXAVAILWEEKS || RandomNumber.IntLessThan(4) == 3 || pYouth->GetAge() > 18)
		{
			pYouth->DoRegenerate(_Country, _Popularity, _ClubDivStrength);
		}
        else
		{
            pYouth->DoWeeklyTasks();
        }
    }

    // Create some more youths if not many available
    if (GetSize() < MINAVAILYOUTHS)
	{
    	MaxCreate = RandomNumber.IntLessThan(MAXAVAILYOUTHS - GetSize());
        for (LoopCount = 0; LoopCount < MaxCreate; LoopCount++)
		{
			Add(FD_ENGINE_NEW CYouth(_Country, _Popularity, _ClubDivStrength));
		}
  	}
}

void CYouthList::FillWithRandomYouths(CCountry &_Country, CPopularity *_Popularity, const byte _ClubDivStrength)
{
	// Create some more youths if not many available
	while (GetSize() < MAXAVAILYOUTHS)
	{
		Add(FD_ENGINE_NEW CYouth(_Country, _Popularity, _ClubDivStrength));
	}
}



/*------------------------------------------------------------------------------
	Method:   	CYouthList::Write
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::Write(CDataFile* _pDataFile)
{
	ushort nCount = GetSize();
    _pDataFile->Write(&nCount, sizeof(nCount));
    for (uint ListPos = 0; ListPos < nCount; ListPos ++)
	{
		GetAt(ListPos)->Write(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::Read
	Access:    	public
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::Read(CDataFile* _pDataFile)
{
	ushort  nCount;
    _pDataFile->Read(&nCount, sizeof(nCount));
    EXT_ASSERT(nCount <= YOUTHSINSQUAD);
    RemoveList();
    for (int ListPos = 0; ListPos < nCount; ListPos ++)
	{
        SetAtGrow(ListPos, FD_ENGINE_NEW CYouth());
		GetAt(ListPos)->Read(_pDataFile);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::DoRequestNewContracts
	Access:    	public
	Parameter: 	CClub& _Club
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::DoRequestNewContracts(CClub& _Club)
{
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        GetAt(LoopCount)->DoCheckIfRequestNewContract(_Club);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::Create
	Access:    	public
	Parameter: 	CCountry& _rCountry
	Parameter: 	CPopularity* _pPopularity
	Parameter: 	const byte _ClubDivStrength
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::DoCreate(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength)
{
    // Setup a youth squad
    for (int num = 0; num <= YOUTHSINSQUAD; num++)
	{
        if (RandomNumber.IntLessThan(10) < 7)
		{
			DoEmployYouth(_Country, _Popularity, _ClubDivStrength);
		}
	}
}



/*------------------------------------------------------------------------------
	Method:   	CYouthList::EmployYouth
	Access:    	public
	Parameter: 	CCountry& _rCountry
	Parameter: 	CPopularity* _pPopularity
	Parameter: 	const byte _ClubDivStrength
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::DoEmployYouth(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength)
{
    if (GetSize() < YOUTHSINSQUAD)
	{
        int Who = Add(FD_ENGINE_NEW CYouth(_Country, _Popularity, _ClubDivStrength));
	}
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::SackYouth
	Access:    	public
	Parameter: 	CYouth* _pWho
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::SackYouth(CYouth* _Youth)
{
    EXT_ASSERT_POINTER(_Youth, CYouth);
    for (int num = 0; num < GetSize(); num ++)
	{
		if (GetAt(num) == _Youth)
		{
			RemoveAt(num);
			break;
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::Find
	Access:    	public
	Parameter: 	CYouth* _pYouth
	Returns:   	ushort - the index(position in list) of the pointer
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
ushort CYouthList::Find(CYouth* _Youth)
{
    EXT_ASSERT_POINTER(_Youth, CYouth);
	ushort iRet = NOPLAYER;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        if (GetAt(LoopCount) == _Youth)
		{
            iRet = LoopCount;
            break;
        }
    }
    EXT_ASSERT(iRet != NOPLAYER);
    return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CYouthList::Remove
	Access:    	public
	Parameter: 	const ushort _YouthNum
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CYouthList::Remove(const ushort _YouthNum)
{
    delete GetAt(_YouthNum);
	RemoveAt(_YouthNum);
}
