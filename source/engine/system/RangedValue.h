
/*

    RangedValue.h - interface of CRangedValue template class


	Copyright 1994-2009 Sports Director Ltd. All rights reserved. 

 ---------------------------------------------------------------------------------------
*/



#ifndef __RANGEDVALUE_H__
#define __RANGEDVALUE_H__

#ifdef TOUCH
#include "Random.h"
extern CRandomNumber	RandomNumber;
#endif

template <class Ttype> class CRangedValue
{
public:

	//---------------------------------------------------------------------------
    void          Write(CDataFile* File)
	{
    File->Write(&m_BasicNumber, sizeof(m_BasicNumber));
    File->Write(&m_RandomNumber, sizeof(m_RandomNumber));
	}


	//---------------------------------------------------------------------------
  void          Read(CDataFile* File)
	{
    File->Read( &m_BasicNumber, sizeof(m_BasicNumber) );
    File->Read( &m_RandomNumber, sizeof(m_RandomNumber) );
	}

	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::DoInitialise
		Access:    	public 
		Parameter: 	const Ttype BasicNum
		Parameter: 	const Ttype RandomNum
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void					DoInitialise(const Ttype BasicNum, const Ttype RandomNum)
	{
		m_BasicNumber = BasicNum;
		m_RandomNumber = RandomNum;
	}
	

	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::RandomAmount
		Access:    	public 
		Returns:   	Ttype
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
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


	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::GetBasicValue
		Access:    	public 
		Returns:   	Ttype
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Ttype					GetBasicValue()
	{
		return m_BasicNumber;
	}
	

	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::GetMaximumValue
		Access:    	public 
		Returns:   	Ttype
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Ttype					GetMaximumValue()
	{
		return m_BasicNumber + m_RandomNumber;
	}


	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::GetRandomValue
		Access:    	public 
		Returns:   	Ttype
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Ttype					GetRandomValue()
	{
		return m_RandomNumber;
	}


	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::GetMidValue
		Access:    	public 
		Returns:   	Ttype
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	Ttype					GetMidValue()
	{
		return m_BasicNumber + (m_RandomNumber >> 1);	
	}


	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::SetValues
		Access:    	public 
		Parameter: 	Ttype _Value
		Parameter: 	Ttype _Value1
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void					SetValues(Ttype _Value, Ttype _Value1)
	{
		m_BasicNumber = _Value;
		m_RandomNumber = _Value1;
	}


	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::SetBasicValue
		Access:    	public 
		Parameter: 	Ttype _Value
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void					SetBasicValue(Ttype _Value)
	{
		m_BasicNumber = _Value;
	}


	/*------------------------------------------------------------------------------
		Method:   	CRangedValue<Ttype>::SetRandomValue
		Access:    	public 
		Parameter: 	Ttype _Value
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
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

    
	/*------------------------------------------------------------------------------
		Method:   	CRangedUInt::DoInitialiseFromResource
		Access:    	public 
		Parameter: 	CBinaryReader & _Resource
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void DoInitialiseFromResource(CBinaryReader& _Resource)
	{
		CRangedValue<uint>::m_BasicNumber = _Resource.ReadUInt32();
		CRangedValue<uint>::m_RandomNumber = _Resource.ReadUInt32();
	}

};
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

class CRangedByte : public CRangedValue<byte>
{
public:

    
	/*------------------------------------------------------------------------------
		Method:   	CRangedByte::DoInitialiseFromResource
		Access:    	public 
		Parameter: 	CBinaryReader & _Resource
		Returns:   	void
		Qualifier: 	
		Purpose:
	------------------------------------------------------------------------------*/
	void DoInitialiseFromResource(CBinaryReader& _Resource)
	{
		CRangedValue<byte>::m_BasicNumber = _Resource.ReadByte();
		CRangedValue<byte>::m_RandomNumber = _Resource.ReadByte();
	}

};
#endif
