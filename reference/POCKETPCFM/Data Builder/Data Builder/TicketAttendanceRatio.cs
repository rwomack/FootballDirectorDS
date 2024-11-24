using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	public class TicketAttendanceRatio : RecordReader
	{
		protected enum TICKETATTENDANCERATIO
		{
			TICKETPRICE,
			DIVISIONSTRENGTH0,
			DIVISIONSTRENGTH1,
			DIVISIONSTRENGTH2,
			DIVISIONSTRENGTH3
		};


        public TicketAttendanceRatio(OleDbConnection _theDB, FormMain _theForm, BinaryWriter _FileWriter)
			: base(_theDB, _theForm, _FileWriter)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.TicketAttendanceRatio.DoCreateData
		// Access:    public 
		// Returns:   void
		// Parameter: BinaryWriter _CupFileWriter
		// Parameter: int _DivisionID
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData(BinaryWriter _theFileWriter, int _DivisionID)
		{
			byte[] nTicketAttendanceRatio = new byte[50];
            ExecuteReader("SELECT * FROM tbl_ticket_attendance_ratio ORDER BY 'Ticket Price' ASC");
            for (int Counter = 0; Counter < 50; Counter++)
			{
				m_Reader.Read();
				nTicketAttendanceRatio[Counter] = m_Reader.GetByte((int)TICKETATTENDANCERATIO.DIVISIONSTRENGTH0 + _DivisionID);
				_theFileWriter.Write(nTicketAttendanceRatio[Counter]);
			}
			m_Reader.Close();
		}
	}
}
