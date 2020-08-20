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
      var encodingsHeaderFilename = System.IO.Path.Combine(outputDirectory, basename + "Encodings.h");
      var dataTypesHeaderFilename = System.IO.Path.Combine(outputDirectory, basename + "DataTypes.h");
      FOMParser fom = new FOMParser(filename);
      FOMDataTypesHeader dataTypesHeader = new FOMDataTypesHeader(fom);
      String dataTypesHeaderHeaderContent = dataTypesHeader.TransformText();
      System.IO.File.WriteAllText(dataTypesHeaderFilename, dataTypesHeaderHeaderContent);

      FOMEncodingHeader encodingsHeader = new FOMEncodingHeader(fom, dataTypesHeaderFilename);
      String encodingsHeaderContent = encodingsHeader.TransformText();
      System.IO.File.WriteAllText(encodingsHeaderFilename, encodingsHeaderContent);
    }
  }
}
