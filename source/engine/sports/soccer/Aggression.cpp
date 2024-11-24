
 /*

	File:- Aggression.cpp

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CAggression::OnInitialise
	Access:    	public 
	Parameter: 	const byte _MinNumToBook
	Parameter: 	const byte _RandomNumToBook
	Parameter: 	const byte _MinNumToSendOff
	Parameter: 	const byte _RandomNumToSendOff
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CAggression::OnInitialise(const byte _MinNumToBook, const byte _RandomNumToBook, const byte _MinNumToSendOff, const byte _RandomNumToSendOff)
{
	EXT_ASSERT(_MinNumToBook + _RandomNumToBook <= PLAYERSINTEAM);
    m_NumberToBook.DoInitialise(_MinNumToBook, _RandomNumToBook);
	EXT_ASSERT(_MinNumToSendOff + _RandomNumToSendOff <= PLAYERSINTEAM);
    m_NumberToSendOff.DoInitialise(_MinNumToSendOff, _RandomNumToSendOff);
}


/*------------------------------------------------------------------------------
	Method:   	CAggression::HowManyPlayersToBook
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CAggression::HowManyPlayersToBook()
{
	byte CalcHowManyPlayersToBook = m_NumberToBook.RandomAmount();
	EXT_ASSERT(CalcHowManyPlayersToBook <= PLAYERSINTEAM);
	return CalcHowManyPlayersToBook;
}


/*------------------------------------------------------------------------------
	Method:   	CAggression::HowManyPlayersToSendOff
	Access:    	public 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
byte CAggression::HowManyPlayersToSendOff()
{
	byte CalcHowManyPlayersToSendOff = m_NumberToSendOff.RandomAmount();
	EXT_ASSERT(CalcHowManyPlayersToSendOff <= PLAYERSINTEAM);
	return CalcHowManyPlayersToSendOff;
}


/*------------------------------------------------------------------------------
	Method:   	CAggressionList::CAggressionList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CAggressionList::CAggressionList()
{
    //                         Book  So
    m_List[0].OnInitialise(0, 1, 0, 0);      
    m_List[1].OnInitialise(1, 1, 0, 0);      
    m_List[2].OnInitialise(1, 1, 0, 0);      
    m_List[3].OnInitialise(1, 2, 0, 0);      
    m_List[4].OnInitialise(1, 3, 0, 1);      
    m_List[5].OnInitialise(2, 1, 0, 1);      
    m_List[6].OnInitialise(2, 2, 0, 1);      
    m_List[7].OnInitialise(2, 2, 1, 1);      
    m_List[8].OnInitialise(3, 1, 1, 2);      
    m_List[9].OnInitialise(3, 3, 1, 2);      
}


/*------------------------------------------------------------------------------
	Method:   	CAggressionList::GetAt
	Access:    	public 
	Parameter: 	const ushort _ElementNumber
	Returns:   	CAggression&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CAggression& CAggressionList::GetAt(const ushort _ElementNumber)
{
	EXT_ASSERT(_ElementNumber < 10);
	return m_List[_ElementNumber];
}
