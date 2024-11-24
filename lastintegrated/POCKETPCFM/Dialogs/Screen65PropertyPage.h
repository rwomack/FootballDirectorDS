#if !defined(AFX_SCREEN65A12_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_)
#define AFX_SCREEN65A12_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_

// Screen65PropertyPage.h : header file
//

class CMatchBitmapPropertyPage : public CBitmapPropertyPage
{
public:
	CMatchBitmapPropertyPage(UINT _IDTemplate) :
		CBitmapPropertyPage(_IDTemplate) {};

	void					OnPaint(const bool _IsPaused);
	BOOL OnInitDialog(const CString _strHelpTopic);
	BOOL					OnSetActive();

	void					DisplayScore();
	void					SetCurrentMatchPlay(CMatchPlay* _pMatchPlay);				
	eWhichTeam				GetVenue();


protected:
	CMatchPlay*				m_pCurrentMatchPlay;
	CVOImage				m_pngPause;
    CExtStatic				m_HomeScoreStatic;
    CExtStatic				m_AwayScoreStatic;
};
#endif // !defined(AFX_SCREEN65A12_H__51BA0AA5_EE54_4E28_B946_9652D03926A9__INCLUDED_)
