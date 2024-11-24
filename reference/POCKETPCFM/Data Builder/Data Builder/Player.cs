using System;
using System.Collections;
using System.Data.OleDb;
using System.Text;
using System.IO;

using Data_Builder;


namespace Data_Builder
{
	public class Player : RecordReader
	{
		enum PLAYER
		{
			ID,
			CLUBID,
			FORENAME,
			SURNAME,
			DATEOFBIRTH,
			NATIONALITYID,
			POSITION,
			SQUADNUMBER,
			BESTFOOT,
			OVERALLSKILL,
			TEMPERAMENT,
			STRENGTH,
			HANDLING,
			TACKLING,
			PASSING,
			SHOOTING,
			PACE,
			HEADING,
			FLAIR
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    Player
        // FullName:  Data_Builder.Player.Player
        // Access:    public 
        // Returns:  
        // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        //////////////////////////////////////////////////////////////////////////
        public Player(OleDbConnection _theDB, FormMain _theForm)
			: base(_theDB, _theForm)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateClubData
		// FullName:  Data_Builder.Player.DoCreateClubData
		// Access:    public 
		// Returns:   void
		// Parameter: short _ClubID
		// Parameter: stData _Data
		// Parameter: int _DBClubID
		//////////////////////////////////////////////////////////////////////////
		public void DoCreateClubData(short _ClubID, stData _Data, int _DBClubID)
		{
			PlayerRecord thePlayerRecord;
            base.ExecuteReader("SELECT * FROM tbl_players WHERE ClubID = " + _DBClubID);
			while (m_Reader.Read())
			{
				thePlayerRecord = new PlayerRecord();
				thePlayerRecord.setClubID(_ClubID);

				thePlayerRecord.setForename(m_Reader.GetString((int)PLAYER.FORENAME).Substring(0, 1));
				thePlayerRecord.setSurnameID((short)_Data.m_SurnameList.FindStringID(m_Reader.GetString((int)PLAYER.SURNAME)));

				thePlayerRecord.setDateOfBirth(m_Reader.GetDateTime((int)PLAYER.DATEOFBIRTH));
				thePlayerRecord.setNationalityID(m_Reader.GetInt16((int)PLAYER.NATIONALITYID));
				thePlayerRecord.setTemperament(m_Reader.GetByte((int)PLAYER.TEMPERAMENT));

				// CSoccerPlayer data,
				thePlayerRecord.setOverallSkill(m_Reader.GetByte((int)PLAYER.OVERALLSKILL));
				thePlayerRecord.setHandling(m_Reader.GetByte((int)PLAYER.HANDLING));
				thePlayerRecord.setTackling(m_Reader.GetByte((int)PLAYER.TACKLING));
				thePlayerRecord.setPassing(m_Reader.GetByte((int)PLAYER.PASSING));
				thePlayerRecord.setShooting(m_Reader.GetByte((int)PLAYER.SHOOTING));
				thePlayerRecord.setPace(m_Reader.GetByte((int)PLAYER.PACE));
				thePlayerRecord.setPosition(m_Reader.GetByte((int)PLAYER.POSITION));
				thePlayerRecord.setBestFoot(m_Reader.GetByte((int)PLAYER.BESTFOOT));
				thePlayerRecord.setHeading(m_Reader.GetByte((int)PLAYER.HEADING));
				thePlayerRecord.setStrength(m_Reader.GetByte((int)PLAYER.HEADING));
				thePlayerRecord.setFlair(m_Reader.GetByte((int)PLAYER.FLAIR));

				_Data.m_PlayerList.Add(thePlayerRecord);
			}
			m_Reader.Close();
		}
	}
}
