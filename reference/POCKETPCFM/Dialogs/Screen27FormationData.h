enum 
{ 
	iPitchXGap = 19,
	iPitchYGap = 11,
};


// Implementation
protected:
	CVOImage				m_Pitch;
	CPNGDrawInfo			m_pngNumbers;
	CExtRect				m_PitchRect;
	CTextSelector			m_FormationSelector;
	ushort					m_iPlayerSelected;

	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	void					OnSelchangeFormationList();
	void					OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
