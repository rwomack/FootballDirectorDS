#ifndef __EXTRECT_H__
#define __EXTRECT_H__



class CExtRect : public CRect
{
public:
	CExtRect() 
		: CRect(0, 0, 0, 0) {};
	CExtRect(int l, int t, int r, int b) 
		:	CRect(l, t, r, b) {};

	bool					PtInRect(const CPoint _ThePoint);

protected:
};
#endif
