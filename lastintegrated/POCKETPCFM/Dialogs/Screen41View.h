#if !defined(AFX_SCREEN41VIEW_H__7B9E99E5_DB4B_437A_96AB_24AC6C5AE25F__INCLUDED_)
#define AFX_SCREEN41VIEW_H__7B9E99E5_DB4B_437A_96AB_24AC6C5AE25F__INCLUDED_

/*

	Screen41View.h : header file


	(c) Rw Software 2003 - 2008

  ---------------------------------------------------------------
*/


class CScreen41View : public CBitmapFormView
{
// Construction
public:
	CScreen41View();           // protected constructor used by dynamic creation
	~CScreen41View();

	DECLARE_DYNCREATE(CScreen41View)

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS 
	};

// Implementation
protected:
	CYouthGrid				m_YouthGrid;
	CTextSelector			m_TypeList;

	void					DoBuildGrid();
	void					OnActionClicked();
	void					OnHire();
	void					OnOfferContract();
	void					OnRelease();
	void					OnViewDetails();

	virtual void			OnDraw(CDC* pDC);
	virtual void			OnInitialUpdate();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnLButtonUp(UINT _nFlags, CPoint _Point);
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN41VIEW_H__7B9E99E5_DB4B_437A_96AB_24AC6C5AE25F__INCLUDED_)
