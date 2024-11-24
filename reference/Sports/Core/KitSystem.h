
/*
	File:- KitSystem.h


	Football Director

	(c) Sports Director Ltd 2008

  ---------------------------------------------------------------------------------------
*/


#ifndef __KITSYSTEM_H__
    #define __KITSYSTEM_H__


	const int MAXKITS = 34;

class CKit
{
public:
	// Constructor
	CKit(void) {};
	~CKit(void) {};

	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	byte					GetID();
	uint					GetPrimaryColour();
	uint					GetSecondaryColour();

protected:
	byte					m_ID;
	COLORREF				m_PrimaryColour;
	COLORREF				m_SecondaryColour;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CKitSystem : public CExtArray
{
public:
	// Constructor
	CKitSystem(void) {};
	~CKitSystem(void);

	CKit&					operator [](const ushort _Value);

	void					DoInitialise();

protected:
	CKit*					m_KitList;
	ushort					m_HowManyItems;
};
#endif