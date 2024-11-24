
/*
    File:- CupGroupRound.h

	C++ Classes

	Cup round class

	Football Director

	(c) RW Software 1994 - 2005

  ---------------------------------------------------------------
*/

#ifndef __CUPGROUPROUND_H__
#define __CUPGROUPROUND_H__


//----------------------------------------------------------------------------
class CCupGroupRound : public CCupRound
{
	// Variables
private:
    uint8					m_NumberInGroup;
    uint8					m_NumberOfGroupMatches;


	uint8					NumberMatchesinGroup(void);
	uint8					GetNumberInGroup(void);
    const uint8				GetNumberOfGroupMatches(void) const				{ return m_NumberOfGroupMatches; }
public:
	void					DoInitialiseFromFile(FILE* File);
    int						NumberOfGroups(void);
    void					GetGroupClubNumbers(int GroupNumber, CNumericArray<uint16>& aClubNumbers, CCupRound* pSecondCupRound);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
