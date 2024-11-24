//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// File        : lcorethread.h
// Description : Thread system include file
// Notes       :
//
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Copyright (C) 2005-2009 Four Door Lemon Ltd. All Rights Reserved.

#ifndef LCORETHREAD_H_INCLUDED
#define LCORETHREAD_H_INCLUDED

#include "lcore.h"


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Thread / Mutex system
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

#ifdef __cplusplus
extern "C" {
#endif

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍
// TODO
// Functions for lock free (exchange functions on platforms)
// TLS utility functions  ?
// Locked Increment / Decrement functions
// Code to support lock free linked lists etc
//
// Wait on multiple mutex / events ?
// Thread pool template - create multiple Thread classes and spawn
// Being able to wake up one thread waiting on your object, or wake up all threads (a broadcast).
// Waitable timers

//鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍鞍

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Thread
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Thread handle
// Remarks: An opaque thread handle returned by lPlatform_ThreadCreate and used by
// the threading functions.
//
// See Also: lPlatform_ThreadCreate
//
typedef void * lTHREAD_HANDLE;

//##
// Invalid thread handle
// Remarks: This value represents an invalid thread handle
//
// See Also: lTHREAD_HANDLE,lPlatform_ThreadCreate
//
#define lINVALID_THREAD_HANDLE (0)

//##
// Describes the priority of a thread
//
enum L_THREAD_PRIORITY
{
  L_THREAD_PRIORITY_IDLE  = 0,
  L_THREAD_PRIORITY_LOW,
  L_THREAD_PRIORITY_BELOWNORMAL,
  L_THREAD_PRIORITY_NORMAL,
  L_THREAD_PRIORITY_ABOVENORMAL,
  L_THREAD_PRIORITY_HIGH,
};

//##
// Describes the status of a thread
//
enum L_THREAD_STATUS
{
  L_THREAD_STATUS_DEAD  = 0,
  L_THREAD_STATUS_PAUSED,
  L_THREAD_STATUS_ALIVE,
};

//##
// Describe parameters for waits
enum
{
  L_THREAD_WAIT_INFINITE = 0xffffffff
};

typedef void *lThreadFunction( void * );
typedef lThreadFunction * plThreadFunction;

LPLATFORM_API lTHREAD_HANDLE       lPlatform_ThreadCreate( plThreadFunction func, void *param );
LPLATFORM_API void                 lPlatform_ThreadDestroy( lTHREAD_HANDLE thread );
LPLATFORM_API void                 lPlatform_ThreadTerminate( lTHREAD_HANDLE thread, luint32_t exitCode );
LPLATFORM_API void                 lPlatform_ThreadSuspend( lTHREAD_HANDLE thread );
LPLATFORM_API void                 lPlatform_ThreadResume( lTHREAD_HANDLE thread );
LPLATFORM_API enum L_THREAD_STATUS lPlatform_ThreadStatus( lTHREAD_HANDLE thread );
LPLATFORM_API bool                 lPlatform_ThreadWait( lTHREAD_HANDLE thread, int ms );
LPLATFORM_API void                 lPlatform_ThreadSetPriority( lTHREAD_HANDLE thread, enum L_THREAD_PRIORITY priority );
LPLATFORM_API bool                 lPlatform_ThreadGetExitCode( lTHREAD_HANDLE thread, luint32_t *exitCode );

// to be called in a thread
LPLATFORM_API lTHREAD_HANDLE       lPlatform_ThreadGetCurrent();
LPLATFORM_API void                 lPlatform_ThreadYield();
LPLATFORM_API void                 lPlatform_ThreadSleep( int ms );

// Thread class which allows you to define a function and then runs a function in the class in the thread, it
// waits on the start of the thread

//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Mutex
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Mutex handle
// Remarks: Mutex handle returned by lPlatform_MutexCreate and used by
// the mutex functions.
//
// See Also: lPlatform_MutexCreate
//
typedef void * lMUTEX_HANDLE;

//##
// Invalid mutex handle
// Remarks: This value represents an invalid mutex handle
//
// See Also: lMUTEX_HANDLE,lPlatform_MutexCreate
//
#define lINVALID_MUTEX_HANDLE (0)

LPLATFORM_API lMUTEX_HANDLE  lPlatform_MutexCreate();
LPLATFORM_API void           lPlatform_MutexDestroy( lMUTEX_HANDLE mutex );
LPLATFORM_API bool           lPlatform_MutexLock( lMUTEX_HANDLE mutex );
LPLATFORM_API bool           lPlatform_MutexLockTimeout( lMUTEX_HANDLE mutex, int timeoutMS );
LPLATFORM_API void           lPlatform_MutexUnlock( lMUTEX_HANDLE mutex );

// Implement MutexLock class which performs a lock in constructor and unlocks in deconstructor


//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北
// Event
//北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北北

//##
// Event handle
// Remarks: Event handle returned by lPlatform_EventCreate and used by
// the mutex functions.
//
// See Also: lPlatform_EventCreate
//
typedef void * lEVENT_HANDLE;

//##
// Invalid mutex handle
// Remarks: This value represents an invalid mutex handle
//
// See Also: lEVENT_HANDLE,lPlatform_EventCreate
//
#define lINVALID_EVENT_HANDLE (0)

//##
// Create an event synchronisation object
// lPlatform_EventCreate
// In:
//   bAutoReset - if true then when the event is posted using Post() it will automatically be reset
//                after waiting threads have been triggered. If false then lPlatform_EventReset() must
//                be called to reset the state
LPLATFORM_API lEVENT_HANDLE  lPlatform_EventCreate( bool bAutoReset );
LPLATFORM_API void           lPlatform_EventDestroy( lEVENT_HANDLE event );
LPLATFORM_API void           lPlatform_EventPost( lEVENT_HANDLE event );
LPLATFORM_API void           lPlatform_EventPulse( lEVENT_HANDLE event );
LPLATFORM_API void           lPlatform_EventReset( lEVENT_HANDLE event );
LPLATFORM_API bool           lPlatform_EventWait( lEVENT_HANDLE event, int timeoutMS );

//##
// Wait for one of multiple events to be set
LPLATFORM_API bool           lPlatform_EventWaitMultiple( luint32_t eventCount, lEVENT_HANDLE *eventList, int timeoutMS );
LPLATFORM_API bool           lPlatform_EventQuery( lEVENT_HANDLE event );


#ifdef __cplusplus
}
#endif


#endif

