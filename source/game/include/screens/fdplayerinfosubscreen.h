//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdplayerinfoscreen.h
// Description : FDPlayerInfoSubScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef FDPLAYERINFOSUBSCREEN_H_INCLUDED
#define FDPLAYERINFOSUBSCREEN_H_INCLUDED

#include <fdscreen.h>

class CPlayer;

BEGIN_L_NAMESPACE

class FDBarWidget;
class lUIScreenWidget;
class lUITextWidget;
class lUISpriteWidget;


//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDPlayerInfoSubScreen - Player information screen (61)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDPlayerInfoSubScreen : public FDScreen
{
 public:
  FDPlayerInfoSubScreen( FDMainMenuUI *parent );
  virtual ~FDPlayerInfoSubScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDPlayerInfoSubScreen interface

  enum PlayerInfoType
  {
    PIT_NORMAL,
    PIT_WAGES,
  };

  void SetPlayerInfo( CPlayer *player, PlayerInfoType type = PIT_NORMAL );

 private:
  enum PlayerInfoFieldType
  {
	  PIFT_HANDLING,
	  PIFT_TACKLING,
	  PIFT_PASSING,
	  PIFT_SHOOTING,
	  PIFT_PACE,
    PIFT_OVERALLSKILL,
	  PIFT_FITNESS,
	  PIFT_MORALE,
	  PIFT_BEST_FOOT,
	  PIFT_AGE,
	  PIFT_DISCIPLINARY_POINTS,
    PIFT_CONTRACTEND,
	  PIFT_WAGE,
	  PIFT_VALUE,
	  PIFT_MAX
  };

  void Init();
  void SetValueTextPercentageFromInt(const PlayerInfoFieldType eField, const int iValue, const float fMaxValue, const bool bReverseColors);

  lUIScreenWidget       * m_playerScreenSub;

  lUITextWidget         * m_playerInfoName;
  lUITextWidget         * m_playerInfoPos;

  lUISpriteWidget * m_kit;
  lUISpriteWidget * m_kitBG;
  lUISpriteWidget * m_flags;

  lUISpriteWidget * m_teamBadge;
  lUISpriteWidget * m_badgeStripe1;
  lUISpriteWidget * m_badgeStripe2;


  lUISpriteWidget *m_backgroundStripes[7];
  lUISpriteWidget *m_separatorStripes[7];
  FDBarWidget *m_fieldStripes[PIFT_MAX];
  lUITextWidget *m_fieldLabels[PIFT_MAX];
  lUITextWidget *m_fieldValues[PIFT_MAX];

  PlayerInfoType m_type;
};

END_L_NAMESPACE
#endif

