#if !defined(AFX_SCREEN26VIEW_H__CF3EB213_4B35_47EC_981A_73A79860227C__INCLUDED_)
#define AFX_SCREEN26VIEW_H__CF3EB213_4B35_47EC_981A_73A79860227C__INCLUDED_


// Screen26View.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreen26View form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CScreen26View : public CBitmapFormView
{
public:
	CScreen26View();           // protected constructor used by dynamic creation
	virtual ~CScreen26View();
	DECLARE_DYNCREATE(CScreen26View)

// Form Data
	enum 
	{ 
		IDD = IDD_SCREENNOCONTROLS 
	};

	class CAccountsGridItem
	{
	public:
		bool m_IsIncome;
		CString m_Description;
		CExtString m_IncomeStr;
		CExtString m_ExpenditureStr;

		void	DoAddIncome(int _StringID, int _Amount)
		{
			m_IsIncome = true;
			m_Description.LoadString(_StringID);
			m_IncomeStr.FormatLargeNumber(_Amount);
		}

		void	DoAddExpenditure(int _StringID, int _Amount)
		{
			m_IsIncome = false;
			m_Description.LoadString(_StringID);
			m_ExpenditureStr.FormatLargeNumber(_Amount);
		}
	};

	virtual void			OnInitialUpdate();

protected:
	CGrid					m_Grid;
	CClubBalanceTextRegion	m_ClubBalanceText;
	CTextRegion				m_Header;
	int						m_NumOnScreen;
	eFINANCIALPERIOD		m_eWhichPeriod;

	virtual void			OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void					OnVScroll(UINT _SBCode, UINT _Pos, CScrollBar* _ScrollBar);
	void					DoBuildGrid();
	void					OnActionClicked();
	void 					OnViewCurrentWeek();
	void 					OnViewYearToDate();

	DECLARE_MESSAGE_MAP()

	// Implementation


#define ProfitOut(Amount, textID) \
	theList[NumFound].DoAddIncome(textID, Amount); \
	NumFound ++; \
	Income += Amount;

#define DebitOut(Amount, textID) \
	theList[NumFound].DoAddExpenditure(textID, Amount); \
	NumFound ++; \
	Expenditure += Amount;

};

/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SCREEN26VIEW_H__CF3EB213_4B35_47EC_981A_73A79860227C__INCLUDED_)
