#if !defined(AFX_SCREEN65A6_H__97D13E7A_9E70_400C_9F2D_3BCC936C7372__INCLUDED_)
#define AFX_SCREEN65A6_H__97D13E7A_9E70_400C_9F2D_3BCC936C7372__INCLUDED_

// Screen65Substitutions.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen65Substitutions dialog

class CScreen65Substitutions : public CMatchBitmapPropertyPage
{
	DECLARE_DYNCREATE(CScreen65Substitutions)

// Construction
public:
	CScreen65Substitutions();
	~CScreen65Substitutions();

// Dialog Data
	enum 
	{
		TOPY = 92,
		GAPY = 14,
		
		IDD = IDD_SCREEN65a6 
	};

// Overrides
protected:
	SYSTEMTIME				m_SystemTime;
	CPNGDrawInfo			m_pngArrowLR;
	CPNGDrawInfo			m_bmpPlayerStatus;

	CClub&					GetClub();

	void					DisplayStatus(CPlayer& _Player, const ushort nNumber, CRect& DataRect);
    BOOL					OnInitDialog();
	void					OnPaint();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					DoSetSwap(const ushort aSquadNumber);
	ushort					DoCalcPlayerClickedOn(CPoint point);

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN65A6_H__97D13E7A_9E70_400C_9F2D_3BCC936C7372__INCLUDED_)
