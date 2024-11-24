using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class Popularity : RecordReader
	{
		enum POPULARITY
		{
			ID,
			BASIC_ATTENDANCE,
			MAX_ATTENDANCE,
			ANNUAL_MAXIMUM_LOANAMOUNT,
			BASIC_EXPENSES,
			BASIC_WEEKLY_RUNNING_COSTS,
			RND_WEEKLY_RUNNING_COSTS,
			MAX_SKILL,
			GOOD_SKILL,
			GOOD_PLAYER,
			SACK_SKILL,
			PLAYER_PURCHASE_THRESHOLD,
			MAXIMUM_PLAYER_PURCHASE_VALUE,
			BASIC_MANAGER_WEEKLY_WAGE,
			RND_MANAGER_WEEKLY_WAGE,
			WAGE_ADJUSTMENT_RATIO,
			BASIC_TICKET_PRICE,
			RND_TICKET_PRICE,
			BASIC_INITIAL_CLUB_BALANCE,
			RND_INITIAL_CLUB_BALANCE,
			BASIC_GENERAL_WEEKLY_WAGE,
			RND_GENERAL_WEEKLY_WAGE,
			BASIC_CONTRACT,
			RND_CONTRACT,
			RELEGATION_DECREASE,
			PROMOTION_INCREASE,
			GROUND_SIZE,
			BASIC_EMPLOYEE_SKILL,
			RND_EMPLOYEE_SKILL,
			MINIMUM_CLUB_BALANCE,
			NUMBER_STAR_PLAYERS1,
			NUMBER_STAR_PLAYERS2,
			NUMBER_STAR_PLAYERS3,
			NUMBER_STAR_PLAYERS4,
			BASIC_WEEKLY_SPONSORSHIP,
			RND_WEEKLY_SPONSORSHIP,
			WEEKLY_CLUB_MERCHANDISE,
			BASIC_WEEKLY_TV_REVENUE,
			RND_WEEKLY_TV_REVENUE,
			PLAYER_MIN_SKILL1,
			PLAYER_RND_SKILL1,
			STARPLAYER_MIN_SKILL1,
			STARPLAYER_RND_SKILL1,
			PLAYER_MIN_SKILL2,
			PLAYER_RND_SKILL2,
			STARPLAYER_MIN_SKILL2,
			STARPLAYER_RND_SKILL2,
			PLAYER_MIN_SKILL3,
			PLAYER_RND_SKILL3,
			STARPLAYER_MIN_SKILL3,
			STARPLAYER_RND_SKILL3,
			PLAYER_MIN_SKILL4,
			PLAYER_RND_SKILL4,
			STARPLAYER_MIN_SKILL4,
			STARPLAYER_RND_SKILL4,
			MINIMUM_MANAGER_RATING_TO_MANAGE_CLUB,
			MAX_FIELDS
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    Popularity
        // FullName:  Data_Builder.Popularity.Popularity
        // Access:    public 
        // Returns:  
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public Popularity(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateSeries60Data
		// FullName:  Data_Builder.Popularity.DoCreateSeries60Data
		// Access:    public 
		// Returns:   void
		// Parameter: string _Path
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateSeries60Data(string _Path)
		{
            short iRecordCount = base.DoCountRecords();
	
			int[] MaximumAttendance = new int[iRecordCount];
			int[] BaseAttendance = new int[iRecordCount];
			int[] AnnualMaximumLoanAmount = new int[iRecordCount];
			int[] TicketPrice = new int[iRecordCount];
			int[] rangeWeeklyGeneralWage = new int[iRecordCount];
			int[] InitialClubBalance = new int[iRecordCount];
			int[] WeeklyGeneralWage = new int[iRecordCount];
			int[] ManagerAnnualWage = new int[iRecordCount];
			int[] BasicExpenses = new int[iRecordCount];
			int[] rangeRunningCosts = new int[iRecordCount];
			int[] PlayerPurchaseThreshold = new int[iRecordCount];
			int[] MaximumPlayerPurchaseValue = new int[iRecordCount];
			int[] MinimumClubBalance = new int[iRecordCount];
			int[] rangeWeeklySponsorship = new int[iRecordCount];
			int[] rangeWeeklyTvRevenue = new int[iRecordCount];
			int[] WeeklyClubMerchandise = new int[iRecordCount];
			byte[] MaxSkill = new byte[iRecordCount];
			byte[] GoodSkill = new byte[iRecordCount];
			byte[] SackSkill = new byte[iRecordCount];
			byte[] GoodPlayer = new byte[iRecordCount];
			byte[] PlayerMinSkill = new byte[iRecordCount];
			byte[] PlayerRndSkill = new byte[iRecordCount];
			byte[] rangeEmployeeSkill = new byte[iRecordCount];
			byte[] MinimumManagerRatingToManageClub = new byte[iRecordCount];

            base.ExecuteReader();
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
				m_Reader.Read();
				MaximumAttendance[LoopCount] = m_Reader.GetInt32((int)POPULARITY.MAX_ATTENDANCE);
				BaseAttendance[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_ATTENDANCE);
				AnnualMaximumLoanAmount[LoopCount] = m_Reader.GetInt32((int)POPULARITY.ANNUAL_MAXIMUM_LOANAMOUNT);
				TicketPrice[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_TICKET_PRICE);
				rangeWeeklyGeneralWage[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_GENERAL_WEEKLY_WAGE);
				InitialClubBalance[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_INITIAL_CLUB_BALANCE);
				WeeklyGeneralWage[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_GENERAL_WEEKLY_WAGE);
				ManagerAnnualWage[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_MANAGER_WEEKLY_WAGE);
				BasicExpenses[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_EXPENSES);
				rangeRunningCosts[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_WEEKLY_RUNNING_COSTS);
				PlayerPurchaseThreshold[LoopCount] = m_Reader.GetInt32((int)POPULARITY.PLAYER_PURCHASE_THRESHOLD);
				MaximumPlayerPurchaseValue[LoopCount] = m_Reader.GetInt32((int)POPULARITY.MAXIMUM_PLAYER_PURCHASE_VALUE);
				MinimumClubBalance[LoopCount] = m_Reader.GetInt32((int)POPULARITY.MINIMUM_CLUB_BALANCE);
				rangeWeeklySponsorship[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_WEEKLY_SPONSORSHIP);
				rangeWeeklyTvRevenue[LoopCount] = m_Reader.GetInt32((int)POPULARITY.BASIC_WEEKLY_TV_REVENUE);
				WeeklyClubMerchandise[LoopCount] = m_Reader.GetInt32((int)POPULARITY.WEEKLY_CLUB_MERCHANDISE);
				MaxSkill[LoopCount] = m_Reader.GetByte((int)POPULARITY.MAX_SKILL);
				GoodSkill[LoopCount] = m_Reader.GetByte((int)POPULARITY.GOOD_SKILL);
				SackSkill[LoopCount] = m_Reader.GetByte((int)POPULARITY.SACK_SKILL);
				GoodPlayer[LoopCount] = m_Reader.GetByte((int)POPULARITY.GOOD_PLAYER);
				PlayerMinSkill[LoopCount] = m_Reader.GetByte((int)POPULARITY.PLAYER_MIN_SKILL1);
				PlayerRndSkill[LoopCount] = m_Reader.GetByte((int)POPULARITY.PLAYER_RND_SKILL1);
				rangeEmployeeSkill[LoopCount] = m_Reader.GetByte((int)POPULARITY.BASIC_EMPLOYEE_SKILL);
				MinimumManagerRatingToManageClub[LoopCount] = m_Reader.GetByte((int)POPULARITY.MINIMUM_MANAGER_RATING_TO_MANAGE_CLUB);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(MaximumAttendance[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(BaseAttendance[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(AnnualMaximumLoanAmount[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(TicketPrice[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(rangeWeeklyGeneralWage[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(InitialClubBalance[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(WeeklyGeneralWage[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(ManagerAnnualWage[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(BasicExpenses[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(rangeRunningCosts[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(PlayerPurchaseThreshold[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(MaximumPlayerPurchaseValue[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(MinimumClubBalance[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(rangeWeeklySponsorship[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(rangeWeeklyTvRevenue[LoopCount]);
			}
			for (int LoopCount = 0; LoopCount < iRecordCount; LoopCount++)
			{
                m_FileWriter.Write(WeeklyClubMerchandise[LoopCount]);
			}
            m_FileWriter.Write(MaxSkill, 0, iRecordCount);
            m_FileWriter.Write(GoodSkill, 0, iRecordCount);
            m_FileWriter.Write(SackSkill, 0, iRecordCount);
            m_FileWriter.Write(GoodPlayer, 0, iRecordCount);
            m_FileWriter.Write(PlayerMinSkill, 0, iRecordCount);
            m_FileWriter.Write(PlayerRndSkill, 0, iRecordCount);
            m_FileWriter.Write(rangeEmployeeSkill, 0, iRecordCount);
            m_FileWriter.Write(MinimumManagerRatingToManageClub, 0, iRecordCount);
            base.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.Popularity.DoCreateData
		// Access:    public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
            m_FileWriter.Write(iRecordCount);

            base.ExecuteReader();
			while (m_Reader.Read())
			{
				m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.ANNUAL_MAXIMUM_LOANAMOUNT));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.BASIC_MANAGER_WEEKLY_WAGE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.RND_MANAGER_WEEKLY_WAGE));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.WAGE_ADJUSTMENT_RATIO));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.BASIC_TICKET_PRICE));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.RND_TICKET_PRICE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.BASIC_INITIAL_CLUB_BALANCE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.RND_INITIAL_CLUB_BALANCE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.BASIC_GENERAL_WEEKLY_WAGE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.RND_GENERAL_WEEKLY_WAGE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.MAX_ATTENDANCE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.BASIC_EXPENSES));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.BASIC_WEEKLY_RUNNING_COSTS));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.RND_WEEKLY_RUNNING_COSTS));

                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.MAX_SKILL));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.GOOD_SKILL));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.SACK_SKILL));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.GOOD_PLAYER));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.PLAYER_PURCHASE_THRESHOLD));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.MAXIMUM_PLAYER_PURCHASE_VALUE));

                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.RELEGATION_DECREASE));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PROMOTION_INCREASE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.BASIC_ATTENDANCE));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.BASIC_EMPLOYEE_SKILL));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.RND_EMPLOYEE_SKILL));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.MINIMUM_CLUB_BALANCE));

                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.NUMBER_STAR_PLAYERS1));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.NUMBER_STAR_PLAYERS2));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.NUMBER_STAR_PLAYERS3));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.NUMBER_STAR_PLAYERS4));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.STARPLAYER_MIN_SKILL1));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.STARPLAYER_MIN_SKILL2));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.STARPLAYER_MIN_SKILL3));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.STARPLAYER_MIN_SKILL4));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.STARPLAYER_RND_SKILL1));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.STARPLAYER_RND_SKILL2));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.STARPLAYER_RND_SKILL3));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.STARPLAYER_RND_SKILL4));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PLAYER_MIN_SKILL1));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PLAYER_MIN_SKILL2));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PLAYER_MIN_SKILL3));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PLAYER_MIN_SKILL4));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PLAYER_RND_SKILL1));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PLAYER_RND_SKILL2));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PLAYER_RND_SKILL3));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.PLAYER_RND_SKILL4));

                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.BASIC_WEEKLY_SPONSORSHIP));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.RND_WEEKLY_SPONSORSHIP));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.BASIC_WEEKLY_TV_REVENUE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.RND_WEEKLY_TV_REVENUE));
                m_FileWriter.Write(m_Reader.GetInt32((int)POPULARITY.WEEKLY_CLUB_MERCHANDISE));
                m_FileWriter.Write(m_Reader.GetByte((int)POPULARITY.MINIMUM_MANAGER_RATING_TO_MANAGE_CLUB));
			}
            base.Close();
		}	
	}
}
