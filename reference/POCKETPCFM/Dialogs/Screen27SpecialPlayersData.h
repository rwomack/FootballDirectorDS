
// Overrides
protected:
	CPNGDrawInfo			m_pngRoles;
	CPlayerSelector			m_Captain;
	CPlayerSelector			m_PenaltyTaker;
	CPlayerSelector			m_CornerTaker;
	CPlayerSelector			m_FreeKickTaker;

	// Implementation
	DECLARE_MESSAGE_MAP()

	void					DoUpdateData();

	void					OnLButtonUp(UINT nFlags, CPoint point);
	void					OnLButtonDown(UINT nFlags, CPoint point);
	void					OnPaint();
	void					OnSize(UINT nType, int cx, int cy);
	void 					OnActionCaptainNext();
	void 					OnActionCaptainPrev();
	void 					OnActionPenaltyNext();
	void 					OnActionPenaltyPrev();
	void 					OnActionCornerNext();
	void 					OnActionCornerPrev();
	void 					OnActionFreeKickNext();
	void 					OnActionFreeKickPrev();
