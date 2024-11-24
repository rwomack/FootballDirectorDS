#if !defined(AFX_PNGBUTTON_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_)
#define AFX_PNGBUTTON_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_

/*
	PNGButton.h : header file for Graphic button

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------
*/



class CPNGButton
{
// Construction
public:
	CPNGButton(CWnd* pParent = null);

	// Implementation
	void					DoDrawHighlight(COffScreenBuffer* _dc);
	void					DoSetDrawInfo(const int nNormalID, CWnd* pParent, const uint _NumImages = 2);
	ePNGButtonStatus		GetStatus() const;
	int						GetWidth();
	bool					IsHighLight();
	virtual bool			IsPointInRect(CPoint hitPoint, const ePNGButtonStatus nStatusIfTrue);
	void					MoveWindow(CRect& _Rect);
	void					MoveWindow(const uint _XPos, const uint _YPos);
	void					OnPaint(COffScreenBuffer* _dc);
	void					SetStatus(const ePNGButtonStatus status);
	void					ToggleHighlight();

protected:
	CRect					m_theRect;
	CPNGDrawInfo			m_pngButton;
	ePNGButtonStatus		m_Status;
	CWnd*					m_pParentWnd;
	bool					m_Highlight;
};

#endif