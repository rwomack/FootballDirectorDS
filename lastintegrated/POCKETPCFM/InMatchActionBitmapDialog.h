
/*

	InMatchActionBitmapDialog.h

	Football Director

	(c) Sports Director Ltd 2007 - 2008

  ---------------------------------------------------------------
*/


enum 
{
	SUBS_SUBS,
	SUBS_ROLES,
	SUBS_TACTICS,
	SUBS_FORMATION,
	SUBS_MATCH
};

class CInMatchActionBitmapDialog : public CBitmapDialog
{
public:
	CInMatchActionBitmapDialog(UINT nIDTemplate, CWnd* pParentWnd = NULL);   // standard constructor
	CInMatchActionBitmapDialog(void);
	virtual ~CInMatchActionBitmapDialog(void);


	// Implementation
protected:
	void 					OnMakeSubstitution();
	void 					OnTactics();
	void 					OnFormation();
	void 					OnOK();
	void 					OnRole();

	BOOL OnInitDialog(const int _HelpTopic, int _StrID);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnPaint();


	DECLARE_MESSAGE_MAP()
};
