
 /*

    Program:- CCupGroupRound.cpp

    The cup round class routines for rounds containing groups (mini leagues)


	Football Director

	(c) RW Software 1994 - 2005

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


 // ----------------------------------------------------------------------------------------------------------------------------
void CCupGroupRound::DoInitialiseFromFile(FILE* File)
{
	CCupRound::DoInitialiseFromFile(File);
    fread(&m_NumberInGroup, sizeof m_NumberInGroup, 1, File);
    fread(&m_NumberOfGroupMatches, sizeof m_NumberOfGroupMatches, 1, File);
}


 //---------------------------------------------------------------------------------------
void CCupGroupRound::Write(FILE* File)
{
	CCupRound::Write(File);
    fwrite(&m_NumberInGroup, sizeof m_NumberInGroup, 1, File);
    fwrite(&m_NumberOfGroupMatches, sizeof m_NumberOfGroupMatches, 1, File);
}


 //---------------------------------------------------------------------------------------
void CCupGroupRound::Read(FILE* File)
{
	CCupRound::Read(File);
    fread(&m_NumberInGroup, sizeof m_NumberInGroup, 1, File);
    fread(&m_NumberOfGroupMatches, sizeof m_NumberOfGroupMatches, 1, File);
}


 //---------------------------------------------------------------------------------------
uint8 CCupGroupRound::NumberMatchesinGroup(void)
{

	return GetNumberInGroup() >> 1;
}


 //---------------------------------------------------------------------------------------
int CCupGroupRound::NumberOfGroups(void)
{
	ASSERT (GetRoundType() == GROUP415372648 || GetRoundType() == GROUPMATCH || GetRoundType() == GROUPMATCH);
	return GetNumInRound() / GetNumberInGroup();
}


 //---------------------------------------------------------------------------------------
uint8  CCupGroupRound::GetNumberInGroup(void)
{
	return m_NumberInGroup;
}

void CCupGroupRound::CreateMatchList(CCup& pCup, CNumericArray<uint16>& aDrawClubs)
{
	if (GetRoundType() == GROUPMATCH && GetNumberInGroup() == 5)
	{
		// 5 in a group, 5th club doesn't play in 1st round
		for (uint32  n = 0; n < GetNumInRound(); n += 5)
		{
			ASSERT (aDrawClubs[n] != aDrawClubs[n + 1]);
			Match.InitialiseClubs(aDrawClubs[n], aDrawClubs[n + 1]);
#ifdef	ACTIVEREFEREES
			Match.SetReferee(RefereeNumber);
#endif
			getMatchList().Add(new CFixture(Match));

			Match.InitialiseClubs(aDrawClubs[n + 2], aDrawClubs[n + 3]);
#ifdef	ACTIVEREFEREES
			Match.SetReferee(RefereeNumber);
#endif
			getMatchList().Add(new CFixture(Match));
		}
	}
}


 // -------------------------------------------------------------------------------------------------------------------------
void CCupGroupRound::GetGroupClubNumbers(int GroupNumber, CNumericArray<uint16>& aClubNumbers, CCupRound* pSecondCupRound)
{
    int FirstClubNumber = GroupNumber * GetNumberInGroup();
	int MatchNumber;
	aClubNumbers.RemoveAll();

	if (GetNumberInGroup() == 5)
	{
		FirstClubNumber = GroupNumber * 2;
		aClubNumbers.Add(m_MatchList.GetAt(GroupNumber * 2)->getClubData(HOME).GetClubID());
		aClubNumbers.Add(m_MatchList.GetAt(GroupNumber * 2)->getClubData(AWAY).GetClubID());
		aClubNumbers.Add(m_MatchList.GetAt(GroupNumber * 2 + 1)->getClubData(HOME).GetClubID());
		aClubNumbers.Add(m_MatchList.GetAt(GroupNumber * 2 + 1)->getClubData(AWAY).GetClubID());
		// 5th club had a bye in first round
		aClubNumbers.Add(pSecondCupRound->getMatchList().GetAt(GroupNumber * 2)->getClubData(AWAY).GetClubID());
	}
	else
	{
		for (uint32 x = 0; x < GetNumberInGroup(); x += 2)
		{
			MatchNumber = (FirstClubNumber >> 1) + (x >> 1);
			if (MatchNumber < m_MatchList.GetSize())
			{
                aClubNumbers.Add(m_MatchList.GetAt(MatchNumber)->getClubData(HOME).GetClubID());
                aClubNumbers.Add(m_MatchList.GetAt(MatchNumber)->getClubData(AWAY).GetClubID());
			}
		}
    }
}
