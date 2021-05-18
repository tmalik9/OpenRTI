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
    public FOMObjectsHeader(FOMParser fom, string objectsHeaderFilename)
    {
      FOM = fom;
      ObjectsHeaderFilename = System.IO.Path.GetFileName(objectsHeaderFilename);
    }
  }
  }