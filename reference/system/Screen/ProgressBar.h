
/*

	Progress.h : header file

	Progress bar functionality

	(c) Sports Director Ltd 1994 - 2008

---------------------------------------------------------------

*/


#if !defined(AFX_PROGRESSBAR_H__5502E84B_CDE1_4308_9DAE_21DA93AC0115__INCLUDED_)
#define AFX_PROGRESSBAR_H__5502E84B_CDE1_4308_9DAE_21DA93AC0115__INCLUDED_

#define PROGRESSBARWIDTH 12


class CProgressBar
{
public:
	CProgressBar();

	void					DoReset(const byte _Numsteps);
	void					DoStepIt();
	void					MoveWindow(const CRect& _TheRect);
	void					OnPaint(HDC _HDC);
	void					SetDrawInfo(CVOImage* _PNG);

protected:
	CVOImage*				m_PNG;
	byte					m_NumSteps;
	byte					m_CurrentStep;
	CRect					m_WindowRect;
};
/////////////////////////////////////////////////////////////////////////////
#endif // !defined(AFX_SCREEN03VIEW_H__5502E84B_CDE1_4308_9DAE_21DA93AC0115__INCLUDED_)
