
/*

	File:- PlayerSearch.h


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/

#ifndef PLAYERSEARCH_H_INCLUDED
#define PLAYERSEARCH_H_INCLUDED

#include <SoccerDefines.h>

	const uint MONEYSTEPS[] =
	{
		0, 100000, 250000, 500000, 1000000,
		5000000, 7500000, 10000000, 15000000, 20000000, 30000000, 40000000
	};
	const uint NUMMONEYSTEPS = 12;


class CPlayerSearch
{
public:
	// Constructors
    CPlayerSearch();
	CPlayerSearch(bool FindThisPos);

    // IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	enum eSearchStatusMask
	{
		INCLUDEONTRANSFERLIST = 0x01,
		INCLUDEONLOANLIST = 0x02,
		INCLUDEINJURED = 0x04,
		INCLUDESUSPENDED = 0x08,
		INCLUDEOUTOFCONTRACT = 0x10,
		INCLUDENONLISTED = 0x20
	};

    // Methods
  void          SetMaximumPlayers( int max );
  int           GetMaximumPlayers() { return m_maxPlayers; }

	void					DoAddToList(const ushort _PlayerID, const ClubID _ClubID, const uint _PlayerValue);
	void					DoRemovePlayer(const ushort _PlayerListIndex);
	void					DoEmpty();
	bool					IsEmpty();
	float					GetExchangeRate();
	bool					GetIncludeFlag(const eSearchStatusMask _FlagToTest);
	CNumericArray<ushort>&	GetPlayerList();
	CNumericArray<uint>&	GetPlayerValueList();
	CNumericArray<ClubID>&	GetPlayerClubNumberList();
	ePlayerPosition			GetFindPosn();
	void					SetListCreated(const bool _ListCreated);
	byte					GetMinAge();
	byte					GetMaxAge();
	byte					GetMinSkill();
	byte					GetMaxSkill();
	uint					GetMinPrice();
	uint					GetMaxPrice();
    int						GetMinPriceStep();
    int						GetMaxPriceStep();
	uint					GetSize();
	void					SetFindPosition(ePlayerPosition _Position);
	void					SetMinPrice(const uint _Value);
	void					SetMaxPrice(const uint _Value);
	void					SetMinPriceStep(const int _Value);
	void					SetMaxPriceStep(const int _Value);
	void					SetIncludeFlag(const eSearchStatusMask _FlagToSet);
	void					SetIncludeFlags(const byte _Flags);
	void					SetOutOfContract(bool _Value);
	void					SetMinAge(byte _Value);
	void 					SetMaxAge(byte _Value);
	void 					SetMinSkill(byte _Value);
	void 					SetMaxSkill(byte _Value);
	void 					SetExchangeRate(float _Value);
    void					SetDefaults();
	void					DoInitSearch(byte _MinAge, byte _MaxAge, byte _MinSkill, byte _MaxSkill, uint _MinPrice, uint _MaxPrice, const byte _Flags);
	CString					MaxPriceStr();
	CString					MinPriceStr();

	// Variables
private:
  float         m_ExchangeRate;
  uint          m_MinimumPrice;
  uint          m_MaximumPrice;
  ePlayerPosition     m_FindPosn;
  byte          m_MinimumAge;
  byte          m_MaximumAge;
  byte          m_MinimumSkill;
  byte          m_MaximumSkill;
  int           m_maxPlayers;
  CNumericArray<ushort> m_PlayerList;
	CNumericArray<uint>		m_PlayerValueList;
	CNumericArray<ClubID>	m_PlayerClubNumberList;
	byte					m_Flags;
	bool					m_bListCreated;
};
#endif
