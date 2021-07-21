namespace FOMCodeGen
{
  using System.Linq;
  using System.Text;
  using System.Collections.Generic;
  using System;
  public partial class FOMObjectsHeader : FOMObjectsHeaderBase
  {
    FOMParser FOM { get; set; }
    string ObjectsHeaderFilename { get; set; }
    string ObjectInterfacesHeaderFilename { get; set; }
    string EncodingHeaderFilename { get; set; }
    public FOMObjectsHeader(FOMParser fom, string objectsHeaderFilename, string objectInterfacesHeaderFilename, string encodingHeaderFilename, string includePath)
    {
      FOM = fom;
      ObjectsHeaderFilename = System.IO.Path.GetFileName(objectsHeaderFilename);
      if (includePath.Length > 0)
      {
        ObjectInterfacesHeaderFilename = includePath + "/" + System.IO.Path.GetFileName(objectInterfacesHeaderFilename);
      }
      else
      {
        ObjectInterfacesHeaderFilename = System.IO.Path.GetFileName(objectInterfacesHeaderFilename);
      }
      EncodingHeaderFilename = System.IO.Path.GetFileName(encodingHeaderFilename);
    }
  }
  }