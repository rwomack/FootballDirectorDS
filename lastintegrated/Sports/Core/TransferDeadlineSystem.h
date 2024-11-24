
/*
	File:- TransferDeadlineSystem.h


	Football Director

	(c) Sports Director Ltd 2008

  ---------------------------------------------------------------------------------------
*/


#ifndef __TRANSFERDEADLINESYSTEM_H__
    #define __TRANSFERDEADLINESYSTEM_H__


class CTransferDeadlineItem
{
public:
	// Constructor
	CTransferDeadlineItem(void);
	~CTransferDeadlineItem(void) {};

	bool					ClosesToday(const byte _WeekNumber);
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	byte					GetType();
	bool					IsOpen(const byte _WeekNumber);
	bool					OpensToday(const byte _WeekNumber);

protected:
	byte					m_Type;
	byte					m_OpenWeekNumber;
	byte					m_CloseWeekNumber;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CTransferDeadline
{
public:
	// Constructor
	CTransferDeadline(void) {};
	~CTransferDeadline(void) {};

	bool 					CanBuyPlayer();
	bool 					CanLoanPlayer();
	void					DoAnnouncements(CActiveCountry& _ActiveCountry);
	void					DoInitialiseFromResource(CBinaryReader& _Resource);

protected:
	ushort					m_TransferDeadlineItemID[4];
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CTransferDeadlineSystem : public CExtArray
{
public:
	// Constructor
	CTransferDeadlineSystem(void) {};
	~CTransferDeadlineSystem(void);

	CTransferDeadline&		operator [](const ushort nIndex);

	void					DoInitialise();
	CTransferDeadline&		GetAt(const ushort _ElementNumber);
	CTransferDeadlineItem&	GetItem(ushort _ElementNumber);

protected:
	CTransferDeadline*		m_TransferDeadlineList;
	ushort					m_HowManyItems;
	CTransferDeadlineItem*	m_TransferDeadlineItemList;
};
#endif