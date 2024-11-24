
/*

    Match Substitutes Class -


	Football Director

	(c) Rw Software 1994 - 2008

    ---------------------------------------------------------------
 */


#ifndef __MATCHSUBS_H__
#define __MATCHSUBS_H__

class CMatchSubs 
{
public:
	CMatchSubs();

	bool					AnySubstitutionsLeft(const eWhichTeam _Who);
	uint					DoCountSubsUsed(const eWhichTeam _Who);
	byte					DoCountSubstitutionsLeft(const eWhichTeam _Who);
	byte					GetNumSubstitutionsAllowed() const;
	bool					HasPlayerBeenSubstitutedOn(const ushort _PlayerID);
	bool					HasPlayerBeenSubstitutedOff(const ushort _PlayerID);
	bool					MakeSubstitution(const byte _TheTime, const eWhichTeam _Who, const ushort _PlayerOnListId, const ushort _PlayerOffListId);
	void					OnInitialise(const byte _SubsUse = MAXSUBS);

// Variables
private:
	byte					m_NumSubstitutionsAllowed;
	CNumericArray<byte>		m_TimeSubstituteUsed;       // Time brought on
	CNumericArray<ushort> 	m_PlayerIDTakenOff;			// Actual subs used, brought off
	CNumericArray<ushort>	m_PlayerIDBroughtOn;        // Sub Number brought on
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif