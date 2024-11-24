#if !defined(EMPLOYEEGRID_INCLUDED_)
#define EMPLOYEEGRID_INCLUDED_

enum
{
	GRIDWIDTH2 = 105
};


class CEmployeeGrid : public CGrid
{
public:
	CEmployeeGrid()
		: CGrid() {};
	virtual					~CEmployeeGrid() {};

	void					DoBuildGrid(CEmployeeList& _List);
	void					OnInitialUpdate(CWnd* _CWnd, CBasicDisplay* _BasicDisplay);
	void					SetCurrentEmployee();
};
#endif
