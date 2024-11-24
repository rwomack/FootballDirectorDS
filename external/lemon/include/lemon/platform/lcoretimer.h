//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcoretimer.h
// Description : Time system include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCORETIMER_H_INCLUDED
#define LCORETIMER_H_INCLUDED

#include "lcore.h"

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Timer
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus
extern "C" {
#endif

//##
//Timer scale
//Remarks: Timer scale for use in lPlatform_TimerStop
//
enum lTIMER_SCALE
{
  L_TIMER_SECONDS  = 0,   //## Return time in seconds
  L_TIMER_MSECONDS = 1,   //## Return time in milliseconds (thousandths of a second)
};

#if defined(L_PLATFORM_WIN32) || defined(L_PLATFORM_XBOX)
//##
//Timer sample
//Remarks: This structure stores platform specific data for sampled system timers.
//See Also: lPlatform_TimerSample, lPlatform_TimerDelta
//
struct lTimerSample
{
  luint64_t tsc;       // reliable in clocks, conversion to seconds goes wrong on laptops with speedstep
  luint64_t qpc;       // system bus around 50 MHz, can jump forward or back badly
  luint32_t tickCount; // reliable, in milliseconds, wraps 32 over bits ever 49 days
};
#else
struct lTimerSample
{
  luint64_t count;
};
#endif


//##
// High resolution timer initialise
// Remarks: This eliminates any stall on the first use of the timer due to calibration.
// See Also: lPlatform_TimerStart, lPlatform_TimerStop, lPlatform_TimerSample, lPlatform_TimerDelta
//
LPLATFORM_API void lPlatform_TimerInit(void);

//##
// High resolution timer sample
// Remarks: Fills lTimerSample with timing data sampled from the system, note first use may incur a 2-8ms initialisation time.
// Out: Fills lTimerSample with system timer sample data. See lPlatform_TimerDelta
// for more details.
// See Also: lPlatform_TimerInit, lPlatform_TimerDelta
//
LPLATFORM_API void lPlatform_TimerSample(lTimerSample &);

//##
// High resolution timer delta
// Remarks: Returns time period in requested scale between the two specified samples
// In:
//   start - lTimerSample start sample
//   end   - lTimerSample stop sample
//   scale - required timer scale lTIMER_SCALE
// Out:
//   Returns the positive time period between the two samples in requested scale.
// Notes:
//   This routine will never return a negative timer period. It is assumed the
//   end sample is always be taken after the start sample.
//
// Example:
//<code>
// lTimerSample start,end;
// lPlatform_TimerSample(start);
// lPlatform_TimerSample(end);
// float seconds = lPlatform_TimerDelta( start, end, L_TIMER_SECONDS );
// float milliseconds = lPlatform_TimerStop( start, end, L_TIMER_MSECONDS );
//</code>
//
LPLATFORM_API float lPlatform_TimerDelta( const lTimerSample & start, const lTimerSample & end, enum lTIMER_SCALE scale );

//##
// Get the number of seconds passed since the first call to lPlatform_GetSeconds.
// Remarks: Returns the number of seconds passed since the first call was made to lPlatform_GetSeconds.
// A 2-8ms initialisation time may be incurred when first called.
// Out: Returns the number of seconds passed since the first call to lPlatform_GetSeconds.
// See Also: lPlatform_TimerInit.
//
LPLATFORM_API float lPlatform_GetSeconds(void);



//##
// DEPRECATED
// Get the system tick count
// Out: Returns time since the started started in milliseconds
//
LPLATFORM_API luint32_t lPlatform_TickCount(void);

//##
// DEPRECATED
// High resolution timer start
// Remarks: Returns a 64bit start time index, note first use may incur initialisation time
// Out: Returns a 64bit start time index. See lPlatform_TimerStop
// for more details.
// See Also: lPlatform_TimerInit, lPlatform_TimerStop
//
#if defined(L_HAS_INTRINSIC_INT64)
LPLATFORM_API luint64_t      lPlatform_TimerStart(void);
#endif

//##
// DEPRECATED
// High resolution timer stop
// Remarks: Returns time period in requested scale
// In:
//   start - 64bit start time index
//   scale - required timer scale lTIMER_SCALE
// Out:
//   Returns time period in requested scale
//
// Example:
//<code>
// luint64_t start = lPlatform_TimerStart();
// float seconds = lPlatform_TimerStop( start, L_TIMER_SECONDS );
// float milliseconds = lPlatform_TimerStop( start, L_TIMER_MSECONDS );
//</code>
//
#if defined(L_HAS_INTRINSIC_INT64)
LPLATFORM_API float        lPlatform_TimerStop( luint64_t start, enum lTIMER_SCALE scale );
#endif


#ifdef __cplusplus
}
#endif


#endif

