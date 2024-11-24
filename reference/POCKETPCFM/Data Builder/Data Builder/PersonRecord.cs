using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Data_Builder
{
	public class PersonRecord
	{
		protected String m_Forename;
		protected short m_SurnameID;
		protected DateTime m_dateOfBirth;
		protected short m_NationalityID;

		public void setForename(String _Forename)
		{
			m_Forename = _Forename;
		}
		public void setSurnameID(short _SurnameID)
		{
			m_SurnameID = _SurnameID;
		}
		public void setDateOfBirth(DateTime _dateOfBirth)
		{
			m_dateOfBirth = _dateOfBirth;
		}
		public void setNationalityID(short _NationalityID)
		{
			m_NationalityID = _NationalityID;
		}


		// -----------------------------------------------------------------------
		public virtual void Write(BinaryWriter _theFileWriter)
		{
			_theFileWriter.Write((Byte)m_Forename[0]);
			_theFileWriter.Write(m_SurnameID);
			Date theDate = new Date(m_dateOfBirth);
			_theFileWriter.Write(theDate.getJulianDate());
			_theFileWriter.Write(m_NationalityID);
		}
	}
}
