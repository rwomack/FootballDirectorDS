using System;
using System.Collections;
using System.Data.OleDb;
using System.Text;
using System.IO;

using Data_Builder;


namespace Data_Builder
{
	class Club : RecordReader
	{
		public enum CLUB
		{
			ID,
			NAME,
			DIVISIONID,
			POPULARITYID,
			AVERAGEATTENDANCE,
			STADIUMNAME,
			STADIUMCAPACITY,
			YEARFOUNDED,
			MANAGERID,
			REGIONID
		};

		short                   m_ClubID;


        //////////////////////////////////////////////////////////////////////////
        // Method:    Club
        // FullName:  Data_Builder.Club.Club
        // Access:    public 
        // Returns: 
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        // Parameter: string _theTable
        // Parameter: string _theFile
        //////////////////////////////////////////////////////////////////////////
        public Club(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
            : base(_theDB, _theForm, _theTable, _theFile)
        {
        }

        
        //////////////////////////////////////////////////////////////////////////
        // Method:    Club
        // FullName:  Data_Builder.Club.Club
        // Access:    public 
        // Returns:   // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        //////////////////////////////////////////////////////////////////////////
        public Club(OleDbConnection _theDB, FormMain _theForm)
            : base(_theDB, _theForm)
        {
        }


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateDivisionClubsData
		// FullName:  Data_Builder.Club.DoCreateDivisionClubsData
		// Access:    public 
		// Returns:   void
		// Parameter: int _DivisionID
		// Parameter: stData _Data
		// Parameter: Manager _ManagerList
		// Parameter: int _NumClubs
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateDivisionClubsData(int _DivisionID, stData _Data, Manager _ManagerList, int _NumClubs)
		{
            base.ExecuteReader("SELECT * FROM tbl_clubs WHERE DivisionID = " + _DivisionID);
			Stadium nStadium;
			ClubRecord theClubRecord;
			Player thePlayer = new Player(m_theDB, m_theForm);
			while (m_Reader.Read())
			{
				theClubRecord = new ClubRecord();
				theClubRecord.Name = m_Reader.GetString((int)CLUB.NAME);
				nStadium = new Stadium(m_Reader.GetString((int)CLUB.STADIUMNAME), m_Reader.GetInt32((int)CLUB.STADIUMCAPACITY));
                _Data.m_StadiumList.Add(nStadium);
				theClubRecord.StadiumID = (short)_Data.m_StadiumList.Count;
				theClubRecord.PopularityID = m_Reader.GetByte((int)CLUB.POPULARITYID);
				theClubRecord.setRegionID(m_Reader.GetByte((int)CLUB.REGIONID));
				theClubRecord.setDivisionID((byte)_DivisionID);
				theClubRecord.setYearFounded(m_Reader.GetInt16((int)CLUB.YEARFOUNDED));
				theClubRecord.setManagerID(_ManagerList.DoCreateClubData(m_Reader.GetInt16((int)CLUB.MANAGERID), _Data));

				_Data.m_ClubList.Add(theClubRecord);
				thePlayer.DoCreateClubData(m_ClubID, _Data, m_Reader.GetInt16((int)CLUB.ID));
				m_ClubID++;
			}
			m_Reader.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateDivisionClubsSeries60Data
		// FullName:  Data_Builder.Club.DoCreateDivisionClubsSeries60Data
		// Access:    public 
		// Returns:   void
		// Parameter: int _DivisionID
		// Parameter: stData _Data
		// Parameter: Manager _ManagerList
		// Parameter: int _NumClubs
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateDivisionClubsSeries60Data(int _DivisionID, stData _Data, Manager _ManagerList, int _NumClubs)
		{
            base.ExecuteReader("SELECT * FROM tbl_clubs WHERE DivisionID = " + _DivisionID);
			Stadium nStadium;
			ClubRecord theClubRecord;
			Player thePlayer = new Player(m_theDB, m_theForm);
			while (m_Reader.Read())
			{
				theClubRecord = new ClubRecord();
				theClubRecord.Name = m_Reader.GetString((int)CLUB.NAME);
				nStadium = new Stadium(m_Reader.GetString((int)CLUB.STADIUMNAME), m_Reader.GetInt32((int)CLUB.STADIUMCAPACITY));
                _Data.m_StadiumList.Add(nStadium);
                theClubRecord.StadiumID = (short)_Data.m_StadiumList.Count;
				theClubRecord.PopularityID = m_Reader.GetByte((int)CLUB.POPULARITYID);
				theClubRecord.setRegionID(m_Reader.GetByte((int)CLUB.REGIONID));
				theClubRecord.setDivisionID((byte)_DivisionID);
				theClubRecord.setManagerID(_ManagerList.DoCreateClubData(m_Reader.GetInt16((int)CLUB.MANAGERID), _Data));

				_Data.m_ClubList.Add(theClubRecord);
				thePlayer.DoCreateClubData(m_ClubID, _Data, m_Reader.GetInt16((int)CLUB.ID));
				m_ClubID++;
			}
			m_Reader.Close();
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateCupClubsData
		// FullName:  Data_Builder.Club.DoCreateCupClubsData
		// Access:    public 
		// Returns:   void
		// Parameter: BinaryWriter _theFileWriter
		// Parameter: int _NumClubs
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateCupClubsData(BinaryWriter _theFileWriter, int _NumClubs)
		{
            base.ExecuteReader("SELECT * FROM tbl_clubs WHERE DivisionID = 4 LIMIT 0, " + _NumClubs);	// Assumes all cup clubs are in div 4
			while (m_Reader.Read())
			{
				_theFileWriter.Write(m_Reader.GetString((int)CLUB.NAME));
				_theFileWriter.Write((short)0);//m_Reader.GetInt16((int)CLUB.STADIUMNAME));
			}
			m_Reader.Close();
		}
	}
}
