
// File:- Screen8Display.cpp

 // Used when a club details are stored for historic reasons
 // eg When wins  a cup

//---------------------------------------------------------------------------------------


#include "stdafx.h"

#include "Screen08Display.h"


// Constructor
//---------------------------------------------------------------------------------------
CScreen08Display::CScreen08Display(CString name, CString position, CString rating, int fitness, int age, CString wage, CString contract, eDataType type, CPlayer* pPlayer, int index, COLORREF	ForeColor)
{
	m_Name = name;
	m_Position = position;
	m_Rating = rating;
	m_Fitness = fitness;
	m_Age = age;
	m_Wage = wage;
	m_Contract = contract;
	m_pPlayer = pPlayer;
	m_Type = type;
	m_Index = index;
	m_ForeColor = ForeColor;
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


 //---------------------------------------------------------------------------------------
void CScreen08DisplayList::AddNew(CString name,
								  CString position,
								  CString rating,
								  int fitness,
								  int age,
								  CString wage,
								  CString contract,
								  eDataType type,
								  CPlayer* pPlayer,
								  int index,
								  COLORREF ForeColor)
{
    m_List.Add(new CScreen08Display(name, position, rating, fitness, age, wage, contract, type, pPlayer, index, ForeColor));
}


 //---------------------------------------------------------------------------------------
void CScreen08DisplayList::DeleteAll()
{
	for (uint ListPos = 0; ListPos < m_List.GetSize(); ListPos ++)
	{
        delete(CScreen08Display*) m_List.GetAt(ListPos);
	}
	m_List.removeElementAll();
}
