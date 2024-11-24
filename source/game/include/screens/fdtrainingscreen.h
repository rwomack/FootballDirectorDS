//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : fdsquadscreen.h
// Description : FDSquadScreen interface
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#include <fdscreen.h>

#include <ui/luiscreenwidget.h>
#include <ui/luispritewidget.h>
#include <ui/luitextwidget.h>

#include <fdbarwidget.h>
#include <fdcombowidget.h>
#include <fdcheckboxwidget.h>
#include <fdbuttonwidget.h>
#include <fdbuttonrolloutwidget.h>
#include <fdsliderwidget.h>
#include <fdbackingwidget.h>

#ifndef FDTRAININGSCREEN_H_INCLUDED
#define FDTRAININGSCREEN_H_INCLUDED

BEGIN_L_NAMESPACE

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// FDTrainingScreen - Training screen (36)
//
//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
class FDTrainingScreen : public FDScreen, public lHasSlots<>
{
 public:
  FDTrainingScreen( FDMainMenuUI *parent );
  virtual ~FDTrainingScreen();

  // FDScreen interface
  virtual void DoShow();
  virtual void DoHide();

  // FDTrainingScreen interface
  void Update();

 private:
  enum TrainingSliderType
  {
		FDTS_FIVEASIDE = 0,
		FDTS_TACTICS,
		FDTS_SETPIECES,
		FDTS_FREETIME,
		FDTS_MAX
  };

  void Init();
  void OnSlider( lUIWidget *, float );
  void OnLocks( lUIControlWidget *);

  void GetUnlockedSliderIndices(Vector<TrainingSliderType> &cResult);
  FDSliderWidget *GetSlider(const TrainingSliderType eSlider);
  FDSliderWidget *GetSliderForCheckBox(FDCheckBoxWidget *pCheck);

  int GetChangeableValueOfSliders(Vector<TrainingSliderType> &cSliders, const bool bWantToRemovePoints);
  void DistributeChangeToSliders(Vector<TrainingSliderType> &cSliders, int *values, const int iPoints, const bool bWantToRemovePoints);

  lUIScreenWidget       * m_trainingScreen;
  lUIScreenWidget       * m_trainingScreenSub;

  FDComboWidget         * m_trainingFormation;
  lArray<lUITextWidget *> m_trainingFormationRatings;
  lArray<lUISpriteWidget *> m_backgroundStripes;
  lArray<FDBarWidget *> m_trainingFormationBars;

  FDSliderWidget        * m_trainingFiveASideSlider;
  FDSliderWidget        * m_trainingTacticsSlider;
  FDSliderWidget        * m_trainingSetPiecesSlider;
  FDSliderWidget        * m_trainingFreeTimeSlider;

  FDCheckBoxWidget        * m_trainingFiveASideButton;
  FDCheckBoxWidget        * m_trainingTacticsButton;
  FDCheckBoxWidget        * m_trainingSetPiecesButton;
  FDCheckBoxWidget        * m_trainingFreeTimeButton;

};

END_L_NAMESPACE
#endif

