
/*

	PackedByte.h: interface for the CPackedByte class.


	(c) RW Software 2004

    ---------------------------------------------------------------
 */


#if !defined(AFX_PACKEDBYTE_H__64E2BCD2_A19A_4EC6_A9C0_A19AB3CA06EA__INCLUDED_)
#define AFX_PACKEDBYTE_H__64E2BCD2_A19A_4EC6_A9C0_A19AB3CA06EA__INCLUDED_

class CPackedByte  
{
public:
	CPackedByte();
	virtual ~CPackedByte();

	void					Read(FILE* File);
	void					Write(FILE* File) const;

	const UInt8				getHighBits();
	const UInt8				getLowBits();
	const UInt8				getValue();
	void					setLowBits(UInt8 aValue);
	void					setHighBits(UInt8 aValue);
	void					DoAddHighBits(UInt8 aValue);
	void					DoAddLowBits(UInt8 aValue);
    void					DoIncrementHighBits();
    void					DoIncrementLowBits();
	void					DoInitialise(const UInt8 aHighBits = 0, const UInt8 aLowBits = 0);

protected:
	UInt8		m_Value;
};

#endif // !defined(AFX_PACKEDBYTE_H__64E2BCD2_A19A_4EC6_A9C0_A19AB3CA06EA__INCLUDED_)
