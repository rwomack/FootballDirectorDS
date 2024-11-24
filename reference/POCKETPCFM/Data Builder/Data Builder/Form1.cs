using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;



namespace Data_Builder
{
	public partial class FormMain : Form
	{
		public bool m_bJava = true;
		public bool m_bSeries60 = true;

		public FormMain()
		{
			InitializeComponent();
			StatusLabel.Text = "";
			Java_Button.Checked = true;
			C_Button.Checked = false;
			Series60_Button.Checked = true;
			PocketPC_Button.Checked = false;
		}

		private void button1_Click(object sender, EventArgs e)
		{
			DataBuilder theDataBuilder = new DataBuilder();
			if (theDataBuilder.DoCreatePocketPCData(this) == true)
			{
				StatusLabel.Text = "Data Generation Complete";
			}
		}


		private void button2_Click(object sender, EventArgs e)
		{
			Close();
		}

		private void button3_Click(object sender, EventArgs e)
		{
			ResourceBuilder theResBuilder = new ResourceBuilder();
			if (theResBuilder.DoCreateData(this) == true)
			{
				StatusLabel.Text = "Resource Generation Complete";
			}
		}

		private void Java_Button_CheckedChanged(object sender, EventArgs e)
		{
			m_bJava = true;
		}

		private void Series60_Button_CheckedChanged(object sender, EventArgs e)
		{
			m_bSeries60 = true;
		}

		private void PocketPC_Button_CheckedChanged(object sender, EventArgs e)
		{
			m_bSeries60 = false;
		}

		private void C_Button_CheckedChanged(object sender, EventArgs e)
		{
			m_bJava = false;
		}

		private void button4_Click(object sender, EventArgs e)
		{
			DataBuilder theDataBuilder = new DataBuilder();
			if (theDataBuilder.DoCreateSeries60Data(this) == true)
			{
				StatusLabel.Text = "Data Generation Complete";
			}
		}

	}
}