#ifndef DEFINES_H_INCLUDED
#define DEFINES_H_INCLUDED

//#ifdef WIN32
typedef unsigned char BYTE;

	struct					uint24
	{
		BYTE	m_Byte[3];
	};

	typedef signed char		sbyte;
	typedef unsigned char	byte;
  typedef unsigned char uint8;
  typedef unsigned short  ushort;
  typedef unsigned short  uint16;
  typedef unsigned short  UInt16;
  typedef unsigned int    uint;
  typedef unsigned int    uint32;
	#define null NULL
//#endif

	typedef union
	{
		typedef struct
		{
			uint b0 : 8;
			uint b1 : 8;
			uint b2 : 8;
			uint b3 : 8;
		} bytes;
		bytes by;
		int intValue;
	} fourbyte;

	typedef union
	{
		typedef struct
		{
			uint b0 : 8;
			uint b1 : 8;
		} bytes;
		bytes by;
		short intValue;
	} twobyte;

#include "Random.h"

  #define         OKBOX       MB_OK + MB_ICONEXCLAMATION //+ MB_APPLMODAL + MB_DEFBUTTON1
  #define         YESNOBOX    MB_YESNO + MB_ICONEXCLAMATION //+ MB_APPLMODAL + MB_DEFBUTTON1
  #define         HELPBOX     MB_RETRYCANCEL //+ MB_APPLMODAL + MB_DEFBUTTON1

	const int				GAPY = 16;			// With normal size font, gap between rows of text
	const int				TABHEIGHT = 18;		// Optimum height of PPC tab control
	const uint				NOTFOUND = 0xffffffff;
	const ushort			wNOTFOUND = 0xffff;
	const byte				bNOTFOUND = 0xff;

	enum ePNGButtonStatus
	{
		BUTTONSTATENORMAL,
		BUTTONSTATEDOWN,
		BUTTONSTATERELEASED,
		BUTTONSTATEHIDDEN,
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

	const int  COL_NORMAL =		(COL_TEXT | COL_ALIGNL);
	const int  COL_ALIGNMENT =	(COL_ALIGNL | COL_ALIGNR | COL_ALIGNC);
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

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class BetweenType> bool IsBetween(BetweenType ValToTest, BetweenType TopVal, BetweenType BottomVal)
{
	if(ValToTest < BottomVal || ValToTest > TopVal)
	{
		return false;
	}
	else
	{
		return true;
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif
