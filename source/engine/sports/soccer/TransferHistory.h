
/*
 
	File:- tranhist.h

	Transfer history list class declarations


	Football Director

	(c) Rw Software 1994 - 2008

  ---------------------------------------------------------------------------------------
*/



class CTransferHistory 
{
public:
	// Constructors
    CTransferHistory();
	CTransferHistory(const CPersonName& Name, const CStringID& ClubNameID, const uint HowMuch, const CDate& When, const ushort PlayerID);

	// IO Methods
	void					Write(CDataFile* _pDataFile);
	void					Read(CDataFile* _pDataFile);

	CString					FeeText(CCountry* _Country);
	uint					GetFee() const;

	// Variables
private:
	CPersonName				m_PlayerNameID;
	CStringID				m_ClubID;
	uint					m_Fee;
	CDate					m_TheDate;
	ushort					m_PlayerID;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


class CTransferHistoryList : public CPointerArray<CTransferHistory>
{
public:
	// Constructor
	CTransferHistoryList() {};
	~CTransferHistoryList();

    // IO Methods
  void          Write(CDataFile* File);
  void          Read(CDataFile* File);
};
