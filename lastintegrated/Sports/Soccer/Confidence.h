
/*

	Confidence class

	Football Director

	(c) Rw Software 1994 - 2008

---------------------------------------------------------------
*/


#ifndef __CONFIDENCE_H__
#define __CONFIDENCE_H__

	const sbyte				MINIMUMCONFIDENCE = 0;
	const sbyte				MAXIMUMCONFIDENCE = 99;

class CConfidence
{
public:
	CConfidence()
		: m_Value(MINIMUMCONFIDENCE)
	{
	}

	// IO Methods
	void					Read(CDataFile* _pDataFile);
	void					Write(CDataFile* _pDataFile);

	// Methods
	void					DoResetVariables();
	void					DoIncrease(const sbyte _HowMuch);
	void					DoDecrease(const sbyte _HowMuch);
	void					DoRegenerate();
	byte					GetValue() const;

private:
	byte					m_Value;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#endif