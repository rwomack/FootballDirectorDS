#if !defined(YOUTHGRID_INCLUDED_)
#define YOUTHGRID_INCLUDED_


enum
{
	YGRIDWIDTH2 = 105
};

class CYouthGrid : public CGrid
{
public:
	CYouthGrid(void)
		: CGrid() {};
	virtual					~CYouthGrid(void) {};

	void					DoBuildGrid(CYouthList& _List);
	void					OnInitialUpdate(CWnd* _CWnd, CBasicDisplay* _BasicDisplay);
	void					SetCurrentYouth();
};
#endif