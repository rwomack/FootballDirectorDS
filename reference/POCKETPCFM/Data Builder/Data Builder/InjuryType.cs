using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class InjuryType : RecordReader
	{
		public enum INJURYTYPE
		{
			ID,
			RANDOMEVENTINJURY,
			MAXRND,
			TYPE,
			BASEPERIOD,
			RNDPERIOD,
			PLURAL,
			REOCCURRENCECHANCE,
			WEEKSTOSURGERY,
			MATCHPERFORMANCEEFFECT,
			RNDMATCHPERFORMANCEEFFECT,
			DESCRIPTION,
			PLURALDESCRIPTION,
			CARRYOFF,
			GENERATENEWSPAPERHEADLINES,
			PHYSIOACTIVE
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    InjuryType
        // FullName:  Data_Builder.InjuryType.InjuryType
        // Access:    public 
        // Returns:   
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public InjuryType(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.InjuryType.DoCreateData
		// Access:    virtual public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public virtual void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
			m_FileWriter.Write((short)iRecordCount);

            base.ExecuteReader("SELECT * FROM tbl_injury WHERE RandomEventInjury = 0");
			while (m_Reader.Read())
			{
				DoCreateElementData();
			}
            base.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateElementData
		// FullName:  Data_Builder.InjuryType.DoCreateElementData
		// Access:    public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateElementData()
		{
            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.MAXRND));
            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.TYPE));
            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.BASEPERIOD));
            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.RNDPERIOD));
            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.PLURAL));
            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.MATCHPERFORMANCEEFFECT));
            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.RNDMATCHPERFORMANCEEFFECT));
			DoCreateDescription(m_Reader.GetInt32((int)INJURYTYPE.DESCRIPTION));

            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.CARRYOFF));
            m_FileWriter.Write(m_Reader.GetByte((int)INJURYTYPE.PHYSIOACTIVE));
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateDescription
		// FullName:  Data_Builder.InjuryType.DoCreateDescription
		// Access:    public 
		// Returns:   void
		// Parameter: int _DescriptionID
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateDescription(int _DescriptionID)
		{
		    OleDbCommand cmd = new OleDbCommand("SELECT * FROM tbl_injury_description Where ID = " + _DescriptionID, m_theDB);
			OleDbDataReader descriptionReader = cmd.ExecuteReader();
			descriptionReader.Read();
			m_FileWriter.Write(descriptionReader.GetString(2));
		}
	}
}
