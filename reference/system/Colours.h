#ifndef __COLOURS_H__
#define __COLOURS_H__

#ifdef WIN32
	const COLORREF BLACK = RGB(0x00, 0x00, 0x00);						// 0
	const COLORREF OFFBLACK = RGB(0x36, 0x2F, 0x32);					//
    const COLORREF WHITE = RGB(0xff, 0xff, 0xff);
	const COLORREF RED = RGB(0xff, 0x00, 0x00);							// 267386880
	const COLORREF BLUE = RGB(0x00, 0x00, 0xff);						// 15
	const COLORREF LIGHTBLUE = RGB(0x00, 0x08, 0xd0);					// 2303
	const COLORREF MIDBLUE = RGB(0x8a, 0x99, 0xff);				
	const COLORREF NAVY = RGB(0x00, 0x00, 0x80);						
	const COLORREF GREEN = RGB(0x00, 0xf0, 0x00);
	const COLORREF BRIGHTGREEN = RGB(0x08, 0xe8, 0x00);
	const COLORREF DARKGREEN = RGB(0x30, 0x78, 0x40);
	const COLORREF CYAN = RGB(0x00, 0xff, 0xff);
	const COLORREF DARKCYAN = RGB(0x00, 0x80, 0x80);
	const COLORREF YELLOW = RGB(0xf0, 0xf0, 0x00);
    const COLORREF LIGHTYELLOW = 0x000FFF0;
	const COLORREF MUSTARD = 0x00179bae;
    const COLORREF GREY = 0x00808080;
	const COLORREF CONCRETEGREY = 0x00635D6B;
	const COLORREF MAROON = 0x00000080;
    const COLORREF ORANGE = 0x000060F0;
	const COLORREF VDARKORANGE = RGB(0xD9,0x60, 0x00);
	const COLORREF DARKORANGE = RGB(0xF6,0x87, 0x07);
	const COLORREF PALEORANGE = RGB(0xF9,0xAA, 0x19);
	const COLORREF PASTELORANGE = 0x001091F0;
    const COLORREF EGGSHELL = 0x00947b47;
    const COLORREF PLUM = 0x003d4985;
    const COLORREF PINK = 0x00c080ff;
	const COLORREF PALEGREEN = RGB(159, 224, 159);
	const COLORREF VPALEGREEN = RGB(176, 252, 176);
	const COLORREF PALEBLUE = RGB(180 & ~15, 180 & ~15, 240 & ~15);
	const COLORREF VPALEBLUE = RGB(224 & ~15, 214 & ~15, 240 & ~15);
	const COLORREF PALEYELLOW = RGB(228 & ~15, 255 & ~15, 120 & ~15);
	const COLORREF VPALEYELLOW = RGB(228 & ~15, 255 & ~15, 180 & ~15);
	const COLORREF PALERED = RGB(240 & ~15, 180 & ~15, 170 & ~15);
	const COLORREF VPALERED = RGB(240 & ~15, 210 & ~15, 210 & ~15);
    const COLORREF PALEGREY = 0x00c0c0c0;
	const COLORREF VPALEGREY = 0x00f0f0f0;
	const COLORREF TRANSCOLOUR = 0x008F00FF;
#else
	const int BLACK = 0;
	const int OFFBLACK = 0x362F32;
	const int WHITE = 0xffffff;
	const int RED = 0xff0000;
	const int BLUE = 0x0000ff;
	const int LIGHTBLUE = 0x0008d0;
	const int MIDBLUE = 0x8a99ff;
	const int NAVY = 0x000080;
	const int GREEN = 0x00f000;
	const int BRIGHTGREEN = 0x08e800;
	const int DARKGREEN = 0x307840;
	const int CYAN = 0x00ffff;
	const int DARKCYAN = 0x008080;
	const int YELLOW = 0xf0f000;
	const int LIGHTYELLOW = 0x000FFF0;
	const int MUSTARD = 0x00179bae;
	const int GREY = 0x00808080;
	const int CONCRETEGREY = 0x00635D6B;
	const int MAROON = 0x00000080;
	const int ORANGE = 0x000060F0;
	const int VDARKORANGE = 0xD96000;
	const int DARKORANGE = 0xF68707;
	const int PALEORANGE = 0xF9AA19;
	const int PASTELORANGE = 0x001091F0;
	const int EGGSHELL = 0x00947b47;
	const int PLUM = 0x003d4985;
	const int PINK = 0x00c080ff;
	const int PALEGREEN = 159;
	const int VPALEGREEN = 176;
	const int PALEBLUE = 15;
	const int VPALEBLUE = 15;
	const int PALEYELLOW = 15;
	const int VPALEYELLOW = 15;
	const int PALERED = 70;
	const int VPALERED = 10;
	const int PALEGREY = 0x00c0c0c0;
	const int VPALEGREY = 0x00f0f0f0;
	const int TRANSCOLOUR = 0x008F00FF;
#endif
#endif