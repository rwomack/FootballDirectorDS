
 /*

    Class:- Current Form


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "CurrentForm.h"


/*------------------------------------------------------------------------------
	Method:   	CMatchResult::DoResetVariables
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchResult::DoResetVariables()
{ 
	m_Conceded = 0xff; 
	m_Scored = 0xff; 
}


/*------------------------------------------------------------------------------
	Method:   	CMatchResult::GetScored
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchResult::GetScored() const
{
	return m_Scored;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchResult::GetConceded
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CMatchResult::GetConceded() const
{
	return m_Conceded;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchResult::HasBeenPlayed
	Access:    	public const 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchResult::HasBeenPlayed() const
{
	return m_Scored != 0xff;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchResult::WinDrawLoseStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString	CMatchResult::WinDrawLoseStr()
{
    CString str;
	if (HasBeenPlayed() == true)
	{
        if (m_Scored > m_Conceded)
		{
               str.LoadString(IDS_CURRENTFORMWON);
        }

		if (m_Scored == m_Conceded)
		{
			str.LoadString(IDS_CURRENTFORMDRAWN);
        }
		if (m_Scored < m_Conceded)
		{
           str.LoadString(IDS_CURRENTFORMLOST);
        }
	}
	else
	{
		// Not played the match
        str.LoadString(IDS_CURRENTFORMNOTPLAYED);
    }
	return str;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchResult::HasWonMatch
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchResult::HasWonMatch()
{
	if 	(HasBeenPlayed() == true)
	{
		if (m_Scored > m_Conceded)
		{
			return true;
		}
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchResult::HasDrawnMatch
	Access:    	public 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CMatchResult::HasDrawnMatch()
{
	if (HasBeenPlayed() == true)
	{
		if (m_Scored == m_Conceded)
		{
			return true;
		}
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CMatchResult::SetResult
	Access:    	public 
	Parameter: 	byte _Scored
	Parameter: 	byte _Conceded
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CMatchResult::SetResult(byte _Scored, byte _Conceded)
{
	m_Scored = _Scored;
	m_Conceded = _Conceded;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::CCurrentForm
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCurrentForm::CCurrentForm()
{
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::DoResetVariables
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentForm::DoResetVariables()
{
	m_CurrentFormPts = 0;
	m_CurrentFormPosn = 0;
    m_CurrentFormTotalConceded = 0;
    m_CurrentFormScored = 0;
    for (uint LoopCount = 0; LoopCount < NUMFORMMATCHES; LoopCount++)
	{
		m_CurrentForm[LoopCount].DoResetVariables();
	}
 }


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::WinDrawLoseStr
	Access:    	public 
	Parameter: 	void
	Returns:   	CString - the match status of the matches, eg. WWDDLL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCurrentForm::WinDrawLoseStr()
{
    CString str;
    for (uint LoopCount = 0; LoopCount < NUMFORMMATCHES; LoopCount++)
	{
		str += m_CurrentForm[LoopCount].WinDrawLoseStr();
    }
	str.MakeReverse();
    return str;
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::GetCurrentFormPts
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCurrentForm::GetCurrentFormPts() const
{
	return m_CurrentFormPts;
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::GetCurrentFormPlayed
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCurrentForm::GetCurrentFormPlayed() const
{
    byte Won = 0;
    for (uint LoopCount = 0; LoopCount < NUMFORMMATCHES; LoopCount++)
	{
		if (m_CurrentForm[LoopCount].HasBeenPlayed() == true)
		{
			Won++;
		}
	}
    return Won;
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::GetCurrentFormWon
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CCurrentForm::GetCurrentFormWon()
{
    byte Won = 0;
	int HowMany = min(GetCurrentFormPlayed(), NUMFORMMATCHES);
    for (int LoopCount = 0; LoopCount < HowMany; LoopCount++)
	{
		Won += m_CurrentForm[LoopCount].HasWonMatch();
	}
    return Won;
 }


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::GetCurrentFormDrawn
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCurrentForm::GetCurrentFormDrawn()
{
    byte Drawn = 0;
	int HowMany = min(GetCurrentFormPlayed(), NUMFORMMATCHES);
    for (int LoopCount = 0; LoopCount < HowMany; LoopCount++)
	{
		Drawn += m_CurrentForm[LoopCount].HasDrawnMatch();
	}
    return Drawn;
}


/*------------------------------------------------------------------------------
Method:   	CCurrentForm::GetCurrentFormLost
Access:    	public 
Parameter: 	void
Returns:    byte
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
byte CCurrentForm::GetCurrentFormLost()					
{ 
	return GetCurrentFormPlayed() - (GetCurrentFormWon() + GetCurrentFormDrawn()); 
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::UpdateCurrentForm
	Access:    	public 
	Parameter: 	byte scored
	Parameter: 	const byte _Conceded
	Parameter: 	const byte _PointsForWin
	Parameter: 	const byte _PointsForDraw
	Returns:   	void
	Qualifier: 	
	Purpose:	Update the current form data
------------------------------------------------------------------------------*/
void CCurrentForm::DoUpdateCurrentForm(byte _Scored, const byte _Conceded, const byte _PointsForWin, const byte _PointsForDraw)
{
	int LoopCount;
	// Move all down a match
	for (LoopCount = NUMFORMMATCHES - 1; LoopCount > 0; LoopCount--)
	{
		m_CurrentForm[LoopCount] = m_CurrentForm[LoopCount - 1];
	}

	// Add the current result
	m_CurrentForm[0].SetResult(_Scored, _Conceded);

	// Clear the totals
    m_CurrentFormPts = m_CurrentFormTotalConceded = m_CurrentFormScored = 0;

	// Calculate the totals
	for (LoopCount = 0; LoopCount < NUMFORMMATCHES; LoopCount++)
	{
		if 	(m_CurrentForm[LoopCount].HasBeenPlayed() == true)
		{
			m_CurrentFormTotalConceded += m_CurrentForm[LoopCount].GetConceded();
			m_CurrentFormScored += m_CurrentForm[LoopCount].GetScored();
			if (m_CurrentForm[LoopCount].HasWonMatch() == true)
			{
				m_CurrentFormPts += _PointsForWin;
			}
			if (m_CurrentForm[LoopCount].HasDrawnMatch() == true)
			{
				m_CurrentFormPts += _PointsForDraw;
			}
		}
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::GetCurrentFormPosn
	Access:    	public const 
	Parameter: 	void
	Returns:    byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCurrentForm::GetCurrentFormPosn() const		        
{ 
	return m_CurrentFormPosn; 
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::GetCurrentFormConceded
	Access:    	public const 
	Parameter: 	void
	Returns:    byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCurrentForm::GetCurrentFormConceded() const 			
{ 
	return m_CurrentFormTotalConceded; 
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::GetCurrentFormScored
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCurrentForm::GetCurrentFormScored() const			
{ 
	return m_CurrentFormScored; 
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::SetCurrentFormPts
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentForm::SetCurrentFormPts(const byte x)			
{ 
	m_CurrentFormPts = x; 
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::SetCurrentFormPosn
	Access:    	public 
	Parameter: 	const byte _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentForm::SetCurrentFormPosn(const byte _Value)
{ 
	m_CurrentFormPosn = _Value; 
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::SetCurrentFormConceded
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentForm::SetCurrentFormConceded(const byte x)		
{ 
	m_CurrentFormTotalConceded = x; 
}


/*------------------------------------------------------------------------------
	Method:   	CCurrentForm::SetCurrentFormScored
	Access:    	public 
	Parameter: 	const byte x
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrentForm::SetCurrentFormScored(const byte x)			
{ 
	m_CurrentFormScored = x; 
}
