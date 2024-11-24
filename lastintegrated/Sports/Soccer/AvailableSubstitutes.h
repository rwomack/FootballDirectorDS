
/*

	File:- AvailableSubstitutes.h

	C++ Classes

	Available Substitutes class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/

#ifndef __AVAILABLESUBSTITUTES_H__
#define __AVAILABLESUBSTITUTES_H__

class CAvailableSubstitutes
{
public:

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	void					DoInitialiseFromResource(CBinaryReader& _Resource);
    byte					GetSubsSelect() const;
    byte					GetSubsUse() const;
	void					SetSubstitutes(const byte _Select, const byte _Use);

    // Variables
protected:
    byte					m_SubsSelect;
    byte					m_SubsUse;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif