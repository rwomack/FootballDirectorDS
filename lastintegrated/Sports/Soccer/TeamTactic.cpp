
/*

Tactic.cpp: implementation of the CTactic class.


Football Director

(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/


#include "stdafx.h"



/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::CTeamTactic
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTeamTactic::CTeamTactic()
{
	SetDefaults();
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::operator=
	Access:    	public 
	Parameter: 	CTeamTactic * _TeamTactic
	Returns:   	CTeamTactic&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTeamTactic& CTeamTactic::operator =(CTeamTactic* _TeamTactic)
{
	Copy(*_TeamTactic);
	return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::operator=
	Access:    	public 
	Parameter: 	CTeamTactic _TeamTactic
	Returns:   	CTeamTactic&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTeamTactic& CTeamTactic::operator =(CTeamTactic _TeamTactic)
{
	Copy(_TeamTactic);
	return *this;
}


/*------------------------------------------------------------------------------
Method:   	CTeamTactic::SetDefaults
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::SetDefaults()
{
	m_StyleOfPlay = 4;
	m_MarkingStyle = 4;
	m_Aggression = 4;               // Medium aggression
	m_Flags = 0;
}


/*------------------------------------------------------------------------------
Method:   	CTeamTactic::Copy
Access:    	public 
Parameter: 	CTeamTactic & _TeamTactic
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::Copy(CTeamTactic& _TeamTactic)
{
	m_StyleOfPlay = _TeamTactic.m_StyleOfPlay;
	m_MarkingStyle = _TeamTactic.m_MarkingStyle;
	m_Aggression = _TeamTactic.m_Aggression;
	m_Flags = _TeamTactic.m_Flags;
}


/*------------------------------------------------------------------------------
	Method:   	CClub::Read
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::Read(CDataFile* _pDataFile)
{
	_pDataFile->Read(&m_StyleOfPlay, sizeof m_StyleOfPlay);
	_pDataFile->Read(&m_MarkingStyle, sizeof m_MarkingStyle);
	_pDataFile->Read(&m_Aggression, sizeof m_Aggression);
	_pDataFile->Read(&m_Flags, sizeof m_Flags);
}


/*------------------------------------------------------------------------------
	Method:   	CClub::Write
	Access:    	public 
	Parameter: 	FILE * _pFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::Write(CDataFile* _pDataFile)
{
	_pDataFile->Write(&m_StyleOfPlay, sizeof m_StyleOfPlay);
	_pDataFile->Write(&m_MarkingStyle, sizeof m_MarkingStyle);
	_pDataFile->Write(&m_Aggression, sizeof m_Aggression);
	_pDataFile->Write(&m_Flags, sizeof m_Flags);
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::GetCounterAttack
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CTeamTactic::GetCounterAttack()
{
	if (m_Flags & PLAYONTHEBREAK)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::GetStyleOfPlay
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTeamTactic::GetStyleOfPlay()
{
	return m_StyleOfPlay;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::GetMarkingStyle
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTeamTactic::GetMarkingStyle()
{
	return m_MarkingStyle;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::GetAggression
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CTeamTactic::GetAggression()
{
	return m_Aggression;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::SetStyleOfPlay
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::SetStyleOfPlay(const byte _Value)
{
	m_StyleOfPlay = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::SetMarkingStyle
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::SetMarkingStyle(const byte _Value)
{
	m_MarkingStyle = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::SetAggression
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::SetAggression(const byte _Value)
{
	m_Aggression = _Value;
}


/*------------------------------------------------------------------------------
Method:   	CTeamTactic::GetOffsideTrap
Access:    	public 
Returns:   	bool
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
bool CTeamTactic::GetOffsideTrap()
{
	if (m_Flags & OFFSIDETRAP)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::SetCounterAttack
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::SetCounterAttack(bool _Value)
{
	if (_Value == true)
	{
		m_Flags |= PLAYONTHEBREAK;
	}
	else
	{
		m_Flags &= ~PLAYONTHEBREAK;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::SetOffsideTrap
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::SetOffsideTrap(bool _Value)
{
	if (_Value == true)
	{
		m_Flags |= OFFSIDETRAP;
	}
	else
	{
		m_Flags &= ~OFFSIDETRAP;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::ToggleCounterAttack
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::ToggleCounterAttack()
{
	SetCounterAttack(GetCounterAttack() ^ 1);
}


/*------------------------------------------------------------------------------
	Method:   	CTeamTactic::ToggleOffsideTrap
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CTeamTactic::ToggleOffsideTrap()
{
	SetOffsideTrap(GetOffsideTrap() ^ 1);
}
