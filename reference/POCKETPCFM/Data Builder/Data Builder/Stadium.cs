using System;
using System.Collections.Generic;
using System.Text;
using System.IO;

namespace Data_Builder
{
	public class Stadium
	{
		protected String m_Name;
		protected int m_Capacity;

		public Stadium(String _Name, int _Capacity)
		{
			m_Name = _Name;
			m_Capacity = _Capacity;
		}

		public void Write(BinaryWriter _theFileWriter)
		{
			_theFileWriter.Write(m_Name);
			_theFileWriter.Write(m_Capacity);
		}
	}
}
