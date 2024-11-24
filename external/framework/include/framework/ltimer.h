//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : ltimer.h
// Description : Hierarchical Timer
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LTIMER_H_INCLUDED
#define LTIMER_H_INCLUDED

#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>
#include <framework/lsigslot.h>

BEGIN_L_NAMESPACE

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lTimer
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
class lTimer
{
public:

  lTimer( lTimer* parent );
  lTimer();
  ~lTimer();

  lTimer*       AddTimer();

  void          Tick(float delta);

  void          SetTime(float time);
  void          SetMultiplier(float mul);
  float         GetTime();
  float         GetMultiplier();

  float         TimeToNextSignal();

  void          Pause();
  void          Resume();
  bool          IsPaused();

  void          Destroy();
  void          Remove( lTimer* );

  lSignal0<>    TimerSignal;

private:

  lArray<lTimer*>   m_timerList;
  float             m_fTime;
  float             m_fMultiplier;
  bool              m_bPaused;
  float             m_fCount;
  lTimer*           m_parent;
};

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// lTimerBase
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
class lTimerBase
{
public:

  static void         Tick(float delta);
  static lTimer*      AddTimer();
};



END_L_NAMESPACE

#endif

