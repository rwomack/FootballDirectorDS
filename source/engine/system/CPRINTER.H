//	C++ Class for Printer Support - RW SOftware

//  25/04/96	15:30
//	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// Printer Support Class

#ifndef __PRINT_H__
#define __PRINT_H__

class CPrinter : public CObject
{
	DECLARE_DYNAMIC (CPrinter);
	public:
		// Constructors
			CPrinter();

		// Methods
			void	InitPrinter();
			void	DeletePrinter();

		// Variables
			CDC		printDC;
};

#endif
