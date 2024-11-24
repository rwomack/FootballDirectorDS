
/*

	ContractEndTime.h: interface for the CContractEndTime class.


	Football Director

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------
*/

#ifndef __CONTRACTENDTIME_H__
#define __CONTRACTENDTIME_H__

#ifdef NDS
#include <date.h>
#endif

class CContractEndTime : public CDate
{
public:
	CContractEndTime(CContractEndTime& SomeTime);
	CContractEndTime(CDate& SomeTime);
	CContractEndTime(const uint c_nSomeTime);
	CContractEndTime() {};

	// Methods
	uint					ContractLengthInDays();
	void					DoEndOfSeason();
	bool					HasExpired();
	void					IncreaseLength(const ushort _Years);
	void					ResetDate();
	void					SetEndDate(const ushort _Years);
	ushort					SeasonsRemaining();
	ushort					WeeksRemaining();
};
#endif
