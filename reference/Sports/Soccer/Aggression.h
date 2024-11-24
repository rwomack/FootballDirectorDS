
/*

	File:- Aggression.h

	Aggression class


	Football Director

	(c) Rw Software 1994 - 2008

	---------------------------------------------------------------------------------------
*/


  // Aggression 0 - 9

#ifndef __AGGRESS_H__
#define __AGGRESS_H__


class CAggression 
{
public:
    // Methods
	void					OnInitialise(const byte bNumToBook, const byte rNumToBook, const byte bNumToSendOff, const byte rNumToSendOff);
	byte HowManyPlayersToBook();
	byte HowManyPlayersToSendOff();

    // Variables
protected:
   CRangedValue<byte>		m_NumberToBook;
   CRangedValue<byte>		m_NumberToSendOff;
};

//----------------------------------------------------------------------------
class CAggressionList
{
public:
    CAggressionList();				// Constructor

	CAggression& GetAt(const ushort _ElementNumber);

	// Variables
private:
	CAggression				m_List[10];
};
#endif
