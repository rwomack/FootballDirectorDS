

/*

    CustomisableSoccerTactic.h: interface for the CCustomisableSoccerTactic class.


	Football Director

	(c) RW Software 1994 - 2005

  ---------------------------------------------------------------
 */


#if !defined(AFX_CUSTOMTACTIC_H__9D909FC8_3302_4D06_AD84_1E65FC7399AB__INCLUDED_)
#define AFX_CUSTOMTACTIC_H__9D909FC8_3302_4D06_AD84_1E65FC7399AB__INCLUDED_


class CCustomisableSoccerTactic : public CSoccerTactic
{
public:
	CCustomisableSoccerTactic();
	virtual ~CCustomisableSoccerTactic();

    uint8            GetPlayerArrows(uint16 x)
	{
		return m_Arrows[x];
	}
    void             SetPlayerArrows(uint16 x, const uint8 y)
	{
		m_Arrows[x] = y;
	}
	bool					CanMoveToPosition(CExtPoint* pNewPosition, int iPositionToIgnore);
	bool					PositionIsValid(CExtPoint* pNewPosition);
	bool					IsBizarre(void);
	const bool				GetBizarreWarningIssued(void);
	const bool				IsCustomizable(void);
	void					SetPlayerPosition(uint16 x, CExtPoint* pNewPosition);
	void					SetPlayerPositionX(uint16 x, uint16 y);
    void					SetPlayerPositionY(uint16 x, uint16 y);
    void                    SetTitle(CString x);
	void         			SetBizarreWarningIssued(void);

	const bool				GetBizarreWarningIssued(void)
	{
		return m_BizarreWarningIssued;
	}
	const bool				IsCustomizable(void)
	{
		return m_IsCustomizable;
	}
	void					SetPlayerPosition(uint16 x, CExtPoint* pNewPosition)
	{
		m_Formations[x] = *pNewPosition;
	}
	void					SetPlayerPositionX(uint16 x, uint16 y)
	{
		m_Formations[x].SetX(y);
	}
    void					SetPlayerPositionY(uint16 x, uint16 y)
	{
		m_Formations[x].SetY(y);
	}
    void                    SetTitle(CString x)
	{
		m_Name = x;
	}
	void         			SetBizarreWarningIssued(void)
	{
		m_BizarreWarningIssued = true;
	}

private:
    uint8					m_Arrows[NUMBERPLAYERS];
	bool					m_IsCustomizable;
	bool					m_BizarreWarningIssued;
#endif
};

#endif // !defined(AFX_CUSTOMTACTIC_H__9D909FC8_3302_4D06_AD84_1E65FC7399AB__INCLUDED_)
