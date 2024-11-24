using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;


namespace Data_Builder
{
	class TextString : RecordReader
	{
		public enum TEXTSTRING
		{
			ID,
			STRINGID,
			TEXT,
			POCKETPC,
			SERIES60
		};


        //////////////////////////////////////////////////////////////////////////
        // Method:    TextString
        // FullName:  Data_Builder.TextString.TextString
        // Access:    public 
        // Returns:   // Parameter: OleDbConnection _theDB
        // Parameter: FormMain _theForm
        //////////////////////////////////////////////////////////////////////////
        public TextString(OleDbConnection _theDB, FormMain _theForm, string _theTable, string _theFile)
			: base(_theDB, _theForm, _theTable, _theFile)
		{
		}


		//////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.TextString.DoCreateData
		// Access:    virtual public 
		// Returns:   void
		// Parameter: bool bJava
		// Parameter: bool bSeries60
		//////////////////////////////////////////////////////////////////////////
		public virtual void DoCreateData(bool bJava, bool bSeries60)
		{
		//	StreamWriter theTextWriter = new StreamWriter(theSourceFile);
		//	Console.SetOut(theTextWriter);

			if (bJava == true)
			{
				Console.WriteLine("/*");
				Console.WriteLine(" *	Resource.java");
				Console.WriteLine(" * ");
				Console.WriteLine(" * (c) Rw Software 2005");
				Console.WriteLine(" */");
				Console.WriteLine("");
				Console.WriteLine("package SoccerClasses;");
				Console.WriteLine("");
				Console.WriteLine("/**");
				Console.WriteLine("* @author Roger Womack");
				Console.WriteLine("*");
				Console.WriteLine("*/");
				Console.WriteLine("");
				Console.WriteLine("public class Resource");
				Console.WriteLine("{");
			}
			else
			{
				Console.WriteLine("");
				Console.WriteLine("/*");
				Console.WriteLine("");
				Console.WriteLine("	String Resource Defines");
				Console.WriteLine("");
				Console.WriteLine("");
				Console.WriteLine("	Football Director");
				Console.WriteLine("");
				Console.WriteLine("	(c) Rw Software 1994 - 2005");
				Console.WriteLine("");
				Console.WriteLine("---------------------------------------------------------------");
				Console.WriteLine("*/");
				Console.WriteLine("");
				Console.WriteLine("");
				Console.WriteLine("#ifndef __STRINGRESOURCE_H__");
				Console.WriteLine("#define __STRINGRESOURCE_H__");
				Console.WriteLine("");
				Console.WriteLine("class Resource");
				Console.WriteLine("{");
				Console.WriteLine("	public:");
				Console.WriteLine("	enum");
				Console.WriteLine("	{");
			}
			if (bSeries60 == true)
			{
				m_SQLCommandStr = "SELECT * FROM tbl_strings Where Series60 = 1 ORDER BY 'StringID' ASC";
			}
			else
			{
				m_SQLCommandStr = "SELECT * FROM tbl_strings Where PocketPC = 1 ORDER BY 'StringID' ASC";
			}
            short iRecordCount = base.DoCountRecords();
			m_FileWriter.Write((short)iRecordCount);

            base.ExecuteReader();
			int Count = 0;
			while (m_Reader.Read())
			{
				//theFileWriter.Write(m_Reader.GetString((int)TEXTSTRING.TEXT));
				if (bJava == true)
				{
					Console.WriteLine("	public static final int " + m_Reader.GetString((int)TEXTSTRING.STRINGID) + " = " + (Count++) + ";");
				}
				else
				{
					Console.WriteLine("		" + m_Reader.GetString((int)TEXTSTRING.STRINGID) + ",");
				}
			}
			if (bJava == true)
			{
				Console.WriteLine("}");
			}
			else
			{
				Console.WriteLine("	};");
				Console.WriteLine("};");
				Console.WriteLine("#endif // __STRINGRESOURCE_H__");
			}
			//theTextWriter.Close();
            base.Close();
		}
	}
}
