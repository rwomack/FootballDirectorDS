
/*

    File:- Screen08Display.h


    Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef __SC8D_H__
#define __SC8D_H__

	enum eDataType
	{
		HEADER,
		MANAGERCONTRACT,
		EMPLOYEECONTRACT,
		PLAYERCONTRACT,
		YOUTHCONTRACT
	};


class CScreen08Display
{
public:

    // Constructor
    CScreen08Display() {};
    CScreen08Display(CString name,
					 CString position,
					 CString rating,
					 int fitness,
					 int age,
					 CString wage,
					 CString contract,
					 eDataType type,
					 CPlayer* pPlayer,
					 int index,
					 COLORREF ForeColor);

    // Variables
	CString					m_Name;
	CString					m_Position;
	CString					m_Rating;
	int						m_Fitness;
	int						m_Age;
	CString					m_Wage;
	CString					m_Contract;
    eDataType				m_Type;
	CPlayer*				m_pPlayer;
	int						m_Index;
	COLORREF				m_ForeColor;
};
/////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------
class CScreen08DisplayList
{
    public:

	// Constructor
        CScreen08DisplayList() {};

	void					AddNew(CString name,
									CString position,
									CString rating,
									int fitness,
									int age,
									CString wage,
									CString contract,
									eDataType type,
									CPlayer* pPlayer,
									int index,
									COLORREF ForeColor = BLACK);
	void					DeleteAll();
	int						GetSize()									{ return m_List.GetSize(); }
	CScreen08Display*		GetAt(int Id)								{ return m_List.GetAt(Id); }

protected:
	Vector<CScreen08Display*> m_List;
};
#endif
