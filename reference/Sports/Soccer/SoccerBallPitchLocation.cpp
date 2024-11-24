// SoccerBallPitchLocation.cpp: implementation of the CSoccerBallPitchLocation class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


/*------------------------------------------------------------------------------
	Method:   	CSoccerBallPitchLocation::DoInitialise
	Access:    	public 
	Parameter: 	const ushort nLength
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerBallPitchLocation::DoInitialise(const ushort nLength)
{
	m_MinuteEventBallX.SetSize(nLength);
	m_MinuteEventBallY.SetSize(nLength);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerBallPitchLocation::DoLocateX
	Access:    	public 
	Parameter: 	const ushort nTimePlayed
	Parameter: 	const byte xPosition
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerBallPitchLocation::DoLocateX(const ushort nTimePlayed, const byte xPosition)
{
	EXT_ASSERT(nTimePlayed < m_MinuteEventBallX.GetSize());
	m_MinuteEventBallX.SetAt(nTimePlayed, xPosition);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerBallPitchLocation::DoCalculateBallLocation
	Access:    	public 
	Parameter: 	const ushort nTimePlayed
	Parameter: 	uint nEventID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerBallPitchLocation::DoCalculateBallLocation(const ushort nTimePlayed, uint nEventID)
{
	// Default ball x, y if can be random then the event handler will modify them
	SetBallLocation(nTimePlayed, BallXPos[nEventID], BallYPos[nEventID]);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerBallPitchLocation::GetBallXLocation
	Access:    	public 
	Parameter: 	const ushort nTimePlayed
	Returns:   	const ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CSoccerBallPitchLocation::GetBallXLocation(ushort _TimePlayed)
{
	EXT_ASSERT(_TimePlayed < m_MinuteEventBallX.GetSize());
	return m_MinuteEventBallX[_TimePlayed];
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerBallPitchLocation::GetBallYLocation
	Access:    	public 
	Parameter: 	ushort _TimePlayed
	Returns:   	ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
ushort CSoccerBallPitchLocation::GetBallYLocation(ushort _TimePlayed)
{
	EXT_ASSERT(_TimePlayed < m_MinuteEventBallX.GetSize());
	return m_MinuteEventBallY[_TimePlayed];
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerBallPitchLocation::SetBallLocation
	Access:    	public 
	Parameter: 	const ushort nTimePlayed
	Parameter: 	const byte xPosition
	Parameter: 	const byte yPosition
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerBallPitchLocation::SetBallLocation(const ushort _TimePlayed, const byte _XPosition, const byte _YPosition)
{
	EXT_ASSERT(_TimePlayed < m_MinuteEventBallX.GetSize());
	m_MinuteEventBallX.SetAt(_TimePlayed, _XPosition);
	m_MinuteEventBallY.SetAt(_TimePlayed, _YPosition);
}


/*------------------------------------------------------------------------------
	Method:   	CSoccerBallPitchLocation::SetBallLocationY
	Access:    	public 
	Parameter: 	const ushort _TimePlayed
	Parameter: 	const byte _YPosition
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CSoccerBallPitchLocation::SetBallLocationY(const ushort _TimePlayed, const byte _YPosition)
{
	EXT_ASSERT(_TimePlayed < m_MinuteEventBallX.GetSize());
	m_MinuteEventBallY.SetAt(_TimePlayed, _YPosition);
}
