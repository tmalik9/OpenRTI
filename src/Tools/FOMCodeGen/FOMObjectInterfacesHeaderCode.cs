namespace FOMCodeGen
{
  using System.Linq;
  using System.Text;
  using System.Collections.Generic;
  using System;
  public partial class FOMObjectInterfacesHeader : FOMObjectInterfacesHeaderBase
  {
    FOMParser FOM { get; set; }
    string ObjectInterfacesHeaderFilename { get; set; }
    string DataTypesHeaderFilename { get; set; }
    public FOMObjectInterfacesHeader(FOMParser fom, string objectInterfacesHeaderFilename, string encodingHeaderFilename)
    {
      FOM = fom;
      ObjectInterfacesHeaderFilename = System.IO.Path.GetFileName(objectInterfacesHeaderFilename);
      DataTypesHeaderFilename = System.IO.Path.GetFileName(encodingHeaderFilename);
    }
  }
  }