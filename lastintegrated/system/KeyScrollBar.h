// KeyScrollBar.h : header file
//

#if !defined(KEYSCROLLBAR_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_)
#define KEYSCROLLBAR_H__4FDE4335_8D98_4E1B_BF19_7A3423A3CC1F__INCLUDED_


/////////////////////////////////////////////////////////////////////////////
// CKeyScrollBar window

class CKeyScrollBar : public CScrollBar
{
// Construction
public:
	CKeyScrollBar();
	virtual ~CKeyScrollBar();

protected:
	void					OnKeyDown(UINT _nChar, UINT _nRepCnt, UINT _nFlags);
	void					OnLButtonDown(UINT _nFlags, CPoint _point);

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
#endif