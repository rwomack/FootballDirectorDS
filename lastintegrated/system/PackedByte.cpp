
 /*

    Class:- CPackedByte


	(c) RW Software 2004

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


#include "stdafx.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


 //---------------------------------------------------------------------------------------
CPackedByte::CPackedByte()
	: m_Value(0)
{

}


 //---------------------------------------------------------------------------------------
CPackedByte::~CPackedByte()
{

}


 //---------------------------------------------------------------------------------------
const UInt8	CPackedByte::getValue(void)
{
	return m_Value;
}


 //---------------------------------------------------------------------------------------
void CPackedByte::DoInitialise(const UInt8 aHighBits /*= 0*/, const UInt8 aLowBits /*= 0*/)
{
	setHighBits(aHighBits);
	setLowBits(aLowBits);
}


 //---------------------------------------------------------------------------------------
const UInt8	CPackedByte::getHighBits(void)
{
	return (m_Value >> 4) & 0x0f;
}


 //---------------------------------------------------------------------------------------
const UInt8	CPackedByte::getLowBits(void)
{
	return m_Value & 0x0f;
}


 //---------------------------------------------------------------------------------------
void CPackedByte::setLowBits(UInt8 aValue)
{
	m_Value &= 0xf0;
	m_Value += aValue;
}


 //---------------------------------------------------------------------------------------
void CPackedByte::setHighBits(UInt8 aValue)
{
	m_Value &= 0x0f;
	m_Value += ((aValue) << 4);
}


 //---------------------------------------------------------------------------------------
void CPackedByte::DoIncrementHighBits(void)
{
	UInt8 bits = getHighBits();
	ASSERT(bits < 0x0f);
	bits++;
	setHighBits(bits);
}


 //---------------------------------------------------------------------------------------
void CPackedByte::DoIncrementLowBits(void)
{
	UInt8 bits = getLowBits();
	ASSERT(bits < 0x0f);
	bits++;
	setLowBits(bits);
}


 //---------------------------------------------------------------------------------------
void CPackedByte::DoAddHighBits(UInt8 aValue)
{
	UInt8 bits = getHighBits();
	bits += aValue;
	ASSERT(bits < 0x10);
	setHighBits(bits);
}


 //---------------------------------------------------------------------------------------
void CPackedByte::DoAddLowBits(UInt8 aValue)
{
	UInt8 bits = getLowBits();
	bits += aValue;
	ASSERT(bits < 0x10);
	setLowBits(bits);
}


 //---------------------------------------------------------------------------------------
void CPackedByte::Write(FILE* File) const
{
    ASSERT (File != NULL);
    fwrite(&m_Value, sizeof m_Value, 1, File);
 }


 //---------------------------------------------------------------------------------------
void CPackedByte::Read(FILE* File)
{
    ASSERT (File != NULL);
	fread(&m_Value, sizeof m_Value, 1, File);
 }