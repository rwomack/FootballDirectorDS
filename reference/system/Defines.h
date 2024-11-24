#ifndef sbyte
	typedef signed char		sbyte;
#endif
#ifndef byte
	typedef unsigned char	byte;
#endif
#ifndef BYTE
	typedef unsigned char	BYTE;
#endif
#ifndef ushort
	typedef USHORT			ushort;
#endif
#ifndef uint
	typedef unsigned int	uint;
#endif

#ifndef null
	#define null NULL
#endif

#ifndef MB_OK
	#define MB_OK 0
#endif

#ifndef MB_ICONINFORMATION
	#define MB_ICONINFORMATION 0
#endif

#define EXT_ASSERT_POINTER(p, type) EXT_ASSERT(((p) != NULL) && AfxIsValidAddress((p), sizeof(type), FALSE))


#define EXT_ASSERT(exp)		DEBUG_ONLY((void)  ((exp) ? 1 : (AfxDebugBreak())))
	
#define DEBUG_OUTPUT(format, msg) DEBUG_ONLY(g_DebugStr.Format(_T format, msg)); DEBUG_ONLY(g_DebugStr.OutputDebug());
	struct uint24 
	{ 
		BYTE	m_Byte[3]; 
	};

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

	#define					OKBOX       0		//MB_OK + MB_ICONEXCLAMATION + MB_APPLMODAL + MB_DEFBUTTON1
	#define					YESNOBOX    0		//MB_YESNO + MB_ICONEXCLAMATION + MB_APPLMODAL + MB_DEFBUTTON1
	#define					HELPBOX		0		//MB_RETRYCANCEL + MB_APPLMODAL + MB_DEFBUTTON1

	#define MENU_HEIGHT 26
	
	const int				GAPY = 16;			// With normal size font, gap between rows of text
	const int				TABHEIGHT = 18;		// Optimum height of PPC tab control

	const uint				NOTFOUND = 0xffffffff;
	const ushort			wNOTFOUND = 0xffff;
	const byte				bNOTFOUND = 0xff;

	#define 				VK_1 		49
	#define 				VK_2 		50
	#define 				VK_3 		51
	#define 				VK_4 		52
	#define 				VK_5 		53
	#define 				VK_6 		54
	#define 				VK_7 		55
	#define 				VK_8 		56
	#define 				VK_9 		57

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
