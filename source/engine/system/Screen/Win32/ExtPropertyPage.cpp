
#include "stdafx.h"

#include "voimage.h"
#include "OffScreenBuffer.h"
#include "DateDisplay.h"
#include "PNGDrawInfo.h"
#include "PNGButton.h"
#include "BasicDisplay.h"
#include "ExtPropertyPage.h"
#include "FootballDirector.h"


BEGIN_MESSAGE_MAP(CBitmapPropertyPage, CPropertyPage)
	ON_WM_HELPINFO()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_GETDLGCODE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


/*------------------------------------------------------------------------------
Method:   	CBitmapPropertyPage::OnHelpInfo
Access:    	public 
Parameter: 	HELPINFO * pHelpInfo
Returns:   	BOOL
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapPropertyPage::OnHelpInfo(HELPINFO* _HelpInfo)
{
	return GetBasicDisplay().OnHelpInfo(_HelpInfo);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::OnEraseBkgnd
	Access:    	public 
	Parameter: 	CDC * pDC
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapPropertyPage::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::OnPaint
	Access:    	public 
	Parameter: 	bool _ShouldBlit
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapPropertyPage::OnPaint(bool _ShouldBlit)
{
	GetBasicDisplay().OnPaint(true, _ShouldBlit);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::OnInitDialog
	Access:    	public 
	Parameter: 	const CString _strHelpTopic
	Parameter: 	CRect & _Rect
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CBitmapPropertyPage::OnInitDialog(const CString _strHelpTopic)
{
#ifdef POCKETPC2002
	ModifyStyle(0, WS_NONAVDONEBUTTON, 0);
#endif
	GetBasicDisplay().OnInitDialog(this, _strHelpTopic, false, BLACK);
	return CPropertyPage::OnInitDialog();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::OnInitRedrawArea
	Access:    	virtual public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapPropertyPage::OnInitRedrawArea()
{
	GetBasicDisplay().OnInitRedrawArea(this);
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::ReDraw
	Access:    	virtual public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapPropertyPage::ReDraw()
{
	Invalidate();
	UpdateWindow();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::GetMemDC
	Access:    	public 
	Returns:   	COffScreenBuffer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COffScreenBuffer* CBitmapPropertyPage::GetMemDC()
{
	return GetBasicDisplay().GetOffScreenBuffer();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::ResetDataRect
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBitmapPropertyPage::ResetDataRect()
{
	GetBasicDisplay().ResetDataRect();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::GetDataRect
	Access:    	public 
	Returns:   	CRect&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CRect& CBitmapPropertyPage::GetDataRect()
{
	return GetBasicDisplay().GetDataRect();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::GetBasicDisplay
	Access:    	public 
	Returns:   	CBasicDisplay&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay& CBitmapPropertyPage::GetBasicDisplay()
{
	return theApp.GetBasicDisplay();
}


/*------------------------------------------------------------------------------
	Method:   	CBitmapPropertyPage::OnGetDlgCode
	Access:    	public 
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CBitmapPropertyPage::OnGetDlgCode()
{
	return DLGC_WANTARROWS; // LLKEYS;
}
