
/*

	PlayOff Class


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------
 */



#if !defined(AFX_PLAYOFF_H__F985DB61_90B3_11D1_A3F5_00001C303EC4__INCLUDED_)
#define AFX_PLAYOFF_H__F985DB61_90B3_11D1_A3F5_00001C303EC4__INCLUDED_




class CPlayOff : public CCup
{
public:
    CPlayOff();

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

    // Methods
    void                    DeleteAll();
	void					DoBuildFixtureList(const eDivisionMatchType _eMatchType, CSoccerResultList& _ResultList);
	void					DoDraw();
	void					DoEndOfSeason();
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	CCup*					GetCup();
	byte					GetNumberPromoted()	const;
	void					GetWinners(CNumericArray<ushort>& _ClubIDList, bool _AwardTrophy);
	byte					GetID()	const;
	ushort					GetRoundNumber(const eDivisionMatchType eMatchType);

    // Variables
private:
	byte					m_ID;
	byte					m_NumberPromoted;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////



class CPlayOffList : public CExtArray
{
public:
    CPlayOffList();  // Constructor
	~CPlayOffList();

        // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

        // Methods
	void					DoDraws();
	bool					DoPlayMatches(const bool _ShowSummary);
	CPlayOff*				FindFromId(const byte _Value);
	CPlayOff*				GetAt(const ushort _ElementNumber);
	bool					IsClubMatchToday(const CCalendar& _CurrentDate, const ushort _ClubID, CMatchInfo& _Match);

	// Variables
private:
	CPlayOff*				m_pList;
};
#endif // !defined(AFX_PLAYOFF_H__F985DB61_90B3_11D1_A3F5_00001C303EC4__INCLUDED_)
