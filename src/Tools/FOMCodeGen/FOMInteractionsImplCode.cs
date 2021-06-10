namespace FOMCodeGen
{
  using System.Linq;
  using System.Text;
  using System.Collections.Generic;
  using System;
  public partial class FOMInteractionsImpl : FOMInteractionsImplBase
  {
    FOMParser FOM { get; set; }
    string InteractionsHeaderFilename { get; set; }
    string ObjectsHeaderFilename { get; set; }
    public FOMInteractionsImpl(FOMParser fom, string interactionsHeaderFilename, string objectsHeaderFilename)
    {
      FOM = fom;
      InteractionsHeaderFilename = System.IO.Path.GetFileName(interactionsHeaderFilename);
      ObjectsHeaderFilename = System.IO.Path.GetFileName(objectsHeaderFilename);
    }
  }
  }