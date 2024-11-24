

/*

	TeamTactic.h: interface for the CTeamTactic class.


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
*/



#if !defined(AFX_TEAMTACTIC_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_TEAMTACTIC_H__258740C3_5CD7_11D1_A3F5_00001C303EC4__INCLUDED_

enum eFormation
{
	FORMATION_442,
	FORMATION_433,
	FORMATION_424,
	FORMATION_532,
	FORMATION_523,
	FORMATION_541,
	FORMATION_451,
	FORMATION_334,
	FORMATION_352,
	FORMATION_343,
	NUMFORMATIONS
};


class CTeamTactic
{
	enum eTransferStatusMasks 
	{
		PLAYONTHEBREAK = 0x01,
		OFFSIDETRAP = 0x02
	};

	// Variables
private:
	byte					m_Aggression;
	byte					m_StyleOfPlay;
	byte					m_MarkingStyle;
	byte					m_Flags;

public:
	CTeamTactic();

	CTeamTactic&			operator =(CTeamTactic* _TeamTactic);
	CTeamTactic&			operator =(CTeamTactic _TeamTactic);

	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	// Methods
	void					Copy(CTeamTactic& _TeamTactic);
	byte					GetAggression();
	byte					GetStyleOfPlay();
	byte					GetMarkingStyle();
	bool					GetOffsideTrap();
	bool					GetCounterAttack();
	void					SetAggression(const byte _Value);
	void					SetDefaults();
	void					SetStyleOfPlay(const byte _Value);
	void					SetMarkingStyle(const byte _Value);
	void					SetOffsideTrap(bool _Value);
	void					SetCounterAttack(bool _Value);
	void					ToggleOffsideTrap();
	void					ToggleCounterAttack();
};
#endif