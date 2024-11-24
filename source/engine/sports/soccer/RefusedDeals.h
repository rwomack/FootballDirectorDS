

/*

	Class - C++ Classes

	RefusedDeal class

	Football Director

	(c) Rw Software 1994 - 2008

 ---------------------------------------------------------------
 */


class CRefusedDealList
{
public:
	// IO methods
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

	void					DoAddLoan(const ushort _ID);
	void					DoAddTransfer(const ushort _PlayerID);
	bool					DoCheckIfPlayerRefusedLoan(const ushort PlayerNum);
	bool					DoCheckIfPlayerRefusedTransfer(const ushort _PlayerID);
	void					DoEndOfWeekTasks();
	ushort					GetRefusedPlayerTransfersSize() { return m_RefusedPlayerTransfers.GetSize(); }
	void					RemoveSome(CNumericArray<ushort>& _RefusedList);

protected:
    CNumericArray<ushort>	m_RefusedPlayerTransfers;         // One for every player who refuses to join club
    CNumericArray<ushort>	m_RefusedPlayerLoans;             // One for every player who refuses to join club
};
