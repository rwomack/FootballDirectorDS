
/*

    File:- CupDisplayEvent.h


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __CUPDISPLAYEVENT_H

#define __CUPDISPLAYEVENT_H



class CCupRound;

class CCupDisplayEvent
{
public:
	CCupDisplayEvent(const eCupEventType _eType, CCupRound* _pCupRound);
	CCupDisplayEvent();

	eCupEventType			GetType();
	CCupRound*				GetCupRound();
	CString					TitleStr();

private:
	eCupEventType			m_eType;
	CCupRound*				m_pCupRound;
};
/////////////////////////////////////////////////////////////////////////////


class CCupDisplayEventList
{
public:
	~CCupDisplayEventList();

	void					AddNew(const eCupEventType _eType, CCupRound* _pCupRound);
	void					DeleteAll();
	int						GetSize();
	CCupDisplayEvent&		operator[](const uint _ID);

protected:
	Vector<CCupDisplayEvent*> m_List;
};

#endif
