using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class InjuryEventType : InjuryType
	{



        //////////////////////////////////////////////////////////////////////////
        // Method:    InjuryEventType
        // FullName:  Data_Builder.InjuryEventType.InjuryEventType
        // Access:    public 
        // Returns:   
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public InjuryEventType(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile) 
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.InjuryEventType.DoCreateData
		// Access:    public 
		// Returns:   override  void
		//////////////////////////////////////////////////////////////////////////
		override public void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
			m_FileWriter.Write((short)iRecordCount);

            base.ExecuteReader("SELECT * FROM tbl_injury WHERE RandomEventInjury = 1");
			while (m_Reader.Read())
			{
				base.DoCreateData();
				base.DoCreateDescription(m_Reader.GetInt32((int)INJURYTYPE.PLURALDESCRIPTION));
			}
            base.Close();
		}
	}
}
