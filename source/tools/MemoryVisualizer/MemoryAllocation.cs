using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace MemoryVisualizer
{
    public class MemoryAllocation
    {
        public MemoryAllocation()
        {
        }

        public string FileName = String.Empty;
        public uint LineNumber = 0;
        public uint Size = 0;
        public uint Sequence = 0;
        public UInt32 Address = 0;

        static public MemoryAllocation CreateFromDocument(XmlNode cNode)
        {
            MemoryAllocation cNew = new MemoryAllocation();

            XmlNode cAddressNode = XmlUtils.GetNodeByName(cNode, "Address");
            if (cAddressNode != null)
                UInt32.TryParse(cAddressNode.FirstChild.Value, out cNew.Address);

            XmlNode cSizeNode = XmlUtils.GetNodeByName(cNode, "Size");
            if (cSizeNode != null)
                uint.TryParse(cSizeNode.FirstChild.Value, out cNew.Size);

            XmlNode cFilenameNode = XmlUtils.GetNodeByName(cNode, "Filename");
            if (cFilenameNode != null)
                cNew.FileName = cFilenameNode.FirstChild.Value;

            XmlNode cLineNumberNode = XmlUtils.GetNodeByName(cNode, "LineNumber");
            if (cLineNumberNode != null)
                uint.TryParse(cLineNumberNode.FirstChild.Value, out cNew.LineNumber);

            XmlNode cSequenceNode = XmlUtils.GetNodeByName(cNode, "Sequence");
            if (cSequenceNode != null)
                uint.TryParse(cSequenceNode.FirstChild.Value, out cNew.Sequence);

            return cNew;
        }
    }
}
