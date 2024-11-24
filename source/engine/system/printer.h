//	C++ Class for Printer Support - RW Software

//  03/03/97
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Printer Support Class

#ifndef __PRINT_H__
#define __PRINT_H__

    #define     BLACK       0x00000000
    #define     WHITE   	0x00FFFFFF
	#define 	RED	 		0x000000FF
	#define 	BLUE  		0x00FF0000
	#define		NAVY		0x00800000
	#define 	GREEN		0x0000FF00
	#define 	CYAN		0x00FFFF00
	#define		DARKGREEN	0x00008000
	#define 	YELLOW		0x0000FFFF
    #define		LIGHTYELLOW	0x000FFFFF
    #define     GREY    	0x00808080
	#define		MAROON		0x00000080
	#define		DARKCYAN	0x00808000
    #define     ORANGE      0x000080FF
    #define     MUSTARD     0x00179bae
    #define     EGGSHELL    0x00947b47
    #define     PLUM        0x003d4985
    #define     PINK        0x00c080ff
    #define     LIGHTBLUE   0x00d08c00


class CPrinter : public CObject
{
	DECLARE_DYNAMIC (CPrinter);
	public:
		// Constructors
			CPrinter(void);

		// Methods
			BOOL	InitPrinter(void);
			void	DeletePrinter(void);
			void	SetPrintAlign(HDC pDC);
			void	PrintHeader(CString DivTitle, UINT ScreenTitle, CString Date, LPCTSTR CentreHeader = NULL);
			CSize	GetPageSize(void);
			int		ConvX (int ScreenX);
			int		ConvY (int ScreenY);
			void	PrintStr(int x, int y, int ptSize, CString ptFace, COLORREF col, CString str);
			void	PrintBoldStr(int x, int y, int ptSize, CString ptFace, COLORREF col, CString str);
			void	PrintBoldTxt(int x, int y, int ptSize, CString ptFace, COLORREF col, int num);
			void	PrintTxt(int x, int y, int ptSize, CString ptFace, COLORREF col, int num);
			void	PrintNum(int x, int y, int ptSize, CString ptFace, COLORREF col, int num);
			void	PrintBoldNum(int x, int y, int ptSize, CString ptFace, COLORREF col, int num);
			void	MapModeAnisotropic(void);
			void	MapModeText(void);


		// Variables
			HDC				pDC;
			CDC*			printDC;
			CPrintDialog*	pPrintDlg;
			BOOL			pConnect;
			int				pResolution;
			int				pLength;
			int				pWidth;
			int				pfHeight;
			PLOGFONT		plf;
			HFONT			hfont, hfontOld;

};

#endif
