

 /*

	Youth class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
*/


#ifndef __YOUTH_H__
#define __YOUTH_H__


class CYouth : public CPerson 
{
	// Variables
private:
    CSoccerPlayer               m_Skills;

public:
	// Constructors
    CYouth();				
    CYouth(CYouth* _Youth);
    CYouth(CCountry& _rCountry, CPopularity* _pPopularity, const byte _ClubDivStrength);				// Creates a random youth

	// IO Methods
	virtual void			Write(CDataFile* _pDataFile);
	virtual void			Read(CDataFile* _pDataFile);

	// Methods
	void                    DoCheckIfRequestNewContract(CClub& _Club);
    CClub&                  GetClub();
	void					DoRegenerate(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength);
	void					DoWeeklyTasks();
    void					DoWeeklyTasks(CClub& _Club);
    void					DoEndOfSeason();
    void					DoGenerateRandomContract(CContract& _ContractOffered, CCountry& _rCountry);
    void                    DoResetVariables();
    void                    PromoteYouth();
	void					SetTrainingSkillsPercent(const byte _x);
	void					SetTrainingPacePercent(const byte _x);
	void					SetTrainingStrengthPercent(const byte _x);
	CString					GetContractResponseStr(eContractOfferResponse _eResponse, CContract* _ContractOffered);
	eContractOfferResponse	TryAcceptContract(CContract* _ContractRequired, CCountry& _rCountry, CContract* _ContractOffered);
	CSoccerPlayer&			GetSkills();							
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CYouthList : public CPointerArray<CYouth>
{
public:
	// Constructor
    CYouthList() {};
	~CYouthList();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	void					DoCreate(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength);
    void                    DoWeeklyTasks(CClub& _Club);
	void					DoWeeklyTasks(CCountry& _rCountry, CPopularity* _pPopularity, const byte _ClubDivStrength);
	void					DoEndOfSeason(CClub* _Club);
	void					DoEmployYouth(CCountry& _Country, CPopularity* _Popularity, const byte _ClubDivStrength);
	ushort					Find(CYouth* _Youth);
    void					Remove(const ushort _YouthNum);
    void                    DoRequestNewContracts(CClub& _Club);
	void 					SackYouth(CYouth* _Youth);

	void 					FillWithRandomYouths(CCountry &_Country, CPopularity *_pPopularity, const byte _ClubDivStrength);
};
#endif
