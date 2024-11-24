
enum
{
	eSTYLEOFPLAY,
	eAGGRESSION,
	eMARKING,

	eMAXSELECTOR
};

class CScreen27TacticsData
{
public: 
	CBasicDisplay*			GetBasicDisplay()
	{
		EXT_ASSERT_POINTER(m_BasicDisplay, CBasicDisplay);
		return m_BasicDisplay;
	}
	void					OnInitialUpdate(CWnd* _Wnd, CBasicDisplay* _BasicDisplay)
	{
		EXT_ASSERT_POINTER(_BasicDisplay, CBasicDisplay);
		m_BasicDisplay = _BasicDisplay;
		EXT_ASSERT_POINTER(_Wnd, CWnd);

		m_CounterAttackTextID = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
		GetBasicDisplay()->GetTextRegion(m_CounterAttackTextID)->DoInitialise(GetBasicDisplay(), IDS_COUNTERATTACK);
		m_CounterAttackToggle.OnInitialUpdate(m_BasicDisplay, WorldData().GetCurrentUser()->GetCounterAttack(), _Wnd->GetSafeHwnd());

		m_OffsideTrapTextID = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
		GetBasicDisplay()->GetTextRegion(m_OffsideTrapTextID)->DoInitialise(GetBasicDisplay(), IDS_OFFSIDETRAP);
		m_OffsideTrapToggle.OnInitialUpdate(m_BasicDisplay, WorldData().GetCurrentUser()->GetOffsideTrap(), _Wnd->GetSafeHwnd());

		m_TitledTextSelector[eSTYLEOFPLAY].DoInitialise(50, IDS_STYLEOFPLAY, _Wnd, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
		m_TitledTextSelector[eAGGRESSION].DoInitialise(80, IDS_AGGRESSION, _Wnd, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
		m_TitledTextSelector[eMARKING].DoInitialise(110, IDS_MARKSTYLE, _Wnd, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);

		int strID[] = 
		{
			IDS_AGGRESSION1, IDS_AGGRESSION2, IDS_AGGRESSION3, IDS_AGGRESSION4,
			IDS_AGGRESSION5, IDS_AGGRESSION6, IDS_AGGRESSION7, IDS_AGGRESSION8,
			IDS_AGGRESSION9, IDS_AGGRESSION10
		};
		m_TitledTextSelector[eAGGRESSION].DoAddStrIDList(&strID[0], 10, WorldData().GetCurrentUser()->GetCurrentAggression());

		int StyleStrID[] = 
		{
			IDS_PLAYSTYLE1, IDS_PLAYSTYLE2, IDS_PLAYSTYLE3, IDS_PLAYSTYLE4,
			IDS_PLAYSTYLE5, IDS_PLAYSTYLE6, IDS_PLAYSTYLE7, IDS_PLAYSTYLE8,
			IDS_PLAYSTYLE9, IDS_PLAYSTYLE10
		};
		m_TitledTextSelector[eSTYLEOFPLAY].DoAddStrIDList(StyleStrID, 10, WorldData().GetCurrentUser()->GetCurrentStyleOfPlay());

		int MarkingStrID[] =
		{
			IDS_MARKING1, IDS_MARKING2, IDS_MARKING3,
			IDS_MARKING4, IDS_MARKING5
		};
		m_TitledTextSelector[eMARKING].DoAddStrIDList(MarkingStrID, 5, WorldData().GetCurrentUser()->GetCurrentMarkingStyle()); 
		m_CurrentTextSelector.OnInitialise(&m_TitledTextSelector[0], eMAXSELECTOR);
	}
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		m_CurrentTextSelector.OnKeyDown(nChar, nRepCnt, nFlags);
	}
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
	{
		m_CurrentTextSelector.OnKeyUp(nChar, nRepCnt, nFlags);
	}

	void					OnLButtonUp(const int _ButtonPressed,UINT _nFlags, CPoint _Point)
	{
		m_TitledTextSelector[eSTYLEOFPLAY].OnLButtonUp(_ButtonPressed);
		m_TitledTextSelector[eMARKING].OnLButtonUp(_ButtonPressed);
		m_TitledTextSelector[eAGGRESSION].OnLButtonUp(_ButtonPressed);

		WorldData().GetCurrentUser()->SetCurrentStyleOfPlay(m_TitledTextSelector[eSTYLEOFPLAY].GetCurSel());
		WorldData().GetCurrentUser()->SetCurrentMarkingStyle(m_TitledTextSelector[eMARKING].GetCurSel());
		WorldData().GetCurrentUser()->SetCurrentAggression(m_TitledTextSelector[eAGGRESSION].GetCurSel());
		m_CounterAttackToggle.OnLButtonUp(_nFlags, _Point);
		m_OffsideTrapToggle.OnLButtonUp(_nFlags, _Point);
	}
	void					OnPaint()
	{
		m_CurrentTextSelector.OnPaint();
		m_TitledTextSelector[eSTYLEOFPLAY].OnPaint();
		m_TitledTextSelector[eMARKING].OnPaint();
		m_TitledTextSelector[eAGGRESSION].OnPaint();
		m_CounterAttackToggle.OnPaint();
		m_OffsideTrapToggle.OnPaint();
	}
	void					OnSize()
	{
		m_TitledTextSelector[eSTYLEOFPLAY].MoveWindow();
		m_TitledTextSelector[eMARKING].MoveWindow();
		m_TitledTextSelector[eAGGRESSION].MoveWindow();
		GetBasicDisplay()->GetTextRegion(m_CounterAttackTextID)->MoveWindow(CRect(26, 134, 144, 154));
		m_CounterAttackToggle.OnSize(CPoint(2, 134));
		GetBasicDisplay()->GetTextRegion(m_OffsideTrapTextID)->MoveWindow(CRect(26, 158, 144, 178));
		m_OffsideTrapToggle.OnSize(CPoint(2, 158));
	}
	void					DoToggleOffsideTrap()
	{
		m_OffsideTrapToggle.DoToggle();
	}
	void					DoToggleCounterAttack()
	{
		m_CounterAttackToggle.DoToggle();
	}

protected:
	CCurrentTitledTextSelector	m_CurrentTextSelector;
	CTitledTextSelector		m_TitledTextSelector[eMAXSELECTOR];
	CExtCheckBox			m_CounterAttackToggle;
	int						m_CounterAttackTextID;
	CExtCheckBox			m_OffsideTrapToggle;
	int						m_OffsideTrapTextID;
	CBasicDisplay*			m_BasicDisplay;
};