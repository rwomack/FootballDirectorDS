

/*
    Class - CSurgicalInjury


	Football Director

	(c) RW Software 1994 - 2005

  ---------------------------------------------------------------
 */



#ifndef __SURGICALINJURY_H__
#define __SURGICALINJURY_H__

class CSurgicalInjury : public CInjury
{
public:
	CSurgicalInjury(void);
    CSurgicalInjury& operator = (const CSurgicalInjury& p);

    // IO Methods
    void          Write(CDataFile* File);
    void          Read(CDataFile* File);

	const int				getSurgeryResult(void) const;
	const bool				getRequiresPhysioAttention(void) const;
	CString					SurgeryResultStr(CClub& _Club);
    bool                    DailyImprove(CClub& _Club);
	void					DoCreateInjury(CPlayer& _Player, CClub& _Club, const CInjuryType::eInjurySeverity aSeverity, bool bTraining );
	void					DoCreateEvent(CPlayer& _Player, CClub& _Club);
    void					DoResetVariables(void);

    // Variables
private:
	uint8					m_WeeksToSurgery;
	int						m_SurgeryResult;
	bool					m_PhysioAttention;
};
#endif