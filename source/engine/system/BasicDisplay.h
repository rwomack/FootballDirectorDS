
/*

    BasicDisplay.h: interface for the CBasicDisplay class.


	(c) Rw Software 1994 - 2007

  ---------------------------------------------------------------------------------------
*/



#if !defined(AFX_BASICDISPLAY_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_BASICDISPLAY_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_
#ifdef ORIGINAL_CODE

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
	class CVOImage;

class CBasicDisplay
{
public:
	CBasicDisplay();
	~CBasicDisplay();

	CPointerArray&			ButtonList();
	void					OnPaint(const bool _bShowDate, const bool _bShouldBlit = false);
	void					DrawDataRectSeperators();
	CVOImage&				GetBitmap();
	CRect&					GetDataRect();
	CFont*					GetNormalFont(void);
	CFont*					GetSmallFont(void);
	CFont*					GetBigFont(void);
	CFont*					GetBoldFont(void);
	COffScreenBuffer*		GetOffScreenBuffer();
	CRect&					GetWindowRect();
	BOOL					OnEraseBkgnd(CDC* pDC);
	BOOL					OnHelpInfo(HELPINFO* _HelpInfo);
	void					OnInitDialog(CWnd* _pParentWnd, const CString _strHelpTopic, bool _bColourBackGround /*= false*/, COLORREF _Colour /*= 0x00000000*/);
	void					OnInitRedrawArea(CWnd* _pParentWnd);
	void					OnSize(CWnd* _pParentWnd);
	void					PrintCentredString(const uint _YPos, CString& _Str, COLORREF _Colour = TRANSPARENT, eFONTSIZE _Font = MAXFONT);
	void					PrintCentredStringID(const uint _YPos, const int _StrID, COLORREF _Colour /*= TRANSPARENT*/, eFONTSIZE _Font /*= MAXFONT*/);
	void					PrintString(uint _XPos, const uint _YPos, CString& _Str, COLORREF _Colour = TRANSPARENT, eFONTSIZE _Font = MAXFONT, eALIGNMENT _Align = ALIGNL);
	void					PrintStringID(uint _XPos, const uint _YPos, const int _StrID, COLORREF _Colour = TRANSPARENT, eFONTSIZE _Font = MAXFONT, eALIGNMENT _Align = ALIGNL);
	void					PrintInt(const uint _Number, const uint _XPos, const uint _YPos = 0xffff);
	void					ResetDataRect();
	void 					RestoreFont();
	void					SetBitmap(UINT _BitmapID);
	void					SetDataRect(CRect& _Rect);
	void					SetDisplayHeader(bool _Value, int _StrID = 0);
	void					SetDisplayBitmap(bool _Value);
	void					SetFullScreen();
	void 					SetFontSmall();
	void 					SetFontNormal();
	void 					SetFont(CFont& _Font);
	void					SetFont(eFONTSIZE _Font);
	void 					SetFontBig();
	void 					SetFontBold();
	int						GetCentreX();

	int						DoAddButton(uint _ResourceID, CWnd* _pParentWnd);
	void					DoAddButtons(uint* _ResourceID, CWnd* _pParentWnd);
	void					DoMoveButton(uint _ButtonID, CRect& _Rect);
	void					DoRemoveButtonList();
	void					SetButtonStatus(uint _ButtonID, ePNGButtonStatus _Status);
	CPNGButton*				GetButton(uint _ButtonID);
	void					OnLButtonDown(UINT _Flags, CPoint _Point);
	int						OnLButtonUp(UINT _Flags, CPoint _Point);

protected:
	COffScreenBuffer*		m_pMemDC;
	CVOImage				m_bmpTitle;
#ifdef DISPLAYDATE
	CDateDisplay			m_date;
#endif
	CRect					m_ClientRect;
	CRect					m_DataRect;
	CRect					m_OriginalDataRect;
	CPointerArray			m_listButtons;
	CString					m_HeaderTitle;
	bool					m_DisplayHeader;
	bool					m_DisplayBitmap;

	CFont*					m_pfontCurrent;
	CFont					m_NormalFont;
	CFont					m_SmallFont;
	CFont					m_BigFont;
	CFont					m_BoldFont;
	CFont					m_HeaderFont;

	CString					m_strHelpTopic;
	bool					m_bColourBackGround;
	COLORREF				m_TextColour;
};
#endif

#ifndef ORIGINAL_CODE

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
	class CVOImage;

class CBasicDisplay
{
public:
  CBasicDisplay()
  {
  }

  ~CBasicDisplay()
  {
  }
};
#endif

#endif
