using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class DivisionSchedule : RecordReader
	{
		private static int SCHEDULE_SIZE = 106;

		enum DIVISIONSCHEDULE
		{
			ID,
			EVENTID,
			EVENTDATE
		};

		protected enum SCHEDULEEVENT
		{
			NOMATCH = 200,
			ENDSEASON,
			NEWSEASON,
			CLOSESEASON,
			TRAININGONLY,
			PLAYOFFINIT,		// Create the playoffs
			PLAYOFFMATCH1,
			ARRANGEFRIENDLIES,
			FRIENDLYMATCH1,
			FRIENDLYMATCH2,
			FRIENDLYMATCH3,
			FRIENDLYMATCH4,
			FRIENDLYMATCH5,
			FRIENDLYMATCH6,
			FRIENDLYMATCH7,
			FRIENDLYMATCH8,
			PLAYOFFMATCH2,
			PLAYOFFMATCH3,
			PLAYOFFMATCH4,
			PLAYOFFMATCH5,
			PLAYOFFMATCH6,
			PLAYOFFMATCH7,
			PLAYOFFMATCH8,
			PLAYOFFMATCH9,
			PLAYOFFMATCH10,
			PLAYOFFMATCH11,
			PLAYOFFMATCH12,
			PLAYOFFMATCH13,
			PLAYOFFMATCH14,
			PLAYOFFMATCH15,
			PLAYOFFMATCH16,
			PLAYOFFMATCH17,
			PLAYOFFMATCH18,
			PLAYOFFMATCH19,
			PLAYOFFMATCH20
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    DivisionSchedule
        // FullName:  Data_Builder.DivisionSchedule.DivisionSchedule
        // Access:    public 
        // Returns:  
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: BinaryWriter _FileWriter
        //////////////////////////////////////////////////////////////////////////
        public DivisionSchedule(OleDbConnection _theDB, FormMain _theForm, BinaryWriter _FileWriter)
			: base(_theDB, _theForm, _FileWriter)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.DivisionSchedule.DoCreateData
		// Access:    public 
		// Returns:   void
		// Parameter: BinaryWriter _DivisionFileWriter
		// Parameter: int _DivisionID
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData(BinaryWriter _DivisionFileWriter, int _DivisionID)
		{
			byte[] DivSchedule = new byte[SCHEDULE_SIZE];
			// Clear the match schedule
			for (int DivIDCounter = 0; DivIDCounter < SCHEDULE_SIZE; DivIDCounter++)
			{
				DivSchedule[DivIDCounter] = (byte)SCHEDULEEVENT.NOMATCH;
			}

            short iRecordCount = base.DoCountRecords();
            base.ExecuteReader("SELECT * FROM tbl_division_schedules Where ID = " + _DivisionID);
			while (m_Reader.Read())
			{
				//Console.WriteLine("Date " + + "Event " + m_Reader.GetByte((int)DIVISIONSCHEDULE.EVENTID));
				DivSchedule[m_Reader.GetByte((int)DIVISIONSCHEDULE.EVENTDATE)] = m_Reader.GetByte((int)DIVISIONSCHEDULE.EVENTID);
				if (DivSchedule[m_Reader.GetByte((int)DIVISIONSCHEDULE.EVENTDATE)] < (byte)SCHEDULEEVENT.NOMATCH)
				{
					DivSchedule[m_Reader.GetByte((int)DIVISIONSCHEDULE.EVENTDATE)]++;
				}
			}
			m_Reader.Close();

			for (int DivIDCounter = 0; DivIDCounter < SCHEDULE_SIZE; DivIDCounter++)
			{
				_DivisionFileWriter.Write(DivSchedule[DivIDCounter]);
			}
		}
	}
}
