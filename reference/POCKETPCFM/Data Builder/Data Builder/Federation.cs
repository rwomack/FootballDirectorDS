using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class Federation : RecordReader
	{
		enum FEDERATION
		{
			ID,
			ABBREV_NAME,
			ABBREV_NAME_FRENCH,
			ABBREV_NAME_ITALIAN,
			ABBREV_NAME_GERMAN,
			ABBREV_NAME_SPANISH,
			NAME,
			ADDRESS1,
			ADDRESS2,
			ADDRESS3,
			POSTCODE,
			COUNTRYID,
			TELEPHONENUMBER
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    Federation
        // FullName:  Data_Builder.Federation.Federation
        // Access:    public 
        // Returns:  
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public Federation(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile) 
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.Federation.DoCreateData
		// Access:    public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData()
		{
			short iRecordCount = 7;	/******* Hard code number of federations *****/

            m_FileWriter.Write((short)iRecordCount);

            base.ExecuteReader();
			while (m_Reader.Read())
			{
                m_FileWriter.Write(m_Reader.GetByte((int)FEDERATION.ID));
			}
			Close();
		}
	}
}
