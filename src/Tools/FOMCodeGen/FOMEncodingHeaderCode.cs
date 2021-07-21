namespace FOMCodeGen
{
  using System.Linq;
  using System.Text;
  using System.Collections.Generic;
  using System;
  public partial class FOMEncodingHeader : FOMEncodingHeaderBase
  {
    FOMParser FOM { get; set; }
    string DataTypesHeaderFilename { get; set; }
    public FOMEncodingHeader(FOMParser fom, string dataTypesHeaderFilename, string includePath)
    {
      FOM = fom;
      if (includePath.Length > 0)
      {
        DataTypesHeaderFilename = includePath + "/" + System.IO.Path.GetFileName(dataTypesHeaderFilename);
      }
      else
      {
        DataTypesHeaderFilename = System.IO.Path.GetFileName(dataTypesHeaderFilename);
      }
    }
  }
  }