/*

	GridBitmapFormView.h : header file

	Football Director

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------
*/



class CGridBitmapFormView : public CBitmapFormView
{
public:
	CGridBitmapFormView(UINT nIDTemplate);
	virtual ~CGridBitmapFormView(void);

	// Implementation
protected:
	CGrid					m_Grid;

	DECLARE_MESSAGE_MAP()
};
