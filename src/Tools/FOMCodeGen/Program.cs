using System;
using System.Linq;
using System.Collections.Generic;

namespace FOMCodeGen
{
  class Program
  {
    static void Main(string[] args)
    {
      List<String> argList = new List<string>(args);
      try
      {
        string filename = "D:/vfs/CANoe/feature/14.0_DistSim_HLA/Projects_Source/CANoe/Source/RTEVENT/DistSim/RTFederate.xml";
        string enclosingNamespace = "N" + System.IO.Path.GetFileNameWithoutExtension(filename) + "Encoding";
        bool usePrecompiledHeaders = false;
        bool doDryRun = false;
        bool haveFilename = false;
        bool haveNamespace = false;
        while (argList.Count() > 0)
        {
          if (argList.First() == "-p")
          {
            usePrecompiledHeaders = true;
            argList.RemoveAt(0);
          }
          else if (argList.First() == "-d")
          {
            doDryRun = true;
            argList.RemoveAt(0);
          }
          else if (!haveFilename)
          {
            haveFilename = true;
            filename = argList.First();
            argList.RemoveAt(0);
          }
          else if (!haveNamespace)
          {
            haveNamespace = true;
            enclosingNamespace = argList.First();
            argList.RemoveAt(0);
          }
        }
        var outputDirectory = System.IO.Path.GetDirectoryName(filename);
        var basename = System.IO.Path.GetFileNameWithoutExtension(filename);
        var encodingsHeaderFilename = System.IO.Path.Combine(outputDirectory, basename + "Encodings.h");
        var encodingsImplFilename = System.IO.Path.Combine(outputDirectory, basename + "Encodings.cpp");
        var dataTypesHeaderFilename = System.IO.Path.Combine(outputDirectory, basename + "DataTypes.h");
        var objectsHeaderFilename = System.IO.Path.Combine(outputDirectory, basename + "Objects.h");
        var objectsImplFilename = System.IO.Path.Combine(outputDirectory, basename + "Objects.cpp");
        System.Console.WriteLine("dataTypesHeaderFilename={0}", dataTypesHeaderFilename);
        System.Console.WriteLine("encodingsHeaderFilename={0}", encodingsHeaderFilename);
        System.Console.WriteLine("encodingsImplFilename={0}", encodingsImplFilename);
        System.Console.WriteLine("objectsHeaderFilename={0}", objectsHeaderFilename);
        System.Console.WriteLine("objectsImplFilename={0}", objectsImplFilename);
        FOMParser fom = new FOMParser(filename, enclosingNamespace);
        fom.UsePrecompiledHeaders = usePrecompiledHeaders;

        FOMDataTypesHeader dataTypesHeader = new FOMDataTypesHeader(fom);
        String dataTypesHeaderHeaderContent = dataTypesHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(dataTypesHeaderFilename, dataTypesHeaderHeaderContent);

        FOMEncodingHeader encodingsHeader = new FOMEncodingHeader(fom, dataTypesHeaderFilename);
        String encodingsHeaderContent = encodingsHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(encodingsHeaderFilename, encodingsHeaderContent);

        FOMEncodingImpl encodingsImpl = new FOMEncodingImpl(fom, encodingsHeaderFilename);
        String encodingsImplContent = encodingsImpl.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(encodingsImplFilename, encodingsImplContent);

        FOMObjectsHeader objectsHeader = new FOMObjectsHeader(fom, objectsHeaderFilename, encodingsHeaderFilename);
        String objectsHeaderContent = objectsHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(objectsHeaderFilename, objectsHeaderContent);

        FOMObjectsImpl objectsImpl = new FOMObjectsImpl(fom, objectsHeaderFilename);
        String objectsImplContent = objectsImpl.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(objectsImplFilename, objectsImplContent);
      }
      catch (Exception e)
      {
        System.Console.WriteLine("{0}", e.Message);
      }
    }
  }
}
