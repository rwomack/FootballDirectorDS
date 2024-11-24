using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Drawing;
using System.Data;
using System.Text;
using System.Windows.Forms;

namespace MemoryVisualizer
{
    public partial class HeapView : UserControl
    {
        public HeapView()
        {
            InitializeComponent();
        }

        [Browsable(false)]
        [DesignerSerializationVisibility(DesignerSerializationVisibility.Hidden)]
        public IEnumerable<MemoryHeap> Heaps
        {
            get { return m_cHeaps; }
            set
            {
                m_cHeaps = value;
                RebuildView();
            }
        }

        private void RebuildView()
        {
            treeView1.Nodes.Clear();

            if (m_cHeaps == null)
                return;

            try
            {
                treeView1.BeginUpdate();
                foreach (MemoryHeap cHeap in m_cHeaps)
                {
                    TreeNode cHeapNode = new TreeNode();
                    uint uiHeapSizeTotal = 0;

                    foreach (string sFilename in cHeap.AllocationsByFilename.Keys)
                    {
                        TreeNode cFilenameNode = new TreeNode();
                        List<MemoryAllocation> cFilenameAllocations = cHeap.AllocationsByFilename[sFilename];
                        uint uiFileSizeTotal = 0;

                        foreach (MemoryAllocation cAllocation in cFilenameAllocations)
                        {
                            TreeNode cAllocationNode = new TreeNode();
                            cAllocationNode.Text = "Line " + cAllocation.LineNumber.ToString() + ", " + cAllocation.Size.ToString() + " bytes";
                            uiFileSizeTotal += cAllocation.Size;
                            uiHeapSizeTotal += cAllocation.Size;
                            cFilenameNode.Nodes.Add(cAllocationNode);
                        }

                        cFilenameNode.Text = sFilename + " (" + cFilenameAllocations.Count.ToString() + " allocations, " + uiFileSizeTotal.ToString() + " bytes)";
                        cHeapNode.Nodes.Add(cFilenameNode);
                    }

                    cHeapNode.Text = cHeap.Name + " (" + cHeap.Allocations.Count.ToString() + " allocations, " + uiHeapSizeTotal.ToString() + " bytes)";
                    cHeapNode.Tag = cHeap;
                    treeView1.Nodes.Add(cHeapNode);
                }
            }
            catch (Exception cException)
            {
                MessageBox.Show(cException.ToString());
            }
            finally
            {
                treeView1.EndUpdate();
            }
        }

        private IEnumerable<MemoryHeap> m_cHeaps;
    }
}
