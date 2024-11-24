
 /*

    Class:- Player Transfer Status

	Football Director

	(c) Rw Software 1994 - 2008

  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

 */

#include "stdafx.h"


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::DoResetVariables
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerTransferStatus::DoResetVariables()
{
  m_stablePrice = 0;
  m_lastPriceUpdate = 0;
	m_AskingPrice = 0;
	m_RejectBelowPrice = 0;
	m_Flags = 0;
    m_WeeksAvailable = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::DoJoinedNewClub
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerTransferStatus::DoJoinedNewClub()
{
	DoResetVariables();
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::IsBidAllowed
	Access:    	public 
	Parameter: 	CClubTransferOffer & _TheOffer
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerTransferStatus::IsBidAllowed(CClubTransferOffer& _TheOffer)
{
	if (IsRejectAllOffers() || (IsRejectOffersBelow() && m_RejectBelowPrice > _TheOffer.GetAmountOffered()))
	{
		return false;
	}
	return true;
}


 // Get a fair but random transfer valuation of player
 //----------------------------------------------------------------------------
uint CPlayerTransferStatus::GetValuation(CCountry& _Country, uint _Ret)
{
    if (IsTransferListed())
	{
		// If the player is on the transfer list then offer around that amount
		if (GetAskingPrice())
		{
      // only return a random value based on asking price after a week has gone by
      CCalendar calender = WorldData().GetCurrentDate();
      if( calender.GetWeek() > m_lastPriceUpdate  || m_stablePrice == 0 )
      {
        m_stablePrice = (uint)(_Country.GetCurrency().RoundedPounds(GetAskingPrice() * .9 + RandomNumber.IntLessThan(static_cast<uint>(GetAskingPrice() * .2))));
        m_lastPriceUpdate = calender.GetWeek();
      }

      return m_stablePrice;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		return _Ret;
	}
}


 // Returns the current wage of the player in the currency of the
 // country currently employed in
 //---------------------------------------------------------------------------
CString CPlayerTransferStatus::GetAskingPriceStr(CCountry& rCountry)
{
	EXT_ASSERT(IsTransferListed() == true);						// Ensure not listed
	return rCountry.GetCurrency().GetForeignStr(GetAskingPrice());
 }


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::IsTransferListed
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerTransferStatus::IsTransferListed()
{
	if (m_Flags & ONTRANSFERLIST)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::IsLoanListed
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerTransferStatus::IsLoanListed()
{
	if (m_Flags & ONLOANLIST)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::IsRejectOffersBelow
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerTransferStatus::IsRejectOffersBelow()
{
	if (m_Flags & REJECTOFFERSBELOW)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::IsOutOfContract
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerTransferStatus::IsOutOfContract()
{
	if (m_Flags & OUTOFCONTRACT)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::IsRejectAllOffers
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerTransferStatus::IsRejectAllOffers()
{
	if (m_Flags & REJECTALLOFFERS)
	{
		return true;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::SetTransferListed
	Access:    	public 
	Parameter: 	const bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerTransferStatus::SetTransferListed(const bool _Value)
{
	if (_Value == true)
	{
		m_Flags |= ONTRANSFERLIST;
	}
	else
	{
		m_Flags &= ~ONTRANSFERLIST;
	}
}


 //------------------------------------------------------------------------------------------------------
void CPlayerTransferStatus::SetLoanListed(const bool x)
{
	if (x == true)
	{
		m_Flags |= ONLOANLIST;
	}
	else
	{
		m_Flags &= ~ONLOANLIST;
	}
}


 //------------------------------------------------------------------------------------------------------
void CPlayerTransferStatus::SetRejectOffersBelow(bool x)
{
	if (x == true)
	{
		m_Flags |= REJECTOFFERSBELOW;
	}
	else
	{
		m_Flags &= ~REJECTOFFERSBELOW;
	}
}


 //------------------------------------------------------------------------------------------------------
void CPlayerTransferStatus::SetRejectAllOffers(bool x)
{
	if (x == true)
	{
		m_Flags |= REJECTALLOFFERS;
	}
	else
	{
		m_Flags &= ~REJECTALLOFFERS;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::SetOutOfContract
	Access:    	public 
	Parameter: 	bool _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerTransferStatus::SetOutOfContract(bool _Value)
{
	if (_Value == true)
	{
		m_Flags |= OUTOFCONTRACT;
	}
	else
	{
		m_Flags &= ~OUTOFCONTRACT;
	}
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::DoTransferList
	Access:    	public 
	Parameter: 	const uint _AskingPrice
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerTransferStatus::DoTransferList(const uint _AskingPrice)
{
	EXT_ASSERT(IsTransferListed() == false);
	m_AskingPrice = _AskingPrice;
    m_WeeksAvailable = 0;
	SetTransferListed(true);
}


 //------------------------------------------------------------------------------------------------------
void CPlayerTransferStatus::IncWeeksAvailable()                        
{ 
	m_WeeksAvailable++; 
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::GetAskingPrice
	Access:    	public 
	Returns:   	uint
	Qualifier: 	const
	Purpose:
------------------------------------------------------------------------------*/
uint CPlayerTransferStatus::GetAskingPrice() const
{
	return m_AskingPrice;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::SetAskingPrice
	Access:    	public 
	Parameter: 	const uint _Value
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerTransferStatus::SetAskingPrice(const uint _Value)
{
	m_AskingPrice = _Value;
}


 //------------------------------------------------------------------------------------------------------
uint CPlayerTransferStatus::GetRejectBelowPrice() const
{
	return m_RejectBelowPrice;
}


 //------------------------------------------------------------------------------------------------------
void CPlayerTransferStatus::SetRejectBelowPrice(const uint _Value)
{
	m_RejectBelowPrice = _Value;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::WeeklyTasks
	Access:    	public 
	Returns:   	bool
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
bool CPlayerTransferStatus::WeeklyTasks()
{
	if (IsTransferListed() == true)
	{
		m_WeeksAvailable++;
	}
	return false;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::DoTakeOffTransferList
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerTransferStatus::DoTakeOffTransferList()
{
	DoTransferRequestCanceled();
    m_WeeksAvailable = 0;
}


/*------------------------------------------------------------------------------
	Method:   	CPlayerTransferStatus::DoTransferRequestCanceled
	Access:    	public 
	Returns:   	void
	Qualifier: 	
	Purpose:
------------------------------------------------------------------------------*/
void CPlayerTransferStatus::DoTransferRequestCanceled()
{
	m_AskingPrice = 0;
	SetTransferListed(false);
}
