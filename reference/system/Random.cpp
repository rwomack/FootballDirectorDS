
 /*

	File:- Random.cpp

	(c) Rw Software 2006 - 2008

 ---------------------------------------------------------------------------------------
 */


 #include    "stdafx.h"


// This is the default random seed for the Visual C++ CRT, per the documentation.
unsigned int CRandomNumber::s_uiLastRandomSeed = 1;

/*------------------------------------------------------------------------------
	Method:   	CRandomNumber::IntLessThan
	Access:    	public 
	Parameter: 	const uint maxvalue
	Returns:   	uint
	Qualifier: 	
	Purpose:	Generate a random number between 0 & {maxvalue}
------------------------------------------------------------------------------*/
uint CRandomNumber::IntLessThan(const uint maxvalue)
{
	EXT_ASSERT(maxvalue > 0);
	return static_cast<int> ((rand() / 32767.0) * maxvalue);
}


/*------------------------------------------------------------------------------
	Method:   	Randomize
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CRandomNumber::Randomize()
{
#ifndef _DEBUG
  s_uiLastRandomSeed = GetTickCount();
  srand(s_uiLastRandomSeed);
#endif
}

/*------------------------------------------------------------------------------
	Method:   	SetSeed
	Access:    	public
	Returns:   	void
	Qualifier:
	Purpose:	Allows the random seed to be set to a specific value for debugging purposes.
------------------------------------------------------------------------------*/
void CRandomNumber::SetSeed(const unsigned int uiSeed)
{
	srand(uiSeed);
	s_uiLastRandomSeed = uiSeed;
}
