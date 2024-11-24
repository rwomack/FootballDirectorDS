using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;


namespace Data_Builder
{
	class DivisionStrength : RecordReader
	{
		enum DIVISION_STRENGTH
		{
			STRENGTHID,
			MANAGERPOINTSFORWIN,
			MANAGERPOINTSFORDRAW,
			MANAGERPOINTSFORMOM,
			MANAGERPOINTSFORGOODWINAWAY,
			MANAGERPOINTSFORGOODWINHOME,
			MANAGERPOINTSFORTOPWIN,
			MANAGERINITIALSTRESSRATING,
			SEASONSHIRTSPONSORSHIPAMOUNT,
			SEASONTVINCOME,
			SEASONKITSPONSORSHIPAMOUNT
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    DivisionStrength
        // FullName:  Data_Builder.DivisionStrength.DivisionStrength
        // Access:    public 
        // Returns:   // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        //////////////////////////////////////////////////////////////////////////
        public DivisionStrength(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.DivisionStrength.DoCreateData
		// Access:    public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
            Initialise(iRecordCount);

            TicketAttendanceRatio theTicketAttendanceRatio = new TicketAttendanceRatio(m_theDB, m_theForm,  m_FileWriter);

			iRecordCount = 0;
            base.ExecuteReader();
			while (m_Reader.Read())
			{
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION_STRENGTH.MANAGERPOINTSFORWIN));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION_STRENGTH.MANAGERPOINTSFORDRAW));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION_STRENGTH.MANAGERPOINTSFORMOM));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION_STRENGTH.MANAGERPOINTSFORGOODWINAWAY));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION_STRENGTH.MANAGERPOINTSFORGOODWINHOME));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION_STRENGTH.MANAGERPOINTSFORTOPWIN));
				theTicketAttendanceRatio.DoCreateData(null, iRecordCount);
                m_FileWriter.Write(m_Reader.GetInt32((int)DIVISION_STRENGTH.SEASONSHIRTSPONSORSHIPAMOUNT));
                m_FileWriter.Write(m_Reader.GetInt32((int)DIVISION_STRENGTH.SEASONTVINCOME));
                m_FileWriter.Write(m_Reader.GetInt32((int)DIVISION_STRENGTH.SEASONKITSPONSORSHIPAMOUNT));
				iRecordCount++;
			}
            base.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateDivisionData
		// FullName:  Data_Builder.DivisionStrength.DoCreateDivisionData
		// Access:    public 
		// Returns:   void
		// Parameter: int _DivisionID
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateDivisionData(int _DivisionID)
		{
            base.ExecuteReader("SELECT * FROM tbl_division_strength Where StrengthID = " + _DivisionID);

			TicketAttendanceRatio theTicketAttendanceRatio = new TicketAttendanceRatio(m_theDB, m_theForm, m_FileWriter);
			theTicketAttendanceRatio.DoCreateData(m_FileWriter, _DivisionID);

			m_Reader.Read();
			m_FileWriter.Write(m_Reader.GetInt32((int)DIVISION_STRENGTH.SEASONSHIRTSPONSORSHIPAMOUNT));
			m_FileWriter.Write(m_Reader.GetInt32((int)DIVISION_STRENGTH.SEASONTVINCOME));
			m_FileWriter.Write(m_Reader.GetInt32((int)DIVISION_STRENGTH.SEASONKITSPONSORSHIPAMOUNT));

			m_Reader.Close();
		}
	}
}
