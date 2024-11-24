using System;
using System.Collections.Generic;
using System.Data.OleDb;
using System.Text;
using System.IO;


namespace Data_Builder
{
	class ResourceBuilder
	{

        protected OleDbConnection m_theDB;


        //////////////////////////////////////////////////////////////////////////
		// Method:    DoCreateData
		// FullName:  Data_Builder.ResourceBuilder.DoCreateData
		// Access:    public 
		// Returns:   bool
		// Parameter: FormMain _theForm
		//////////////////////////////////////////////////////////////////////////
		public bool DoCreateData(FormMain _theForm)
		{
			bool bRet = true;
			try
			{
				//m_theDB = new Connection(new ConnectionString("localhost",
				//													  "football director",
				//													  "laptop",
				//													  "").AsString);
				m_theDB.Open();

                TextString theText = new TextString(m_theDB, _theForm, "StringResource", "Resource");
				theText.DoCreateData(_theForm.m_bJava, _theForm.m_bSeries60);
				
				m_theDB.Close();
			}
			catch (Exception ee)
			{
				_theForm.StatusLabel.Text = ee.ToString();
				bRet = false;
			}
			return bRet;
		}
	}
}
