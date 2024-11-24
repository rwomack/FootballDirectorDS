#if !defined(MANAGERCONTRACTOFFER_INCLUDED_)
#define MANAGERCONTRACTOFFER_INCLUDED_

/*

	ManagerContractOffer.h

	Football Director

	(c) Rw Software 1994 - 2008

~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

*/



class CManagerContractOffer
{
public:
	CManagerContractOffer();
	~CManagerContractOffer();

	void					DoResetVariables();
	void					DoCalculateOffer(const uint _WageOffer, const ushort _ContractOffer);
	void					SetWageOffer(const uint _Amount);
	void					SetContractOffer(const ushort _Years);
	uint					GetWageOffer() const;
	ushort					GetContractOffer() const;
	void					SetDetails(const uint _Amount, const ushort _Years);

	// Variables
private:
	uint					m_WageOffer;
	ushort					m_ContractOffer;
};
#endif