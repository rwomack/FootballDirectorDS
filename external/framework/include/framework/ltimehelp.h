//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : ltimehelp.h
// Description : Timer Help functions
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2007 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LTIMEHELP_H_INCLUDED
#define LTIMEHELP_H_INCLUDED

#include <lemon/platform/lcore.h>

BEGIN_L_NAMESPACE

inline void SplitTime( int totalSeconds, int *splitSeconds, int *splitMinutes, int *splitHours = NULL )
{
  int seconds = totalSeconds % 60;
  int minutes = ((totalSeconds-seconds) % 3600) / 60;
  int hours   = (totalSeconds-(minutes*60)-seconds) / 3600;

//  printf("%d = %02d:%02d:%02d (%03d:%02d)\n", totalSeconds, hours, minutes, seconds, hours*60+minutes, seconds);

  if( splitSeconds != NULL )
  {
    *splitSeconds = seconds;
  }

  if( splitHours != NULL )
  {
    *splitHours = hours;
  } else
  {
    // add hours to minutes count
    minutes += hours * 60;
  }

  if( splitMinutes != NULL )
  {
    *splitMinutes = minutes;
  }
}


END_L_NAMESPACE

#endif

