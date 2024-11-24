
 /*

        Class:- Currency


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"
#include "RoundMoney.h"


/*------------------------------------------------------------------------------
	Method:   	CCurrency::DoInitialise
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:	Initialise to £'s
------------------------------------------------------------------------------*/
void CCurrency::DoInitialise()
{
    m_ID = 0;
    m_CurrencySign = TEXT("£");
    m_CurrencySignpos = LEFT;
    m_CommaDecimal = false;
    m_ExchangeRate = 1;
    m_MillionFlag = false;
	m_Currency = false;
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::DoInitialiseFromResource
	Access:    	public 
	Parameter: 	CBinaryResource& _Resource
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CCurrency::DoInitialiseFromResource(CBinaryReader& _Resource)
{
    m_ID = _Resource.ReadUInt16();
    m_CurrencySign.ReadFromResource(_Resource);
    m_CurrencySignpos = _Resource.ReadByte();
	EXT_ASSERT((m_CurrencySignpos == LEFT) || (m_CurrencySignpos == RIGHT));
    m_CommaDecimal = _Resource.ReadByte();
    m_ExchangeRate = _Resource.ReadUInt32();
    EXT_ASSERT(m_ExchangeRate != 0);
    m_MillionFlag = _Resource.ReadByte();
	m_Currency = false;
//@@	afxDump << m_ID << " Sign : " << m_CurrencySign << " Signpos : " << m_CurrencySignpos << " Comma : " << m_CommaDecimal << " Exchange rate : " << m_ExchangeRate << " MillFlag : " << m_MillionFlag << "\n";
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::GetMillionFlag
	Access:    	public const 
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CCurrency::GetMillionFlag() const
{
	if (m_Currency == true)
	{
		return m_MillionFlag;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::GetCurrencySign
	Access:    	public const 
	Returns:   	CString
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
CString	CCurrency::GetCurrencySign() const
{
	if (m_Currency == true)
	{
		return m_CurrencySign;
	}
	else
	{
		return TEXT("£");
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::GetCurrencySignPos
	Access:    	public const 
	Parameter: 	void
	Returns:   	byte
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
byte CCurrency::GetCurrencySignPos() const
{
	if (m_Currency == true)
	{
		return static_cast<eSignPos>(m_CurrencySignpos);
	}
	else
	{
		return LEFT;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::GetCommaDecimal
	Access:    	public const 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CCurrency::GetCommaDecimal() const
{
	if (m_Currency == true)
	{
		return m_CommaDecimal;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::IsMillion
	Access:    	public const 
	Parameter: 	void
	Returns:   	bool
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
bool CCurrency::IsMillion() const
{
	if (m_Currency == true)
	{
		return m_MillionFlag;
	}
	else
	{
		return false;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::GetStrLength
	Access:    	public 
	Parameter: 	uint aAmount
	Returns:   	uint
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
uint CCurrency::GetStrLength(uint _Amount)
{
	CExtString Str;
	Str.FormatLargeNumber(ConvToForeign(_Amount));
	return Str.GetLength();
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::ForeignStr
	Access:    	public const 
	Parameter: 	const uint PoundsAmount
	Returns:   	CString
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
CString CCurrency::GetForeignStr(const uint PoundsAmount) const
{
	CString Temp;
	CString Newstr;
	Temp.Format(TEXT("%u"), ConvToForeign(PoundsAmount));
	Temp.MakeReverse();
	int Deci = Temp.Find(TEXT("."));
	int Total = 0;
	int Lenth = Temp.GetLength();
	for (int Count = 0; Count < Lenth; Count++)
	{
		if (Count >= Deci)
		{
			if (Total == 3)
			{
				Total = 1;
				if (GetCommaDecimal() == false)
				{
					Newstr += TEXT(",");
				}
				else
				{
					Newstr += TEXT(".");
				}
			}
			else
			{
				Total++;
			}
		}
		Newstr += Temp.GetAt(Count);
	}
	Newstr.MakeReverse();

	// If a ridiculously large amount truncate and show as millions
	if (Newstr.GetLength() > 7 && GetMillionFlag() == true)
	{
		if (Newstr.Right(7) != (CString)TEXT("000,000"))
		{
			Newstr = Newstr.Left(Newstr.GetLength() - 4) + TEXT("t");
		}
		else
		{
			Newstr = Newstr.Left(Newstr.GetLength() - 8) + TEXT("m");
		}
	}
	CString szNumBuff;
	if (GetCurrencySignPos() == LEFT)
	{
		szNumBuff.Format(TEXT("%s%s"), GetCurrencySign(), Newstr);
	}
	else
	{
		szNumBuff.Format(TEXT("%s%s"), Newstr, GetCurrencySign());
	}
	return szNumBuff;
 }


/*------------------------------------------------------------------------------
	Method:   	CCurrency::ForeignStr
	Access:    	public const 
	Parameter: 	const int PoundsAmount
	Returns:   	CString
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
CString CCurrency::GetForeignStr(const int _PoundsAmount) const
{
	CString Temp;
	CString Newstr;
	Temp.Format(TEXT("%d"), abs(ConvToForeign(_PoundsAmount)));
	Temp.MakeReverse();
	int Deci = Temp.Find(TEXT("."));
	int Total = 0;
	int Lenth = Temp.GetLength();
	for (int Count = 0; Count < Lenth; Count ++)
	{
		if (Count >= Deci)
		{
			if (Total == 3)
			{
				Total = 1;
				if (GetCommaDecimal() == false)
				{
					Newstr += TEXT(",");
				}
				else
				{
					Newstr += TEXT(".");
				}
			}
			else
			{
				Total++;
			}
		}
		Newstr += Temp.GetAt(Count);
	}
	Newstr.MakeReverse();

	// If a ridiculously large amount truncate and show as millions
	if (Newstr.GetLength() > 7 && GetMillionFlag() == true)
	{
		if (Newstr.Right(7) != (CString)TEXT("000,000"))
		{
			Newstr = Newstr.Left(Newstr.GetLength() - 4) + TEXT("t");
		}
		else
		{
			Newstr = Newstr.Left(Newstr.GetLength() - 8) + TEXT("m");
		}
	}
	CString szNumBuff;
	if (GetCurrencySignPos() == LEFT)
	{
		szNumBuff.Format(TEXT("%s%s"), GetCurrencySign(), Newstr);
	}
	else
	{
		szNumBuff.Format(TEXT("%s%s"), Newstr, GetCurrencySign());
	}
	if (_PoundsAmount < 0)
	{
    szNumBuff = CString("-") + szNumBuff;
	}
	return szNumBuff;
 }


/*------------------------------------------------------------------------------
	Method:   	CCurrency::GetExchangeRate
	Access:    	public const 
	Parameter: 	void
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CCurrency::GetExchangeRate() const
{
	if (m_Currency == true)
	{
		return m_ExchangeRate;
	}
	else
	{
		return 1;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::ConvToPounds
	Access:    	public const 
	Parameter: 	const uint _ForeignAmount
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CCurrency::ConvToPounds(const uint _ForeignAmount) const
{
	if (m_Currency == true)
	{
		return _ForeignAmount / GetExchangeRate();
	}
	else
	{
		return _ForeignAmount;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::ConvToForeign
	Access:    	public const 
	Parameter: 	const uint _PoundsAmount
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CCurrency::ConvToForeign(const uint _PoundsAmount) const
{
	if (m_Currency == true)
	{
		return _PoundsAmount * GetExchangeRate();
	}
	else
	{
		return _PoundsAmount;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::ConvToPounds
	Access:    	public const 
	Parameter: 	const int _ForeignAmount
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CCurrency::ConvToPounds(const int _ForeignAmount) const
{
	if (m_Currency == true)
	{
		return _ForeignAmount / GetExchangeRate();
	}
	else
	{
		return _ForeignAmount;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::ConvToForeign
	Access:    	public const 
	Parameter: 	const int _PoundsAmount
	Returns:   	int
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
int CCurrency::ConvToForeign(const int _PoundsAmount) const
{
	if (m_Currency == true)
	{
		return _PoundsAmount * GetExchangeRate();
	}
	else
	{
		return _PoundsAmount;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CCurrency::RoundedPounds
	Access:    	public const 
	Parameter: 	const uint _Amount
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CCurrency::RoundedPounds(const uint _Amount) const
{
	return ConvToPounds(static_cast<uint>(CRoundMoney::RoundValue(ConvToForeign(_Amount))));
}
