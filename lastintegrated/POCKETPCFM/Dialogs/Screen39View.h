#if !defined(AFX_SCREEN39VIEW_H__5AF1FC31_95E8_423A_8080_17FA8A5FD6E5__INCLUDED_)
#define AFX_SCREEN39VIEW_H__5AF1FC31_95E8_423A_8080_17FA8A5FD6E5__INCLUDED_

// Screen39View.h : header file
//


class CScreen39View : public CBitmapFormView
{
// Construction
public:
	CScreen39View();           // protected constructor used by dynamic creation
	~CScreen39View();

	DECLARE_DYNCREATE(CScreen39View)

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS 
	};

// Implementation
protected:
	CTextSelector			m_TypeList;
	CEmployeeGrid			m_EmployeeGrid;

	void					DoBuildGrid();
	void					OnActionClicked();
	void					OnHire();
	void					OnOfferContract();
	void					OnRelease();
	void					OnViewDetails();

	virtual void			OnDraw(CDC* pDC);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnLButtonUp(UINT nFlags, CPoint point);
	virtual void			OnInitialUpdate();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);

	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_SCREEN39VIEW_H__5AF1FC31_95E8_423A_8080_17FA8A5FD6E5__INCLUDED_)
