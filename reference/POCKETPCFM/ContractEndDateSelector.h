#if !defined(CONTRACTENDDATESELECTOR_INCLUDED_)
#define CONTRACTENDDATESELECTOR_INCLUDED_

/*

	ContractEndDateSelector.h: interface for the ContractEndDateSelector class.


	(c) Sports Director Ltd 2007 - 2008	

  ---------------------------------------------------------------
*/


class CContractEndDateSelector : public CTitledTextSelector
{
public:
	CContractEndDateSelector(void);
	virtual ~CContractEndDateSelector(void);

	void					DoAddDates(const ushort _SeasonsToAddFirst = 1, const ushort _nHowMany = 6, const ushort _nCursel = 3);
	int						GetCurSel();

protected:
	int						m_NumberSeasonsFirst;
};
#endif