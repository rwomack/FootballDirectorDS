using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.IO;
using System.Text;


namespace Data_Builder
{
	public class CareerPath : RecordReader
	{
		protected enum CAREERPATH
		{
			ID,
			DESCRIPTION,
			OUTFIELD,
			FREQUENCY,
			AGE16,
			AGE17,
			AGE18,
			AGE19,
			AGE20,
			AGE21,
			AGE22,
			AGE23,
			AGE24,
			AGE25,
			AGE26,
			AGE27,
			AGE28,
			AGE29,
			AGE30,
			AGE31,
			AGE32,
			AGE33,
			AGE34,
			AGE35,
			AGE36,
			AGE37,
			AGE38,
			AGE39,
			AGE40,
		};


        /// <summary>
        /// Initializes a new instance of the <see cref="CareerPath"/> class.
        /// </summary>
        /// <param name="_theDB">The _the DB.</param>
        /// <param name="_theForm">The _the form.</param>
        /// <param name="_theTable">The _the table.</param>
        /// <param name="_theFile">The _the file.</param>
        public CareerPath(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile) 
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


        /// <summary>
        /// Does the create data.
        /// </summary>
		public void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
            base.Initialise(iRecordCount / 2);   // 2 records per data item

            // Goalkeeper Count
            base.ExecuteReader();
			iRecordCount = 0;
			while (m_Reader.Read())
			{
				if (m_Reader.GetInt32((int)CAREERPATH.OUTFIELD) == 0)
				{
					iRecordCount++;
					m_Reader.Read();
				}
			}
            m_FileWriter.Write((byte)iRecordCount);

			// Goalkeeper
            base.ExecuteReader();
			while (m_Reader.Read())
			{
				if (m_Reader.GetInt32((int)CAREERPATH.OUTFIELD) == 0)
				{
					m_FileWriter.Write(m_Reader.GetByte((int)CAREERPATH.FREQUENCY));
                    DoOutBinCareerPathRecord(m_FileWriter);	// Positive values
					m_Reader.Read();
                    DoOutBinCareerPathRecord(m_FileWriter);	// Negative values
				}
			}

			// Outfield 
            base.ExecuteReader();
			while (m_Reader.Read())
			{
				if (m_Reader.GetInt32((int)CAREERPATH.OUTFIELD) == 1)
				{
					m_FileWriter.Write(m_Reader.GetByte((int)CAREERPATH.FREQUENCY));
                    DoOutBinCareerPathRecord(m_FileWriter);
					m_Reader.Read();
                    DoOutBinCareerPathRecord(m_FileWriter);
				}
			}
            base.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoOutBinCareerPathRecord
		// FullName:  Data_Builder.CareerPath.DoOutBinCareerPathRecord
		// Access:    protected 
		// Returns:   void
		// Parameter: BinaryWriter _FileWriter
		//////////////////////////////////////////////////////////////////////////
		protected void DoOutBinCareerPathRecord(BinaryWriter _FileWriter)
		{
			for (int wCounter = 0; wCounter < 25; wCounter++)
			{
				// Db value is double, app value is byte * 100
				_FileWriter.Write((byte)(m_Reader.GetDouble((int)CAREERPATH.AGE16 + wCounter) * 100));
			}
		}
	}
}
