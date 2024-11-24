using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;


namespace Data_Builder
{
	class Formation : RecordReader
	{
		enum FORMATION
		{
			ID,
			FORMATIONNAME,
			POSX01,
			POSY01,
			POSX02,
			POSY02,
			POSX03,
			POSY03,
			POSX04,
			POSY04,
			POSX05,
			POSY05,
			POSX06,
			POSY06,
			POSX07,
			POSY07,
			POSX08,
			POSY08,
			POSX09,
			POSY09,
			POSX10,
			POSY10,
		};


       //////////////////////////////////////////////////////////////////////////
       // Method:    Formation
       // FullName:  Data_Builder.Formation.Formation
       // Access:    public 
       // Returns:  
       // Parameter: OleDbConnection _theDB
       // Parameter: FormMain _theForm
       // Parameter: string _theTable
       // Parameter: string _theFile
       //////////////////////////////////////////////////////////////////////////
       public Formation(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.Formation.DoCreateData
		// Access:    public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData()
		{
			string debugCurrentClub = "";
			try
			{
				short iRecordCount = 10;		// Hard coded
                m_FileWriter.Write((short)iRecordCount);

                base.ExecuteReader("SELECT * FROM tbl_formations ORDER BY `ID` ASC");
				for (short nFormations = 0; nFormations < iRecordCount; nFormations++)
				{
					m_Reader.Read();
					//Console.WriteLine("Formation " + m_Reader.GetInt16((int)FORMATION.FORMATIONNAME));
					for (int nLoopCount = 0; nLoopCount < 10; nLoopCount++)
					{
                        m_FileWriter.Write(m_Reader.GetInt16((int)FORMATION.POSX01 + nLoopCount * 2));
                        m_FileWriter.Write(m_Reader.GetInt16((int)FORMATION.POSY01 + nLoopCount * 2));
					}
				}
				Close();
			}
			catch (Exception ee)
			{
				m_theForm.StatusLabel.Text = debugCurrentClub + " " + ee.ToString();
			}
		}
	}
}
