
/*

    Class:- PlayerContract


    Football Director

	(c) Rw Software 1994 - 2008

	---------------------------------------------------------------
 */


 #ifndef __PLAYERCONTRACT_H__
 #define __PLAYERCONTRACT_H__


class CContract
{
public:
	CContract();

	//////////////////////////////////////////////////////////////////////////
	// ***********NOTE************
	// If this class ever gains any virtual methods, you must change CTransferNegotiation's
	// Read()/Write() to account for this. (The vtable pointer will mess up the
	// I/O as written.
	//////////////////////////////////////////////////////////////////////////

	bool operator == (const CContract* pContract);
	bool operator >= (const CContract& _Contract);
	bool operator < (const CContract* pPlayerContract);

	CContract&				Copy(const CContract *pContract = NULL);
	void					DoAddContract(const short _ContractLength);				
	CString					GetContractEndDate();
	ushort					GetContractLength() const;
	uint					GetSignOnFee() const;				
    uint					GetWage() const;					
	bool					IsBetterThan(const CContract* pPlayerContract);
	void					SetContractLength(const ushort _ContractLength);				
	void					SetSignOnFee(const uint a_nSignonFee);			
	void					SetValues(const uint _SignonFee, const uint _Wage, const short _ContractLength);
	void					SetWage(const uint _Wage);				

private:
	uint					m_SignOnFee;
    uint					m_Wage;
    ushort					m_ContractLength;
};
#endif
