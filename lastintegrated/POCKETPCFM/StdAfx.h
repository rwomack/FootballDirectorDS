// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__B4920EA6_EE09_4EBA_9F69_98B5D081B96B__INCLUDED_)
#define AFX_STDAFX_H__B4920EA6_EE09_4EBA_9F69_98B5D081B96B__INCLUDED_

#if (_WIN32_WCE <= 200)
#error : This project does not support MFCCE 2.00 or earlier, because it requires CControlBar, available only in MFCCE 2.01 or later
#endif

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#define WINDOWS 1
#define USE_STL
//#define UNICODE
#define POCKETPC
//#define FMOD
#define DISPLAYDATE
//#define WIN_STANDARD

#define ACTIVEFRIENDLIES
#define ASSISTS
#define BALL_PITCH_LOCATION
#define SHOWTERRITORY
#define DETAILEDMANAGERHISTORY
#define USECLUBYEARFOUNDED
#define LATESTSCORES
#define ACTIVEFEDERATIONS
#define ACTIVEFEDERATIONCUPS
#define CUPGROUPS
#define AUTOWINDSCREENSHIELD
//#define	MATCHTRACKING

//#define DEBUG_LOAD_CUP_CLUBS
//#define DEBUG_LOAD_CUP_ROUNDS

#define NOTITLEMENU
//#define DUMPCOMMENTARY
//define TEST_MATCH_REPORT
//#define TEST_DIALOGS

#pragma warning(disable: 4060 4244 4699; 1: 4706)

	class CFootballDirectorApp;
	extern CFootballDirectorApp	theApp;

	enum eView
	{
		NO_VIEW,
		SCREEN03_STATUSVIEW,
		SCREEN05_TEAMVIEW,
		SCREEN50_TABLEVIEW,
		SCREEN11_NEWSVIEW,
		SCREEN26_ACCOUNTSVIEW,
		SCREEN18_CLUBFIXTURESVIEW,
		SCREEN28_PROGRESSVIEW,
		SCREEN48_TRANSFERMARKETVIEW,
		SCREEN08_CONTRACTVIEW,
		SCREEN22_BANKVIEW,
		SCREEN39_JOBCENTREVIEW,
		SCREEN41_YOUTHSQUADVIEW,
		SCREEN36_TRAININGVIEW,
		SCREEN02_CUPSCHEDULEVIEW,
		SCREEN09_CLUBRECORDSVIEW,
		SCREEN19_LEAGUEFIXTURESVIEW,
		SCREEN20_CUPFIXTURESVIEW,
		SCREEN27_MATCHTACTICSVIEW,
		SCREEN27_MATCHSPECIALPLAYERSVIEW,
		SCREEN27_MATCHFORMATIONVIEW,
		SCREEN33_LEADINGGOALSCORERSVIEW,
		SCREEN46_MONTHLYAWARDSVIEW,
		SCREEN87_LOADINGVIEW
	};

//#define RESOURCESFROMFILE

#include "Resource.h"
#include "Defines.h"
#include "COLOURS.h"
//#include "fdmemory.h"
//#include "dlmalloc.h"
#include "DataFile.h"
#include "BinaryResource.h"
#include "Vector.h"
#include "NumericArray.h"
#include "PointerArray.h"
#include "ExtArray.h"
#include "ExtString.h"
#include "HelpInfo.h"
#include "SystemPreferences.h"
#include "SoundVolume.h"
#include "AppPreferences.h"

#include "Date.h"
#include "RangedValue.h"
#include "NonDuplicateStringList.h"

	const int ClubColour[] =
	{
		BLUE, RED
	};

	extern CRandomNumber	RandomNumber;
	extern CExtString		g_DebugStr;


#include "SoccerClasses.h"


#ifdef FMOD
#include "fmod.h"
#include "SoundManager.h"
#endif

#include <afxdlgs.h>

	const int				PosnColour[] =
	{
		DARKGREEN, RED, BLUE, MAROON
	};

	const int COMPETITIONNAMELENGTH = 70;

#endif // !defined(AFX_STDAFX_H__B4920EA6_EE09_4EBA_9F69_98B5D081B96B__INCLUDED_)
