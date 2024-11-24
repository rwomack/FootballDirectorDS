#if !defined(AFX_PNGBUTTON_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_)
#define AFX_PNGBUTTON_H__6BB48EC6_A060_46C3_8A9B_71E2C354AA27__INCLUDED_

/*
	PNGButton.h : header file for Graphic button

	(c) Rw Software 1994 - 2007

---------------------------------------------------------------
*/

#ifdef ORIGINAL_CODE

class CPNGButton
{
// Construction
public:
	CPNGButton(CWnd* pParent = null);

	void					DoSetDrawInfo(const int nNormalID, CWnd* pParent);
	virtual bool			IsPointInRect(CPoint hitPoint, const ePNGButtonStatus nStatusIfTrue);
	void					OnPaint(CDC* dc);
	void					MoveWindow(CRect& _Rect);
	void					SetStatus(const ePNGButtonStatus status);
	ePNGButtonStatus		GetStatus() const;

	// Implementation
protected:
	CRect					m_theRect;
	CPNGDrawInfo			m_pngButton;
	ePNGButtonStatus		m_Status;
	CWnd*					m_pParentWnd;
};

#endif

#endif
