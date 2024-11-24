using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
	class SkillMoneyValue : RecordReader
	{

		enum SKILL_MONEY_VALUE
		{
			RATING,
			PLAYERVALUE,
			RNDPLAYERVALUE,
			SIGNINGFEE,
			RNDSIGNINGFEE,
			PLAYERWAGE,
			RNDPLAYERWAGE,
			EMPWAGE,
			RNDEMPWAGE
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    SkillMoneyValue
        // FullName:  Data_Builder.SkillMoneyValue.SkillMoneyValue
        // Access:    public 
        // Returns:  
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public SkillMoneyValue(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.SkillMoneyValue.DoCreateData
		// Access:    public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
            m_FileWriter.Write(iRecordCount);

            m_Reader = m_Command.ExecuteReader();
			while (m_Reader.Read())
			{
                m_FileWriter.Write(m_Reader.GetInt32((int)SKILL_MONEY_VALUE.PLAYERVALUE));
                m_FileWriter.Write(m_Reader.GetInt32((int)SKILL_MONEY_VALUE.RNDPLAYERVALUE));
                m_FileWriter.Write(m_Reader.GetInt32((int)SKILL_MONEY_VALUE.PLAYERWAGE));
                m_FileWriter.Write(m_Reader.GetInt32((int)SKILL_MONEY_VALUE.RNDPLAYERWAGE));
                m_FileWriter.Write(m_Reader.GetInt32((int)SKILL_MONEY_VALUE.EMPWAGE));
                m_FileWriter.Write(m_Reader.GetInt32((int)SKILL_MONEY_VALUE.RNDEMPWAGE));
			}
            base.Close();
		}
	}
}
