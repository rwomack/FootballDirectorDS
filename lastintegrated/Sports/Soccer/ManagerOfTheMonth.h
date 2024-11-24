
/*

	File:- ManagerOfTheMonth.h


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#ifndef __MOM_H__
#define __MOM_H__


class CManagerOfTheMonth
{
public:
    // Constructor
    CManagerOfTheMonth();
    CManagerOfTheMonth(CClub* _Club, CString _Month);

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	CString					ClubStr();
	void					DoCreate(CClub* _Club, CString _Month);
	void					DoInitialise();
	ushort					GetClubID();
	bool					IsEmpty();
	CString					ManagerStr();
	CString					MonthStr();

    // Variables
private :
    CExtString              m_Month;
    CPersonName             m_ManagerNameID;
    CStringID				m_ClubNameID;
};
#endif
