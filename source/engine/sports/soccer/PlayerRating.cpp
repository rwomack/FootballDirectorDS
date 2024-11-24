
 /*

    PlayerRating.cpp: implementation of the CPlayerRating class.


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//------------------------------------------------------------------------------------------------------
CPlayerRating::CPlayerRating(const byte x)
{
	OnInitialise(x);
}


//------------------------------------------------------------------------------------------------------
CPlayerRating::CPlayerRating()
{
	DoResetVariables();
}


//------------------------------------------------------------------------------------------------------
CPlayerRating::~CPlayerRating()
{
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerRating::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerRating::DoResetVariables()
{
	OnInitialise(0);
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerRating::DoMoveDown
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerRating::DoMoveDown()
{
	for (int LoopCount = 1; LoopCount < NUMPLAYERFORMMATCHES; LoopCount++)
	{
		m_Rating[LoopCount] = m_Rating[LoopCount - 1];
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerRating::OnInitialise
	Access:    	public 
	Parameter: 	const byte _InitialRating
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerRating::OnInitialise(const byte _InitialRating)
{
	for (int LoopCount = 0; LoopCount < NUMPLAYERFORMMATCHES; LoopCount++)
	{
		m_Rating[LoopCount] = _InitialRating;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerRating::DoAddPerformance
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerRating::DoAddPerformance(const byte _Value)
{
	EXT_ASSERT(_Value > 0 && _Value < 101);
    DoMoveDown();
    m_Rating[0] = _Value;
}


 // Performance is calculated as the differenced between the last 2 ratings
//------------------------------------------------------------------------------------------------------
sbyte CPlayerRating::GetPerformance()
{
    return static_cast<sbyte> (m_Rating[0] - m_Rating[1]);
}


//------------------------------------------------------------------------------------------------------
CPlayerRating& CPlayerRating::operator = (const CPlayerRating &p)
{
	for (uint n = 0; n < NUMPLAYERFORMMATCHES; n++)
	{
		m_Rating[n] = p.m_Rating[n];
	}
    return *this;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerRating::GetLastPerformance
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerRating::GetLastPerformance()
{
	return m_Rating[0];
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerRating::CurrentRating
	Access:    	public 
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CPlayerRating::CurrentRating()
{
	int Count = 0;
	for (uint LoopCount = 0; LoopCount < NUMPLAYERFORMMATCHES; LoopCount++)
	{
		Count += m_Rating[LoopCount];
	}
	byte bRet = static_cast <byte>(Count / NUMPLAYERFORMMATCHES);
    return min(max(bRet, MINPLAYSKILL), MAXPLAYSKILL);
}

