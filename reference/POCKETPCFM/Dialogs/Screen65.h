#if !defined(AFX_SCREEN65_H__2578439D_CB93_4556_9393_56F6BEE000A5__INCLUDED_)
#define AFX_SCREEN65_H__2578439D_CB93_4556_9393_56F6BEE000A5__INCLUDED_

// Screen65.h : header file
//


/////////////////////////////////////////////////////////////////////////////
// CScreen65


class CScreen65 : public CPropertySheet
{

// Construction
public:
	CScreen65(UINT nIDCaption, CWnd* pParentWnd = null, UINT iSelectPage = 0, CMatchPlay* pMatchPlay = null);
	virtual ~CScreen65();


    enum ePAGE
	{
		eCOMMENTARY,
		eSKILL,
		eLINEUPS,
		eEVENTS,
		eRADAR,
		eLATESTSCORES,
		eLEAGUETABLE,

		eTACTICS,
		eSUBSTITUTIONS,
		eSPECIALPLAYERS,
		eFORMATION,

		eMAXPAGE
	};

	enum 
	{
		ID_CLOCK = 500,
		ID_MOVIE,
		ID_PENALTY,
		ID_DISPLAY,
		ID_NEXTEVENT,
		ID_KICKOFF,
		ID_SOUND
	};

protected:
	void*					m_pCurrentPage;
	CMatchPlay*				m_pCurrentMatchPlay;

	CScreen65Commentary		m_CommentaryPage;
	CScreen65Skill			m_SkillPage;
	CScreen65LineUps		m_LineUpsPage;
	CScreen65Events			m_EventsPage;
	
	//CScreen65Substitutions 	m_SubstitutionsPage;
	//CScreen27TacticsPage	m_TacticsPage;
	//CScreen27SpecialPlayersPage m_SpecialPlayersPage;
	//CScreen27FormationPage  m_FormationPage;

// Overrides
    BOOL					OnInitDialog();
	void					OnPaint();

// Implementation
	void					UpdateTimeDisplay();
	void					OnTimer(UINT nIDEvent);
	void					DoEndOfHalf();
	void					DoCommenceHalf();
	void					DoPlaySoundFX(const eSOUNDFX theFX);
	void					SetTabText(const int nTabID, const int nStringID);

	DECLARE_MESSAGE_MAP()
};
#endif // !defined(AFX_SCREEN65_H__2578439D_CB93_4556_9393_56F6BEE000A5__INCLUDED_)
