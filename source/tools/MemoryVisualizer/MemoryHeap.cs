using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace MemoryVisualizer
{
    public class MemoryHeap
    {
        public MemoryHeap()
        {
        }

        static public MemoryHeap CreateFromDocument(XmlNode cNode)
        {
            MemoryHeap cHeap = new MemoryHeap();

            XmlNode cNameNode = XmlUtils.GetNodeByName(cNode, "Name");
            if (cNameNode != null)
                cHeap.Name = cNameNode.FirstChild.Value;

            XmlNode cSizeNode = XmlUtils.GetNodeByName(cNode, "Size");
            if (cSizeNode != null)
                uint.TryParse(cSizeNode.FirstChild.Value, out cHeap.m_uiSize);

            XmlNode cAllocationsNode = XmlUtils.GetNodeByName(cNode, "Allocations");
            if (cAllocationsNode != null)
            {
                // Build up the list of allocations.
                foreach (XmlNode cAllocationNode in cAllocationsNode)
                {
                    MemoryAllocation cNewAllocation = MemoryAllocation.CreateFromDocument(cAllocationNode);
                    cHeap.m_cAllocations.Add(cNewAllocation);

                    cHeap.AddAllocationForFilename(cNewAllocation.FileName, cNewAllocation);
                    cHeap.m_cAllocationsBySequence.Add(cNewAllocation.Sequence, cNewAllocation);
                }
            }

            return cHeap;
        }

        static public MemoryHeap CreateAsLeakDiff(MemoryHeap cBaselineHeap, MemoryHeap cSecondHeap)
        {
            MemoryHeap cHeap = new MemoryHeap();

            cHeap.Size = cSecondHeap.Size;
            cHeap.Name = "Difference in " + cSecondHeap.Name;

            // Look for:
            // 1) new allocations in the second heap
            // 2) reallocs that are larger in the second heap (check by sequence number)
            foreach (MemoryAllocation cSecondAllocation in cSecondHeap.Allocations)
            {
                if (cBaselineHeap.AllocationsBySequence.ContainsKey(cSecondAllocation.Sequence) == false)
                {
                    cHeap.Allocations.Add(cSecondAllocation);
                    cHeap.AddAllocationForFilename(cSecondAllocation.FileName, cSecondAllocation);
                    cHeap.AllocationsBySequence.Add(cSecondAllocation.Sequence, cSecondAllocation);
                }
                else
                {
                    MemoryAllocation cBaselineAllocation = cBaselineHeap.AllocationsBySequence[cSecondAllocation.Sequence];
                    if (cBaselineAllocation.Size < cSecondAllocation.Size)
                    {
                        // Allocation has grown. Calculate the size difference and add it.
                        MemoryAllocation cDifferenceAllocation = new MemoryAllocation();

                        cDifferenceAllocation.Address = cSecondAllocation.Address;
                        cDifferenceAllocation.FileName = cSecondAllocation.FileName;
                        cDifferenceAllocation.LineNumber = cSecondAllocation.LineNumber;
                        cDifferenceAllocation.Sequence = cSecondAllocation.Sequence;
                        cDifferenceAllocation.Size = cSecondAllocation.Size - cBaselineAllocation.Size;

                        cHeap.Allocations.Add(cDifferenceAllocation);
                        cHeap.AddAllocationForFilename(cDifferenceAllocation.FileName, cDifferenceAllocation);
                        cHeap.AllocationsBySequence.Add(cDifferenceAllocation.Sequence, cDifferenceAllocation);
                    }
                }
            }

            return cHeap;
        }

        static public MemoryHeap CreateAsBytesDiff(MemoryHeap cBaselineHeap, MemoryHeap cSecondHeap)
        {
            MemoryHeap cHeap = new MemoryHeap();

            cHeap.Size = cSecondHeap.Size;
            cHeap.Name = "Difference in " + cSecondHeap.Name;

            // For each filename/line number combination, build up a number of bytes in each
            // dump. Then, compare them and report the difference.
            uint uiSequenceNumber = 0;
            foreach (string sFilename in cSecondHeap.AllocationsByFilename.Keys)
            {
                List<MemoryAllocation> cToAdd = null;

                if (cBaselineHeap.AllocationsByFilename.ContainsKey(sFilename) == false)
                {
                    // All of these will be new.
                    cToAdd = DiffByLines(new Dictionary<uint, MemoryAllocation>(), TotalByLines(cSecondHeap.AllocationsByFilename[sFilename], ref uiSequenceNumber));
                }
                else
                {
                    // Total each of them up by line.
                    Dictionary<uint, MemoryAllocation> cBaselineLines = TotalByLines(cBaselineHeap.AllocationsByFilename[sFilename], ref uiSequenceNumber);
                    Dictionary<uint, MemoryAllocation> cSecondLines = TotalByLines(cSecondHeap.AllocationsByFilename[sFilename], ref uiSequenceNumber);

                    // Now diff them by line.
                    cToAdd = DiffByLines(cBaselineLines, cSecondLines);
                }

                foreach (MemoryAllocation cAllocation in cToAdd)
                {
                    cHeap.Allocations.Add(cAllocation);
                    cHeap.AddAllocationForFilename(cAllocation.FileName, cAllocation);
                    cHeap.AllocationsBySequence.Add(cAllocation.Sequence, cAllocation);
                }
            }

            return cHeap;
        }

        static private Dictionary<uint, MemoryAllocation> TotalByLines(List<MemoryAllocation> cList, ref uint uiSequenceNumber)
        {
            Dictionary<uint, MemoryAllocation> cResult = new Dictionary<uint, MemoryAllocation>();

            foreach (MemoryAllocation cAllocation in cList)
            {
                if (cResult.ContainsKey(cAllocation.LineNumber) == false)
                {
                    MemoryAllocation cTotal = new MemoryAllocation();
                    cTotal.FileName = cAllocation.FileName;
                    cTotal.LineNumber = cAllocation.LineNumber;
                    cTotal.Size = cAllocation.Size;
                    cTotal.Sequence = uiSequenceNumber++;
                    cResult.Add(cTotal.LineNumber, cTotal);
                }
                else
                {
                    MemoryAllocation cTotal = cResult[cAllocation.LineNumber];
                    cTotal.Size += cAllocation.Size;
                }
            }

            return cResult;
        }

        static private List<MemoryAllocation> DiffByLines(Dictionary<uint, MemoryAllocation> cBaseline, Dictionary<uint, MemoryAllocation> cSecond)
        {
            List<MemoryAllocation> cResult = new List<MemoryAllocation>();

            foreach (uint uiKey in cSecond.Keys)
            {
                MemoryAllocation cSecondAllocation = cSecond[uiKey];

                if (cBaseline.ContainsKey(uiKey) == false)
                {
                    MemoryAllocation cNew = new MemoryAllocation();
                    cNew.FileName = cSecondAllocation.FileName;
                    cNew.LineNumber = cSecondAllocation.LineNumber;
                    cNew.Size = cSecondAllocation.Size;
                    cNew.Sequence = cSecondAllocation.Sequence;

                    cResult.Add(cNew);
                }
                else
                {
                    MemoryAllocation cBaselineAllocation = cBaseline[uiKey];

                    int iDifference = (int)cSecondAllocation.Size - (int)cBaselineAllocation.Size;
                    if (iDifference <= 0)
                        continue;

                    MemoryAllocation cNew = new MemoryAllocation();

                    cNew.FileName = cBaselineAllocation.FileName;
                    cNew.LineNumber = cBaselineAllocation.LineNumber;
                    cNew.Size = (uint)iDifference;
                    cNew.Sequence = cSecondAllocation.Sequence;

                    cResult.Add(cNew);
                }
            }

            return cResult;
        }

        private void AddAllocationForFilename(string sFilename, MemoryAllocation cAllocation)
        {
            if (m_cAllocationsByFilename.ContainsKey(sFilename) == false)
            {
                List<MemoryAllocation> cAllocationsForFile = new List<MemoryAllocation>();
                cAllocationsForFile.Add(cAllocation);
                m_cAllocationsByFilename.Add(sFilename, cAllocationsForFile);
            }
            else
            {
                List<MemoryAllocation> cAllocationsForFile = m_cAllocationsByFilename[sFilename];
                cAllocationsForFile.Add(cAllocation);
            }
        }

        public string Name
        {
            get { return m_sName; }
            set { m_sName = value; }
        }

        public uint Size
        {
            get { return m_uiSize; }
            set { m_uiSize = value; }
        }

        public List<MemoryAllocation> Allocations
        {
            get { return m_cAllocations; }
        }

        public Dictionary<string, List<MemoryAllocation>> AllocationsByFilename
        {
            get { return m_cAllocationsByFilename; }
        }

        public Dictionary<uint, MemoryAllocation> AllocationsBySequence
        {
            get { return m_cAllocationsBySequence; }
        }

        private string m_sName = String.Empty;
        private uint m_uiSize = 0;
        private List<MemoryAllocation> m_cAllocations = new List<MemoryAllocation>();
        private Dictionary<string, List<MemoryAllocation>> m_cAllocationsByFilename = new Dictionary<string, List<MemoryAllocation>>();
        private Dictionary<uint, MemoryAllocation> m_cAllocationsBySequence = new Dictionary<uint, MemoryAllocation>();
    }
}
