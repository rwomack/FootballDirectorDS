#ifndef TESTBED
	class CGridColumn;
	class CGridRow;
	class CSortedStringList;
	class CBasicDisplay;
	class CPNGDrawInfo;
	class CExtCheckBox;

	// Grid Styles
	#define LC_NORMAL			0x01
	#define LC_SHOWCOLUMNS		0x02
	#define LC_SWAPPABLECOLUMNS 0x04
	#define LC_NOHIGHLIGHT		0x08
	#define LC_NOBORDER			0x10
	#define LC_NOBACKGROUND		0x20

	enum ePNGButtonStatus 
	{
		BUTTONSTATENORMAL,
		BUTTONSTATEDOWN,
		BUTTONSTATERELEASED,
		BUTTONSTATEHIDDEN,
		BUTTONSTATECURRENT,
	};

	enum
	{
		eUPDOWNWIDTH = 20,
		eUPDOWNHEIGHT = 20,
		TOPY = 46,			// Allow for header bitmap at top of screen
		DIVISIONSELECTOR_TOPY = TOPY + eUPDOWNHEIGHT + 2,
		DATEDIVISIONSELECTOR_TOPY = TOPY + eUPDOWNHEIGHT * 2 + 4,
		CANCELBUTTONWIDTH = 68,
		CANCELBUTTONHEIGHT = 21,
		CONTINUEBUTTONWIDTH = 230,
		CONTINUEBUTTONHEIGHT = 21,	
	};

	// Column Styles
	const int COL_TEXT =      	0x01;
	const int COL_BITMAP =     	0x02;
	const int COL_COUNTER =    	0x04;
	const int COL_ALIGNL =     	0x08;
	const int COL_ALIGNR =     	0x10;
	const int COL_ALIGNC =     	0x20;
	const int COL_NUMERIC =		0x40;
	const int COL_MONEY =		0x80;
	const int COL_DATE =		0x100;
	const int COL_PERCENTAGE =  0x200;

	const int COL_NORMAL =		(COL_TEXT | COL_ALIGNL);
	const int COL_ALIGNMENT =	(COL_ALIGNL | COL_ALIGNR | COL_ALIGNC);
	const int COL_NUMERICR =	(COL_NUMERIC | COL_ALIGNR);

	enum ePointerType
	{
		PLAYER_POINTER,
		HEADER,
		MANAGERCONTRACT,
		EMPLOYEECONTRACT,
		PLAYERCONTRACT,
		YOUTHCONTRACT,

		MAX_POINTER
	};

	enum eSortOrder
	{
		UNSORTED,
		SORT_LOW,
		SORT_HIGH
	};


	#include "voimage.h"
	#include "OffScreenBuffer.h"
	#include "PNGDrawInfo.h"
	#include "PNGButton.h"
	#include "SortedStringList.h"
	#include "KeyScrollBar.h"
	#include "GridColumn.h"
	#include "Grid.h"
	#include "GridRow.h"
	#include "BasicDisplay.h"
	#include "TextRegion.h"
	#include "TextSelectorRegion.h"
	#include "TitledTextSelectorRegion.h"
	#include "CurrentTextSelector.h"
	#include "CurrentButtonSelector.h"
	#include "ExtRect.h"
	#include "ExtCheckBox.h"
	#include "CurrentPercentSelector.h"
	#include "PercentageSelectorRegion.h"
#endif
