
/*

	BitmapFormView.cpp : implementation file

	(c) Rw Software 1994 - 2007

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "voimage.h"
#include "OffScreenBuffer.h"
#include "DateDisplay.h"
#include "PNGDrawInfo.h"
#include "PNGButton.h"
#include "BasicDisplay.h"
#include "BitmapFormView.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



BEGIN_MESSAGE_MAP(CBitmapFormView, CFormView)
	ON_WM_HELPINFO()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_GETDLGCODE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::CBitmapFormView
Access:    	public
Parameter: 	UINT nIDTemplate
Returns:
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
CBitmapFormView::CBitmapFormView(UINT nIDTemplate)
	: CFormView(nIDTemplate)
	, m_BasicDisplay(NULL)
{
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::Isinitialised
	Access:    	public
	Returns:   	bool
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
bool CBitmapFormView::Isinitialised()
{
	if (m_BasicDisplay == NULL)
	{
		return false;
	}
	return true;
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::GetCentreX
Access:    	public
Returns:   	int
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
int CBitmapFormView::GetCentreX()
{
	CRect rectWnd;
	GetClientRect(&rectWnd);
	return rectWnd.CenterPoint().x;
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnLButtonDown
Access:    	protected
Parameter: 	UINT nFlags
Parameter: 	CPoint point
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnLButtonDown(UINT nFlags, CPoint point)
{
	Default();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::DoInitialiseControlList
Access:    	protected
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::DoInitialiseControlList()
{
	GetBasicDisplay().ButtonList().RemoveList();
	for (int LoopCount = 0; LoopCount < m_listButtons.GetSize(); LoopCount++)
	{
		GetBasicDisplay().ButtonList().Add(m_listButtons.GetAt(LoopCount));
	}
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnHelpInfo
Access:    	public
Parameter: 	HELPINFO * _HelpInfo
Returns:   	BOOL
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapFormView::OnHelpInfo(HELPINFO* _HelpInfo)
{
	return GetBasicDisplay().OnHelpInfo(_HelpInfo);
}

/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnEraseBkgnd
Access:    	public
Parameter: 	CDC * pDC
Returns:   	BOOL
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapFormView::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::DoLoadBitmap
Access:    	public
Parameter: 	const UINT nIDResource
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::DoLoadBitmap(const UINT nIDResource)
{
	HDC dc = ::GetDC(m_hWnd);
	GetBasicDisplay().GetBitmap().SetBitmap(dc, nIDResource, TEXT("PNG"));
	::ReleaseDC(m_hWnd, dc);
	Invalidate();
	UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::OnInitialUpdate
	Access:    	public
	Parameter: 	const CString _strHelpTopic
	Parameter: 	CBasicDisplay * _BasicDisplay
	Parameter: 	bool _ColourBackGround
	Parameter: 	COLORREF _Colour
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnInitialUpdate(const CString _strHelpTopic, CBasicDisplay* _BasicDisplay, int _StrID, bool _ColourBackGround /*= false*/, COLORREF _Colour /*= 0x00000000*/)
{
  FDASSERT_POINTER(_BasicDisplay, CBasicDisplay);
	m_BasicDisplay = _BasicDisplay;
	GetBasicDisplay().DoRemoveButtonList();

	GetBasicDisplay().SetDisplayHeader(true, _StrID);
#ifdef POCKETPC2002
	ModifyStyle(0, WS_NONAVDONEBUTTON, 0);
#endif
	ModifyStyle(WS_HSCROLL | WS_VSCROLL, null);
	GetBasicDisplay().OnInitDialog(this, _strHelpTopic, _ColourBackGround, _Colour);
	CFormView::OnInitialUpdate();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::GetBasicDisplay
Access:    	public
Returns:   	CBasicDisplay&
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay& CBitmapFormView::GetBasicDisplay()
{
  FDASSERT_POINTER(m_BasicDisplay, CBasicDisplay);
	return *m_BasicDisplay;
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::ReDraw
Access:    	public
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::ReDraw()
{
	Invalidate();
	UpdateWindow();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::ResetDataRect
Access:    	public
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::ResetDataRect()
{
	GetBasicDisplay().ResetDataRect();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::GetDataRect
Access:    	public
Returns:   	CRect&
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
CRect& CBitmapFormView::GetDataRect()
{
	return GetBasicDisplay().GetDataRect();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnInitRedrawArea
Access:    	public
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnInitRedrawArea()
{
	GetBasicDisplay().OnInitRedrawArea(this);
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::GetMemDC
Access:    	public
Returns:   	COffScreenBuffer*
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
COffScreenBuffer* CBitmapFormView::GetMemDC()
{
	return GetBasicDisplay().GetOffScreenBuffer();
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnGetDlgCode
Access:    	public
Returns:   	uint
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
uint CBitmapFormView::OnGetDlgCode()
{
	return DLGC_WANTARROWS; // LLKEYS;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::OnSize
	Access:    	virtual public
	Parameter: 	UINT nType
	Parameter: 	int cx
	Parameter: 	int cy
	Returns:   	void
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnSize(UINT nType, int cx, int cy)
{
	CRect theRect;
	GetClientRect(&theRect);
	if (m_BasicDisplay  != NULL && theRect.bottom != 0)
	{
		GetBasicDisplay().OnSize(this);
	}
	CFormView::OnSize(nType, cx, cy);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapFormView::ButtonList
	Access:    	public
	Returns:   	CPointerArray&
	Qualifier:
	Purpose:
------------------------------------------------------------------------------*/
CPointerArray& CBitmapFormView::ButtonList()
{
	return m_listButtons;
}


/*------------------------------------------------------------------------------
Method:   	CBitmapFormView::OnSetFocus
Access:    	protected
Parameter: 	CWnd * _OldWnd
Returns:   	void
Qualifier:
Purpose:
------------------------------------------------------------------------------*/
void CBitmapFormView::OnSetFocus(CWnd* _OldWnd)
{
	GetBasicDisplay().OnInitDialog(this, _T(""), false, BLACK);
//		GetOffScreenBuffer()->SetParentWnd(this);
//	GetBasicDisplay().OnEraseBkgnd();
	DoInitialiseControlList();
	CFormView::OnSetFocus(_OldWnd);
}
