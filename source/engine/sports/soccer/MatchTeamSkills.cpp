// MatchTeamSkills.cpp: implementation of the CMatchTeamSkills class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::CMatchTeamSkills
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchTeamSkills::CMatchTeamSkills() : m_GoalKeeperSkill(0), m_DefenderSkill(0), m_MidfielderSkill(0), m_AttackerSkill(0)
{

}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::~CMatchTeamSkills
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchTeamSkills::~CMatchTeamSkills()
{

}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::GetGoalKeeperSkill
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchTeamSkills::GetGoalKeeperSkill() const
{ 
	return m_GoalKeeperSkill; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::GetDefenderSkill
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchTeamSkills::GetDefenderSkill() const
{ 
	return m_DefenderSkill; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::GetMidfielderSkill
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchTeamSkills::GetMidfielderSkill() const
{ 
	return m_MidfielderSkill; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::GetAttackerSkill
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchTeamSkills::GetAttackerSkill() const
{ 
	return m_AttackerSkill; 
}



/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTeamSkills::DoResetVariables()
{
    m_GoalKeeperSkill = m_MidfielderSkill = m_DefenderSkill = m_AttackerSkill = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::SetGoalKeeperSkill
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTeamSkills::SetGoalKeeperSkill(const byte _Value)
{ 
	EXT_ASSERT(_Value >= MINPLAYSKILL && _Value <= MAXPLAYSKILL);
	m_GoalKeeperSkill = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::SetDefenderSkill
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTeamSkills::SetDefenderSkill(const byte _Value)
{ 
	EXT_ASSERT(_Value >= MINPLAYSKILL && _Value <= MAXPLAYSKILL);
	m_DefenderSkill = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::SetMidfielderSkill
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTeamSkills::SetMidfielderSkill(const byte x)	  		
{ 
	EXT_ASSERT(x >= MINPLAYSKILL && x <= MAXPLAYSKILL);
	m_MidfielderSkill = x; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchTeamSkills::SetAttackerSkill
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchTeamSkills::SetAttackerSkill(const byte x)	   			
{ 
	EXT_ASSERT(x >= MINPLAYSKILL && x <= MAXPLAYSKILL);
	m_AttackerSkill = x; 
}

void CMatchTeamSkills::Write(CDataFile* _pDataFile)
{
	_pDataFile->WriteBits(m_GoalKeeperSkill, GOALKEEPERSKILLBITS);
	_pDataFile->WriteBits(m_DefenderSkill, DEFENDERSKILLBITS);
	_pDataFile->WriteBits(m_MidfielderSkill, MIDFIELDERSKILLBITS);
	_pDataFile->WriteBits(m_AttackerSkill, ATTACKERSKILLBITS);
}

void CMatchTeamSkills::Read(CDataFile* _pDataFile)
{
	m_GoalKeeperSkill = _pDataFile->ReadBits(GOALKEEPERSKILLBITS);
	m_DefenderSkill = _pDataFile->ReadBits(DEFENDERSKILLBITS);
	m_MidfielderSkill = _pDataFile->ReadBits(MIDFIELDERSKILLBITS);
	m_AttackerSkill = _pDataFile->ReadBits(ATTACKERSKILLBITS);
}