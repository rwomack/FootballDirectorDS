using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;



namespace Data_Builder
{
    public struct stData
    {
        public NonDuplicateStringList m_SurnameList;
        public List<Stadium> m_StadiumList;
        public List<PlayerRecord> m_PlayerList;
        public List<PersonRecord> m_ManagerList;
        public List<ClubRecord> m_ClubList;
        public List<TacticRecord> m_TacticList;
    };
    
    public class DataBuilder
	{
        protected OleDbConnection       m_theDB;
		protected stData                m_Data;


        //---------------------------------------------------------------------------
        public DataBuilder()
		{
			m_Data.m_SurnameList = new NonDuplicateStringList();
			m_Data.m_StadiumList = new List<Stadium>();
			m_Data.m_PlayerList = new List<PlayerRecord>();
			m_Data.m_ManagerList = new List<PersonRecord>();
            m_Data.m_ClubList = new List<ClubRecord>();
            m_Data.m_TacticList = new List<TacticRecord>();
        }


		/// <summary>
        /// Does the create pocket PC data.
        /// </summary>
        /// <param name="_theForm">The _the form.</param>
        /// <returns></returns>
		public bool DoCreatePocketPCData(FormMain _theForm)
		{
			bool bRet = true;
			//try
			{
                m_theDB = new OleDbConnection(@"Provider=Microsoft.Jet.OLEDB.4.0;Data Source=D:\PROJECTS\Sports\Data\FootballDirector.mdb");
				m_theDB.Open();

              //  CareerPath theCareerPath = new CareerPath(m_theDB, _theForm, "tblPlayerCareerPaths", "CareerPath");
				//theCareerPath.DoCreateData();

                //Popularity thePopularity = new Popularity(m_theDB, _theForm, "tblPopularity", "Popularity");
				//thePopularity.DoCreateData();

                //DivisionStrength theDivisionStrength = new DivisionStrength(m_theDB, _theForm, "tblDivStrength", "DivisionStrength");
				//theDivisionStrength.DoCreateData();

				//SkillMoneyValue theSkillMoneyValue = new SkillMoneyValue(m_theDB, _theForm, "tbl_skill_money_value", "SkillMoneyValue");
				//theSkillMoneyValue.DoCreateData();

                //Formation theFormations = new Formation(m_theDB, _theForm, "tbl_formations", "Formations");
				//theFormations.DoCreateData();

                //Federation theFederation = new Federation(m_theDB, _theForm, "tblFederations", "Federation");
				//theFederation.DoCreateData();

                //Country theCountry = new Country(m_theDB, _theForm, "tblCountries", "Country");
				//theCountry.DoCreateData();

                //Division theDivision = new Division(m_theDB, _theForm, "tblDivisions", "Division");
				//theDivision.DoCreateData(m_Data);

                //Playoff thePlayoff = new Playoff(m_theDB, _theForm, "tblCups", "Playoff");
				//thePlayoff.DoCreateData();

                //CharityShield theCharityShield = new CharityShield(m_theDB, _theForm, "tblCups", "CharityShield");
				//theCharityShield.DoCreateData();

                //Cup theCup = new Cup(m_theDB, _theForm, "tblCups", "Cup");
				//theCup.DoCreateData();

               // InjuryType theInjuryType = new InjuryType(m_theDB, _theForm, "tbl_injury", "InjuryType");
				//theInjuryType.DoCreateData();

                //InjuryEventType theInjuryEventType = new InjuryEventType(m_theDB, _theForm, "tbl_injury", "InjuryEventType");
				//theInjuryEventType.DoCreateData();

                //TacticRecord theTacticType = new TacticRecord(m_theDB, _theForm, "tblTactic", "Tactic");
                //theTacticType.DoCreateData();

                FileStream theFile = new FileStream("Surnames.bin", FileMode.Create, FileAccess.Write);
				BinaryWriter theFileWriter = new BinaryWriter(theFile);
				m_Data.m_SurnameList.Write(theFileWriter);
				theFileWriter.Close();
				theFile.Close();

				theFile = new FileStream("Stadium.bin", FileMode.Create, FileAccess.Write);
				theFileWriter = new BinaryWriter(theFile);
				theFileWriter.Write((short)m_Data.m_StadiumList.Count);
				foreach (Stadium stad in m_Data.m_StadiumList)
				{
					stad.Write(theFileWriter);
				}
				theFileWriter.Close();
				theFile.Close();

                theFile = new FileStream("Club.bin", FileMode.Create, FileAccess.Write);
				theFileWriter = new BinaryWriter(theFile);
				theFileWriter.Write((short)m_Data.m_ClubList.Count);
				foreach (ClubRecord club in m_Data.m_ClubList)
				{
					club.Write(theFileWriter);
				}
				theFileWriter.Close();
				theFile.Close();

				theFile = new FileStream("Player.bin", FileMode.Create, FileAccess.Write);
				theFileWriter = new BinaryWriter(theFile);
				theFileWriter.Write((short)m_Data.m_PlayerList.Count);
				foreach (PlayerRecord player in m_Data.m_PlayerList)
				{
					player.Write(theFileWriter);
				}
				theFileWriter.Close();
				theFile.Close();

				theFile = new FileStream("Manager.bin", FileMode.Create, FileAccess.Write);
				theFileWriter = new BinaryWriter(theFile);
				theFileWriter.Write((short)m_Data.m_ManagerList.Count);
				foreach (PersonRecord manager in m_Data.m_ManagerList)
				{
					manager.Write(theFileWriter);
				}
				theFileWriter.Close();
				theFile.Close();

				m_theDB.Close();
			}
			/*catch (Exception ee)
			{
				_theForm.StatusLabel.Text = ee.ToString();
				bRet = false;
			}*/
			return bRet;
		}


