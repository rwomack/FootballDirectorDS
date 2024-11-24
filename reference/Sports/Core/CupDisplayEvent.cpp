
/*

    File:- CupDisplayEvent.cpp


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "CupDisplayEvent.h"
#include "CupRound.h"



/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEvent::CCupDisplayEvent
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupDisplayEvent::CCupDisplayEvent()
{
}


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEvent::CCupDisplayEvent
	Access:    	public 
	Parameter: 	const eCupEventType eType
	Parameter: 	CCupRound* pCupRound
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupDisplayEvent::CCupDisplayEvent(const eCupEventType eType, CCupRound* pCupRound)
{
	EXT_ASSERT_POINTER(pCupRound, CCupRound);
	m_pCupRound = pCupRound;
	m_eType = eType;
}


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEvent::GetType
	Access:    	public 
	Parameter: 	void
	Returns:   	eCupEventType
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
eCupEventType CCupDisplayEvent::GetType()
{ 
	return m_eType; 
}


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEvent::GetCupRound
	Access:    	public 
	Parameter: 	void
	Returns:   	CCupRound*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupRound* CCupDisplayEvent::GetCupRound()
{ 
	return m_pCupRound; 
}


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEvent::TitleStr
	Access:    	public 
	Returns:   	CString
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CString CCupDisplayEvent::TitleStr()
{
	CString strPrint;
	switch (GetType())
	{
		case DRAWDATE:
			strPrint = m_pCupRound->DrawTitleStr();
			break;
		case MATCHDATE:
			strPrint = m_pCupRound->GetTitle();
			break;
		case REPLAYDATE:
			strPrint = m_pCupRound->GetReplayTitle();
			break;
		default:
			EXT_ASSERT(false);
			break;
	}
	return strPrint;
}

/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEventList::~CCupDisplayEventList
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupDisplayEventList::~CCupDisplayEventList()
{
	DeleteAll();
}


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEventList::AddNew
	Access:    	public 
	Parameter: 	const eCupEventType _eType
	Parameter: 	CCupRound * _pCupRound
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupDisplayEventList::AddNew(const eCupEventType _eType, CCupRound* _pCupRound)
{
	EXT_ASSERT_POINTER(_pCupRound, CCupRound);
    m_List.Add(new CCupDisplayEvent(_eType, _pCupRound));
}


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEventList::DeleteAll
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCupDisplayEventList::DeleteAll()
{
	for (uint ListPos = 0; ListPos < m_List.GetSize(); ListPos++)
	{
        delete(CCupDisplayEvent*) m_List.GetAt(ListPos);
	}
	m_List.RemoveAll();
}


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEventList::GetSize
	Access:    	public 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CCupDisplayEventList::GetSize()
{ 
	return m_List.GetSize(); 
}


/*------------------------------------------------------------------------------
	Method:   	CCupDisplayEventList::operator[]
	Access:    	public 
	Parameter: 	const uint _ID
	Returns:   	CCupDisplayEvent&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CCupDisplayEvent& CCupDisplayEventList::operator[](const uint _ID)	
{ 
	return *(m_List.GetAt(_ID)); 
}
