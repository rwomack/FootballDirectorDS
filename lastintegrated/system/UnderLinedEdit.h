#if !defined(AFX_UNDERLINEDEDIT_H__D4B7F3C6_2868_40CD_9686_5A4A299203E6__INCLUDED_)
#define AFX_UNDERLINEDEDIT_H__D4B7F3C6_2868_40CD_9686_5A4A299203E6__INCLUDED_

// UnderLinedEdit.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CUnderLinedEdit

class CUnderLinedEdit : public CEdit
{
public:
	void				OnPaint(CPaintDC& dc)
	{
	/*	CRect rectEdit;
		GetWindowRect(rectEdit);

		dc.MoveTo(rectEdit.left, rectEdit.bottom + 1 - GetSystemMetrics(SM_CYCAPTION));
		dc.LineTo(rectEdit.right, rectEdit.bottom + 1 - GetSystemMetrics(SM_CYCAPTION));*/
	}
	void				SetHeight(const int nHeight = 18)
	{
		CRect rectEdit;
		GetWindowRect(rectEdit);
	//	ClientToScreen(rectEdit);
		rectEdit.bottom = rectEdit.top + nHeight;
	//	MoveWindow(rectEdit);
	}

protected:
};
#endif