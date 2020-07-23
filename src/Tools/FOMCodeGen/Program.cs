using System;
using System.Linq;

namespace FOMCodeGen
{
  class Program
  {
    static void Main(string[] args)
    {
      string filename = "D:/vfs/CANoe/feature/14.0_DistSim_HLA/Projects_Source/CANoe/Source/RTEVENT/DistSim/RTFederate.xml";
      if (args.Count() > 0)
      {
        filename = args[0];
      }
      var outputDirectory = System.IO.Path.GetDirectoryName(filename);
      var basename = System.IO.Path.GetFileNameWithoutExtension(filename);
      var headerFilename = System.IO.Path.Combine(outputDirectory, basename + "Encodings.h");
      EncodingHeader header = new EncodingHeader(filename);
      String headerContent = header.TransformText();
      System.IO.File.WriteAllText(headerFilename, headerContent);
    }
  }
}
