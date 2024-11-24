
 /*

        Class:- Currency


	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */


 #ifndef __CURRENCY_H__
 #define __CURRENCY_H__


class CCurrency
{
	enum eSignPos
	{
		LEFT, RIGHT
	};

 public:

	// Methods
	void					DoInitialise();
	void					DoInitialiseFromResource(CBinaryReader& _Resource);
	uint					ConvToPounds(const uint _ForeignAmount) const;
	uint					ConvToForeign(const uint _PoundsAmount) const;
	int					ConvToPounds(const int _ForeignAmount) const;
	int					ConvToForeign(const int _PoundsAmount) const;
	CString					GetForeignStr(const uint _PoundsAmount) const;
	CString					GetForeignStr(const int _PoundsAmount) const;
	uint					GetExchangeRate() const;
	bool					GetCommaDecimal() const;
	CString					GetCurrencySign() const;
	byte					GetCurrencySignPos() const;
	bool					IsMillion() const;
	uint					GetStrLength(uint _Amount);
	bool					GetMillionFlag() const;
	uint					RoundedPounds(const uint _Amount) const;

   // Variables
 private:
	uint					m_ID:16;
	uint					m_CurrencySignpos:2;
	uint					m_CommaDecimal:1;         // if true then decimal point is a comma
	uint					m_MillionFlag:1;
	uint					m_Currency:1;
	uint					m_ExchangeRate;
	CExtString	            m_CurrencySign;
};
#endif




















































