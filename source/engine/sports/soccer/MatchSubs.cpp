
/*

    Match Substitutes Class -


	Football Director

	(c) Rw Software 1994 - 2008

    ---------------------------------------------------------------
 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::CMatchSubs
	Access:    	public 
	Returns:   	
	Qualifier: 	: m_NumSubstitutionsAllowed(0)
	Purpose:
------------------------------------------------------------------------------*/
CMatchSubs::CMatchSubs()
	: m_NumSubstitutionsAllowed(0)
{
	m_TimeSubstituteUsed.SetSize(MAXSUBS * 2, 0);
	m_PlayerIDTakenOff.SetSize(MAXSUBS * 2, NOPLAYER);
	m_PlayerIDBroughtOn.SetSize(MAXSUBS * 2, NOPLAYER);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::OnInitialise
	Access:    	public 
	Parameter: 	const byte _SubsUse
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchSubs::OnInitialise(const byte _SubsUse /*= MAXSUBS*/)
{
	EXT_ASSERT(_SubsUse <= MAXSUBS);
	m_TimeSubstituteUsed.FillWithValue(MAXSUBS * 2, 0);
	m_PlayerIDTakenOff.FillWithValue(NOPLAYER);
	m_PlayerIDBroughtOn.FillWithValue(NOPLAYER);
	m_NumSubstitutionsAllowed = _SubsUse;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::DoCountSubsUsed
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	uint
	Qualifier: 	
	Purpose:	How many substitutes has the team made?
------------------------------------------------------------------------------*/
uint CMatchSubs::DoCountSubsUsed(const eWhichTeam _Who)
{
	uint iRet = 0;
	for (uint LoopCount = 0 + (_Who == AWAY) * MAXSUBS; LoopCount < MAXSUBS + (_Who == AWAY) * MAXSUBS; LoopCount++)
	{
		iRet += (m_PlayerIDTakenOff[LoopCount] != NOPLAYER);
	}
	return iRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::HasPlayerBeenSubstitutedOn
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchSubs::HasPlayerBeenSubstitutedOn(const ushort _PlayerID)
{
	bool bRet = false;
	for (uint LoopCount = 0; LoopCount < MAXSUBS * 2; LoopCount++)
	{
		if (m_PlayerIDBroughtOn[LoopCount] == _PlayerID)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::HasPlayerBeenSubstitutedOff
	Access:    	public 
	Parameter: 	const ushort _PlayerID
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchSubs::HasPlayerBeenSubstitutedOff(const ushort _PlayerID)
{
	bool bRet = false;
	for (uint LoopCount = 0; LoopCount < MAXSUBS * 2; LoopCount++)
	{
		if (m_PlayerIDTakenOff[LoopCount] == _PlayerID)
		{
			bRet = true;
			break;
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::MakeSubstitution
	Access:    	public 
	Parameter: 	const byte _TheTime
	Parameter: 	const eWhichTeam _Who
	Parameter: 	const ushort _PlayerOnListId
	Parameter: 	const ushort _PlayerOffListId
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchSubs::MakeSubstitution(const byte _TheTime, const eWhichTeam _Who, const ushort _PlayerOnListId, const ushort _PlayerOffListId)
{
	EXT_ASSERT(_PlayerOffListId != _PlayerOnListId);
	bool bRet = false;
	if (DoCountSubsUsed(_Who) < GetNumSubstitutionsAllowed() && HasPlayerBeenSubstitutedOn(_PlayerOnListId) != true)
	{
		for (uint LoopCount = 0 + (_Who == AWAY) * MAXSUBS; LoopCount < MAXSUBS + (_Who == AWAY) * MAXSUBS; LoopCount++)
		{
			if (m_PlayerIDTakenOff[LoopCount] == NOPLAYER)
			{
 				m_PlayerIDTakenOff.SetAt(LoopCount, _PlayerOffListId);
				EXT_ASSERT(m_PlayerIDBroughtOn.GetAt(LoopCount) == NOPLAYER);
				m_PlayerIDBroughtOn.SetAt(LoopCount, _PlayerOnListId);
				m_TimeSubstituteUsed.SetAt(LoopCount, _TheTime);
				bRet = true;
				break;
			}
			else if (m_PlayerIDTakenOff[LoopCount] == _PlayerOnListId)
			{
				bRet = true;
				break;
			}
		}
	}
	return bRet;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::DoCountSubstitutionsLeft
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchSubs::DoCountSubstitutionsLeft(const eWhichTeam _Who)
{
	return GetNumSubstitutionsAllowed() - DoCountSubsUsed(_Who);
}


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::GetNumSubstitutionsAllowed
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchSubs::GetNumSubstitutionsAllowed() const
{
	return m_NumSubstitutionsAllowed;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchSubs::AnySubstitutionsLeft
	Access:    	public 
	Parameter: 	const eWhichTeam _Who
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchSubs::AnySubstitutionsLeft(const eWhichTeam _Who)
{
	if (DoCountSubsUsed(_Who) == GetNumSubstitutionsAllowed())
	{
		return false;
	}
	else
	{
		return true;
	}
}
