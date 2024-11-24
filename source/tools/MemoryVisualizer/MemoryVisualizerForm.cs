using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MemoryVisualizer
{
    public partial class MemoryVisualizerForm : Form
    {
        public MemoryVisualizerForm()
        {
            InitializeComponent();
        }

        private void loadToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                m_cBaseline = new MemoryDump();
                m_cBaseline.LoadFromFile(openFileDialog1.FileName);

                RebuildView();
            }
        }

        private void compareAgainstToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_cBaseline == null)
            {
                MessageBox.Show("You need to load a memory dump first, before loading a comparison file.");
                return;
            }

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                m_cSecond = new MemoryDump();
                m_cSecond.LoadFromFile(openFileDialog1.FileName);

                m_cDiff = MemoryDump.CreateAsLeakDiff(m_cBaseline, m_cSecond);
                m_cDiff.Name = "Unmatched/New Allocations";

                RebuildView();
            }
        }

        private void compareAgainstbytesToolStripMenuItem_Click(object sender, EventArgs e)
        {
            if (m_cBaseline == null)
            {
                MessageBox.Show("You need to load a memory dump first, before loading a comparison file.");
                return;
            }

            if (openFileDialog1.ShowDialog() == DialogResult.OK)
            {
                m_cSecond = new MemoryDump();
                m_cSecond.LoadFromFile(openFileDialog1.FileName);

                m_cDiff = MemoryDump.CreateAsBytesDiff(m_cBaseline, m_cSecond);
                m_cDiff.Name = "Leaks By Increased # of Bytes";

                RebuildView();
            }
        }

        private void listViewFileSelection_SelectedIndexChanged(object sender, EventArgs e)
        {
            if (listViewFileSelection.SelectedItems.Count == 0)
            {
                dumpView1.Dump = null;
                return;
            }

            dumpView1.Dump = listViewFileSelection.SelectedItems[0].Tag as MemoryDump;
        }

        /// <summary>
        /// Called when the form is loaded.
        /// </summary>
        /// <param name="sender">event sender</param>
        /// <param name="e">event args</param>
        private void MemoryVisualizer_Load(object sender, EventArgs e)
        {
            RebuildView();
        }

        private void exitToolStripMenuItem_Click(object sender, EventArgs e)
        {
            Application.Exit();
        }

        private void RebuildView()
        {
            // Set up the list view with some items.
            listViewFileSelection.Items.Clear();

            ListViewItem cBaseline = new ListViewItem("Baseline file");
            cBaseline.Tag = m_cBaseline;
            listViewFileSelection.Items.Add(cBaseline);

            ListViewItem cSecond = new ListViewItem("Second file");
            cSecond.Tag = m_cSecond;
            listViewFileSelection.Items.Add(cSecond);

            ListViewItem cDiff = new ListViewItem("Diff");
            if (m_cDiff != null)
                cDiff.Text = m_cDiff.Name;
            cDiff.Tag = m_cDiff;
            listViewFileSelection.Items.Add(cDiff);
        }

        private MemoryDump m_cBaseline;
        private MemoryDump m_cSecond;
        private MemoryDump m_cDiff;
    }
}