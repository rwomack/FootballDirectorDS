#define NDS		// RW allow DS code to compile
#include <fdmemory.h>

//#ifdef L_PLATFORM_NDS
////NOTE: Force ARM mode on engine side (fdmainmenuui.h will override)
//#include <nitro/code32.h>
//#endif

#ifdef L_PLATFORM_NDS

//NOTE: All constants treated as float
#pragma float_constants on

// disable DS warnings
#pragma showmessagenumber on
#pragma warning off (10411)             // incorrect spelt headers
#pragma warning off (10480)             // deleting void * undefined
#pragma warning off (10107)             // identifier expected

#else

#define _CRT_SECURE_NO_DEPRECATE        // disable deprecation of CRT functions

#endif




#include "_script.h"

// BAROG
//#define   WINVER 0x0501
//#define VC_EXTRALEAN    // Exclude rarely-used stuff from Windows headers
//#include <afxwin.h>         // MFC core and standard components
//#include <afxext.h>         // MFC extensions
// BAROG

#include <lemon/platform/lcore.h>
#include <lemon/containers/larray.h>
#include <lemon/containers/lstring.h>
#include <framework/ltext.h>
using namespace L_NAMESPACE_NAME;

// if windef hasnt been included do these hacks
#ifndef DONT_DO_HACKS

#define WIN32                           // some #ifdefs rely on this for function usage (memset instead of MemSet)

// type hacks
typedef int HMODULE;                    // hack
typedef int HGLOBAL;                    // hack
typedef int HRSRC;                    // hack
typedef int HDC;                    // hack
typedef void VOID;                    // hack
typedef char * LPCTSTR;
typedef char * PBYTE;
typedef bool boolean;
typedef int DWORD;
typedef unsigned int UINT;

#ifndef L_PLATFORM_NDS
typedef bool BOOL;
const bool TRUE = true;
const bool FALSE = false;
#endif

typedef int COLORREF;
#define RGB( _r, _g, _b )      ((unsigned int)(((int)(_r)<<16) | ((int)(_g)<<8)  | (int)(_b) | 0xff000000))


#define TRACE lLogFmt

typedef struct tagPOINT
{
    int  x;
    int y;
} POINT;


#define __TEXT(quote) quote
#define TEXT(quote) __TEXT(quote)

#define __T(quote) quote
#define _T(quote) __T(quote)

// our debug new is just new
#define DEBUG_NEW new


//BAROG - used in SoccerClasses.h / Club.cpp for message boxes
  #define MB_OK                 0x001
  #define MB_ICONINFORMATION    0x002
  #define MB_YESNO              0x004
  #define MB_ICONEXCLAMATION    0x008
  #define MB_DEFBUTTON1         0x010
  #define IDYES                 0x020
  #define IDNO                  0x040
  #define MB_RETRYCANCEL        0x080
  #define MB_ENDOFGAME          0x100
//

inline int min( int a, int b ) { return (a<b) ? (a) : (b); }
inline int max( int a, int b ) { return (a>b) ? (a) : (b); }


#endif                                  // end of hacks




#include "cstring.h"
#include "ccombobox.h"
#include "cdatafile.h"

bool LoadText( CString &str, int id );

#define RESOURCESFROMFILE

#ifdef FD_RELEASEBUILD
#ifdef L_PLATFORM_NDS
#pragma warning off (10208)   // possible unwanted compare
#endif
  #define FDASSERT
  #define FDASSERT_POINTER(a,b)
  #define FDASSERTMSG(msg,eval)
#else
  #define FDASSERT          lAssert
  #define FDASSERT_POINTER(a,b)  //lAssert     //TODO
  #define FDASSERTMSG(msg,eval) lAssert(eval)      // lAssert    //TODO
#endif

#define DEBUG_OUTPUT(format, msg) g_DebugStr.Format(_T format, msg); g_DebugStr.OutputDebug();

#define EXT_ASSERT FDASSERT
#define ASSERT_POINTER FDASSERT_POINTER
#define EXT_ASSERT_POINTER FDASSERT_POINTER

#define WINDOWS
#define __AFXWIN_H__                    // HACK to show we've included stdafx.h

#define DISPLAYDATE

//#define DUMPCOMMENTARY
//#define TEST_MATCH_REPORT

//TODO: These set up the valid features

#define ACTIVEFRIENDLIES              //TODO
//#define ASSISTS
#define DETAILEDMANAGERHISTORY
#define DETAILEDMANAGERHISTORYONLYFORUSER
#define USECLUBYEARFOUNDED
#define LATESTSCORES
#define ACTIVEFEDERATIONS
#define ACTIVEFEDERATIONCUPS
#define CUPGROUPS
#define AUTOWINDSCREENSHIELD
#define PLAYER_MATCH_STATS

#include "..\..\system\Defines.h"
#include "..\..\system\HelpInfo.h"
#include "..\..\system\PointerArray.h"
#include "COLOURS.h"
#include "BinaryResource.h"

#include "VOImage.h"
#include "OffScreenBuffer.h"
#include "ExtOffScreenBuffer.h"
#include "BasicDisplay.h"
#include "BitmapFormView.h"

#include "Vector.h"


//TODO: Need better generator than this possibly
// Generate a random number between 0 & {maxvalue}
//---------------------------------------------------------------------------
inline int RandomIntLessThan(int maxvalue)
{
  return rand() % maxvalue;
}

extern void ProcessingCallback( const char *txt = NULL );

	class CFootballDirectorApp;
	extern CFootballDirectorApp	theApp;

	enum eView
	{
		STATUSVIEW = 1,
		TEAMVIEW,
		TABLEVIEW,
		NEWSVIEW,
		ACCOUNTSVIEW,
		CLUBFIXTURESVIEW,
		PROGRESSVIEW,
		TRANSFERMARKETVIEW,
		CONTRACTVIEW,
		BANKVIEW,
		JOBCENTREVIEW,
		YOUTHSQUADVIEW,
		TRAININGVIEW,
		CUPSCHEDULEVIEW,
		CLUBRECORDSVIEW,
		LEAGUEFIXTURESVIEW,
		CUPFIXTURESVIEW,
		MATCHTACTICSVIEW,
		MATCHSPECIALPLAYERSVIEW,
		MATCHFORMATIONVIEW,
		LEADINGGOALSCORERSVIEW,
		MONTHLYAWARDSVIEW
	};

  class CDebug
	{
	public:
		void Assert(bool _Test, char* _Msg)
		{
      if( !_Test )
      {
        lLogFmt("CDebug::Assert: %s\n", _Msg);
        lAssert( _Test );
      }
//TODO
//      TCHAR wStr[255];
//      MultiByteToWideChar(CP_UTF8, 0, _Msg, -1, wStr, 255);
//
//      if (!_Test)
//      {
//        FDASSERT(_Test);
//      }
		}
  };

#include "Resource.h"
#include "Defines.h"
#include "COLOURS.h"
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

  extern CRandomNumber RandomNumber;
  extern CExtString    g_DebugStr;

#include "SoccerClasses.h"


	const int				PAUSEX = 4;
	const int				PAUSEY = 24;

	const int				PosnColour[] =
	{
		DARKGREEN, PURPLE ,RED, BLUE,
	};

	const int				ClubColour[] =
	{
		BLUE, RED
	};

	enum
	{
		eDOWNX = 132,
		eUPX = eDOWNX + 81,
		eUPDOWNWIDTH = 20,
		eUPDOWNHEIGHT = 20,

	};









