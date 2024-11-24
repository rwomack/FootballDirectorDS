
/*

	ContractEndTime.h: interface for the CContractEndTime class.


	Football Director

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------
*/

#ifndef __CONTRACTENDTIME_H__
#define __CONTRACTENDTIME_H__


class CContractEndTime : public CDate
{
public:
	CContractEndTime(CContractEndTime& SomeTime);
	CContractEndTime(CDate& SomeTime);
	CContractEndTime(const uint c_nSomeTime);
	CContractEndTime() {};

	// Methods
	void					DoEndOfSeason();
	bool					HasExpired();
	ushort					SeasonsRemaining();
	void					IncreaseLength(const ushort _Years);
	ushort					WeeksRemaining();
};
#endif
