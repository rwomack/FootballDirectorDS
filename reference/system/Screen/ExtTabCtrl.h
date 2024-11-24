#if !defined(EXTTABCTRL_INCLUDED_)
#define EXTTABCTRL_INCLUDED_
 
/*

	ExtTabCtrl.h: interface for the Extended TabCtrl class.


	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/



class CExtTabCtrl : public CTabCtrl
{
	DECLARE_DYNAMIC(CExtTabCtrl)

public:
	CExtTabCtrl();
	virtual ~CExtTabCtrl();

	CWnd*					GetParentWnd();
	int						GetWidth();
	void					OnInitialUpdate(CWnd* _ParentWnd, int _ResourceID, int _Width, int* _PageArray, int _NumPages);
	void					OnInitialUpdate(CWnd* _ParentWnd, int _ResourceID, int _Width);
	void					OnInitialUpdate(CWnd* _ParentWnd, int _ResourceID, int _Width, CStringArray& _PageArray);

protected:
	CWnd*					m_ParentWnd;
	int						m_Width;
	CFont					m_Font;
	bool					m_bFontCreated;

public:
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnSize(UINT nType, int cx, int cy);
	void					SetFontSize(const int _iPoints, const int _nWeight = FW_MEDIUM);

	DECLARE_MESSAGE_MAP()
};
#endif

