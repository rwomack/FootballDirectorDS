
 /*

	Tactic.cpp: implementation of the CTactic class.


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#ifndef TESTBED
#include "UI.h"
#endif

	// These values affect the likelihood of the player taking the shot when multiplied by his shooting skill
	const byte				SHOTEFFECTGOALKEEPER = 0;
	const byte				SHOTEFFECTDEFENDER = 1;
	const byte				SHOTEFFECTMIDFIELDER = 4;
	const byte				SHOTEFFECTATTACKER = 10;

	const int				POINTSNEEDEDFORTACTICSADJUSTMENT = 10;
	const int				FAMILIARITYPOINTSFORWIN = 6;
	const int				FAMILIARITYPOINTSFORDRAW = 4;
	const int				FAMILIARITYPOINTSFORDEFEAT = 2;
	const int				MaxX = 12;
	const int				MaxY = 8;



/*------------------------------------------------------------------------------
	Method:   	CTactic::CTactic
	Access:    	public 
	Returns:  
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CTactic::CTactic()
	: m_BaseFamiliarity(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::OnInitialise
	Access:    	public 
	Parameter: 	const int _Title
	Parameter: 	CBinaryReader & _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::DoInitialiseFromResource(const int _Title, CBinaryReader& _Resource)
{
	m_Name.LoadString(_Title);		// Load from string table, so can be edited by user
	m_TeamTactic.SetDefaults();
	for (int LoopCount = 0; LoopCount < NUMBERPLAYERS; LoopCount++)
	{
		m_Formations[LoopCount].SetX(_Resource.ReadByte());
		m_Formations[LoopCount].SetY(_Resource.ReadByte());
	}
	m_BaseFamiliarity = _Resource.ReadByte();
	m_Familiarity = 0;
	m_FamiliarityExperience = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::operator=
	Access:    	public 
	Parameter: 	const CTactic & _Tactic
	Returns:   	CTactic&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTactic& CTactic::operator = (const CTactic& _Tactic)
{
	Copy(_Tactic);
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::Copy
	Access:    	public 
	Parameter: 	const CTactic * _Tactic
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::Copy(const CTactic& _Tactic)
{
    m_Name = _Tactic.m_Name;
	for (int LoopCount = 0; LoopCount < NUMBERPLAYERS; LoopCount++)
	{
		m_Formations[LoopCount] = _Tactic.m_Formations[LoopCount];
	}
	m_TeamTactic = _Tactic.m_TeamTactic;
	m_Familiarity = _Tactic.m_Familiarity;
	m_FamiliarityExperience = _Tactic.m_FamiliarityExperience;
	m_BaseFamiliarity = _Tactic.m_BaseFamiliarity;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::ReCreate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::ReCreate()
{
	m_Familiarity = m_BaseFamiliarity + RandomNumber.IntLessThan(9);
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::GetTeamTactic
	Access:    	public 
	Returns:   	CTeamTactic&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTeamTactic& CTactic::GetTeamTactic()
{
	return m_TeamTactic;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::GetFamiliarity
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTactic::GetFamiliarity()
{
	return m_Familiarity;
}


 //---------------------------------------------------------------------------------------
CString CTactic::FamiliarityStr()
{
	CString str;
	str.Format(TEXT("%d"), m_Familiarity);
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::TitleAndFamiliarityStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CTactic::TitleAndFamiliarityStr()
{
	CString str;
	str.Format(TEXT("%s(%d)"), m_Name, m_Familiarity);
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::CountMatchingPositions
	Access:    	public 
	Parameter: 	CTactic & _Tactic
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTactic::CountMatchingPositions(CTactic& _Tactic)
{
	byte bRet = 0;
	for (int OuterLoopCount = 0; OuterLoopCount < NUMBERPLAYERS; OuterLoopCount++)
	{
		for (int InnerLoopCount = 0; InnerLoopCount < NUMBERPLAYERS; InnerLoopCount++)
		{
			bRet += (GetPlayerPositionY(OuterLoopCount) == _Tactic.GetPlayerPositionY(InnerLoopCount)) && (GetPlayerPositionX(OuterLoopCount) == _Tactic.GetPlayerPositionX (InnerLoopCount));
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::GetPlayerPositionX
	Access:    	public 
	Parameter: 	ushort _PositionNum
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CTactic::GetPlayerPositionX(ushort _PositionNum)
{
	EXT_ASSERT(_PositionNum < NUMBERPLAYERS);
	return m_Formations[_PositionNum].GetX();
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::GetPlayerPositionY
	Access:    	public 
	Parameter: 	ushort _PositionNum
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CTactic::GetPlayerPositionY(ushort _PositionNum)
{
	EXT_ASSERT(_PositionNum < NUMBERPLAYERS);
	return m_Formations[_PositionNum].GetY();
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::SetFamiliarity
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::SetFamiliarity(const byte _Value)
{
	m_Familiarity = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::SetFamiliarityExperience
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::SetFamiliarityExperience(const byte _Value)
{
	m_FamiliarityExperience = _Value;
}


//---------------------------------------------------------------------------------------
CString CTactic::TitleStr()
{
	return m_Name;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::PlayerPosition
	Access:    	public 
	Parameter: 	ushort _PlayerNum
	Returns:   	ePlayerPosition
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ePlayerPosition CTactic::PlayerPosition(ushort _PlayerNum)
{
	EXT_ASSERT(_PlayerNum < NUMBERPLAYERS);
	if (IsDefender(_PlayerNum) == true)
	{
		return DEFENDER;
	}
	if (IsMidfielder(_PlayerNum) == true)
	{
		return MIDFIELDER;
	}
	return ATTACKER;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::PlayerAtPosition
	Access:    	public 
	Parameter: 	CExtPoint & _Coords
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CTactic::PlayerAtPosition(CExtPoint& _Coords)
{
	ushort iRet = wNOTFOUND;
	for (int LoopCount = 0; LoopCount < NUMBERPLAYERS; LoopCount++)
	{
		if (GetPlayerPositionY(LoopCount) == _Coords.GetY() && GetPlayerPositionX(LoopCount) == _Coords.GetX ())
		{
			iRet = LoopCount;
			break;
		}
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::IsDefender
	Access:    	public 
	Parameter: 	int _PlayerNum
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTactic::IsDefender(int _PlayerNum)
{
	EXT_ASSERT(_PlayerNum < NUMBERPLAYERS);
	bool bRet[] =
	{
		true, true, true, true, true,
		false, false, false, false,
		false, false, false, false,
	};
	return bRet[GetPlayerPositionX(_PlayerNum)];
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::IsMidfielder
	Access:    	public 
	Parameter: 	int _PlayerNum
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTactic::IsMidfielder(int _PlayerNum)
{
	EXT_ASSERT(_PlayerNum < NUMBERPLAYERS);
	bool bRet[] =
	{
		false, false, false, false,
		true, true, true, true, true,
		false, false, false, false,
	};
	return bRet[GetPlayerPositionX(_PlayerNum)];
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::IsAttacker
	Access:    	public 
	Parameter: 	int _PlayerNum
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTactic::IsAttacker(int _PlayerNum)
{
	EXT_ASSERT(_PlayerNum < NUMBERPLAYERS);
	bool bRet[] =
	{
		false, false, false, false,
		false, false, false, false,
		true, true, true, true, true,
	};
	return bRet[GetPlayerPositionX(_PlayerNum)];
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::DoPlayedMatch
	Access:    	public 
	Parameter: 	const byte _OurGoals
	Parameter: 	const byte _TheirGoals
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::DoPlayedMatch(const byte _OurGoals, const byte _TheirGoals)
{
	if (_OurGoals > _TheirGoals)
	{
		m_FamiliarityExperience += FAMILIARITYPOINTSFORWIN;
	}
	if (_OurGoals == _TheirGoals)
	{
		m_FamiliarityExperience += FAMILIARITYPOINTSFORDRAW;
	}
	if (_OurGoals < _TheirGoals)
	{
		m_FamiliarityExperience += FAMILIARITYPOINTSFORDEFEAT;
	}
	AdjustFamiliarity();
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::DoSquadChanged
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::DoSquadChanged()
{
	if (RandomNumber.IntLessThan(99) > m_Familiarity && m_Familiarity > 50)
	{
		m_Familiarity--;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::DoTrain
	Access:    	public 
	Parameter: 	const byte _Hours
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::DoTrain(const byte _Hours)
{
	const byte ExperiencePointsBelow90[] =
	{
		0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6
	};
	const byte ExperiencePointsOver90[] =
	{
		0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3
	};

	if (m_Familiarity < 90)
	{
		m_FamiliarityExperience += ExperiencePointsBelow90[min(11, _Hours)];
	}
	else
	{
		m_FamiliarityExperience += ExperiencePointsOver90[min(11, _Hours)];
	}
	AdjustFamiliarity();
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::AdjustFamiliarity
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::AdjustFamiliarity()
{
	if (m_FamiliarityExperience >= POINTSNEEDEDFORTACTICSADJUSTMENT)
	{
		m_Familiarity++;
		m_FamiliarityExperience -= POINTSNEEDEDFORTACTICSADJUSTMENT;
	}
	m_Familiarity = min(m_Familiarity, 99);
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::GetEffectiveness
	Access:    	public 
	Returns:   	int - the table index to use depending on how familiar the team
					  is with this formation
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CTactic::GetEffectiveness()
{
	if (m_Familiarity > 94)
	{
		return 0;
	}
	if (m_Familiarity > 89)
	{
		return 1;
	}
	if (m_Familiarity > 84)
	{
		return 2;
	}
	if (m_Familiarity > 79)
	{
		return 3;
	}
	if (m_Familiarity > 69)
	{
		return 4;
	}
	if (m_Familiarity > 59)
	{
		return 5;
	}
	return 6;
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::Read(CDataFile* _pDataFile)
{
#ifdef CUSTOMFORMATIONS
	m_Name.Read(_pDataFile);
	_pDataFile->Read(&m_Formations[0], sizeof  CExtPoint, NUMBERPLAYERS, _pFile);
#endif
	_pDataFile->Read(&m_TeamTactic, sizeof  CTeamTactic);
    _pDataFile->Read(&m_Familiarity, sizeof m_Familiarity);
    _pDataFile->Read(&m_FamiliarityExperience, sizeof m_FamiliarityExperience);
}


/*------------------------------------------------------------------------------
	Method:   	CTactic::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTactic::Write(CDataFile* _pDataFile)
{
#ifdef CUSTOMFORMATIONS
	m_Name.Write(_pDataFile);
	_pDataFile->Write(&m_Formations[0], sizeof  CExtPoint, NUMBERPLAYERS, _pFile);
#endif
	_pDataFile->Write(&m_TeamTactic, sizeof  CTeamTactic);
    _pDataFile->Write(&m_Familiarity, sizeof m_Familiarity);
    _pDataFile->Write(&m_FamiliarityExperience, sizeof m_FamiliarityExperience);
}
///////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CTacticList::CTacticList
	Access:    	public 
	Returns: 
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CTacticList::CTacticList()
	: m_TacticsInUse(0)
{
	CBinaryReader theResource(0, IDR_TACTIC, TEXT("FDDATA"));
	ASSERT(theResource.IsLoaded() == true);
	byte HowMany = theResource.ReadByte();
	m_TacticList[FORMATION_442].DoInitialiseFromResource(IDS_442, theResource);
	m_TacticList[FORMATION_433].DoInitialiseFromResource(IDS_433, theResource);
	m_TacticList[FORMATION_424].DoInitialiseFromResource(IDS_424, theResource);
	m_TacticList[FORMATION_532].DoInitialiseFromResource(IDS_532, theResource);
	m_TacticList[FORMATION_523].DoInitialiseFromResource(IDS_523, theResource);
	m_TacticList[FORMATION_541].DoInitialiseFromResource(IDS_541, theResource);
	m_TacticList[FORMATION_451].DoInitialiseFromResource(IDS_451, theResource);
	m_TacticList[FORMATION_334].DoInitialiseFromResource(IDS_334, theResource);
	m_TacticList[FORMATION_352].DoInitialiseFromResource(IDS_352, theResource);
	m_TacticList[FORMATION_343].DoInitialiseFromResource(IDS_343, theResource);

#ifdef CUSTOMFORMATIONS
	for (uint n = 0; n < NUMCUSTOMFORMATIONS; n++)
	{
		pTactic++->OnInitialise(pstTactic++, true);
	}
	for (uint n = 0; n < NUMCUSTOMFORMATIONS; n++)
	{
		CalculateFamiliarity(m_TacticList[n + 10]);
	}
#endif
	ReCreate();
}


#ifdef CUSTOMFORMATIONS
 //---------------------------------------------------------------------------------------
void CTacticList::SetPlayerPosition(ushort x, CExtPoint* y)
{
	m_TacticList[m_TacticsInUse].SetPlayerPosition(x, y);
	CalculateFamiliarity(m_TacticList[m_TacticsInUse]);
}


 //---------------------------------------------------------------------------------------
void CTacticList::SetPlayerPositionX (ushort x, ushort y)
{
	m_TacticList[m_TacticsInUse].SetPlayerPositionX (x, y);
	CalculateFamiliarity(m_TacticList[m_TacticsInUse]);
}


 //---------------------------------------------------------------------------------------
void CTacticList::SetPlayerPositionY (ushort x, ushort y)
{
	m_TacticList[m_TacticsInUse].SetPlayerPositionY (x, y);
	CalculateFamiliarity(m_TacticList[m_TacticsInUse]);
}


 //---------------------------------------------------------------------------------------
void CTacticList::CalculateFamiliarity(CTactic& _Tactics)
{
	ushort BestId = wNOTFOUND;
	byte BestFamiliarity = 0;
	byte TheFamiliarity = 50;
	for (int WhichTactic = 0; WhichTactic < GetSize(); WhichTactic++)
	{
		if (&m_TacticList[WhichTactic] != &_Tactics)
		{
			// don't compare against it's self
			if (m_TacticList[WhichTactic].CountMatchingPositions(_Tactics) > BestFamiliarity)
			{
				BestId = WhichTactic;
				BestFamiliarity = m_TacticList[WhichTactic].CountMatchingPositions(_Tactics);
			}
		}
	}

	if (BestId != NOTFOUND)
	{
		if (BestFamiliarity == CTactic::NUMBERPLAYERS)
		{
			TheFamiliarity = 0;
			BestFamiliarity = m_TacticList[BestId].GetFamiliarity();
		}
		else
		{
			BestFamiliarity = (m_TacticList[BestId].GetFamiliarity() / (CTactic::NUMBERPLAYERS * 2)) * m_TacticList[BestId].CountMatchingPositions(_Tactics);
		}
	}
	// Set its familiarity to the closest found
	_Tactics.SetFamiliarity(BestFamiliarity + TheFamiliarity);
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CTacticList::TotalStandardTactics
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTacticList::TotalStandardTactics()
{
#ifdef CUSTOMFORMATIONS
	byte bRet = 0;
	CTactic* pTactic = m_pList;
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        bRet += (pTactic++->IsCustomizable() == false);

	}
	return bRet;
#else
	return 10 + NUMCUSTOMFORMATIONS;
#endif
}


#ifdef CUSTOMFORMATIONS
/*------------------------------------------------------------------------------
	Method:   	CTacticList::SetTitle
	Access:    	public 
	Parameter: 	const ushort Which
	Parameter: 	CString String
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::SetTitle(const ushort Which, CString String)
{
    m_TacticList[Which].SetTitle(String);
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetSize
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CTacticList::GetSize()
{
	return 10 + NUMCUSTOMFORMATIONS;
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::DoSquadChanged
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::DoSquadChanged()
{
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        m_TacticList[LoopCount].DoSquadChanged();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::Write(CDataFile* _pDataFile)
{
    EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    _pDataFile->Write(&m_TacticsInUse, sizeof m_TacticsInUse);
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        m_TacticList[LoopCount].Write(_pDataFile);
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CTacticList::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::Read(CDataFile* _pDataFile)
{
	EXT_ASSERT_POINTER(_pDataFile, CDataFile); 
    _pDataFile->Read(&m_TacticsInUse, sizeof m_TacticsInUse);
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        m_TacticList[LoopCount].Read(_pDataFile);
	}
 }

//		 0     1    2    3    4    5    6    7    8    9   10   11   12
	const byte m_GoalkeeperEffectiveness[8][13] =
	{
        100,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
		 90,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
		 90,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
		 70,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
		 60,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
		 50,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
		 40,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
         30,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,  00,
    };
	const byte m_DefenderEffectiveness[8][13]   =
	{
		100, 100, 100,  80,  70,  20,  00,  00,  00,  00,  00,  00,  00,
		 99,  99,  99,  79,  69,  19,  00,  00,  00,  00,  00,  00,  00,
         98,  98,  98,  78,  68,  19,  00,  00,  00,  00,  00,  00,  00,
         97,  97,  97,  77,  67,  18,  00,  00,  00,  00,  00,  00,  00,
         95,  95,  95,  75,  65,  18,  00,  00,  00,  00,  00,  00,  00,
         93,  93,  93,  74,  65,  17,  00,  00,  00,  00,  00,  00,  00,
         91,  91,  91,  72,  62,  16,  00,  00,  00,  00,  00,  00,  00,
		 70,  70,  70,  55,  45,   5,  00,  00,  00,  00,  00,  00,  00,
	};
	const byte m_MidfielderEffectiveness[8][13] =
	{
         00,  00,  00,  20,  30,  80, 100,  80,  60,  40,  10,  00,  00,
         00,  00,  00,  19,  29,  79,  99,  79,  59,  40,  10,  00,  00,
         00,  00,  00,  19,  28,  78,  98,  78,  58,  39,   9,  00,  00,
         00,  00,  00,  18,  28,  77,  97,  77,  57,  39,   9,  00,  00,
         00,  00,  00,  18,  27,  76,  95,  75,  56,  38,   8,  00,  00,
         00,  00,  00,  17,  27,  76,  93,  74,  56,  38,   8,  00,  00,
         00,  00,  00,  16,  27,  75,  91,  72,  55,  37,   7,  00,  00,
         00,  00,  00,   5,   8,  55,  70,  55,  42,  28,   2,  00,  00,
	};
	const byte m_AttackerEffectiveness[8][13]   =
	{
         00,  00,  00,  00,  00,  00,  00,  20,  40,  60,  90, 100, 100,
         00,  00,  00,  00,  00,  00,  00,  19,  40,  59,  89,  99,  99,
         00,  00,  00,  00,  00,  00,  00,  19,  39,  58,  89,  98,  98,
         00,  00,  00,  00,  00,  00,  00,  18,  39,  57,  88,  97,  97,
         00,  00,  00,  00,  00,  00,  00,  18,  38,  56,  87,  95,  95,
         00,  00,  00,  00,  00,  00,  00,  17,  38,  56,  85,  93,  93,
         00,  00,  00,  00,  00,  00,  00,  16,  37,  55,  84,  91,  91,
         00,  00,  00,  00,  00,  00,  00,   5,  28,  42,  68,  70,  70,
	};

//		 0     1    2    3    4    5    6    7    8
/*	const byte m_LeftSidedEffectiveness[8][9]  =
	{
        100,  90,  70,  40,  00,  00,  00,  00,  00,
         99,  89,  69,  40,  00,  00,  00,  00,  00,
         98,  89,  68,  39,  00,  00,  00,  00,  00,
         97,  88,  67,  39,  00,  00,  00,  00,  00,
         95,  87,  65,  38,  00,  00,  00,  00,  00,
         93,  85,  65,  38,  00,  00,  00,  00,  00,
         91,  84,  62,  37,  00,  00,  00,  00,  00,
         70,  68,  45,  28,  00,  00,  00,  00,  00,
	};
	const byte m_CentralEffectiveness[8][9] =
	{
         00,  10,  30,  60, 100,  60,  30,  10,  00,
         00,  10,  29,  59,  99,  59,  30,  10,  00,
         00,  10,  28,  58,  98,  58,  30,  10,  00,
         00,  10,  28,  57,  97,  57,  30,  10,  00,
         00,  10,  27,  56,  95,  56,  30,  10,  00,
         00,  10,  27,  56,  93,  56,  30,  10,  00,
         00,  10,  27,  55,  91,  55,  30,  10,  00,
         00,  10,   8,  42,  70,  42,  30,  10,  00,
	};
	const byte m_RightSidedEffectiveness[8][9] =
	{
         00,  00,  00,  00,  00,  40,  70,  90, 100,
         00,  00,  00,  00,  00,  40,  69,  89,  99,
         00,  00,  00,  00,  00,  39,  68,  89,  98,
         00,  00,  00,  00,  00,  39,  67,  88,  97,
         00,  00,  00,  00,  00,  38,  65,  87,  95,
         00,  00,  00,  00,  00,  38,  65,  85,  93,
         00,  00,  00,  00,  00,  37,  62,  84,  91,
         00,  00,  00,  00,  00,  28,  45,  68,  70,
	};*/

	// These values affect the likelihood of the player taking the shot when multiplied by his shooting skill
	const byte m_ShootingEffectiveness[]   =
	{
		SHOTEFFECTGOALKEEPER, SHOTEFFECTDEFENDER, SHOTEFFECTDEFENDER,
		SHOTEFFECTDEFENDER, SHOTEFFECTDEFENDER, SHOTEFFECTDEFENDER,
		SHOTEFFECTDEFENDER, SHOTEFFECTMIDFIELDER, SHOTEFFECTMIDFIELDER,
		SHOTEFFECTMIDFIELDER, SHOTEFFECTATTACKER, SHOTEFFECTATTACKER, SHOTEFFECTATTACKER
	};
	const byte m_HeadingEffectiveness[]   =
	{
		SHOTEFFECTGOALKEEPER, SHOTEFFECTDEFENDER, SHOTEFFECTDEFENDER,
		SHOTEFFECTDEFENDER, SHOTEFFECTDEFENDER, SHOTEFFECTDEFENDER,
		SHOTEFFECTDEFENDER, SHOTEFFECTMIDFIELDER, SHOTEFFECTMIDFIELDER,
		SHOTEFFECTMIDFIELDER, SHOTEFFECTATTACKER, SHOTEFFECTATTACKER, SHOTEFFECTATTACKER
	};


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetDefenceEffectiveness
	Access:    	public 
	Parameter: 	const ushort _PlayerNumber
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTacticList::GetDefenceEffectiveness(const ushort _PlayerNumber)
{
	return  m_DefenderEffectiveness[m_TacticList[m_TacticsInUse].GetEffectiveness()][m_TacticList[m_TacticsInUse].GetPlayerPositionX(_PlayerNumber)];
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetMidfieldEffectiveness
	Access:    	public 
	Parameter: 	const ushort _PlayerNumber
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTacticList::GetMidfieldEffectiveness(const ushort _PlayerNumber)
{
	return  m_MidfielderEffectiveness[m_TacticList[m_TacticsInUse].GetEffectiveness()][m_TacticList[m_TacticsInUse].GetPlayerPositionX(_PlayerNumber)];
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetAttackEffectiveness
	Access:    	public 
	Parameter: 	const ushort _PlayerNumber
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTacticList::GetAttackEffectiveness(const ushort _PlayerNumber)
{
	return  m_AttackerEffectiveness[m_TacticList[m_TacticsInUse].GetEffectiveness()][m_TacticList[m_TacticsInUse].GetPlayerPositionX(_PlayerNumber)];
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetShootingEffectiveness
	Access:    	public 
	Parameter: 	const ushort _PlayerNumber
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTacticList::GetShootingEffectiveness(const ushort _PlayerNumber)
{
	return  m_ShootingEffectiveness[m_TacticList[m_TacticsInUse].GetPlayerPositionX(_PlayerNumber)];
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetHeadingEffectiveness
	Access:    	public 
	Parameter: 	const ushort _PlayerNumber
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTacticList::GetHeadingEffectiveness(const ushort _PlayerNumber)
{
	return  m_HeadingEffectiveness[m_TacticList[m_TacticsInUse].GetPlayerPositionX(_PlayerNumber)];
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetPlayerPositionX
	Access:    	public 
	Parameter: 	const ushort _PositionNUm
	Parameter: 	const ushort _Formation
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CTacticList::GetPlayerPositionX(const ushort _PositionNUm, const ushort _Formation /*= NOPLAYER*/)
{
	if (_Formation == NOPLAYER)
	{
		// Not specified which so use current tactic
        return m_TacticList[m_TacticsInUse].GetPlayerPositionX(_PositionNUm);
	}
	else
	{
        return m_TacticList[_Formation].GetPlayerPositionX (_PositionNUm);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetPlayerPositionY
	Access:    	public 
	Parameter: 	const ushort _Value
	Parameter: 	const ushort _Formation
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CTacticList::GetPlayerPositionY(const ushort _Value, const ushort _Formation /*= NOPLAYER*/)
{
	if (_Formation == NOPLAYER)
	{
		// Not specified which so use current tactic
        return m_TacticList[m_TacticsInUse].GetPlayerPositionY (_Value);
	}
	else
	{
        return m_TacticList[_Formation].GetPlayerPositionY (_Value);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetAt
	Access:    	public 
	Parameter: 	const ushort _TacticID
	Returns:   	CTactic&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTactic& CTacticList::GetAt(const ushort _TacticID)
{
	EXT_ASSERT(_TacticID < GetSize());
	return m_TacticList[_TacticID];
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::DoBuildTitleAndFamiliarityStrList
	Access:    	public 
	Parameter: 	CTextSelector& _TextSelector
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
#ifndef TESTBED
void CTacticList::DoBuildTitleAndFamiliarityStrList(CTextSelector& _TextSelector)
{
	for (int nCount = 0; nCount < GetSize(); nCount++)
	{
		_TextSelector.DoAddItem(m_TacticList[nCount].TitleAndFamiliarityStr());
	}
	_TextSelector.SetCurSel(m_TacticsInUse);
}
#endif

/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetCurrentTactics
	Access:    	public 
	Returns:   	CTactic&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTactic& CTacticList::GetCurrentTactics()
{
	return m_TacticList[m_TacticsInUse];
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetCurrentTeamTactic
	Access:    	public 
	Returns:   	CTeamTactic&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTeamTactic& CTacticList::GetCurrentTeamTactic()
{
	return m_TacticList[m_TacticsInUse].GetTeamTactic();
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetTacticsInUse
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CTacticList::GetTacticsInUse()
{
	EXT_ASSERT(m_TacticsInUse < GetSize());
	return m_TacticsInUse;
}


 /*------------------------------------------------------------------------------
 	Method:   	CTacticList::SetTacticsInUse
 	Access:    	public 
 	Parameter: 	const ushort aNewTactic
 	Returns:   	void
 	Qualifier: 	
 	Purpose:
 ------------------------------------------------------------------------------*/
void CTacticList::SetTacticsInUse(const ushort _TacticID)
{
	EXT_ASSERT(_TacticID < GetSize());
	m_TacticsInUse = _TacticID;
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::SetCurrentStyleOfPlay
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::SetCurrentStyleOfPlay(const byte _Value)
{
	m_TacticList[m_TacticsInUse].GetTeamTactic().SetStyleOfPlay(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::SetCurrentMarkingStyle
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::SetCurrentMarkingStyle(const byte _Value)
{
	m_TacticList[m_TacticsInUse].GetTeamTactic().SetMarkingStyle(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::SetCurrentAggression
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::SetCurrentAggression(const byte _Value)
{
	m_TacticList[m_TacticsInUse].GetTeamTactic().SetAggression(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::SetCurrentCounterAttack
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::SetCurrentCounterAttack(bool _Value)
{
	m_TacticList[m_TacticsInUse].GetTeamTactic().SetCounterAttack(_Value);
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::SetCurrentOffsideTrap
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::SetCurrentOffsideTrap(bool _Value)
{
	m_TacticList[m_TacticsInUse].GetTeamTactic().SetOffsideTrap(_Value);
}


#ifdef SOCCERTACTICSARROWS
/*------------------------------------------------------------------------------
	Method:   	CTacticList::GetPlayerArrows
	Access:    	public 
	Parameter: 	const ushort _WhichTacticsID
	Returns:   	const byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
const byte  CTacticList::GetPlayerArrows(const ushort _WhichTacticsID)
{
	return m_TacticList[m_TacticsInUse].GetPlayerArrows(_WhichTacticsID);
}


/*------------------------------------------------------------------------------
	Method:   	CTacticList::SetPlayerArrows
	Access:    	public 
	Parameter: 	const ushort _WhichTacticsID
	Parameter: 	const byte y
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void  CTacticList::SetPlayerArrows(const ushort _WhichTacticsID, const byte y)
{
	m_TacticList[m_TacticsInUse].SetPlayerArrows(_WhichTacticsID, y);
}
#endif


/*------------------------------------------------------------------------------
	Method:   	CTacticList::ReCreate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTacticList::ReCreate()
{
    for (int LoopCount = 0; LoopCount < GetSize(); LoopCount++)
	{
        m_TacticList[LoopCount].ReCreate();
	}
	m_TacticsInUse = RandomNumber.IntLessThan(TotalStandardTactics());
}
