	
	memDC.SetTextColor(BLACK);

	CRect rect(m_Button1X + BUTTON1W, BUTTON5Y, m_Button2X, BUTTON5Y + BUTTON1H);
	memDC.FillSolidRect(rect, PALEBLUE);
	int strID[] = 
	{
		IDS_AGGRESSION1, IDS_AGGRESSION2, IDS_AGGRESSION3, IDS_AGGRESSION4,
		IDS_AGGRESSION5, IDS_AGGRESSION6, IDS_AGGRESSION7, IDS_AGGRESSION8,
		IDS_AGGRESSION9, IDS_AGGRESSION10
	};
	GetBasicDisplay().PrintStringID(m_Button1X + BUTTON1W, BUTTON5Y, strID[WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().GetAggression()], TRANSPARENT, BIGFONT, ALIGNC);

	rect = CRect(m_Button1X + BUTTON1W, BUTTON1Y, m_Button2X, BUTTON1Y + BUTTON1H);
	memDC.FillSolidRect(rect, VPALEBLUE);
	int StyleStrID[] = 
	{
		IDS_PLAYSTYLE1, IDS_PLAYSTYLE2, IDS_PLAYSTYLE3, IDS_PLAYSTYLE4,
		IDS_PLAYSTYLE5, IDS_PLAYSTYLE6, IDS_PLAYSTYLE7, IDS_PLAYSTYLE8,
		IDS_PLAYSTYLE9, IDS_PLAYSTYLE10
	};
	GetBasicDisplay().PrintStringID(m_Button1X + BUTTON1W, BUTTON1Y, StyleStrID[WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().GetDefenceStyle()], TRANSPARENT, BIGFONT, ALIGNC);

	rect = CRect(m_Button1X + BUTTON1W, BUTTON3Y, m_Button2X, BUTTON3Y + BUTTON1H);
	memDC.FillSolidRect(rect, VPALEBLUE);
	int MarkingStrID [] =
	{
		IDS_MARKING1, IDS_MARKING2, IDS_MARKING3,
		IDS_MARKING4, IDS_MARKING5
	};
	GetBasicDisplay().PrintStringID(m_Button1X + BUTTON1W, BUTTON3Y, MarkingStrID[WorldData().GetCurrentUser()->GetTacticsList().GetCurrentTeamTactic().GetMarkingStyle()], TRANSPARENT, BIGFONT, ALIGNC);
	
	GetBasicDisplay().OnPaint(true, true);
