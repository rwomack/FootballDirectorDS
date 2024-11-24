using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Data_Builder
{
	public class PlayerRecord : PersonRecord
	{
		protected short m_ClubID;
		protected byte m_Position;
		protected byte m_Temperament;
		protected byte m_OverallSkill;
		protected byte m_Handling;
		protected byte m_Tackling;
		protected byte m_Passing;
		protected byte m_Shooting;
		protected byte m_Pace;
		protected byte m_Heading;
		protected byte m_Strength;
		protected byte m_Flair;

		// Player types
		enum ePLAYERPOSITION
		{
			FIRSTPOSN,

			GOALKEEPER = FIRSTPOSN,
			DEFENDER,
			MIDFIELDER,
			ATTACKER,

			LASTPOSN = ATTACKER,
			MAXPLAYERPOSITIONS
		};

		enum eBESTFOOT
		{
			LEFTFOOT,
			RIGHTFOOT
		};
		

		public void setClubID(short _ClubID)
		{
			m_ClubID = _ClubID;
		}
		public void setTemperament(byte _Temperament)
		{
			m_Temperament = _Temperament;
		}
		public void setOverallSkill(byte _OverallSkill)
		{
			m_OverallSkill = _OverallSkill;
		}
		public void setHandling(byte _Handling)
		{
			m_Handling = _Handling;
		}
		public void setTackling(byte _Tackling)
		{
			m_Tackling = _Tackling;
		}
		public void setPassing(byte _Passing)
		{
			m_Passing = _Passing;
		}
		public void setShooting(byte _Shooting)
		{
			m_Shooting = _Shooting;
		}
		public void setBestFoot(byte _BestFoot)
		{
			m_Position = (byte)(m_Position | (_BestFoot * 128));
		}
		public void setPace(byte _Pace)
		{
			m_Pace = _Pace;
		}
		public void setPosition(byte _Position)
		{
			// Doesnt retain _BestFoot! set it after
			if ((ePLAYERPOSITION)_Position > ePLAYERPOSITION.MAXPLAYERPOSITIONS)
			{
				Console.WriteLine("Illegal Player Position " + _Position);
			}
			m_Position = (byte)(_Position - 1);
		}
		public void setHeading(byte _Heading)
		{
			m_Heading = _Heading;
		}
		public void setStrength(byte _Strength)
		{
			m_Strength = _Strength;
		}
		public void setFlair(byte _Flair)
		{
			m_Flair = _Flair;
		}


		// -----------------------------------------------------------------------
		override public void Write(BinaryWriter _theFileWriter)
		{
			_theFileWriter.Write(m_ClubID);
			base.Write(_theFileWriter);
			_theFileWriter.Write((byte)(m_Temperament / 10));	// *** In db 0-100, in game 0-10 ***

			_theFileWriter.Write(m_Handling);
			_theFileWriter.Write(m_Tackling);
			_theFileWriter.Write(m_Passing);
			_theFileWriter.Write(m_Shooting);
			_theFileWriter.Write(m_Pace);
			_theFileWriter.Write(m_Position);
			_theFileWriter.Write(m_Heading);
			_theFileWriter.Write(m_Strength);
			_theFileWriter.Write(m_Flair);
		}


		// -----------------------------------------------------------------------
		/*override public void WriteSeries60(BinaryWriter _CupFileWriter)
		{
			_CupFileWriter.Write(m_ClubID);
			base.Write(_CupFileWriter);
			_CupFileWriter.Write((byte)(m_Temperament / 10));	// *** In db 0-100, in game 0-10 ***

			_CupFileWriter.Write(m_Position);
			_CupFileWriter.Write(m_Strength);
			_CupFileWriter.Write(m_Pace);

			_CupFileWriter.Write(m_OverallSkill);
		}*/
	}
}
