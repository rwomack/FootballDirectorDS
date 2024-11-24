
/*

	ListStatic.h : main header file for the ListStatic gadget


	(c) RW Software 2004

  ---------------------------------------------------------------------------------------
 */


#ifndef __LISTSTATIC_H__
#define __LISTSTATIC_H__


class CExtStatic;
class CListStatic;
class CContractDateListStatic;
class CContractEndTime;

/////////////////////////////////////////////////////////////////////////////

class CListStatic : public CExtStatic
{
public:
	CListStatic();

	CStringArray&			GetStringList();
	void					DoIncrement();
	void					DoDecrement();
	void  SetCurSel(const ushort _nCurSel);
	const ushort			GetCurSel() const;
	void  AddString(CString _strText);

	// Implementation
protected:
	ushort					m_nCurSel;
	CStringArray			m_strList;
};
/////////////////////////////////////////////////////////////////////////////

class CContractDateListStatic : public CListStatic
{
public:
	CContractDateListStatic() 
		: CListStatic()	
	  {
	  }

	void  DoAddDates(const ushort _SeasonsToAddFirst = 1, const ushort _nHowMany = 5, const ushort _nCursel = 3);
	const ushort GetNumberOfSeasons() const;

protected:
	ushort					m_nNumberSeasonsFirst;
};
#endif