using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Data_Builder
{
    /// <summary>
    /// 
    /// </summary>
	public class ClubRecord
	{
        protected String m_Name;
        public String Name { get { return m_Name; } set { m_Name = value; } }
		
        protected short m_StadiumID;
        public short StadiumID { get { return m_StadiumID; } set { m_StadiumID = value; } }
		
        protected byte m_PopularityID;
        public byte PopularityID { get { return m_PopularityID; } set { m_PopularityID = value; } }
		
        protected byte m_RegionID;

		protected byte m_DivisionID;
		protected short m_YearFounded;
		protected short m_ManagerID;

		public void setRegionID(byte _RegionID)
		{
			m_RegionID = _RegionID;
		}
		public void setDivisionID(byte _DivisionID)
		{
			m_DivisionID = _DivisionID;
		}
		public void setYearFounded(short _YearFounded)
		{
			m_YearFounded = _YearFounded;
		}
		public void setManagerID(short _ManagerID)
		{
			m_ManagerID = _ManagerID;
		}


        /// <summary>
        /// Writes the specified _the file writer.
        /// </summary>
        /// <param name="_theFileWriter">The _the file writer.</param>
		public void Write(BinaryWriter _theFileWriter)
		{
			WriteSeries60(_theFileWriter);
			_theFileWriter.Write(m_YearFounded);
		}


        /// <summary>
        /// Writes the series60.
        /// </summary>
        /// <param name="_theFileWriter">The _the file writer.</param>
		public void WriteSeries60(BinaryWriter _theFileWriter)
		{
			_theFileWriter.Write(m_Name);
			_theFileWriter.Write(m_StadiumID);
			_theFileWriter.Write(m_PopularityID);
			_theFileWriter.Write(m_RegionID);
			_theFileWriter.Write(m_DivisionID);
			_theFileWriter.Write(m_ManagerID);
		}
	}
}
