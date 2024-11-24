
/*
	File:- DivisionFederationCupData.cpp

	Football Director

	(c) RW Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"


#ifdef ACTIVEFEDERATIONCUPS
/*------------------------------------------------------------------------------
	Method:   	CDivisionFederationCupData::CDivisionFederationCupData
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 
	Purpose:
------------------------------------------------------------------------------*/
CDivisionFederationCupData::CDivisionFederationCupData(void)
	: m_ID(wNOTFOUND)
    , m_NumberClubs(0)
{
}


/*------------------------------------------------------------------------------
	Method:   	CDivisionFederationCupData::DoInitialiseFromFile
	Access:    	public 
	Parameter: 	CDataFile* _pDataFile
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDivisionFederationCupData::DoInitialiseFromFile(CDataFile* _pDataFile)
{
    _pDataFile->Read(&m_ID, sizeof(m_ID));
    _pDataFile->Read(&m_NumberClubs, sizeof(m_NumberClubs));
}
#endif
///////////////////////////////////////////////////////////////////////////////////////////////
