
/*

	File:- DivisionFederationCupData.h

	Divison class


	Football Director

	(c) RW Software 1994 - 2004

  ---------------------------------------------------------------
 */

#ifndef __DIVISIONFEDERATIONCUPDATA_H__
#define __DIVISIONFEDERATIONCUPDATA_H__

class CDivisionFederationCupData
{
public:
	CDivisionFederationCupData(void);
	~CDivisionFederationCupData(void) {};

	void					DoInitialiseFromFile(FILE* File);

	// Variables
protected:
    ushort					m_ID;
    byte					m_NumberClubs;
};
#endif
