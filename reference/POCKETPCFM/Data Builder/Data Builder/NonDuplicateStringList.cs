using System;
using System.Collections;
using System.Text;
using System.IO;

namespace Data_Builder
{
	public class NonDuplicateStringList
	{
		private ArrayList m_strList = new ArrayList();


		// -----------------------------------------------------------------------
		public int Add(String str)
		{
			return m_strList.Add(str);
		}


		// -----------------------------------------------------------------------
		public String GetString(short ID)
		{
			return (String)m_strList[ID];
		}


		// Find the ID of the string, if not found then adds the string to the list
		// -----------------------------------------------------------------------
		public int FindStringID(String _nNewName)
		{
			int iFound = m_strList.IndexOf(_nNewName);
			if (iFound == -1)
			{
				m_strList.Add(_nNewName);
				iFound = m_strList.Count - 1;
			}
			return (int)iFound;
		}


		// -----------------------------------------------------------------------
		public void Write(BinaryWriter _theFileWriter)
		{
			_theFileWriter.Write((short)m_strList.Count);
			for (short nLoopCount = 0; nLoopCount < m_strList.Count; nLoopCount++)
			{
				Console.WriteLine((String)m_strList[nLoopCount]);
				_theFileWriter.Write((String)m_strList[nLoopCount]);
			}
		}
	}
}
