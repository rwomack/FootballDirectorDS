using System;
using System.Collections;
using System.Data.OleDb;
using System.Text;
using System.IO;

using Data_Builder;


namespace Data_Builder
{
	class Division : RecordReader
	{
		public enum DIVISION
		{
			ID,
			NAME,
			NAME_PROPER,
			NAME_FRENCH,
			NAME_ITALIAN,
			NAME_GERMAN,
			NAME_SPANISH,
			POCKETPC,
			COUNTRYID,
			NUMBEROFCLUBS,
			NUMBERPROMOTED,
			NUMBERRELEGATED,
			LASTCHAMPIONS,
			LASTRUNNERSUP,
			POINTSFORWIN,
			DIVISIONSTRENGTH,
			SUBSSELECT,
			SUBSUSE,
			NUMMATCHAGAINTSOPP,
			CLASHCHECK,
			TROPHYGFX,
			CHANCEBUYSAMEFED,
			CHANCESELLSAMEFED,
			CHANCEBUYDIFFERENTFED,
			CHANCESELLDIFFERENTFED,
			SHIELDNAME,
			FEDCUP1,
			FEDCUP2,
			NUMCLUBSFEDCUP1,
			NUMCLUBSFEDCUP2,
			TOPPLAYOFFID,
			BOTTOMPLAYOFFID,
			NUMCLUBSTOPPLAYOFF,
			NUMCLUBSBOTTOMPLAYOFF,
			PLAYOFFFORTITLE,
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    Division
        // FullName:  Data_Builder.Division.Division
        // Access:    public 
        // Returns:
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public Division(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.Division.DoCreateData
		// Access:    public 
		// Returns:   void
		// Parameter: stData _Data
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateData(stData _Data) 
		{
            short iRecordCount = base.DoCountRecords();
            m_FileWriter.Write((short)iRecordCount);

			short nFirstClub = 0;
			DivisionSchedule theSchedule = new DivisionSchedule(m_theDB, m_theForm, m_FileWriter);
			Club theClub = new Club(m_theDB, m_theForm);
			Manager theManager = new Manager(m_theDB, m_theForm, m_FileWriter);

            base.ExecuteReader("SELECT * FROM tbl_divisions Where PocketPC = 1");
			while (m_Reader.Read())
			{
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.SUBSSELECT));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.SUBSUSE));
                m_FileWriter.Write(m_Reader.GetString((int)DIVISION.NAME));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.POINTSFORWIN));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.DIVISIONSTRENGTH));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.NUMBERPROMOTED));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.NUMBERRELEGATED));
                m_FileWriter.Write(m_Reader.GetInt16((int)DIVISION.COUNTRYID));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.CHANCEBUYSAMEFED));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.CHANCESELLSAMEFED));
                m_FileWriter.Write(m_Reader.GetInt16((int)DIVISION.SHIELDNAME));
                m_FileWriter.Write(m_Reader.GetInt16((int)DIVISION.TOPPLAYOFFID));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.NUMCLUBSTOPPLAYOFF));
                m_FileWriter.Write(m_Reader.GetInt16((int)DIVISION.BOTTOMPLAYOFFID));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.NUMCLUBSBOTTOMPLAYOFF));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.PLAYOFFFORTITLE));

                theSchedule.DoCreateData(m_FileWriter, m_Reader.GetInt16((int)DIVISION.ID));
				theClub.DoCreateDivisionClubsData(m_Reader.GetInt16((int)DIVISION.ID), _Data, theManager, nFirstClub);
                m_FileWriter.Write(nFirstClub);
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.NUMBEROFCLUBS));
				nFirstClub += m_Reader.GetByte((int)DIVISION.NUMBEROFCLUBS);
			}
            base.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateSeries60Data
		// FullName:  Data_Builder.Division.DoCreateSeries60Data
		// Access:    public 
		// Returns:   void
		// Parameter: stData _Data
		// Parameter: string _Path
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateSeries60Data(stData _Data, string _Path)
		{
			DivisionSchedule theSchedule = new DivisionSchedule(m_theDB, m_theForm, m_FileWriter);
			//DivisionStrength theDivisionStrength = new DivisionStrength(m_theDB, m_theForm, m_FileWriter);
			Club theClub = new Club(m_theDB, m_theForm);
			Manager theManager = new Manager(m_theDB, m_theForm, m_FileWriter);

			//int iRecordCount = 0;
            base.ExecuteReader("SELECT * FROM tbl_divisions Where PocketPC = 1");
			while (m_Reader.Read())
			{
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.NUMBEROFCLUBS));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.NUMBERPROMOTED));
                m_FileWriter.Write(m_Reader.GetByte((int)DIVISION.NUMBERRELEGATED));

              //  theDivisionStrength.DoCreateDivisionData(iRecordCount++);
                theSchedule.DoCreateData(m_FileWriter, m_Reader.GetInt16((int)DIVISION.ID));

				theClub.DoCreateDivisionClubsSeries60Data(m_Reader.GetInt16((int)DIVISION.ID), _Data, theManager, (byte)m_Reader.GetByte((int)DIVISION.NUMBEROFCLUBS));
			}
            Close();
		}
	}
}
