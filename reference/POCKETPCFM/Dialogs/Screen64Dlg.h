#if !defined(AFX_SCREEN64_H__73D884DD_307D_4E46_ADA9_28F151DB5704__INCLUDED_)
#define AFX_SCREEN64_H__73D884DD_307D_4E46_ADA9_28F151DB5704__INCLUDED_

// Screen64Dlg.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen64Dlg dialog

class CScreen64Dlg : public CInMatchActionBitmapDialog
{
// Construction
public:
	CScreen64Dlg(CMatchPlay* _MatchPlay, CWnd* pParent = null);   // standard constructor
	~CScreen64Dlg();

// Dialog Data
	enum 
	{
		IDD = IDD_SCREENDLGNOCONTROLS,
	
		STATUS_COL_WIDTH = 12,
		PLAYER_NAME_LENGTH = 86,
		TEAM_GRID_WIDTH = STATUS_COL_WIDTH + PLAYER_NAME_LENGTH + 18,
		SUB_GRID_WIDTH = STATUS_COL_WIDTH + PLAYER_NAME_LENGTH,
	};

	enum eSUBSTATUS
	{
		eOFF,
		eTAKENOFF,
		eBROUGHTON,
		eINJURED,
		eSENTOFF,

		eMAXSUBSTATUS
	};

	enum eWHICHGRID
	{
		eTEAMGRID,
		eSUBGRID
	};

protected:
	CGrid					m_TeamGrid;
	CGrid					m_SubGrid;
	eWHICHGRID				m_WhichGrid;
	CMatchPlay*				m_MatchPlay;
	CPNGDrawInfo			m_pngArrowLR;

	void					DoBuildGrids();
	void					DoSetSwap(const ushort _SquadNumber);
	BOOL					OnInitDialog();
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnLButtonDown(UINT _Flags, CPoint _Point);
	void					OnLButtonUp(UINT _nFlags, CPoint _Point);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	CMatchSubs&				SubsInfo();

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN64_H__73D884DD_307D_4E46_ADA9_28F151DB5704__INCLUDED_)
