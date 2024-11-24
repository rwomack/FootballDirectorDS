using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace MemoryVisualizer
{
    public partial class DumpView : UserControl
    {
        public DumpView()
        {
            InitializeComponent();
        }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public MemoryDump Dump
        {
            get { return m_cDump; }
            set
            {
                m_cDump = value;
                RebuildView();
            }
        }

        private void RebuildView()
        {
            if (m_cDump != null)
            {
                // Populate listbox.
                heapView1.Heaps = m_cDump.Heaps;
                listViewHeaps.Items.Clear();

                foreach (MemoryHeap cHeap in Dump.Heaps)
                {
                    ListViewItem cNewItem = new ListViewItem(cHeap.Name);
                    cNewItem.Tag = cHeap;
                    listViewHeaps.Items.Add(cNewItem);
                }
            }
            else
            {
                heapView1.Heaps = null;
            }
        }

        private MemoryDump m_cDump;
    }
}
