
 /*

    Class:- CMatchEventsStats


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::CMatchEventsStats
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CMatchEventsStats::CMatchEventsStats()
	: m_PercentageOfPlay(50),
	m_ShotsOnTarget(0),
	m_ShotsOffTarget(0),
	m_FreeKicks(0),
	m_OffSides(0),
	m_Fouls(0),
	m_Corners(0),
	m_ClubID(NOCLUB)
#ifdef STOREMATCHINJURIES
	, m_Injuries(0)
#endif
#ifdef STOREMATCHPOSESSION
    , m_Posession(0)
#endif
{
}


 // -----------------------------------------------------------------------
void CMatchEventsStats::CopyStats(CMatchEventsStats& pStats)
{
    m_PercentageOfPlay = pStats.m_PercentageOfPlay;
    SetShotsOnTarget(pStats.GetShotsOnTarget());
    SetShotsOffTarget(pStats.GetShotsOffTarget());
    SetCorners(pStats.GetCorners());
    SetFreeKicks(pStats.GetFreeKicks());
    SetOffsides(pStats.GetOffsides());
    SetFouls(pStats.GetFouls());
#ifdef STOREMATCHINJURIES
	m_Injuries = pStats.m_Injuries;
#endif
#ifdef STOREMATCHPOSESSION
    m_Posession = pStats.m_Posession;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::SetClubID
	Access:    	public 
	Parameter: 	const ushort _ClubID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::SetClubID(const ClubID _ClubID)
{
	m_ClubID = _ClubID;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetTeamSkills
	Access:    	public 
	Returns:   	CMatchTeamSkills&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CMatchTeamSkills& CMatchEventsStats::GetTeamSkills()
{
	return m_TeamSkills;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetClubID
	Access:    	public 
	Returns:   	ushort
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
ClubID CMatchEventsStats::GetClubID() const	
{ 
	return m_ClubID; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetShotsOnTarget
	Access:    	public 
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CMatchEventsStats::GetShotsOnTarget()			       		
{ 
	EXT_ASSERT(m_ShotsOnTarget < 90); 
	return m_ShotsOnTarget; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetShotsOffTarget
	Access:    	public 
	Returns:   	sbyte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
sbyte CMatchEventsStats::GetShotsOffTarget()
{ 
	EXT_ASSERT(m_ShotsOffTarget < 90); 
	return m_ShotsOffTarget; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetOffsides
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchEventsStats::GetOffsides()	
{ 
	EXT_ASSERT(m_OffSides < 90);
	return m_OffSides; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetFouls
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchEventsStats::GetFouls()	         					
{ 
	EXT_ASSERT(m_Fouls < 90);
	return m_Fouls; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetCorners
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchEventsStats::GetCorners()
{ 
	EXT_ASSERT(m_Corners < 90);
	return m_Corners; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetFreeKicks
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchEventsStats::GetFreeKicks()	         				
{ 
	EXT_ASSERT(m_FreeKicks < 90);
	return m_FreeKicks; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::GetPercentageOfPlay
	Access:    	public 
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchEventsStats::GetPercentageOfPlay() const	  			{ return m_PercentageOfPlay; }


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::DoIncrementCorners
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::DoIncrementCorners()		          	
{ 
	m_Corners++; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::DoIncrementFreeKicks
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::DoIncrementFreeKicks()		        	
{ 
	m_FreeKicks++; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::DoIncrementOffsides
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::DoIncrementOffsides()					
{ 
	m_OffSides++; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::DoIncrementFouls
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::DoIncrementFouls()						
{ 
	m_Fouls++; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::DoIncrementShotsOnTarget
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::DoIncrementShotsOnTarget()				
{ 
	m_ShotsOnTarget++; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::DoIncrementShotsOffTarget
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::DoIncrementShotsOffTarget()				
{ 
	m_ShotsOffTarget++; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::DoAddShotsOnTarget
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::DoAddShotsOnTarget(const byte _Value)
{ 
	m_ShotsOnTarget += _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::SetShotsOnTarget
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::SetShotsOnTarget(const byte _Value)				
{ 
	EXT_ASSERT(_Value < 90); 
	m_ShotsOnTarget = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::SetShotsOffTarget
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::SetShotsOffTarget(const byte _Value)			
{ 
	EXT_ASSERT(_Value < 90); 
	m_ShotsOffTarget = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::SetCorners
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::SetCorners(const byte _Value)					
{ 
	EXT_ASSERT(_Value < 90); 
	m_Corners = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::SetFreeKicks
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::SetFreeKicks(const byte _Value)					
{ 
	EXT_ASSERT(_Value < 90); 
	m_FreeKicks = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::SetOffsides
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::SetOffsides(const byte _Value)					
{ 
	EXT_ASSERT(_Value < 90); 
	m_OffSides = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::SetFouls
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::SetFouls(const byte _Value)						
{ 
	EXT_ASSERT(_Value < 90); 
	m_Fouls = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::SetPercentageOfPlay
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::SetPercentageOfPlay(const byte _Value)			
{ 
	m_PercentageOfPlay = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchEventsStats::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchEventsStats::DoResetVariables()
{
	GetTeamSkills().DoResetVariables();
    m_PercentageOfPlay = 50;
    SetShotsOffTarget(0);
	SetShotsOnTarget(0);
    SetCorners(0);
    SetFreeKicks(0);
    SetOffsides(0);
    SetFouls(0);
#ifdef STOREMATCHINJURIES
	m_Injuries = 0;
#endif
#ifdef STOREMATCHPOSESSION
    m_Posession = 0;
#endif
}


 // -----------------------------------------------------------------------
byte CMatchEventsStats::Performance()
{
	return(GetTeamSkills().GetGoalKeeperSkill() + GetTeamSkills().GetDefenderSkill() + GetTeamSkills().GetMidfielderSkill() + GetTeamSkills().GetAttackerSkill()) / 4;
}

void CMatchEventsStats::Write(CDataFile* _pDataFile)
{
	m_TeamSkills.Write(_pDataFile);
	_pDataFile->WriteBits(m_PercentageOfPlay, PERCENTAGEOFPLAYBITS);
	_pDataFile->WriteBits(m_ShotsOnTarget, SHOTSONTARGETBITS);
	_pDataFile->WriteBits(m_ShotsOffTarget, SHOTSOFFTARGETBITS);
	_pDataFile->WriteBits(m_FreeKicks, FREEKICKSBITS);
	_pDataFile->WriteBits(m_OffSides, OFFSIDESBITS);
	_pDataFile->WriteBits(m_Fouls, FOULSBITS);
	_pDataFile->WriteBits(m_Corners, CORNERSBITS);
	_pDataFile->WriteBits(m_ClubID.id, sizeof(m_ClubID) * 8);
#ifdef STOREMATCHINJURIES
	_pDataFile->WriteBits(m_Injuries, 8);
#endif

#ifdef STOREMATCHPOSESSION
	_pDataFile->WriteBits((unsigned int)((void *)m_Posession), sizeof(m_Posession) * 8);
#endif
}

void CMatchEventsStats::Read(CDataFile* _pDataFile)
{
	m_TeamSkills.Read(_pDataFile);
	m_PercentageOfPlay = _pDataFile->ReadBits(PERCENTAGEOFPLAYBITS);
	m_ShotsOnTarget = _pDataFile->ReadBits(SHOTSONTARGETBITS);
	m_ShotsOffTarget = _pDataFile->ReadBits(SHOTSOFFTARGETBITS);
	m_FreeKicks = _pDataFile->ReadBits(FREEKICKSBITS);
	m_OffSides = _pDataFile->ReadBits(OFFSIDESBITS);
	m_Fouls = _pDataFile->ReadBits(FOULSBITS);
	m_Corners = _pDataFile->ReadBits(CORNERSBITS);
	m_ClubID.id = _pDataFile->ReadBits(sizeof(m_ClubID) * 8);
#ifdef STOREMATCHINJURIES
	m_Injuries = _pDataFile->ReadBits(8);
#endif

#ifdef STOREMATCHPOSESSION
	m_Posession = (float)((void *)_pDataFile->ReadBits(sizeof(m_Posession) * 8));
#endif
}
