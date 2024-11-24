

/*

    DateDisplay.h: interface for the CDateDisplay class.


	(c) RW Software 2004

  ---------------------------------------------------------------
 */



#if !defined(AFX_DATEDISPLAY_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_DATEDISPLAY_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_


class CDateDisplay
{
public:
	CDateDisplay();
	~CDateDisplay();

	void					Draw(CDC* _pdc, CString _strDate);

protected:
	CFont					m_Font;
	CRect					m_rectDate;
};
#endif