

/*

    Tactic.h: interface for the CTactic class.


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */



#if !defined(AFX_TACTIC_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_TACTIC_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_


	class CTextSelector;
	class CExtPoint;
    class CTactic;

	const ushort			NUMCUSTOMFORMATIONS = 0;
	static const int		NUMBERPLAYERS = PLAYERSINTEAM - 1;


class CTactic
{
	// Variables
private:
    CExtString              m_Name;
	CExtPoint               m_Formations[NUMBERPLAYERS];
	CTeamTactic				m_TeamTactic;
	byte					m_Familiarity;
	byte					m_BaseFamiliarity;
	byte					m_FamiliarityExperience;
	
public:
	CTactic();
	CTactic(const int Title, const POINT Formations[NUMBERPLAYERS], const bool IsCustomizable = false);

	CTactic& operator		=(const CTactic& p);
	void					Copy(const CTactic& _Tactic);

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	CTeamTactic&			GetTeamTactic();
	void					AdjustFamiliarity();
	byte					CountMatchingPositions(CTactic& _Tactic);
    CString		            FamiliarityStr();
    byte					GetFamiliarity();
	ushort					GetPlayerPositionX(ushort _PositionNum) const;
	ushort					GetPlayerPositionY(ushort _PositionNum) const;
	int						GetEffectiveness();
	void					DoInitialiseFromResource(const int _Title, CBinaryReader& _Resource);
	bool					IsDefender(int _iPlayerNum) const;
	bool					IsMidfielder(int iPlayerNum) const;
	bool					IsAttacker(int iPlayerNum) const;
	int						GetNumberOfPlayersAtPosition(const ePlayerPosition ePosition) const;
	void					ReCreate();
	ePlayerPosition			PlayerPosition(ushort _PlayerNum) const;
	ushort					PlayerAtPosition(CExtPoint& _Coords);
	void					DoTrain(const byte _Hours);
	void 					SetFamiliarity(const byte _Value);
	void 					SetFamiliarityExperience(const byte _Value);
	void					DoSquadChanged();
    CString					TitleStr();
	CString					TitleAndFamiliarityStr();
	void					DoPlayedMatch(const byte _OurGoals, const byte _TheirGoals);
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CTacticList
{
	// Variables
private:
    ushort					m_TacticsInUse;
	CTactic					m_TacticList[10 + NUMCUSTOMFORMATIONS];

public:
    CTacticList();             // Constructor

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
	byte					GetDefenceEffectiveness(const ushort _PlayerNumber);
	byte					GetMidfieldEffectiveness(const ushort _PlayerNumber);
	byte					GetAttackEffectiveness(const ushort _PlayerNumber);
	byte					GetHeadingEffectiveness(const ushort _PlayerNumber);
	byte					GetShootingEffectiveness(const ushort _PlayerNumber);
	CTactic&				GetAt(const ushort _TacticID);
	CTactic&				GetCurrentTactics();							
	CTeamTactic&			GetCurrentTeamTactic();
	int						GetPlayerPositionX(const ushort _PositionNUm, const ushort _Formation = NOPLAYER);
	int						GetPlayerPositionY(const ushort _Value, const ushort _Formation = NOPLAYER);
	void					DoSquadChanged();
	byte					TotalStandardTactics();
	void					DoBuildTitleAndFamiliarityStrList(CTextSelector& _TextSelector);
    ushort					GetTacticsInUse();
	void 					SetTacticsInUse(const ushort _TacticID);
	void					SetCurrentStyleOfPlay(const byte _Value);
	void					SetCurrentMarkingStyle(const byte _Value);
	void 					SetCurrentAggression(const byte _Value);
	void 					SetCurrentCounterAttack(bool _Value);
	void 					SetCurrentOffsideTrap(bool _Value);
	int						GetSize();
    void                    ReCreate();
#ifdef SOCCERTACTICSARROWS
    byte					GetPlayerArrows(const ushort _WhichTacticsID);
    void					SetPlayerArrows(const ushort _WhichTacticsID, const byte y);
#endif
#ifdef CUSTOMFORMATIONS
	void					CalculateFamiliarity(CTactic& _Tactics);
    void					SetPlayerPosition(ushort x, CExtPoint* y);
    void					SetPlayerPositionX(ushort x, ushort y);
    void					SetPlayerPositionY(ushort x, ushort y);
    void                    SetTitle(const ushort Which, const CString &String);
    bool                    IsCustomizable(int x)
	{
		return GetAt(x)->IsCustomizable();
	}
#endif
};
#endif // !defined(AFX_TACTIC_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_)
