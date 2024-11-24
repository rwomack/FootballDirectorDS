
/*

	PlayerNameTextRegion.cpp: implementation for the NameTextRegion classes.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"
#include "PlayerNameTextRegion.h"


/*------------------------------------------------------------------------------
	Method:   	CPlayerNameTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CPlayer & _Player
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerNameTextRegion::DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CPlayer& _Player)
{
	CString strPrint;
	strPrint.Format(_T("%s, %s"), _Player.GetName(), _Player.GetSkills().PositionStr());
	CNameTextRegion::DoInitialise(_YPos, _Wnd, _BasicDisplay, strPrint);
	CNameTextRegion::GetTextRegion()->SetTextColour(PosnColour[_Player.GetSkills().GetPosition()]);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CYouthNameTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CYouth & _Youth
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CYouthNameTextRegion::DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CYouth& _Youth)
{
	CString strPrint;
	strPrint.Format(_T("%s, %s"), _Youth.GetName(), _Youth.GetSkills().PositionStr());
	CNameTextRegion::DoInitialise(_YPos, _Wnd, _BasicDisplay, strPrint);
	CNameTextRegion::GetTextRegion()->SetTextColour(PosnColour[_Youth.GetSkills().GetPosition()]);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CEmployeeNameTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CEmployee & _Employee
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CEmployeeNameTextRegion::DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CEmployee& _Employee)
{
	CString strPrint;
	strPrint.Format(_T("%s, %s"), _Employee.GetName(), _Employee.TypeTextStr());
	CNameTextRegion::DoInitialise(_YPos, _Wnd, _BasicDisplay, strPrint);
	CNameTextRegion::GetTextRegion()->SetTextColour(PosnColour[_Employee.GetType()]);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CClubNameTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CClub & _Club
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubNameTextRegion::DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CClub& _Club)
{
	CNameTextRegion::DoInitialise(_YPos, _Wnd, _BasicDisplay, _Club.GetName());
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CClubBalanceTextRegion::CClubBalanceTextRegion
	Access:    	public 
	Returns:   	
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CClubBalanceTextRegion::CClubBalanceTextRegion()
{
	m_Club = NULL;
	m_TextRegion = NULL;
	m_Abbreviated = false;
}


/*------------------------------------------------------------------------------
	Method:   	CClubBalanceTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CClub * _Club
	Parameter: 	const int _Style
	Parameter: 	const bool _Abbreviated
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubBalanceTextRegion::DoInitialise(CBasicDisplay* _BasicDisplay, CClub* _Club, const int _Style, const bool _Abbreviated /*= false*/)
{
	EXT_ASSERT_POINTER(_Club, CClub);
	m_Club = _Club;
	m_Abbreviated = _Abbreviated;
	m_TextRegion = new CTextRegion;
	EXT_ASSERT_POINTER(m_TextRegion, CTextRegion);
	m_TextRegion->DoInitialise(_BasicDisplay, _T(""), _Style);
	DoUpdate();
	_BasicDisplay->TextRegionList().Add(m_TextRegion);		// destructor will delete the instance
}


/*------------------------------------------------------------------------------
	Method:   	CClubBalanceTextRegion::DoUpdate
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubBalanceTextRegion::DoUpdate()
{
	CString str, str1;
	if (m_Abbreviated == true)
	{
		str1.LoadString(IDS_CLUBBAL);
	}
	else
	{
		str1.LoadString(IDS_CLUBBALANCE);
	}
	str.Format(str1, m_Club->GetCashStr());
	EXT_ASSERT_POINTER(m_TextRegion, CTextRegion);
	m_TextRegion->SetText(str);
}


/*------------------------------------------------------------------------------
	Method:   	CClubBalanceTextRegion::MoveWindow
	Access:    	public 
	Parameter: 	CRect _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubBalanceTextRegion::MoveWindow(CRect _Rect)
{
	EXT_ASSERT_POINTER(m_TextRegion, CTextRegion);
	m_TextRegion->MoveWindow(_Rect);
}


/*------------------------------------------------------------------------------
	Method:   	CClubBalanceTextRegion::SetFont
	Access:    	public 
	Parameter: 	CFont * _Font
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CClubBalanceTextRegion::SetFont(CFont* _Font)
{
	m_TextRegion->SetFont(_Font);
}


/*------------------------------------------------------------------------------
	Method:   	CClubBalanceTextRegion::SetTextColour
	Access:    	public 
	Parameter: 	COLORREF _Colour
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CClubBalanceTextRegion::SetTextColour(COLORREF _Colour)
{
	EXT_ASSERT_POINTER(m_TextRegion, CTextRegion);
	return m_TextRegion->SetTextColour(_Colour);
}
/////////////////////////////////////////////////////////////////////////////


/*------------------------------------------------------------------------------
	Method:   	CNameTextRegion::CNameTextRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNameTextRegion::CNameTextRegion(void)
	: m_TextRegion(NULL)
{
	m_Font.CreateFont(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0,
		ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
}


/*------------------------------------------------------------------------------
	Method:   	CNameTextRegion::~CNameTextRegion
	Access:    	public 
	Parameter: 	void
	Returns:   	
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CNameTextRegion::~CNameTextRegion(void)
{
	m_Font.DeleteObject();
}


/*------------------------------------------------------------------------------
	Method:   	CNameTextRegion::DoInitialise
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	CWnd * _Wnd
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	CString _Name
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNameTextRegion::DoInitialise(int _YPos, CWnd* _Wnd, CBasicDisplay* _BasicDisplay, CString _Name)
{
	m_YPos = _YPos;
	EXT_ASSERT(m_TextRegion == NULL);
	m_TextRegion = new CTextRegion;
	EXT_ASSERT_POINTER(_Wnd, CWnd);
	m_TextRegion->SetParentWnd(_Wnd);
	EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	m_TextRegion->DoInitialise(_BasicDisplay, _Name, COL_ALIGNC);
	m_NameTextID = _BasicDisplay->DoAddTextRegion(GetTextRegion());
	m_TextRegion->SetFont(&m_Font);
}


/*------------------------------------------------------------------------------
	Method:   	CNameTextRegion::GetTextRegion
	Access:    	public 
	Returns:   	CTextRegion*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTextRegion* CNameTextRegion::GetTextRegion()
{
	EXT_ASSERT_POINTER(m_TextRegion, CTextRegion);
	return m_TextRegion;
}


/*------------------------------------------------------------------------------
	Method:   	CNameTextRegion::MoveWindow
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CNameTextRegion::MoveWindow()
{
	CRect theWindow;
	EXT_ASSERT_POINTER(m_TextRegion, CTextRegion);
	m_TextRegion->GetParentWnd()->GetClientRect(&theWindow);
	m_TextRegion->MoveWindow(CRect(0, m_YPos, theWindow.right, m_YPos + 22));
}
