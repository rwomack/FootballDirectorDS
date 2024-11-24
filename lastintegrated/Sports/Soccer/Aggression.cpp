
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
	Parameter: 	const byte bNumToBook
	Parameter: 	const byte rNumToBook
	Parameter: 	const byte bNumToSendOff
	Parameter: 	const byte rNumToSendOff
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CAggression::OnInitialise(const byte bNumToBook, const byte rNumToBook, const byte bNumToSendOff, const byte rNumToSendOff)
{
    m_NumberToBook.DoInitialise(bNumToBook, rNumToBook);
    m_NumberToSendOff.DoInitialise(bNumToSendOff, rNumToSendOff);
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
	return m_NumberToBook.RandomAmount();
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
	return m_NumberToSendOff.RandomAmount();
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
	return m_List[_ElementNumber];
}
