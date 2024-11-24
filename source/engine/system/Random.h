

 /*
	C++ Classes


	(c) Rw Software 1994 - 2007

  	---------------------------------------------------------------
 */

 #ifndef __RANDOM_H__
 #define __RANDOM_H__

class CRandomNumber
{
public:
	uint					IntLessThan(const uint maxvalue);
	void					Randomize();
	void					SetSeed(const unsigned int uiSeed);

	static unsigned int s_uiLastRandomSeed;
};
#endif