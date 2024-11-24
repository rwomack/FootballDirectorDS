using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;


namespace Data_Builder
{
	class Cup : RecordReader
	{
		public enum CUPTYPE
		{
			FEDERATIONCUP,
			DOMESTICCUP,
			CHARITYSHIELD,
			PLAYOFF
		};

		public enum CUP
		{
			ID,
			NAME,
			CUPNAME_FRENCH,
			CUPNAME_ITALIAN,
			CUPNAME_GERMAN,
			CUPNAME_SPANISH,
			POCKETPC,
			CUPTYPE,
			FEDERATION,
			COUNTRY,
			SHIELDNAMEQUAL,
			FEDCUP1QUAL,
			RUNNERUPQUALIFIESFOR,
			SPECIALCASEID,
			BYEROUND,
			NUMBEROFROUNDS,
			NUMCUPCLUBS,
			CHAMPIONS,
			RUNNERSUP,
			SEMILOSERS1,
			SEMILOSERS2,
			SUBSSELECT,
			SUBSUSE,
			NEUTRALVENUE1,
			NEUTRALVENUE2,
			NEUTRALVENUE3,
			NEUTRALVENUE4,
			NEUTRALVENUE5,
			NEUTRALVENUE6,
			NEUTRALVENUE7,
			NEUTRALVENUE8,
			CLASHCHECK,
			TROPHYGFX,
			PERCENTAGEPOINTSFORPLAYOFFS,
			NUMBERPROMOTED
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    Cup
        // FullName:  Data_Builder.Cup.Cup
        // Access:    public 
        // Returns:  
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public Cup(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.Cup.DoCreateData
		// Access:    virtual public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public virtual void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
			m_FileWriter.Write((short)iRecordCount);

            base.ExecuteReader("SELECT * FROM tbl_cups Where PocketPC = 1 And CupType = " + (int)CUPTYPE.DOMESTICCUP);
			while (m_Reader.Read())
			{
                m_FileWriter.Write(m_Reader.GetInt16((int)CUP.COUNTRY));
                m_FileWriter.Write(m_Reader.GetByte((int)CUP.NUMCUPCLUBS));
				Club theClub = new Club(m_theDB, m_theForm);
                theClub.DoCreateCupClubsData(m_FileWriter, m_Reader.GetByte((int)CUP.NUMCUPCLUBS));
                DoCreateInitData();

				CupSchedule theSchedule = new CupSchedule(m_theDB, m_theForm, m_FileWriter);
                theSchedule.DoCreateData(m_FileWriter, m_Reader.GetInt16((int)CUP.ID));
			}
            base.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateInitData
		// FullName:  Data_Builder.Cup.DoCreateInitData
		// Access:    public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateInitData()
		{
			m_FileWriter.Write(m_Reader.GetByte((int)CUP.SUBSSELECT));
			m_FileWriter.Write(m_Reader.GetByte((int)CUP.SUBSUSE));
			m_FileWriter.Write(m_Reader.GetString((int)CUP.NAME));

			m_FileWriter.Write(m_Reader.GetInt16((int)CUP.NEUTRALVENUE1));
			m_FileWriter.Write(m_Reader.GetByte((int)CUP.BYEROUND));
			m_FileWriter.Write(m_Reader.GetInt16((int)CUP.SHIELDNAMEQUAL));
			m_FileWriter.Write(m_Reader.GetInt16((int)CUP.NUMBEROFROUNDS));
		}
	}
}
