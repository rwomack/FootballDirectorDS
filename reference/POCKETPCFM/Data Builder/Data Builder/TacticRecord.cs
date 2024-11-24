using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.IO;
using System.Text;


namespace Data_Builder
{
    /// <summary>
    /// 
    /// </summary>
    public class TacticRecord : RecordReader
    {
        protected enum TACTICRECORD
        {
            ID,
            TACTICNAME,
            PLAYER1X,
            PLAYER1Y,
            PLAYER2X,
            PLAYER2Y,
            PLAYER3X,
            PLAYER3Y,
            PLAYER4X,
            PLAYER4Y,
            PLAYER5X,
            PLAYER5Y,
            PLAYER6X,
            PLAYER6Y,
            PLAYER7X,
            PLAYER7Y,
            PLAYER8X,
            PLAYER8Y,
            PLAYER9X,
            PLAYER9Y,
            PLAYER10X,
            PLAYER10Y,
            FAMILIARITY
        };

        protected string m_Name;
        public String Name { get { return m_Name; } set { m_Name = value; } }

        protected byte m_DefenceRow;
        public byte DefenceRow { get { return m_DefenceRow; } set { m_DefenceRow = value; } }

        protected byte m_MidfieldRow;
        public byte MidfieldRow { get { return m_MidfieldRow; } set { m_MidfieldRow = value; } }

        protected byte m_AttackRow;
        public byte AttackRow { get { return m_AttackRow; } set { m_AttackRow = value; } }

        protected byte m_Familiarity;
        public byte Familiarity { get { return m_Familiarity; } set { m_Familiarity = value; } }


        /// <summary>
        /// Initializes a new instance of the <see cref="CareerPath"/> class.
        /// </summary>
        /// <param name="_theDB">The _the DB.</param>
        /// <param name="_theForm">The _the form.</param>
        /// <param name="_theTable">The _the table.</param>
        /// <param name="_theFile">The _the file.</param>
        public TacticRecord(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile) 
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


        /// <summary>
        /// Does the create data.
        /// </summary>
		public void DoCreateData()
		{
            short iRecordCount = base.DoCountRecords();
            m_FileWriter.Write((byte)iRecordCount);
            string test;
            base.ExecuteReader();
			while (m_Reader.Read())
			{
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER1X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER1Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER2X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER2Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER3X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER3Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER4X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER4Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER5X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER5Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER6X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER6Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER7X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER7Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER8X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER8Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER9X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER9Y));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER10X));
                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.PLAYER10Y));

                m_FileWriter.Write((byte)m_Reader.GetInt32((byte)TACTICRECORD.FAMILIARITY));
            }

            base.Close();
		}
    }
}
