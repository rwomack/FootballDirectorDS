//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdstrategyformationscreen.h
// Description : Screen for setting the formation used by the squad.
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2008 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDSTRATEGYFORMATIONSCREEN_H_INCLUDED
#define FDSTRATEGYFORMATIONSCREEN_H_INCLUDED

#include <fdscreen.h>
#include <framework/lsigslot.h>

class CClub;
class CMatchPlay;

BEGIN_L_NAMESPACE

class FDMainMenuUI;
class FDPlayerBlobWidget;
class FDButtonWidget;
class FDSquadTableWidget;
class FDDragWidget;

//////////////////////////////////////////////////////////////////////////
// FDStrategyFormationScreen - Screen for setting the squad formation
//////////////////////////////////////////////////////////////////////////
class FDStrategyFormationScreen : public FDScreen, public lHasSlots<>
{
public:
	FDStrategyFormationScreen( FDMainMenuUI *parent );
	virtual ~FDStrategyFormationScreen();

	// FDScreen interface
	virtual void DoShow();
	virtual void DoHide();

	void SetClub(CClub *pClub);
	void SetMatchPlay( CMatchPlay *pMatchPlay );

private:
	void Init();
	void RefreshInformation();
	void RefreshFormationDisplay();

	void OnFormationSelect( FDComboWidget *, int );

	void OnBlobDropped(FDPlayerBlobWidget *pBlob, lUIPoint cDropLocation);
	void OnSignalSelect( FDTableWidget * table, CPlayer *pPlayer, int row);
  void OnPlayerDropped(FDTableWidget *pTable, int iRow, int iCol, lUIPoint pos);

	lUIScreenWidget *m_formationScreen;
	lUIScreenWidget *m_formationScreenSub;

  FDDragWidget *		  m_dragWidget;
	FDSquadTableWidget *m_table;
	lUISpriteWidget       * m_squadFormation;
	FDComboWidget         * m_squadFormationCombo;
	CClub *m_pClub;

	CMatchPlay        * m_matchPlay;


	lArray<FDPlayerBlobWidget *> m_playerBlobList;
};

END_L_NAMESPACE

#endif // #ifndef FDSTRATEGYFORMATIONSCREEN_H_INCLUDED
