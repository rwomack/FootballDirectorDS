using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class Playoff : Cup
	{

        //////////////////////////////////////////////////////////////////////////
        // Method:    Playoff
        // FullName:  Data_Builder.Playoff.Playoff
        // Access:    public 
        // Returns:  
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public Playoff(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.Playoff.DoCreateData
		// Access:    public 
		// Returns:   override  void
		//////////////////////////////////////////////////////////////////////////
		override public void DoCreateData()
		{
			string debugCurrentPlayoff = "";
			try
			{
                short iRecordCount = base.DoCountRecords();
				m_FileWriter.Write((short)iRecordCount);

                base.ExecuteReader("SELECT * FROM tbl_cups Where PocketPC = 1 And CupType = " + (int)Cup.CUPTYPE.PLAYOFF);
				while (m_Reader.Read())
				{
					debugCurrentPlayoff = m_Reader.GetString((int)CUP.NAME);
                    m_FileWriter.Write(m_Reader.GetByte((int)CUP.ID));
                    m_FileWriter.Write(m_Reader.GetByte((int)CUP.NUMBERPROMOTED));
                    m_FileWriter.Write(m_Reader.GetInt16((int)CUP.COUNTRY));
                    m_FileWriter.Write(m_Reader.GetByte((int)CUP.NUMCUPCLUBS));
                    base.DoCreateInitData();
					CupSchedule theSchedule = new CupSchedule(m_theDB, m_theForm, m_FileWriter);
                    theSchedule.DoCreateData(m_FileWriter, m_Reader.GetInt16((int)CUP.ID));
				}
                base.Close();
			}
			catch (Exception ee)
			{
				m_theForm.StatusLabel.Text = debugCurrentPlayoff + " " + ee.ToString();
			}
		}
	}
}
