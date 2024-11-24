#pragma once

class CSoccerResultGrid : public CGrid
{
public:
	CSoccerResultGrid(void);
	~CSoccerResultGrid(void);

	void					DoBuild(const int _CurrentTime);
	CFixtureList&			GetFixtures();
	void OnInitialUpdate(CWnd* _ParentWnd, HWND _hWnd, CBasicDisplay* _BasicDisplay);

protected:
	CFixtureList			m_Fixtures;
	int						m_CurrentTime;
};
