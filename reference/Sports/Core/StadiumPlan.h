
 /*

    StadiumPlan class


	Football Director

	(c) RW Software 1994 - 2002

  ---------------------------------------------------------------
 */


#if !defined(AFX_STADIUMPLAN_H__610A2FC2_2AC0_11D1_8100_444553540000__INCLUDED_)
#define AFX_STADIUMPLAN_H__610A2FC2_2AC0_11D1_8100_444553540000__INCLUDED_



class CStadiumPlan
{
public:

    CStadiumPlan(FILE* File);
    CStadiumPlan(void) {};
    ~CStadiumPlan(void) {};

	CExtByteArray*			GetPlanArrayPtr(void)
	{
		return &m_Plan;
	}

protected:
    int						m_ID;
    CExtByteArray			m_Plan;
};
//////////////////////////////////////////////////////////////////////

class CStadiumPlanList : public CPtrArray
{
    //DECLARE_DYNAMIC (CStadiumPlanList);
public:

    CStadiumPlanList(void);
    ~CStadiumPlanList(void);

    // Methods
    CStadiumPlan*			GetAt(int x)
	{
		return static_cast<CStadiumPlan*> (CPtrArray::GetAt(x));
	}

};

#endif // !defined(AFX_STADIUMPLAN_H__610A2FC2_2AC0_11D1_8100_444553540000__INCLUDED_)
