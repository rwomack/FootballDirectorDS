using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.IO;
using System.Text;
using System.Windows.Forms;

namespace SaveSizeWatcher
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        ~Form1()
        {
            m_cWatcher.Dispose();
        }

        private void Form1_Load(object sender, EventArgs e)
        {
            m_cWatcher = new FileSystemWatcher(Directory.GetCurrentDirectory());
            m_cWatcher.Created += new FileSystemEventHandler(HandleFileCreated);
            m_cWatcher.Changed += new FileSystemEventHandler(HandleFileChanged);
        }

        void HandleFileChanged(object sender, FileSystemEventArgs e)
        {
            if (String.Compare(e.Name, m_sFileName, true) == 0)
                listView1.Invoke(new UpdateFileSizeDelegate(AddFileSizeRecord));
        }

        void HandleFileCreated(object sender, FileSystemEventArgs e)
        {
            if (String.Compare(e.Name, m_sFileName, true) == 0)
                listView1.Invoke(new UpdateFileSizeDelegate(AddFileSizeRecord));
        }

        private void buttonStartWatching_Click(object sender, EventArgs e)
        {
            m_cWatcher.EnableRaisingEvents = true;
        }

        private void buttonStopWatching_Click(object sender, EventArgs e)
        {
            m_cWatcher.EnableRaisingEvents = false;
        }

        private void buttonReset_Click(object sender, EventArgs e)
        {
            listView1.Items.Clear();
            m_uiLargest = 0;
        }

        private void textBoxFileToWatch_TextChanged(object sender, EventArgs e)
        {
            m_sFileName = textBoxFileToWatch.Text;
        }

        private void AddFileSizeRecord()
        {
            try
            {
                long uiSize = new FileInfo(Path.Combine(Directory.GetCurrentDirectory(), textBoxFileToWatch.Text)).Length;
                ListViewItem cNewItem = new ListViewItem();

                cNewItem.SubItems[0].Text = uiSize.ToString();
                listView1.Items.Add(cNewItem);

                if (uiSize > m_uiLargest)
                {
                    m_uiLargest = uiSize;
                    labelLargest.Text = "Largest: " + uiSize.ToString();
                }
            }
            catch (System.Exception e)
            {
            }
        }

        private FileSystemWatcher m_cWatcher;
        private long m_uiLargest = 0;
        private string m_sFileName = "game.save";

        private delegate void UpdateFileSizeDelegate();
    }
}