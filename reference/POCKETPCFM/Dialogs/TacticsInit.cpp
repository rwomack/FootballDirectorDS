	m_Text1ID = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
	GetBasicDisplay()->GetTextRegion(m_Text1ID)->DoInitialise(GetBasicDisplay(), IDS_COUNTERATTACK);

	m_Text2ID = GetBasicDisplay()->DoAddTextRegion(new CTextRegion);
	GetBasicDisplay()->GetTextRegion(m_Text2ID)->DoInitialise(GetBasicDisplay(), IDS_OFFSIDETRAP);

	m_TextSelector[eSTYLEOFPLAY].DoInitialise(50, IDS_STYLEOFPLAY, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eAGGRESSION].DoInitialise(80, IDS_AGGRESSION, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);
	m_TextSelector[eMARKING].DoInitialise(110, IDS_MARKSTYLE, this, GetBasicDisplay(), IDR_PNGLEFT, IDR_PNGRIGHT);

	int strID[] = 
	{
		IDS_AGGRESSION1, IDS_AGGRESSION2, IDS_AGGRESSION3, IDS_AGGRESSION4,
		IDS_AGGRESSION5, IDS_AGGRESSION6, IDS_AGGRESSION7, IDS_AGGRESSION8,
		IDS_AGGRESSION9, IDS_AGGRESSION10
	};
	m_TextSelector[eAGGRESSION].DoAddStrIDList(&strID[0], 10, WorldData().GetCurrentUser()->GetCurrentAggression());

	int StyleStrID[] = 
	{
		IDS_PLAYSTYLE1, IDS_PLAYSTYLE2, IDS_PLAYSTYLE3, IDS_PLAYSTYLE4,
		IDS_PLAYSTYLE5, IDS_PLAYSTYLE6, IDS_PLAYSTYLE7, IDS_PLAYSTYLE8,
		IDS_PLAYSTYLE9, IDS_PLAYSTYLE10
	};
	m_TextSelector[eSTYLEOFPLAY].DoAddStrIDList(StyleStrID, 10, WorldData().GetCurrentUser()->GetCurrentStyleOfPlay());

	int MarkingStrID[] =
	{
		IDS_MARKING1, IDS_MARKING2, IDS_MARKING3,
		IDS_MARKING4, IDS_MARKING5
	};
	m_TextSelector[eMARKING].DoAddStrIDList(MarkingStrID, 5, WorldData().GetCurrentUser()->GetCurrentMarkingStyle()); 
	m_CurrentTextSelector.OnInitialise(&m_TextSelector[0], eMAXSELECTOR);

	SetIsInitialised(true);
