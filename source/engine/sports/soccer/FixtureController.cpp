
/*
	File:- FixtureController.cpp

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



/*------------------------------------------------------------------------------
	Method:   	CFixtureController::CFixtureController
	Access:    	public 
	Returns:   
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFixtureController::CFixtureController()
{
	CBinaryReader theResource(0, IDR_FIXT20, TEXT("FDDATA"));
	m_FixtureList20Clubs.ReadFromResource(FIXT20MATCHDAYS * FIXT20CLUBS, theResource);

	CBinaryReader theResource24(0, IDR_FIXT24, TEXT("FDDATA"));
	m_FixtureList24Clubs.ReadFromResource(FIXT24MATCHDAYS * FIXT24CLUBS, theResource24);
}


/*------------------------------------------------------------------------------
	Method:   	CFixtureController::GetFixtureList
	Access:    	public 
	Parameter: 	const int _nNumberOfClubs
	Returns:   	CNumericArray<byte>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNumericArray<byte>& CFixtureController::GetFixtureList(const int _nNumberOfClubs)
{
	switch (_nNumberOfClubs)
	{
		case 20:
			return m_FixtureList20Clubs;
		case 24:
			return m_FixtureList24Clubs;
		default:
			EXT_ASSERT(false);
	};
	return m_FixtureList20Clubs;
}
