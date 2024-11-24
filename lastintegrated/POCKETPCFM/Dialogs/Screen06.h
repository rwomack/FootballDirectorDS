


class CCompactPlayerSearch
{
public:
	int					GetValue();
	int					GetAge();
	int					GetSkill();
	int					GetPosition();
	int					GetAvailability();
	void				SetFromPlayerSearch(CPlayerSearch& _Search);
	void				SetToPlayerSearch(CPlayerSearch& _Search);

protected:
	int						m_Value;
	int						m_Age;
	int						m_Skill;
	int						m_Position;
	int						m_Availability;
};

// CScreen06 dialog

class CScreen06 : public CBitmapDialog
{
	DECLARE_DYNAMIC(CScreen06)

public:
	CScreen06(CWnd* pParent = NULL);   // standard constructor
	virtual ~CScreen06();

// Dialog Data
	enum 
	{ 
		IDD = IDD_SCREENDLGNOCONTROLS 
	};

	enum 
	{
		eVALUE,
		eAGE,
		eSKILL,
		ePOSITION,
		eAVAILABILITY,

		eMAXSELECTOR
	};

protected:
	CCurrentTitledTextSelector	m_CurrentTextSelector;
	CTitledTextSelector		m_TextSelector[eMAXSELECTOR];
	CCompactPlayerSearch	m_CompactSearch;

	BOOL					OnInitDialog();
	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
};
