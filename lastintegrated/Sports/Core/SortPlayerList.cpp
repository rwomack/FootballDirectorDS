
 /*

    Class:- Player

	Football Director

	(c) RW Software 1994 - 2005

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByPosnSkill(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
    int n;
    while(t_Swap == true)
	{
        t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = GetAt(n);
            pPlayer2 = GetAt(n + 1);
            if (pPlayer1->getSkills().getOverallSkill() < pPlayer2->getSkills().getOverallSkill())
			{
                Swap(n, n + 1);
                t_Swap = true;
            }
        }
    }
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByFitness(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->getSkills().GetFitness() < pPlayer2->getSkills().GetFitness())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByDisciplinary(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->getDiscipline().GetDisciplinaryPoints() < pPlayer2->getDiscipline().GetDisciplinaryPoints())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByInjuryLength(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->HowLongUnableToPlay() < pPlayer2->HowLongUnableToPlay())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortBySuspension(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->getDiscipline().getMatchesSuspended() < pPlayer2->getDiscipline().getMatchesSuspended())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortBySeasonAppearances(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->GetClubSeasonAppear() < pPlayer2->GetClubSeasonAppear())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortBySeasonGoals(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
	int Goals1, Goals2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
			Goals1 = pPlayer1->GetNumberGoalsThisSeason() * (pPlayer1->getSkills().GetPosition() != GOALKEEPER);
			Goals2 = pPlayer2->GetNumberGoalsThisSeason() * (pPlayer2->getSkills().GetPosition() != GOALKEEPER);
			if (Goals1 < Goals2)
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByCareerAppearances(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->GetCareerAppearances() < pPlayer2->GetCareerAppearances())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByCareerGoals(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
	int Goals1, Goals2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
			Goals1 = pPlayer1->GetCareerGoals() * (pPlayer1->getSkills().GetPosition() != GOALKEEPER);
			Goals2 = pPlayer2->GetCareerGoals() * (pPlayer2->getSkills().GetPosition() != GOALKEEPER);
			if (Goals1 < Goals2)
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByAge(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->getAge() < pPlayer2->getAge())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByWage(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->GetWage() < pPlayer2->GetWage())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}


 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::SortByContract(void)
{
    CPlayer* pPlayer1;
    CPlayer* pPlayer2;
    bool t_Swap = true;
	int n;
    while(t_Swap == true)
	{
		t_Swap = false;
        for (n = 0; n < GetSize() - 1; n++)
		{
            pPlayer1 = static_cast<CPlayer*> (GetAt(n));
            pPlayer2 = static_cast<CPlayer*> (GetAt(n + 1));
            if (pPlayer1->GetContractEndDate().SeasonsRemaining() < pPlayer2->GetContractEndDate().SeasonsRemaining())
			{
				Swap(n, n + 1);
				t_Swap = true;
			}
		}
	}
}
#endif

 //------------------------------------------------------------------------------------------------------
void CSortPlayerList::RemoveAllPosn(int Posn)
{
    CPlayer* pPlayer;
    for (uint32 ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        pPlayer = GetAt(ListPos);
        if (pPlayer != NULL)
		{
            if (pPlayer->getSkills().GetPosition() == Posn)
			{
                RemoveAt(ListPos);
            }
        }
    }
}


 //--------------------------------------------------------------------------
void CSortPlayerList::CheckForSwapClubs(const uint16 a_ClubAID, const uint16 a_ClubBID)
{
	ASSERT(a_ClubAID != NOCLUB && a_ClubBID != NOCLUB);
    CPlayer* pPlayer;
    for (uint32 ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        pPlayer = GetAt(ListPos);
        if (pPlayer != NULL)
		{
			pPlayer->CheckForSwapClubs(a_ClubAID, a_ClubBID);
        }
    }
}


 //--------------------------------------------------------------------------
/*void CSortPlayerList::DoRegenerateRetiredPlayers(CCountry* a_pCountry)
{
    CPlayer* pPlayer;
    for (uint32 ListPos = 0; ListPos < GetSize(); ListPos ++)
	{
        pPlayer = GetAt(ListPos);
        if (pPlayer != NULL && pPlayer->IsRetired() == true)
		{
			pPlayer->DoRegenerate(ListPos, a_pCountry);
        }
    }
}*/


 //------------------------------------------------------------------------------------------------------
CPlayer*  CSortPlayerList::GetAt(uint16 nIndex)
{
	return static_cast<CPlayer*> (CPointerArray::GetAt(nIndex));
}

