
/*

    Class:- CBasicDisplay


	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/


#include "stdafx.h"

#include "UI.h"

#ifndef TEST_FRAMEWORK
#include "FootballDirector.h"
#endif


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::CBasicDisplay
	Access:    	public 
	Returns:   	
	Qualifier: 	: m_pMemDC(null)
	Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay::CBasicDisplay()
	: m_pMemDC(null)
	, m_pfontCurrent(null)
{
	m_SmallFont.CreateFont(12, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET,  OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	m_NormalFont.CreateFont(15, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	m_BigFont.CreateFont(18, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET,  OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	m_BoldFont.CreateFont(16, 0, 0, 0, FW_SEMIBOLD, FALSE, FALSE, 0, ANSI_CHARSET,  OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
	m_HeaderFont.CreateFont(22, 0, 0, 0, FW_SEMIBOLD, TRUE, FALSE, 0, ANSI_CHARSET,  OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, TEXT("Arial"));
}


/*------------------------------------------------------------------------------
Method:   	CBasicDisplay::~CBasicDisplay
Access:    	public 
Returns:   	
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
CBasicDisplay::~CBasicDisplay()
{
	delete m_pMemDC;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoAddButtons
	Access:    	public 
	Parameter: 	CButtonInfo * _Buttons
	Parameter: 	CWnd * _pParentWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::DoAddButtons(uint* _ResourceID, CWnd* _pParentWnd)
{
	while (*_ResourceID != NOTFOUND)
	{
		DoAddButton(*_ResourceID, _pParentWnd);
		_ResourceID++;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::AddButton
	Access:    	public 
	Parameter: 	CPNGButton * _Button
	Parameter: 	uint _ResourceID
	Parameter: 	CWnd * _pParentWnd
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::DoAddButton(uint _ResourceID, CWnd* _pParentWnd)
{
	int ButtonID = ButtonList().Add(new CPNGButton());
	GetButton(ButtonID)->DoSetDrawInfo(_ResourceID, _pParentWnd);
	return ButtonID;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetButtonStatus
	Access:    	public 
	Parameter: 	uint _ButtonID
	Parameter: 	ePNGButtonStatus _Status
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetButtonStatus(uint _ButtonID, ePNGButtonStatus _Status)
{
	GetButton(_ButtonID)->SetStatus(_Status);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoMoveButton
	Access:    	public 
	Parameter: 	uint _ButtonID
	Parameter: 	CRect & _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::DoMoveButton(uint _ButtonID, CRect& _Rect)
{
	GetButton(_ButtonID)->MoveWindow(_Rect);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetButton
	Access:    	public 
	Parameter: 	uint _ButtonID
	Returns:   	CPNGButton*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPNGButton* CBasicDisplay::GetButton(int _ButtonID)
{
	EXT_ASSERT(_ButtonID < ButtonList().GetSize());
	CPNGButton* pButton = (CPNGButton*)(ButtonList().GetAt(_ButtonID));
	EXT_ASSERT_POINTER(pButton, CPNGButton);
	return pButton;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::TextRegionList
	Access:    	public 
	Returns:   	CPointerArray<CTextRegion>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPointerArray<CTextRegion>& CBasicDisplay::TextRegionList()
{
	return m_listTextRegions;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoRemoveTextRegionList
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::DoRemoveTextRegionList()
{
	for (int n = 0; n < m_listTextRegions.GetSize(); n++)
	{
		DEBUG_OUTPUT(("Deleting :- %s"), GetTextRegion(n)->GetText());
	}
	m_listTextRegions.RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoAddTextRegion
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::DoAddTextRegion()
{
	int TextID = TextRegionList().Add(new CTextRegion());
	GetTextRegion(TextID)->SetBasicDisplay(this);
	return TextID;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoAddCentredTextRegion
	Access:    	public 
	Parameter: 	int _YPos
	Parameter: 	int _StringID
	Parameter: 	eFONTSIZE _FontSize
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::DoAddCentredTextRegion(int _YPos, int _StringID, eFONTSIZE _FontSize /*= NORMALFONT*/)
{
	int TextID = TextRegionList().Add(new CTextRegion());
	GetTextRegion(TextID)->DoInitialise(this, _StringID, COL_ALIGNC);
	GetTextRegion(TextID)->MoveWindow(CRect(0, _YPos, 2, _YPos + 16));
	return TextID;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoAddTextRegion
	Access:    	public 
	Parameter: 	CTextRegion * _TextRegion
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::DoAddTextRegion(CTextRegion* _TextRegion)
{
	EXT_ASSERT_POINTER(_TextRegion, CTextRegion);
	DEBUG_OUTPUT(("Added text region %d"), (_TextRegion));
	_TextRegion->SetBasicDisplay(this);
	return TextRegionList().Add(_TextRegion);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoAddTextRegions
	Access:    	public 
	Parameter: 	int _HowMany
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::DoAddTextRegions(int _HowMany)
{
	for (int LoopCount = 0; LoopCount < _HowMany; LoopCount++)
	{
		DoAddTextRegion();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoMoveTextRegion
	Access:    	public 
	Parameter: 	uint _TextRegionID
	Parameter: 	CRect & _Rect
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::DoMoveTextRegion(uint _TextRegionID, CRect& _Rect)
{
	GetTextRegion(_TextRegionID)->MoveWindow(_Rect);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetTextRegion
	Access:    	public 
	Parameter: 	int _TextRegionID
	Returns:   	CTextRegion*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CTextRegion* CBasicDisplay::GetTextRegion(int _TextRegionID)
{
	EXT_ASSERT(_TextRegionID < TextRegionList().GetSize());
	CTextRegion* pTextRegion = TextRegionList().GetAt(_TextRegionID);
	EXT_ASSERT_POINTER(pTextRegion, CTextRegion);
	return pTextRegion;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::RestoreFont
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::RestoreFont()
{
	if (m_pfontCurrent != null)
	{
	//	m_pMemDC->SelectObject(m_pfontCurrent);
		m_pfontCurrent = null;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetFont
	Access:    	public 
	Parameter: 	eFONTSIZE _Font
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetFont(eFONTSIZE _Font)
{
	if (_Font < MAXFONT)
	{
		CFont* Fonts[] = 
		{
			&m_NormalFont,
			&m_SmallFont,
			&m_BigFont,
			&m_BoldFont,
			&m_HeaderFont,
		};
		SetFont(*Fonts[_Font]);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetFont
	Access:    	public 
	Parameter: 	CFont & _Font
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetFont(CFont& _Font)
{
	if (m_pfontCurrent == null)
	{
		m_pfontCurrent = (CFont*)m_pMemDC->SelectObject(_Font);
	}
	else
	{
		m_pMemDC->SelectObject(_Font);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetFontBig
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetFontBig()
{
	SetFont(m_BigFont);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetFontBold
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetFontBold()
{
	SetFont(m_BoldFont);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetFontNormal
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetFontNormal()
{
	SetFont(m_NormalFont);
}


/*------------------------------------------------------------------------------
Method:   	CBasicDisplay::SetFontSmall
Access:    	public 
Returns:   	void
Qualifier: 	
Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetFontSmall()
{
	SetFont(m_SmallFont);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::OnInitDialog
	Access:    	public 
	Parameter: 	const CString _strHelpTopic
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::OnInitDialog(CWnd* _pParentWnd, const int _HelpTopic, const int _StrID)
{
	EXT_ASSERT_POINTER(_pParentWnd, CWnd);
	if (m_pngTitle.GetHBitmap() == 0)
	{
		m_pngTitle.SetBitmap(m_pMemDC->operator HDC(), IDR_PNGHEADER, TEXT("PNG"));
#ifdef DISPLAYDATE
		m_pngCalendar.SetBitmap(m_pMemDC->operator HDC(), IDR_PNGCALENDAR, TEXT("PNG"));
		m_pngDay.SetBitmap(m_pMemDC->operator HDC(), IDR_PNGDAY, TEXT("PNG"));
		m_pngMonth.SetBitmap(m_pMemDC->operator HDC(), IDR_PNGMONTH, TEXT("PNG"));
		m_pngYear.SetBitmap(m_pMemDC->operator HDC(), IDR_PNGYEAR, TEXT("PNG"));
#endif
		GetGrid().OnInitialUpdate(_pParentWnd, this);
		_pParentWnd->SetWindowText(TEXT("Football Director"));
	}
	if (m_pMemDC == null)
	{
		OnSize(_pParentWnd);
	}
	else
	{
		//@@OnEraseBkgnd(m_pMemDC);
	}
	
	m_DisplayHeader = true;
	m_DisplayBitmap = true;
	m_HelpTopicID = _HelpTopic;
	m_TextColour = BLACK;
	SetDisplayHeader(true, _StrID);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoRemoveAllData
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::DoRemoveAllData()
{
	DoRemoveButtonList();
	DoRemoveTextRegionList();
	GetGrid().RemoveAllData(true);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::DoRemoveButtonList
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::DoRemoveButtonList()
{
	m_listButtons.RemoveList();
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::OnHelpInfo
	Access:    	public 
	Parameter: 	HELPINFO * _HelpInfo
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CBasicDisplay::OnHelpInfo(HELPINFO* _HelpInfo)
{
#ifdef POCKETPC
	CString HelpText;
	HelpText.LoadString(m_HelpTopicID);
	CString strHelp = TEXT("file:FDHelp.htm#") + HelpText;
	CreateProcess(TEXT("peghelp.exe"), strHelp, null, null, FALSE, 0, null, null, null, null);
#endif

	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::OnEraseBkgnd
	Access:    	public 
	Parameter: 	CDC * _pDC
	Returns:   	BOOL
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
BOOL CBasicDisplay::OnEraseBkgnd(CDC* _pDC)
{
	EXT_ASSERT_POINTER(_pDC, CDC);

//	if (m_DisplayBitmap == true)
	{
		// Full screen background
		//m_pngTitle.Draw(m_pMemDC->operator HDC(), 0, 0);
	}
	//else
	{
		//CRect rc;
		//m_pMemDC->GetParentWnd()->GetClientRect(&rc);
		//_pDC->FillSolidRect(&rc, WHITE /*m_Colour*/);
	}
	return TRUE;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::OnSize
	Access:    	public 
	Parameter: 	CRect & _Rect
	Parameter: 	CWnd * _pParentWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::OnSize(CWnd* _pParentWnd)
{
	EXT_ASSERT_POINTER(_pParentWnd, CWnd);
	CRect rc;
	_pParentWnd->GetClientRect(&rc);
	if (rc.bottom > 0)
	{
		if (m_pMemDC != null)
		{
			if (rc.right != m_pMemDC->GetBufferWidth() || rc.bottom != m_pMemDC->GetBufferHeight())
			{
				delete m_pMemDC;
				m_pMemDC = null;
			}
		}
		if (m_pMemDC == null)
		{
			m_pMemDC = new COffScreenBuffer(_pParentWnd);
			EXT_ASSERT_POINTER(m_pMemDC, COffScreenBuffer);
			m_pMemDC->SetParentWnd(_pParentWnd);
		}
		OnEraseBkgnd(m_pMemDC);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetBitmap
	Access:    	public 
	Parameter: 	UINT _BitmapID
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetBitmap(UINT _BitmapID)
{
	m_pngTitle.SetBitmap(m_pMemDC->operator HDC(), _BitmapID, TEXT("PNG"));
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::OnLButtonDown
	Access:    	public 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::OnLButtonDown(UINT _Flags, CPoint _Point)
{
	int nRet = NOTFOUND;
	for (int LoopCount = 0; LoopCount < m_listButtons.GetSize(); LoopCount++)
	{
		if (m_listButtons.GetAt(LoopCount)->IsPointInRect(_Point, BUTTONSTATEDOWN) == true)
		{
			nRet = LoopCount;	// Return the button number
			break;
		}
	}
	return nRet;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::OnLButtonUp
	Access:    	public 
	Parameter: 	UINT _Flags
	Parameter: 	CPoint _Point
	Returns:   	const ushort
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::OnLButtonUp(UINT _Flags, CPoint _Point)
{
	int nRet = NOTFOUND;
	for (int LoopCount = 0; LoopCount < m_listButtons.GetSize(); LoopCount++)
	{
		if (m_listButtons.GetAt(LoopCount)->IsPointInRect(_Point, BUTTONSTATERELEASED) == true)
		{
			nRet = LoopCount;	// Return the button number
			break;
		}
	}
	return nRet;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetDisplayHeader
	Access:    	public 
	Parameter: 	bool _Value
	Parameter: 	CString & _Str
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetDisplayHeader(bool _Value, int _StrID /*= 0*/)
{
	m_DisplayHeader = _Value;
	if (_StrID > 0)
	{
		m_HeaderTitle.LoadString(_StrID);
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetDisplayBitmap
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetDisplayBitmap(bool _Value)
{
	m_DisplayBitmap = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::Draw
	Access:    	public 
	Parameter: 	const bool _bShowDate
	Parameter: 	const bool _bShouldBlit 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::OnPaint(const bool _bShowDate, const bool _bShouldBlit /*= false*/)
{
	if (m_DisplayBitmap == true)
	{
		m_pngTitle.Draw(m_pMemDC->operator HDC(), 0, 0);
	}
	OnPaintControls();

	m_pMemDC->SetBkMode(TRANSPARENT);
	if (_bShowDate == true)
	{
#ifdef DISPLAYDATE
 		m_pngCalendar.Draw(m_pMemDC->operator HDC(), 12, 0);
 		m_pngDay.Draw(m_pMemDC->operator HDC(), 23, 16, 20, 13, (theApp.GetDate().GetDay() - 1) * 20, 0);
 		m_pngMonth.Draw(m_pMemDC->operator HDC(), 23, 2, 20, 12, (theApp.GetDate().GetMonth() - 1) * 20, 0);
 		m_pngYear.Draw(m_pMemDC->operator HDC(), 23, 31, 20, 6, (theApp.GetDate().GetYear() - 2007) * 20, 0);
#endif
	}
	if (m_DisplayHeader == true)
	{
		CFont* pfontCurrent = m_pMemDC->SelectObject(&m_HeaderFont);
		CRect rect(0, 8, m_pMemDC->GetBufferWidth() - 2, 140);
		rect.left = (rect.right - 2) - m_pMemDC->GetTextExtent(m_HeaderTitle).cx;

		m_pMemDC->SetTextColor(BLACK);
		rect.left--;
		rect.top++;
		m_pMemDC->ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, m_HeaderTitle, m_HeaderTitle.GetLength(), null);
		m_pMemDC->SetTextColor(WHITE);
		rect.left++;
		rect.top--;
		m_pMemDC->ExtTextOut(rect.left, rect.top, ETO_CLIPPED, rect, m_HeaderTitle, m_HeaderTitle.GetLength(), null);
		RestoreFont();
	}
	if (_bShouldBlit == true)
	{
		m_pMemDC->Blit();
	}
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::OnPaintControls
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::OnPaintControls()
{
	for (int LoopCount = 0; LoopCount < m_listButtons.GetSize(); LoopCount++)
	{
		m_listButtons.GetAt(LoopCount)->OnPaint(m_pMemDC);
	}
	for (int LoopCount = 0; LoopCount < m_listTextRegions.GetSize(); LoopCount++)
	{
		DEBUG_OUTPUT(("Painted text region %s"), (m_listTextRegions.GetAt(LoopCount)->GetText()));
		m_listTextRegions.GetAt(LoopCount)->OnPaint();
	}
	m_Grid.OnPaint();
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::OnInitRedrawArea
	Access:    	public 
	Parameter: 	CWnd * _pParentWnd
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::OnInitRedrawArea(CWnd* _pParentWnd)
{	
	m_pMemDC->SetParentWnd(_pParentWnd);
	OnEraseBkgnd(m_pMemDC);
	m_pMemDC->SetTextColor(BLACK);
	m_pMemDC->SetBkMode(TRANSPARENT);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::SetFullScreen
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::SetFullScreen()
{
	/*CRect theRect(0, 0, 240, 320);
	m_OriginalDataRect = theRect;
	m_DataRect = m_OriginalDataRect;*/
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::PrintStringID
	Access:    	public 
	Parameter: 	uint _XPos
	Parameter: 	const uint _YPos
	Parameter: 	const int _StrID
	Parameter: 	COLORREF _Colour
	Parameter: 	eFONTSIZE _Font
	Parameter: 	eALIGNMENT _Align
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::PrintStringID(uint _XPos, const uint _YPos, const int _StrID, COLORREF _Colour /*= TRANSPARENT*/, eFONTSIZE _Font /*= MAXFONT*/, eALIGNMENT _Align /*= ALIGNL*/)
{
	CString str;
	str.LoadString(_StrID);
	PrintString(_XPos, _YPos, str, _Colour, _Font, _Align);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::PrintString
	Access:    	public 
	Parameter: 	int _XPos
	Parameter: 	const uint _YPos
	Parameter: 	CString & _Str
	Parameter: 	COLORREF _Colour
	Parameter: 	eFONTSIZE _Font
	Parameter: 	eALIGNMENT _Align
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::PrintString(int _XPos, const uint _YPos, CString& _Str, COLORREF _Colour /*= TRANSPARENT*/, eFONTSIZE _Font /*= MAXFONT*/, eALIGNMENT _Align /*= ALIGNL*/)
{
	if (_Colour != TRANSPARENT)
	{
		m_pMemDC->SetTextColor(_Colour);
	}
	SetFont(_Font);
	CRect DataRect(_XPos, _YPos, _XPos + m_pMemDC->GetTextExtent(_Str).cx, _YPos + m_pMemDC->GetTextExtent(_Str).cy);
	switch (_Align)
	{
		case ALIGNR:
			_XPos = DataRect.right - m_pMemDC->GetTextExtent(_Str).cx;
			break;
		case ALIGNL:
			_XPos = DataRect.left;
			break;
		case ALIGNC:
			_XPos = DataRect.left + (DataRect.right - DataRect.left) / 2 - m_pMemDC->GetTextExtent(_Str).cx / 2;
			break;
		default :
			EXT_ASSERT(false);
	}
	_XPos = max(_XPos, DataRect.left + 1);
	m_pMemDC->ExtTextOut(_XPos, DataRect.top, ETO_CLIPPED, DataRect, _Str, _Str.GetLength(), null);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::PrintCentredStringID
	Access:    	public 
	Parameter: 	const uint _YPos
	Parameter: 	const int _StrID
	Parameter: 	COLORREF _Colour
	Parameter: 	eFONTSIZE _Font
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::PrintCentredStringID(const uint _YPos, const int _StrID, COLORREF _Colour /*= TRANSPARENT*/, eFONTSIZE _Font /*= MAXFONT*/)
{
	CString str;
	str.LoadString(_StrID);
	PrintCentredString(_YPos, str, _Colour, _Font);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::PrintCentredString
	Access:    	public 
	Parameter: 	const uint _YPos
	Parameter: 	CString & _Str
	Parameter: 	COLORREF _Colour
	Parameter: 	eFONTSIZE _Font
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::PrintCentredString(const uint _YPos, CString& _Str, COLORREF _Colour /*= TRANSPARENT*/, eFONTSIZE _Font /*= MAXFONT*/)
{
	if (_Colour != TRANSPARENT)
	{
		m_pMemDC->SetTextColor(_Colour);
	}
	SetFont(_Font);
// 	if (GetCentreX() < m_pMemDC->GetTextExtent(_Str).cx / 2)
// 	{
// 		LOGFONT theFont;
// 		m_pfontCurrent->GetLogFont(&theFont);
// 		int ab = theFont.lfHeight;
// 	}

	CRect DataRect(GetCentreX() - m_pMemDC->GetTextExtent(_Str).cx / 2, _YPos, GetCentreX() * 2, _YPos + m_pMemDC->GetTextExtent(_Str).cy);
	m_pMemDC->ExtTextOut(DataRect.left, DataRect.top, ETO_CLIPPED, DataRect, _Str, _Str.GetLength(), null);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::PrintInt
	Access:    	public 
	Parameter: 	const uint _Number
	Parameter: 	const uint _XPos
	Parameter: 	const uint _YPos 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CBasicDisplay::PrintInt(const uint _Number, const uint _XPos, const uint _YPos /*= 0xffff*/)
{
	CString Str;
	Str.Format(_T("%d"), _Number);
	m_DataRect.left = _XPos - m_pMemDC->GetTextExtent(Str).cx;
	if (_YPos < 0xffff)
	{
		m_DataRect.top = _YPos;
	}
	m_pMemDC->ExtTextOut(m_DataRect.left, m_DataRect.top, ETO_CLIPPED, m_DataRect, Str, Str.GetLength(), null);
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetOffScreenBuffer
	Access:    	public 
	Parameter: 	void
	Returns:   	COffScreenBuffer*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
COffScreenBuffer* CBasicDisplay::GetOffScreenBuffer()
{
	EXT_ASSERT_POINTER(m_pMemDC, COffScreenBuffer);
	return m_pMemDC;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::ButtonList
	Access:    	public 
	Returns:   	CPointerArray<CPNGButton>&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CPointerArray<CPNGButton>& CBasicDisplay::ButtonList()
{
	return m_listButtons;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetBitmap
	Access:    	public 
	Parameter: 	void
	Returns:   	CVOImage&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CVOImage& CBasicDisplay::GetBitmap()
{
#ifndef TESTBED
	return m_pngTitle;
#endif
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetGrid
	Access:    	public 
	Returns:   	CGrid&
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CGrid& CBasicDisplay::GetGrid()
{
	return m_Grid;
}

/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetNormalFont
	Access:    	public 
	Parameter: 	void
	Returns:   	CFont*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFont* CBasicDisplay::GetNormalFont(void)								
{ 
	return &m_NormalFont; 
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetSmallFont
	Access:    	public 
	Parameter: 	void
	Returns:   	CFont*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFont* CBasicDisplay::GetSmallFont(void)								
{ 
	return &m_SmallFont; 
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetBigFont
	Access:    	public 
	Parameter: 	void
	Returns:   	CFont*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFont* CBasicDisplay::GetBigFont(void)								
{ 
	return &m_BigFont; 
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetBoldFont
	Access:    	public 
	Parameter: 	void
	Returns:   	CFont*
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
CFont* CBasicDisplay::GetBoldFont(void)								
{ 
	return &m_BoldFont; 
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetCentreX
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::GetCentreX()
{
	CRect rectWnd;
	m_pMemDC->GetParentWnd()->GetClientRect(&rectWnd);
	return rectWnd.CenterPoint().x;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetHeight
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::GetHeight()
{
	CRect rectWnd;
	m_pMemDC->GetParentWnd()->GetClientRect(&rectWnd);
	return rectWnd.bottom;
}


/*------------------------------------------------------------------------------
	Method:   	CBasicDisplay::GetWidth
	Access:    	public 
	Returns:   	int
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
int CBasicDisplay::GetWidth()
{
	CRect rectWnd;
	m_pMemDC->GetParentWnd()->GetClientRect(&rectWnd);
	return rectWnd.right;
}


