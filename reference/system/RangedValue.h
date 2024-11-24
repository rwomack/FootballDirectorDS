
/*

    RangedValue.h - interface of CRangedValue template class


	(c) RW Software 1994 - 2008

 ---------------------------------------------------------------------------------------
*/



#ifndef __RANGEDVALUE_H__
#define __RANGEDVALUE_H__


template <class Ttype> class CRangedValue
{
public:
    // IO Methods
	
	//---------------------------------------------------------------------------
    void					Write(FILE* File)
	{
		fwrite(&m_BasicNumber, sizeof m_BasicNumber, 1, File);
		fwrite(&m_RandomNumber, sizeof m_RandomNumber, 1, File);
	}

    
	//---------------------------------------------------------------------------
	void					Read(FILE* File)
	{
		fread(&m_BasicNumber, sizeof m_BasicNumber, 1, File);
		fread(&m_RandomNumber, sizeof m_RandomNumber, 1, File);
	}

     //---------------------------------------------------------------------------
	void					DoInitialise(const Ttype BasicNum, const Ttype RandomNum)
	{
		m_BasicNumber = BasicNum;
		m_RandomNumber = RandomNum;
	}
	
	//---------------------------------------------------------------------------
	Ttype					RandomAmount()
	{
		if (m_RandomNumber > 0) 
		{
			return m_BasicNumber + static_cast<Ttype>(RandomNumber.IntLessThan(static_cast<int>(m_RandomNumber)));
		}
		else 
		{
			return m_BasicNumber;
		}
	}

	//---------------------------------------------------------------------------
	Ttype					GetBasicValue()
	{
		return m_BasicNumber;
	}
	
	//---------------------------------------------------------------------------
	Ttype					GetMaximumValue()
	{
		return m_BasicNumber + m_RandomNumber;
	}
	
	Ttype					GetMidValue()
	{
		return m_BasicNumber + (m_RandomNumber >> 1);	
	}

	//---------------------------------------------------------------------------
	void					SetValues(Ttype _Value, Ttype _Value1)
	{
		m_BasicNumber = _Value;
		m_RandomNumber = _Value1;
	}

	//---------------------------------------------------------------------------
	void					SetBasicValue(Ttype _Value)
	{
		m_BasicNumber = _Value;
	}

	//---------------------------------------------------------------------------
	void					SetRandomValue(Ttype _Value)
	{
		m_RandomNumber = _Value;
	}

protected:
	Ttype					m_BasicNumber;
	Ttype			        m_RandomNumber;
};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CRangedUInt : public CRangedValue<uint>
{
public:

    
	//---------------------------------------------------------------------------
	void					ReadResource(CBinaryReader& _Resource)
	{
		CRangedValue<uint>::m_BasicNumber = _Resource.ReadUInt32();
		CRangedValue<uint>::m_RandomNumber = _Resource.ReadUInt32();
	}

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CRangedByte : public CRangedValue<byte>
{
public:

    
	//---------------------------------------------------------------------------
	void					ReadResource(CBinaryReader& _Resource)
	{
		CRangedValue<byte>::m_BasicNumber = _Resource.ReadByte();
		CRangedValue<byte>::m_RandomNumber = _Resource.ReadByte();
	}

};
#endif