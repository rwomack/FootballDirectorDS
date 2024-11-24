#if !defined(AFX_SCREEN65A2_H__97D13E7A_9E70_400C_9F2D_3BCC936C7372__INCLUDED_)
#define AFX_SCREEN65A2_H__97D13E7A_9E70_400C_9F2D_3BCC936C7372__INCLUDED_

// Screen65Skill.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen65Skill dialog

class CScreen65Skill : public CMatchBitmapPropertyPage
{
	DECLARE_DYNCREATE(CScreen65Skill)

// Construction
public:
	CScreen65Skill();
	~CScreen65Skill();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREEN65a2,
		iGapY = 30
	};

protected:
	CExtStatic			m_gStatic;
	CExtStatic			m_dStatic;
	CExtStatic			m_mStatic;
	CExtStatic			m_aStatic;

    BOOL				OnInitDialog();
	void				OnPaint();
	void				OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN65A2_H__97D13E7A_9E70_400C_9F2D_3BCC936C7372__INCLUDED_)
