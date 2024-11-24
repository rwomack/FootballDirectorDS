using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;


namespace Data_Builder
{
	class Country : RecordReader
	{
		public enum COUNTRY
		{
			ID,
			COUNTRYNAME,
			COUNTRYNAME_FRENCH,
			COUNTRYNAME_ITALIAN,
			COUNTRYNAME_GERMAN,
			COUNTRYNAME_SPANISH,
			RATING,
			WAGERATIO,
			FEDERATION,
			NATIONALITY,
			NATIONALITY_FRENCH,
			NATIONALITY_ITALIAN,
			NATIONALITY_GERMAN,
			NATIONALITY_SPANISH,
			NATIONALSTADIUM,
			NEUTRALSTADIUM1,
			NEUTRALSTADIUM2,
			NEUTRALSTADIUM3,
			NEUTRALSTADIUM4,
			NEUTRALSTADIUM5,
			NEUTRALSTADIUM6,
			NEUTRALSTADIUM7,
			CURRENCY,
			NUMDIVISIONS,
			SACKDURINGSEASON,
			ONEYEARSEASON,
			TRANSFERWINDOW1START,
			TRANSFERWINDOW1END,
			TRANSFERWINDOW2START,
			TRANSFERWINDOW2END,
			TRANSFERWINDOW3START,
			TRANSFERWINDOW3END,
			TRANSFERWINDOW4START,
			TRANSFERWINDOW4END,
			CHANCEBUYDIFFFED,
			CHANCESELLDIFFFED,
			FEDERATIONRANKING,
			CHAMPIONSLEAGUEBERTH,
			CUPWINNERSCUPBERTH,
			UEFACUPBERTH
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    Country
        // FullName:  Data_Builder.Country.Country
        // Access:    public 
        // Returns:
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public Country(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.Country.DoCreateData
		// Access:    public 
		// Returns:   void
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
            m_FileWriter.Write((short)iRecordCount);

            m_SQLCommandStr = "SELECT * FROM tbl_countries ORDER BY `ID` ASC";
            OleDbDataReader CountryReader = m_Command.ExecuteReader();		// Member reader gets used elsewhere
			while (CountryReader.Read())
			{
				//	Console.WriteLine("ID " + Reader.GetInt16((int)COUNTRY.ID) + " Name : " + Reader.GetString((int)COUNTRY.NATIONALITY));
                m_FileWriter.Write(CountryReader.GetByte((int)COUNTRY.ID));
                m_FileWriter.Write(CountryReader.GetString((int)COUNTRY.NATIONALITY));
                m_FileWriter.Write(CountryReader.GetByte((int)COUNTRY.RATING));
                m_FileWriter.Write(CountryReader.GetByte((int)COUNTRY.WAGERATIO));
                m_FileWriter.Write(CountryReader.GetByte((int)COUNTRY.FEDERATIONRANKING));

				// Count how many divisions the country has
                base.ExecuteReader("SELECT `ID`,`Division Name` FROM tbl_divisions WHERE CountryID = " + CountryReader.GetByte((int)COUNTRY.ID) + " AND PocketPC = 1 ORDER BY `ID` ASC");
				iRecordCount = 0;
				short nFirstDivisionID = 0;
				while (m_Reader.Read())
				{
					if (iRecordCount == 0)
					{
						nFirstDivisionID = m_Reader.GetInt16((int)Division.DIVISION.ID);
					}
					iRecordCount++;
				}
                m_FileWriter.Write((byte)iRecordCount);

				// If there were active divisions
				if (iRecordCount > 0)
				{
                    m_FileWriter.Write(CountryReader.GetByte((int)COUNTRY.NATIONALSTADIUM));
                    m_FileWriter.Write(CountryReader.GetByte((int)COUNTRY.ONEYEARSEASON));
				}
			}
            base.Close();
			CountryReader.Close();
		}
	}
}
