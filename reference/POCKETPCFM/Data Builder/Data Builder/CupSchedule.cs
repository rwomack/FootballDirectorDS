using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class CupSchedule : RecordReader
	{
		public enum CUPSCHEDULE
		{
			CUPID,
			ROUNDTYPE,
			ORDER,
			NUMBERINROUND,
			EXCLUDEFEDQUAL,
			FIRSTTOADD,
			NUMBERTOADD,
			MATCHTYPE,
			VENUEID,
			DRAWDATE,
			MATCHDATE,
			REPLAYDATE,
			NUMBEROFREGIONS,
			NUMBERINGROUPS,
			NUMBEROFGROUPMATCHES,
			ROUNDSTATUS,
			MINREFRATING,
			MAXREFRATING,
			LOSERQUALIFIESFOR,
			ROUNDTITLE,
			REPLAYTYPE
		};

		public enum CUPROUNDTYPE
		{
			ID,
			TYPETEXT
		};

        OleDbDataReader m_TypeReader = null;

        //////////////////////////////////////////////////////////////////////////
        // Method:    CupSchedule
        // FullName:  Data_Builder.CupSchedule.CupSchedule
        // Access:    public 
        // Returns:   // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: BinaryWriter _CupFileWriter
        //////////////////////////////////////////////////////////////////////////
        public CupSchedule(OleDbConnection _theDB, FormMain _theForm, BinaryWriter _theFileWriter)
			: base(_theDB, _theForm, _theFileWriter)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.CupSchedule.DoCreateData
		// Access:    public 
		// Returns:   void
		// Parameter: BinaryWriter _CupFileWriter
		// Parameter: int _CupID
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData(BinaryWriter _CupFileWriter, int _CupID)
		{
			try
			{
                short iRecordCount = base.DoCountRecords();
                base.ExecuteReader("SELECT * FROM tbl_cup_schedules Where CupID = " + _CupID);
                while (m_Reader.Read())
				{
					_CupFileWriter.Write(m_Reader.GetByte((int)CUPSCHEDULE.ROUNDTYPE));

					DoCreateTitle(_CupFileWriter, m_Reader.GetByte((int)CUPSCHEDULE.ROUNDTYPE));
					DoCreateTitle(_CupFileWriter, m_Reader.GetByte((int)CUPSCHEDULE.ROUNDTITLE));
					DoCreateTitle(_CupFileWriter, m_Reader.GetByte((int)CUPSCHEDULE.REPLAYTYPE));

					_CupFileWriter.Write(m_Reader.GetByte((int)CUPSCHEDULE.NUMBERINROUND));
					_CupFileWriter.Write(m_Reader.GetByte((int)CUPSCHEDULE.MATCHTYPE));
					_CupFileWriter.Write(m_Reader.GetByte((int)CUPSCHEDULE.NUMBEROFREGIONS));

					_CupFileWriter.Write(m_Reader.GetByte((int)CUPSCHEDULE.DRAWDATE));
					_CupFileWriter.Write(m_Reader.GetByte((int)CUPSCHEDULE.MATCHDATE));
					_CupFileWriter.Write(m_Reader.GetByte((int)CUPSCHEDULE.REPLAYDATE));
				}
				m_Reader.Close();
				m_TypeReader.Close();
			}
			catch (Exception ee)
			{
				m_theForm.StatusLabel.Text = ee.ToString();
			}
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateTitle
		// FullName:  Data_Builder.CupSchedule.DoCreateTitle
		// Access:    public 
		// Returns:   void
		// Parameter: BinaryWriter _CupFileWriter
		// Parameter: int _TitleID
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateTitle(BinaryWriter _CupFileWriter, int _TitleID)
		{
			string debugCurrentCupSchedule = "";
			try
			{
                OleDbCommand DbCmd = new OleDbCommand();
                DbCmd.CommandText = "SELECT * FROM tbl_ref_cup_round_type Where ID = " + _TitleID;
				m_TypeReader = DbCmd.ExecuteReader();
				m_TypeReader.Read();
				_CupFileWriter.Write(m_TypeReader.GetString((int)CUPROUNDTYPE.TYPETEXT));
				debugCurrentCupSchedule = m_TypeReader.GetString((int)CUPROUNDTYPE.TYPETEXT);
			}
			catch (Exception ee)
			{
				m_theForm.StatusLabel.Text = debugCurrentCupSchedule + " " + ee.ToString();
			}
		}
	}
}
