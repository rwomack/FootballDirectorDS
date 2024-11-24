using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class CharityShield : Cup
	{

        //////////////////////////////////////////////////////////////////////////
        // Method:    CharityShield
        // FullName:  Data_Builder.CharityShield.CharityShield
        // Access:    public 
        // Returns:  
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public CharityShield(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.CharityShield.DoCreateData
		// Access:    public 
		// Returns:   override  void
		//////////////////////////////////////////////////////////////////////////
		override public void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
            m_FileWriter.Write((short)iRecordCount);

            base.ExecuteReader("SELECT * FROM tbl_cups Where PocketPC = 1 And CupType = " + (int)Cup.CUPTYPE.CHARITYSHIELD);
			while (m_Reader.Read())
			{
				m_FileWriter.Write(m_Reader.GetInt16((int)CUP.ID));
                m_FileWriter.Write(m_Reader.GetInt16((int)CUP.CHAMPIONS));
                m_FileWriter.Write(m_Reader.GetInt16((int)CUP.RUNNERSUP));
                m_FileWriter.Write(m_Reader.GetInt16((int)CUP.COUNTRY));
                m_FileWriter.Write(m_Reader.GetByte((int)CUP.NUMCUPCLUBS));

                base.DoCreateInitData();
                CupSchedule theSchedule = new CupSchedule(m_theDB, m_theForm, m_FileWriter);
                theSchedule.DoCreateData(m_FileWriter, m_Reader.GetInt16((int)CUP.ID));
			}
            base.Close();
		}
	}
}
