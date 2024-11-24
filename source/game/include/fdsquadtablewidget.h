//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadtablewidget.h
// Description : Table that fills itself with basic squad data. Used for the strategy screens.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDSQUADTABLEWIDGETUI_H_INCLUDED
#define FDSQUADTABLEWIDGETUI_H_INCLUDED

#include <fdtablewidget.h>
#include <framework/lsigslot.h>
#include <ui/luiwidget.h>

class CClub;
class CPlayer;
class CMatchPlay;

BEGIN_L_NAMESPACE

class FDScrollBarWidget;

class FDSquadTableWidget : public lUIWidget, public lHasSlots<>
{
	LUI_RTTI( FDSquadTableWidget, lUIWidget );
public:
	enum SquadTableInfoType
	{
		INFOTOSHOW_SKILLS = 0,
		INFOTOSHOW_STATUS,
		INFOTOSHOW_CONTRACT,
		INFOTOSHOW_STATS,
		INFOTOSHOW_WELLBEING,
		INFOTOSHOW_MINIMAL,
		INFOTOSHOW_ROLE,
	};

	enum SquadTableSortType
	{
		SORT_NUMBER = 0,
		SORT_NAME,
		SORT_INVALID,
	};

	static void ClassInitialiser();

	FDSquadTableWidget();
	FDSquadTableWidget( lUIWidget * parent, luint32_t widgetFlags, const char * id,
					const lUIWidgetPosition anchor = LUI_ANCHOR_DEFAULT,
					const lUIWidgetPosition pos = LUI_POS_DEFAULT,
					const lUIWidgetSize size = LUI_SIZE_DEFAULT );

	virtual ~FDSquadTableWidget();

	virtual lUIPoint GetIdealSize() const;

	void SetClub(CClub *pClub);
	void RefreshTable();

	FDScrollBarWidget *GetScrollBar();

  int   GetInfoToShow();
	void SetInfoToShow(const SquadTableInfoType eInfo, bool bLimitTo11 = false );
	void ClearSelection();

	lSignal3<FDTableWidget *, CPlayer *, int> SignalRowSelect;
	lSignal2<CPlayer *, lUIPoint> SignalPlayerDropped;

	int GetPlayerSquadIndex( int row );

	FDTableWidget *GetUnderlyingTable();
  CPlayer *GetLastSelectedPlayer() { return m_pLastSelectedPlayer; }
  void SelectPlayer(CPlayer *, bool bAdjustScroll=true);

  void SetMatchPlay( CMatchPlay * matchplay ) { m_matchPlay = matchplay; };

private:
	struct FDPlayerTemp
	{
		FDPlayerTemp( CPlayer * _player, int _origNumber )
		{
		player = _player;
		origNumber = _origNumber;
		}

		CPlayer * player;
		int origNumber;
	};

	void Init(const lUIPoint cSize);
	void SetupTableColumns();
	void SetupPlayerData();
	void AddPlayerToTable(CPlayer *pPlayer, const int ListPos);

	void OnSquadRowSelect( FDTableWidget *, int row );
	void OnSquadCellSelect( FDTableWidget *, int row, int col );
	void OnSquadPlayerDropped(FDTableWidget *, int row, int col, lUIPoint cPos);

  void RenderFaceCallback(FDTableWidget *, luint32_t, luint32_t, lUIRect);



	FDTableWidget *m_table;
	FDScrollBarWidget *m_scroll;
	CClub *m_pClub;
	SquadTableInfoType m_eInfoShown;
	lArray<FDPlayerTemp> m_playerList;
  CPlayer           *m_pLastSelectedPlayer;

	bool m_bLimitTo11;

	SquadTableSortType m_sortType;
	bool m_ascendingSort;
	int m_selectedColumnHeader;

	lSharedPtr<lSprite> m_faces;

	CMatchPlay * m_matchPlay;
};

END_L_NAMESPACE

#endif // #ifndef FDSQUADTABLEWIDGETUI_H_INCLUDED
