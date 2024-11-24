using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.IO;
using System.Text;



namespace Data_Builder
{
	public class RecordReader
	{
		protected OleDbDataReader   m_Reader;
		protected FormMain          m_theForm;
		protected OleDbConnection   m_theDB;
        protected OleDbCommand      m_Command;
        protected string            m_SQLCommandStr;
        protected BinaryWriter      m_FileWriter;


        //////////////////////////////////////////////////////////////////////////
        // Method:    RecordReader
        // FullName:  Data_Builder.RecordReader.RecordReader
        // Access:    public 
        // Returns:   // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        //////////////////////////////////////////////////////////////////////////
        public RecordReader(OleDbConnection _theDB, FormMain _theForm)
        {
            m_theDB = _theDB;
            m_theForm = _theForm;
        }


        //////////////////////////////////////////////////////////////////////////
        // Method:    RecordReader
        // FullName:  Data_Builder.RecordReader.RecordReader
        // Access:    public 
        // Returns:   
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: BinaryWriter _FileWriter
        //////////////////////////////////////////////////////////////////////////
        public RecordReader(OleDbConnection _theDB, FormMain _theForm, BinaryWriter _FileWriter)
        {
            m_theDB = _theDB;
            m_theForm = _theForm;
            m_FileWriter = _FileWriter;
            m_SQLCommandStr = "SELECT * FROM ";
            m_Command = new OleDbCommand(m_SQLCommandStr, m_theDB);
        }


        //////////////////////////////////////////////////////////////////////////
        // Method:    ExecuteReader
        // FullName:  Data_Builder.RecordReader.ExecuteReader
        // Access:    public 
        // Returns:   void
        // Parameter: string _SQLComandStr
        //////////////////////////////////////////////////////////////////////////
        public void ExecuteReader(string _SQLComandStr)
        {
            m_SQLCommandStr = _SQLComandStr;
            ExecuteReader();
        }


        //////////////////////////////////////////////////////////////////////////
        // Method:    ExecuteReader
        // FullName:  Data_Builder.RecordReader.ExecuteReader
        // Access:    public 
        // Returns:   void
        //////////////////////////////////////////////////////////////////////////
        public void ExecuteReader()
        {
            m_Command.CommandText = m_SQLCommandStr;
            m_Reader = m_Command.ExecuteReader();
        }

        //////////////////////////////////////////////////////////////////////////
        // Method:    RecordReader
        // FullName:  Data_Builder.RecordReader.RecordReader
        // Access:    public 
        // Returns:   
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public RecordReader(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
        {
            m_theDB = _theDB;
            m_theForm = _theForm;
            FileStream theFileStream = new FileStream(_theFile + ".bin", FileMode.Create, FileAccess.Write);
            m_FileWriter = new BinaryWriter(theFileStream);
            m_SQLCommandStr = "SELECT * FROM " + _theTable;
            m_Command = new OleDbCommand(m_SQLCommandStr, m_theDB);
        }


		public OleDbDataReader Reader
		{
            get
            {
                return m_Reader;
            }
		}


        //////////////////////////////////////////////////////////////////////////
        // Method:    Initialise
        // FullName:  Data_Builder.RecordReader.Initialise
        // Access:    public 
        // Returns:   void
        // Parameter: string _FileName
        // Parameter: int _RecordCount
        //////////////////////////////////////////////////////////////////////////
        public void Initialise(int _RecordCount)
        {
            m_FileWriter.Write(_RecordCount);
        }


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCountRecords
		// FullName:  Data_Builder.RecordReader.DoCountRecords
		// Access:    virtual protected 
		// Returns:   short
		//////////////////////////////////////////////////////////////////////////
		protected virtual short DoCountRecords()
		{
			short iCount = 0;
			m_Reader = m_Command.ExecuteReader();
			while (m_Reader.Read())
			{
				iCount++;
			}
			m_Reader.Close();

			int iTableName = m_SQLCommandStr.LastIndexOf(" ");
			iTableName++;
			String header = m_SQLCommandStr.Substring(iTableName, m_SQLCommandStr.Length - iTableName);
			m_theForm.StatusLabel.Text = header + " : "+ iCount + " Records " ;
			m_theForm.Refresh();
			return iCount;
		}


        //////////////////////////////////////////////////////////////////////////
        // Method:    Close
        // FullName:  Data_Builder.RecordReader.Close
        // Access:    public 
        // Returns:   void
        //////////////////////////////////////////////////////////////////////////
        public void Close()
        {
            m_FileWriter.Close();
            m_Reader.Close();
        }
	}
}