		//---------------------------------------------------------------------------
		public bool DoCreateSeries60Data(FormMain _theForm)
		{
			bool bRet = true;
			/*string OutputPath = "\\Projects\\FootballDirector\\Res";
			try
			{
                //m_theDB = new OleDbConnection(new OleDbConnectionString("localhost",
				//													  "soccer",
				//													  "laptop",
																	  "").AsString);
				//m_theDB.Open();

			//	CareerPath theCareerPath = new CareerPath(m_theDB, _theForm);
			//	theCareerPath.DoCreateData();

				Popularity thePopularity = new Popularity(m_theDB, _theForm);
				thePopularity.DoCreateSeries60Data(OutputPath);

				//SkillMoneyValue theSkillMoneyValue = new SkillMoneyValue(m_theDB, _theForm);
				//theSkillMoneyValue.DoCreateData();

				//Formation theFormations = new Formation(m_theDB, _theForm);
				//theFormations.DoCreateData();

				//Federation theFederation = new Federation(m_theDB, _theForm);
				//theFederation.DoCreateData();

				//Country theCountry = new Country(m_theDB, _theForm);
				//theCountry.DoCreateData();

				Division theDivision = new Division(m_theDB, _theForm);
				theDivision.DoCreateSeries60Data(m_Data, OutputPath);

				//Playoff thePlayoff = new Playoff(m_theDB, _theForm);
				//thePlayoff.DoCreateData();

				//CharityShield theCharityShield = new CharityShield(m_theDB, _theForm);
				//theCharityShield.DoCreateData();

				//Cup theCup = new Cup(m_theDB, _theForm);
				//theCup.DoCreateData();

				//InjuryType theInjuryType = new InjuryType(m_theDB, _theForm);
				//theInjuryType.DoCreateData();

				//InjuryEventType theInjuryEventType = new InjuryEventType(m_theDB, _theForm);
				//theInjuryEventType.DoCreateData();

				FileStream theFile = new FileStream(OutputPath + "\\Surnames.bin", FileMode.Create, FileAccess.Write);
				BinaryWriter theFileWriter = new BinaryWriter(theFile);
				m_Data.m_SurnameList.Write(theFileWriter);
				theFileWriter.Close();
				theFile.Close();

				theFile = new FileStream(OutputPath + "\\Stadiums.bin", FileMode.Create, FileAccess.Write);
				theFileWriter = new BinaryWriter(theFile);
				theFileWriter.Write((short)m_Data.m_StadiumList.Count);
				foreach (Stadium stad in m_Data.m_StadiumList)
				{
					stad.Write(theFileWriter);
				}
				theFileWriter.Close();
				theFile.Close();

				theFile = new FileStream(OutputPath + "\\Clubs.bin", FileMode.Create, FileAccess.Write);
				theFileWriter = new BinaryWriter(theFile);
				theFileWriter.Write((short)m_Data.m_ClubList.Count);
				foreach (ClubRecord club in m_Data.m_ClubList)
				{
					club.WriteSeries60(theFileWriter);
				}
				theFileWriter.Close();
				theFile.Close();

				theFile = new FileStream(OutputPath + "\\Players.bin", FileMode.Create, FileAccess.Write);
				theFileWriter = new BinaryWriter(theFile);
				theFileWriter.Write((short)m_Data.m_PlayerList.Count);
				foreach (PlayerRecord player in m_Data.m_PlayerList)
				{
					player.Write(theFileWriter);
				}
				theFileWriter.Close();
				theFile.Close();

				/*theFile = new FileStream("Managers.bin", FileMode.Create, FileAccess.Write);
				theFileWriter = new BinaryWriter(theFile);
				theFileWriter.Write((short)m_Data.m_ManagerList.Count);
				foreach (PersonRecord manager in m_Data.m_ManagerList)
				{
					manager.Write(theFileWriter);
				}
				theFileWriter.Close();
				theFile.Close();

				m_theDB.Close();
			}
			catch (Exception ee)
			{
				_theForm.StatusLabel.Text = ee.ToString();
				bRet = false;
			}*/
			return bRet;
		}
	}
}
