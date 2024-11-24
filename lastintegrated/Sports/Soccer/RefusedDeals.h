

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
	void					DoAddLoan(const ushort _ID);
	void					DoAddTransfer(const ushort _PlayerID);
	bool					DoCheckIfPlayerRefusedLoan(const ushort PlayerNum);
	bool					DoCheckIfPlayerRefusedTransfer(const ushort _PlayerID);
	void					DoEndOfWeekTasks();
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

protected:
    CNumericArray<ushort>	m_RefusedPlayerTransfers;         // One for every player who refuses to join club
    CNumericArray<ushort>	m_RefusedPlayerLoans;             // One for every player who refuses to join club
};
