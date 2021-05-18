namespace FOMCodeGen
{
  using System.Linq;
  using System.Text;
  using System.Collections.Generic;
  using System;
  public partial class FOMObjectsImpl : FOMObjectsImplBase
  {
    FOMParser FOM { get; set; }
    string ObjectsHeaderFilename { get; set; }
    public FOMObjectsImpl(FOMParser fom, string objectsHeaderFilename)
    {
      FOM = fom;
      ObjectsHeaderFilename = System.IO.Path.GetFileName(objectsHeaderFilename);
    }
  }
  }