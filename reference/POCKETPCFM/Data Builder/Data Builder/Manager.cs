using System;
using System.Collections;
using System.Data.OleDb;
using System.Text;
using System.IO;


namespace Data_Builder
{
	public class Manager : RecordReader
	{
		public enum MANAGER
		{
			ID,
			FORENAME,
			SURNAME,
			DATEOFBIRTH,
			NATIONALITY
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    Manager
        // FullName:  Data_Builder.Manager.Manager
        // Access:    public 
        // Returns: 
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public Manager(OleDbConnection _theDB, FormMain _theForm, BinaryWriter _FileWriter)
			: base(_theDB, _theForm, _FileWriter)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.Manager.DoCreateData
		// Access:    virtual public 
		// Returns:   void
		// Parameter: NonDuplicateStringList _Surnames
		//////////////////////////////////////////////////////////////////////////
		public virtual void DoCreateData(NonDuplicateStringList _Surnames)
		{
			FileStream theFile = new FileStream("Managers.bin", FileMode.Create, FileAccess.Write);
			BinaryWriter theFileWriter = new BinaryWriter(theFile);

			short iRecordCount = DoCountRecords();
			theFileWriter.Write((short)iRecordCount);

            base.ExecuteReader("SELECT * FROM tbl_managers ORDER BY `ID` ASC");
			String str;
			int SurnameID;
			while (m_Reader.Read())
			{
				str = m_Reader.GetString((int)MANAGER.FORENAME).Substring(0, 1);
				theFileWriter.Write(str);
				SurnameID = _Surnames.FindStringID(m_Reader.GetString((int)MANAGER.SURNAME));
				theFileWriter.Write(SurnameID);

				DateTime tr = m_Reader.GetDateTime((int)MANAGER.DATEOFBIRTH);
				theFileWriter.Write(tr.ToBinary());
				//theFileWriter.Write((short)nNationalityTranslationID[m_Reader.GetInt16((int)MANAGER.NATIONALITY)]);
			}
			theFileWriter.Close();
			m_Reader.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateClubData
		// FullName:  Data_Builder.Manager.DoCreateClubData
		// Access:    public 
		// Returns:   short
		// Parameter: int _ID
		// Parameter: stData _Data
		//////////////////////////////////////////////////////////////////////////
		public short DoCreateClubData(int _ID, stData _Data)
		{
            base.ExecuteReader("SELECT * FROM tbl_managers WHERE ID = " + _ID);
			m_Reader.Read();

			PersonRecord theManagerRecord = new PersonRecord();
			theManagerRecord.setForename(m_Reader.GetString((int)MANAGER.FORENAME).Substring(0, 1));
			theManagerRecord.setSurnameID((short)_Data.m_SurnameList.FindStringID(m_Reader.GetString((int)MANAGER.SURNAME)));
			theManagerRecord.setDateOfBirth(m_Reader.GetDateTime((int)MANAGER.DATEOFBIRTH));
			
            base.ExecuteReader();
			m_Reader.Read();
			theManagerRecord.setNationalityID(m_Reader.GetInt16(2));
            _Data.m_ManagerList.Add(theManagerRecord);
            short rRet = (short)_Data.m_ManagerList.Count;
            m_Reader.Close();
			return rRet;
		}
	}
}
