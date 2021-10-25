namespace FOMCodeGen
{
  using System.Linq;
  using System.Text;
  using System.Collections.Generic;
  using System;
  public partial class FOMInteractionInterfacesHeader : FOMInteractionInterfacesHeaderBase
  {
    FOMParser FOM { get; set; }
    string InteractionInterfacesHeaderFilename { get; set; }
    string DataTypesHeaderFilename { get; set; }
    string ObjectInterfacesHeaderFilename { get; set; }
    public FOMInteractionInterfacesHeader(FOMParser fom, string interactionInterfacesHeaderFilename, string objectInterfacesHeaderFilename, string dataTypesHeaderFilename)
    {
      FOM = fom;
      InteractionInterfacesHeaderFilename = System.IO.Path.GetFileName(interactionInterfacesHeaderFilename);
      ObjectInterfacesHeaderFilename = System.IO.Path.GetFileName(objectInterfacesHeaderFilename);
      DataTypesHeaderFilename = System.IO.Path.GetFileName(dataTypesHeaderFilename);
    }
  }
  }