
 /*

    Class:- CDateDisplay


	(c) Rw Software 2003 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#include "DateDisplay.h"


/*------------------------------------------------------------------------------
	Method:   	CDateDisplay::CDateDisplay
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDateDisplay::CDateDisplay()
{
	m_rectDate = CRect(4, 12, 76, 200);
	m_Font.CreateFont(16, 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, 
					   ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
					   DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
}


/*------------------------------------------------------------------------------
	Method:   	CDateDisplay::~CDateDisplay
	Access:    	public 
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CDateDisplay::~CDateDisplay()
{
	m_Font.DeleteObject();
}


/*------------------------------------------------------------------------------
	Method:   	CDateDisplay::Draw
	Access:    	public 
	Parameter: 	CDC* _pdc
	Parameter: 	CString _strDate
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CDateDisplay::Draw(CDC* _pdc, CString _strDate)
{
	CFont* pfontCurrent = _pdc->SelectObject(&m_Font);
		
	m_rectDate.left -= 1;
	m_rectDate.top += 1;
	_pdc->SetTextColor(BLACK);
	_pdc->ExtTextOut(m_rectDate.left, m_rectDate.top, ETO_CLIPPED, m_rectDate, _strDate, _strDate.GetLength(), null);

	m_rectDate.left += 1;
	m_rectDate.top -= 1;
	_pdc->SetTextColor(WHITE);
	_pdc->ExtTextOut(m_rectDate.left, m_rectDate.top, ETO_CLIPPED, m_rectDate, _strDate, _strDate.GetLength(), null);
	_pdc->SelectObject(pfontCurrent);
}
