
 /*

	CustomisableSoccerTactic.cpp: implementation of the CCustomisableSoccerTactic class.


	Football Director

	(c) RW Software 1994 - 2005

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


 //---------------------------------------------------------------------------------------
CCustomisableSoccerTactic::CCustomisableSoccerTactic()
	: m_IsCustomizable(false)
	, m_BizarreWarningIssued(false)
{
	for (uint32 n = 0; n < NUMBERPLAYERS; n++)
	{
		m_Arrows[n] = 0;
	}
}


 //---------------------------------------------------------------------------------------
CCustomisableSoccerTactic::~CCustomisableSoccerTactic()
{

}


 //---------------------------------------------------------------------------------------
void CCustomisableSoccerTactic::OnInitialise(const stTactic* _stTactic, const bool _IsCustomizable)
{
	for (uint32 n = 0; n < NUMBERPLAYERS; n++)
	{
		m_Arrows[n] = 0;
	}
	m_IsCustomizable = _IsCustomizable;
	m_BizarreWarningIssued = false;
	CSoccerTactic::OnInitialise( _stTactic);
}


 //---------------------------------------------------------------------------------------
CCustomisableSoccerTactic& CCustomisableSoccerTactic::operator = (const CCustomisableSoccerTactic& p)
{
	Copy(&p);
	return *this;
}

 //---------------------------------------------------------------------------------------
void CCustomisableSoccerTactic::Copy(const CCustomisableSoccerTactic* p)
{
    m_Name = p->m_Name;
	for (uint32 n = 0; n < NUMBERPLAYERS; n++)
	{
#ifdef SOCCERTACTICSARROWS
		m_Arrows[n] = p->m_Arrows[n];
#endif
	}
	m_IsCustomizable = p->m_IsCustomizable;
	m_BizarreWarningIssued = p->m_BizarreWarningIssued;
	CSoccerTactic::Copy(p);
}
//---------------------------------------------------------------------------------------
bool CCustomisableSoccerTactic::PositionIsValid(CExtPoint* pNewPosition)
{
	if (pNewPosition->GetY () < 0 || pNewPosition->GetY () > MaxY)
	{
		return false;
	}
	if (pNewPosition->GetX () < 0 || pNewPosition->GetX () > MaxX)
	{
		return false;
	}
	return true;
}


//---------------------------------------------------------------------------------------
bool CCustomisableSoccerTactic::CanMoveToPosition(CExtPoint* pNewPosition, int iPositionToIgnore)
{
	uint16 OffsetX[] =
	{
		-1, 0, 1
	};
	uint16 OffsetY[] =
	{
		-1, 0, 1
	};
	if (PositionIsValid(pNewPosition) == false)
	{
		return false;
	}

	bool bRet = true;
	int PlayPos;
	CExtPoint OriginalY (*pNewPosition);
	for (uint32 n = 0; n < 3; n++)
	{
		pNewPosition->SetX (OriginalY.GetX () + OffsetX[n]);
		if (pNewPosition->GetX () > -1 && pNewPosition->GetX () <= MaxX)
		{
			for (uint32 z = 0; z < 3; z++)
			{
				pNewPosition->SetY (OriginalY.GetY () + OffsetY[z]);
				if (PositionIsValid(pNewPosition) == true)
				{
					if (OffsetY[z] != 0 && OffsetX[n] != 0)
					{
						// Dont check own square
					}
					else
					{
						PlayPos = PlayerAtPosition(*pNewPosition);
						if (PlayPos != wNOTFOUND && iPositionToIgnore != PlayPos)
						{
							bRet = false;
							break;
						}
					}
				}
			}
		}
	}
	return bRet;
}


 //---------------------------------------------------------------------------------------
bool CCustomisableSoccerTactic::IsBizarre(void)
{
	bool bRet = false;
	uint8 WhichSideCount[] =
	{
		0, 0, 0
	};
	uint8 WhichSide[] =
	{
		0, 0, 0,
		1, 1, 1,
		2, 2, 2
	};

	uint8 DefenderCount = 0;
	uint8 MidfielderCount = 0;
	uint8 AttackerCount = 0;

	if (IsCustomizable() == true)
	{
		for (uint32 n = 0; n < NUMBERPLAYERS; n++)
		{
			ASSERT (m_Formations[n].GetY () < 9);
			WhichSideCount[WhichSide[m_Formations[n].GetY ()]]++;
			DefenderCount += IsDefender(n);
			ASSERT (m_Formations[n].GetX () < 13);
			MidfielderCount += IsMidfielder(n);
			AttackerCount += IsAttacker(n);
		}
		if (WhichSideCount[0] < 2 || WhichSideCount[0] > 5)
		{
			bRet = true;
		}
		if (WhichSideCount[1] < 2 || WhichSideCount[1] > 10)
		{
			bRet = true;
		}
		if (WhichSideCount[2] < 2 || WhichSideCount[2] > 5)
		{
			bRet = true;
		}
		if (DefenderCount < 2 || DefenderCount > 10)
		{
			bRet = true;
		}
		if (MidfielderCount < 2 || MidfielderCount > 10)
		{
			bRet = true;
		}
		if (AttackerCount < 1 || AttackerCount > 7)
		{
			bRet = true;
		}
	}
	return bRet;
}


 // Returns the table index to use depending on how familiar the team is with this formation
 //---------------------------------------------------------------------------------------
uint8 CCustomisableSoccerTactic::GetEffectiveness(void)
{
	if (IsBizarre() == true)
	{
		return 7;
	}
	return CSoccerTactic::GetEffectiveness();
}


 //---------------------------------------------------------------------------------------
void CCustomisableSoccerTactic::ReCreate()
{
	if (m_IsCustomizable == false)
	{
		m_Familiarity = m_BaseFamiliarity + RandomIntLessThan(9);
	}
	m_BizarreWarningIssued = false;
}


 //---------------------------------------------------------------------------------------
void CCustomisableSoccerTactic::Read(FILE* File)
{
	fread(&m_Arrows[0], sizeof(m_Arrows[0]), NUMBERPLAYERS, File);
    fread(&m_IsCustomizable, sizeof m_IsCustomizable, 1, File);
	ASSERT(m_IsCustomizable == false || m_IsCustomizable == true);
    fread(&m_BizarreWarningIssued, sizeof m_BizarreWarningIssued, 1, File);
	CSoccerTactic::Read(File);
}


 //---------------------------------------------------------------------------------------
void CCustomisableSoccerTactic::Write(FILE* File)
{
	fwrite(&m_Arrows[0], sizeof(m_Arrows[0]), NUMBERPLAYERS, File);
    fwrite(&m_IsCustomizable, sizeof m_IsCustomizable, 1, File);
    fwrite(&m_BizarreWarningIssued, sizeof m_BizarreWarningIssued, 1, File);
	CSoccerTactic::Write(File);
}
