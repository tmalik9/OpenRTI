using System;
using System.Linq;
using System.Collections.Generic;

namespace FOMCodeGen
{
  class Program
  {
    public static string FindCommonPath(char separator, List<string> paths)
    {
      string commonPath = string.Empty;
      List<string> separatedPath = paths
        .First(str => str.Length == paths.Max(st2 => st2.Length))
        .Split(new Char[] { separator }, StringSplitOptions.RemoveEmptyEntries)
        .ToList();

      foreach (string PathSegment in separatedPath.AsEnumerable())
      {
        if (commonPath.Length == 0 && paths.All(str => str.StartsWith(PathSegment)))
        {
          commonPath = PathSegment;
        }
        else if (paths.All(str => str.StartsWith(commonPath + separator + PathSegment)))
        {
          commonPath += separator + PathSegment;
        }
        else
        {
          break;
        }
      }

      return commonPath;
    }
    static string GetIncludeDirectory(string interfaceHeaderOutputDirectory, string[] includePaths)
    {
      string interfacesIncludePath = "";
      string longestCommonPath = "";
      foreach (var includePath in includePaths)
      {
        List<string> paths = new List<string>();
        paths.Add(interfaceHeaderOutputDirectory);
        paths.Add(includePath);
        string commonPath = FindCommonPath(System.IO.Path.DirectorySeparatorChar, paths);
        System.Console.WriteLine("common prefix = {0}", commonPath);
        if (commonPath.Length > longestCommonPath.Length)
        {
          longestCommonPath = commonPath;
        }
      }
      if (longestCommonPath.Length > 0)
      {
        interfacesIncludePath = interfaceHeaderOutputDirectory.
          Substring(longestCommonPath.Length + 1).Replace(System.IO.Path.DirectorySeparatorChar, '/');
        System.Console.WriteLine("includePath = {0}", interfacesIncludePath);
      }
      return interfacesIncludePath;
    }
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
        string outputDirectory = "";
        bool outputDirectorySpecified = false;
        string interfaceHeaderOutputDirectory = "";
        bool interfaceHeaderOutputDirectorySpecified = false;
        string pathSeparator = new string(new char[] { System.IO.Path.DirectorySeparatorChar });
        string doublePathSeparator = new string(new char[] { System.IO.Path.DirectorySeparatorChar, System.IO.Path.DirectorySeparatorChar });
        string interfacesIncludePath = "";
        List<String> includePaths = new List<string>();
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
          else if (argList.First() == "-o")
          {
            argList.RemoveAt(0);
            if (argList.Count() > 0)
            {
              outputDirectorySpecified = true;
              outputDirectory = argList.First()
                .Replace(doublePathSeparator, pathSeparator)
                .TrimEnd(new char[] { System.IO.Path.DirectorySeparatorChar });
              argList.RemoveAt(0);
              if (!System.IO.Directory.Exists(outputDirectory))
              {
                throw new ArgumentException("output directory " + outputDirectory + "does not exist");
              }
            }
            else
            {
              throw new ArgumentException("option -o requires argument");
            }
          }
          else if (argList.First() == "-oi")
          {
            argList.RemoveAt(0);
            if (argList.Count() > 0)
            {
              interfaceHeaderOutputDirectorySpecified = true;
              interfaceHeaderOutputDirectory = argList.First()
                .Replace(doublePathSeparator, pathSeparator) 
                .TrimEnd(new char[] { System.IO.Path.DirectorySeparatorChar });
              argList.RemoveAt(0);
              if (!System.IO.Directory.Exists(interfaceHeaderOutputDirectory))
              {
                throw new ArgumentException("interface header output directory " + interfaceHeaderOutputDirectory + "does not exist");
              }
            }
            else
            {
              throw new ArgumentException("option -oi requires argument");
            }
          }
          else if (argList.First() == "-I")
          {
            argList.RemoveAt(0);
            if (argList.Count() > 0)
            {
              string includeDirectory = argList.First()
                .Replace(doublePathSeparator, pathSeparator)
                .TrimEnd(new char[] { System.IO.Path.DirectorySeparatorChar });
              argList.RemoveAt(0);
              includePaths.Add(includeDirectory);
            }
            else
            {
              throw new ArgumentException("option -oi requires argument");
            }
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
        if (!outputDirectorySpecified)
        {
          outputDirectory = System.IO.Path.GetDirectoryName(filename);
        }
        if (!interfaceHeaderOutputDirectorySpecified)
        {
          interfaceHeaderOutputDirectory = outputDirectory;
        }
        var basename = System.IO.Path.GetFileNameWithoutExtension(filename);
        var dataTypesHeaderFilename = System.IO.Path.Combine(interfaceHeaderOutputDirectory, basename + "DataTypes.h");
        var encodingsHeaderFilename = System.IO.Path.Combine(outputDirectory, basename + "Encodings.h");
        var encodingsImplFilename = System.IO.Path.Combine(outputDirectory, basename + "Encodings.cpp");
        var objectInterfacesHeaderFilename = System.IO.Path.Combine(interfaceHeaderOutputDirectory, basename + "ObjectInterfaces.h");
        var objectsHeaderFilename = System.IO.Path.Combine(outputDirectory, basename + "Objects.h");
        var objectsImplFilename = System.IO.Path.Combine(outputDirectory, basename + "Objects.cpp");
        var interactionInterfacesHeaderFilename = System.IO.Path.Combine(interfaceHeaderOutputDirectory, basename + "InteractionInterfaces.h");
        var interactionsHeaderFilename = System.IO.Path.Combine(outputDirectory, basename + "Interactions.h");
        var interactionsImplFilename = System.IO.Path.Combine(outputDirectory, basename + "Interactions.cpp");
        System.Console.WriteLine("dataTypesHeaderFilename={0}", dataTypesHeaderFilename);
        System.Console.WriteLine("encodingsHeaderFilename={0}", encodingsHeaderFilename);
        System.Console.WriteLine("encodingsImplFilename={0}", encodingsImplFilename);
        System.Console.WriteLine("objectInterfacesHeaderFilename={0}", objectInterfacesHeaderFilename);
        System.Console.WriteLine("objectsHeaderFilename={0}", objectsHeaderFilename);
        System.Console.WriteLine("objectsImplFilename={0}", objectsImplFilename);
        System.Console.WriteLine("interactionInterfacesHeaderFilename={0}", interactionInterfacesHeaderFilename);
        System.Console.WriteLine("interactionsHeaderFilename={0}", interactionsHeaderFilename);
        System.Console.WriteLine("interactionsImplFilename={0}", interactionsImplFilename);
        FOMParser fom = new FOMParser(filename, enclosingNamespace);
        fom.UsePrecompiledHeaders = usePrecompiledHeaders;
        if (includePaths.Count > 0)
        {
          interfacesIncludePath = GetIncludeDirectory(interfaceHeaderOutputDirectory, includePaths.ToArray());
        }
        else
        {
          interfacesIncludePath = GetIncludeDirectory(interfaceHeaderOutputDirectory, new string[] { outputDirectory });
        }
        FOMDataTypesHeader dataTypesHeader = new FOMDataTypesHeader(fom);
        String dataTypesHeaderHeaderContent = dataTypesHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(dataTypesHeaderFilename, dataTypesHeaderHeaderContent);

        FOMEncodingHeader encodingsHeader = new FOMEncodingHeader(fom, dataTypesHeaderFilename, interfacesIncludePath);
        String encodingsHeaderContent = encodingsHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(encodingsHeaderFilename, encodingsHeaderContent);

        FOMEncodingImpl encodingsImpl = new FOMEncodingImpl(fom, encodingsHeaderFilename, interfacesIncludePath);
        String encodingsImplContent = encodingsImpl.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(encodingsImplFilename, encodingsImplContent);

        FOMObjectInterfacesHeader objectInterfacesHeader = new FOMObjectInterfacesHeader(fom, objectInterfacesHeaderFilename, dataTypesHeaderFilename);
        String objectInterfacesHeaderContent = objectInterfacesHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(objectInterfacesHeaderFilename, objectInterfacesHeaderContent);

        FOMObjectsHeader objectsHeader = new FOMObjectsHeader(fom, objectsHeaderFilename, objectInterfacesHeaderFilename, encodingsHeaderFilename, interfacesIncludePath);
        String objectsHeaderContent = objectsHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(objectsHeaderFilename, objectsHeaderContent);

        FOMObjectsImpl objectsImpl = new FOMObjectsImpl(fom, objectsHeaderFilename, interfacesIncludePath);
        String objectsImplContent = objectsImpl.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(objectsImplFilename, objectsImplContent);

        FOMInteractionInterfacesHeader interactionInterfacesHeader = new FOMInteractionInterfacesHeader(fom, interactionInterfacesHeaderFilename, objectInterfacesHeaderFilename, dataTypesHeaderFilename);
        String interactionInterfacesHeaderContent = interactionInterfacesHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(interactionInterfacesHeaderFilename, interactionInterfacesHeaderContent);

        FOMInteractionsHeader interactionsHeader = new FOMInteractionsHeader(fom, interactionsHeaderFilename, interactionInterfacesHeaderFilename, objectsHeaderFilename, encodingsHeaderFilename, interfacesIncludePath);
        String interactionsHeaderContent = interactionsHeader.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(interactionsHeaderFilename, interactionsHeaderContent);

        FOMInteractionsImpl interactionsImpl = new FOMInteractionsImpl(fom, interactionsHeaderFilename, objectsHeaderFilename, interfacesIncludePath);
        String interactionsImplContent = interactionsImpl.TransformText();
        if (!doDryRun) System.IO.File.WriteAllText(interactionsImplFilename, interactionsImplContent);
      }
      catch (Exception e)
      {
        System.Console.WriteLine("{0}", e.Message);
      }
    }
  }
}
