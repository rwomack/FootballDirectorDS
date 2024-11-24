
/*

        Class:- Confidence class

		Football Director

		(c) Rw Software 1994 - 2008

---------------------------------------------------------------
*/

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CConfidence::Read
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CConfidence::Read(CDataFile* _pDataFile)
{
  _pDataFile->Read(&m_Value, sizeof(m_Value));
}


/*------------------------------------------------------------------------------
	Method:   	CConfidence::Write
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CConfidence::Write(CDataFile* _pDataFile)
{
  _pDataFile->Write(&m_Value, sizeof(m_Value));
}


/*------------------------------------------------------------------------------
	Method:   	CConfidence::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CConfidence::DoResetVariables()
{
	m_Value = MINIMUMCONFIDENCE;
}


/*------------------------------------------------------------------------------
	Method:   	CConfidence::DoIncrease
	Access:    	public 
	Parameter: 	const sbyte _HowMuch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CConfidence::DoIncrease(const sbyte _HowMuch)
{
	m_Value = min(max(m_Value + _HowMuch, MINIMUMCONFIDENCE), MAXIMUMCONFIDENCE);
}


/*------------------------------------------------------------------------------
	Method:   	CConfidence::DoDecrease
	Access:    	public 
	Parameter: 	const sbyte _HowMuch
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CConfidence::DoDecrease(const sbyte _HowMuch)
{
	m_Value = min(max(m_Value - _HowMuch, MINIMUMCONFIDENCE), MAXIMUMCONFIDENCE);
}


/*------------------------------------------------------------------------------
	Method:   	CConfidence::DoRegenerate
	Access:    	public 
	Parameter: 	void
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CConfidence::DoRegenerate()
{
	m_Value = 70 + RandomNumber.IntLessThan(MAXIMUMCONFIDENCE - 70);
}


/*------------------------------------------------------------------------------
	Method:   	CConfidence::GetValue
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CConfidence::GetValue() const
{
	return m_Value;
}
