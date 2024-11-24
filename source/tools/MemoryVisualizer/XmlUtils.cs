using System;
using System.Collections.Generic;
using System.Text;
using System.Xml;

namespace MemoryVisualizer
{
    public class XmlUtils
    {
        static public XmlNode GetNodeByName(XmlNode cNode, string sName)
        {
            foreach (XmlNode cIter in cNode)
            {
                if (String.Compare(cIter.Name, sName, true) == 0)
                    return cIter;
            }

            return null;
        }
    }
}
