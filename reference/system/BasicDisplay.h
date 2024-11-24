
/*

    BasicDisplay.h: interface for the CBasicDisplay class.


	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/



#if !defined(AFX_BASICDISPLAY_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_BASICDISPLAY_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_


	enum eFONTSIZE
	{
		NORMALFONT,
		SMALLFONT,
		BIGFONT,
		BOLDFONT,
		HEADERFONT,

		MAXFONT
	};

	enum eALIGNMENT
	{
		ALIGNL,
		ALIGNR,
		ALIGNC
	};

	class CButtonInfo;
	class CPNGButton;
	class COffScreenBuffer;
	class CTextRegion;
	class CVOImage;

class CBasicDisplay	
{
public:
	CBasicDisplay();
	~CBasicDisplay();

	CPointerArray<CPNGButton>& ButtonList();
	int						DoAddButton(uint _ResourceID, CWnd* _pParentWnd);
	void					DoAddButtons(uint* _ResourceID, CWnd* _pParentWnd);
	void					DoMoveButton(uint _ButtonID, CRect& _Rect);
	void					DoRemoveButtonList();
	void					DoRemoveAllData();
	CFont*					GetBigFont();
	CVOImage&				GetBitmap();
	CFont*					GetBoldFont();
	int						GetCentreX();
	CGrid&					GetGrid();
	int						GetHeight();
	CFont*					GetNormalFont();
	COffScreenBuffer*		GetOffScreenBuffer();
	CFont*					GetSmallFont();
	CTextRegion*			GetTextRegion(int _TextRegionID);
	int						GetWidth();
	BOOL					OnEraseBkgnd(CDC* _pDC);
	BOOL					OnHelpInfo(HELPINFO* _HelpInfo);
	void OnInitDialog(CWnd* _pParentWnd, const int _HelpTopic, const int _StrID);
	void					OnInitRedrawArea(CWnd* _pParentWnd);
	void					OnPaint(const bool _bShowDate, const bool _bShouldBlit = false);
	void					OnPaintControls();
	void					OnSize(CWnd* _pParentWnd);
	void					PrintCentredString(const uint _YPos, CString& _Str, COLORREF _Colour = TRANSPARENT, eFONTSIZE _Font = MAXFONT);
	void					PrintCentredStringID(const uint _YPos, const int _StrID, COLORREF _Colour /*= TRANSPARENT*/, eFONTSIZE _Font /*= MAXFONT*/);
	void					PrintString(int _XPos, const uint _YPos, CString& _Str, COLORREF _Colour = TRANSPARENT, eFONTSIZE _Font = MAXFONT, eALIGNMENT _Align = ALIGNL);
	void					PrintStringID(uint _XPos, const uint _YPos, const int _StrID, COLORREF _Colour = TRANSPARENT, eFONTSIZE _Font = MAXFONT, eALIGNMENT _Align = ALIGNL);
	void					PrintInt(const uint _Number, const uint _XPos, const uint _YPos = 0xffff);
	void 					RestoreFont();
	void					SetBitmap(UINT _BitmapID);
	void					SetButtonStatus(uint _ButtonID, ePNGButtonStatus _Status);
	void					SetDisplayHeader(bool _Value, int _StrID = 0);
	void					SetDisplayBitmap(bool _Value);
	void					SetFullScreen();
	void 					SetFontSmall();
	void 					SetFontNormal();
	void 					SetFont(CFont& _Font);
	void					SetFont(eFONTSIZE _Font);
	void 					SetFontBig();
	void 					SetFontBold();

	CPNGButton*				GetButton(int _ButtonID);
	int						OnLButtonDown(UINT _Flags, CPoint _Point);
	int						OnLButtonUp(UINT _Flags, CPoint _Point);

	CPointerArray<CTextRegion>&	TextRegionList();
	int						DoAddTextRegion();
	int						DoAddTextRegion(CTextRegion* _TextRegion);
	void					DoAddTextRegions(int _HowMany);
	int						DoAddCentredTextRegion(int _YPos, int _StringID, eFONTSIZE _FontSize = NORMALFONT);
	void					DoMoveTextRegion(uint _TextRegionID, CRect& _Rect);
	void					DoRemoveTextRegionList();

protected:
	COffScreenBuffer*		m_pMemDC;
	CVOImage				m_pngTitle;
#ifdef DISPLAYDATE
	CVOImage				m_pngCalendar;
	CVOImage				m_pngDay;
	CVOImage				m_pngMonth;
	CVOImage				m_pngYear;
#endif
	CRect					m_DataRect;
	CRect					m_OriginalDataRect;
	CPointerArray<CPNGButton> m_listButtons;
	CPointerArray<CTextRegion> m_listTextRegions;
	CString					m_HeaderTitle;
	CGrid					m_Grid;

	bool					m_DisplayHeader;
	bool					m_DisplayBitmap;

	CFont*					m_pfontCurrent;
	CFont					m_NormalFont;
	CFont					m_SmallFont;
	CFont					m_BigFont;
	CFont					m_BoldFont;
	CFont					m_HeaderFont;

	int						m_HelpTopicID;
	COLORREF				m_TextColour;

	CPNGDrawInfo			m_pngLargeKits;
};
#endif