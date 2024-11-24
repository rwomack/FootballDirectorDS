using System;
using System.Collections.Generic;
using System.IO;
using System.Text;
using System.Xml;

namespace MemoryVisualizer
{
    public class MemoryDump
    {
        public MemoryDump()
        {
        }

        public void LoadFromFile(string sFilename)
        {
            m_cHeaps.Clear();

            using (XmlReader cReader = XmlReader.Create(new StreamReader(sFilename)))
            {
                XmlDocument cDocument = new XmlDocument();
                cDocument.Load(cReader);

                XmlNode cIter = cDocument.DocumentElement.FirstChild;

                while (cIter != null)
                {
                    switch (cIter.Name.ToLowerInvariant())
                    {
                        case "heap":
                            CreateHeapFromDocument(cIter);
                            break;
                    }

                    cIter = cIter.NextSibling;
                }
            }
        }

        static public MemoryDump CreateAsLeakDiff(MemoryDump cBaselineDump, MemoryDump cSecondDump)
        {
            MemoryDump cNew = new MemoryDump();

            foreach (MemoryHeap cSecondHeap in cSecondDump.m_cHeaps)
            {
                // Find the matching heap in the baseline.
                MemoryHeap cBaselineHeap = cBaselineDump.GetHeapByName(cSecondHeap.Name);
                if (cBaselineHeap == null)
                {
                    // No matching heap, so it's new.
                    cNew.m_cHeaps.Add(cSecondHeap);
                }

                cNew.m_cHeaps.Add(MemoryHeap.CreateAsLeakDiff(cBaselineHeap, cSecondHeap));
            }

            return cNew;
        }

        static public MemoryDump CreateAsBytesDiff(MemoryDump cBaselineDump, MemoryDump cSecondDump)
        {
            MemoryDump cNew = new MemoryDump();

            foreach (MemoryHeap cSecondHeap in cSecondDump.m_cHeaps)
            {
                // Find the matching heap in the baseline.
                MemoryHeap cBaselineHeap = cBaselineDump.GetHeapByName(cSecondHeap.Name);
                if (cBaselineHeap == null)
                {
                    // No matching heap, so it's new.
                    cNew.m_cHeaps.Add(cSecondHeap);
                }

                cNew.m_cHeaps.Add(MemoryHeap.CreateAsBytesDiff(cBaselineHeap, cSecondHeap));
            }

            return cNew;
        }

        private void CreateHeapFromDocument(XmlNode cHeapNode)
        {
            m_cHeaps.Add(MemoryHeap.CreateFromDocument(cHeapNode));
        }

        private MemoryHeap GetHeapByName(string sName)
        {
            foreach (MemoryHeap cHeap in m_cHeaps)
            {
                if (String.Compare(sName, cHeap.Name, true) == 0)
                    return cHeap;
            }

            return null;
        }

        public IList<MemoryHeap> Heaps
        {
            get { return m_cHeaps; }
        }

        public string Name
        {
            get { return m_sName; }
            set { m_sName = value; }
        }

        private List<MemoryHeap> m_cHeaps = new List<MemoryHeap>();
        private string m_sName = String.Empty;
    }
}
