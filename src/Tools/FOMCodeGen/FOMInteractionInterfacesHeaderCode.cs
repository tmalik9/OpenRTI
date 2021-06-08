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
    string EncodingHeaderFilename { get; set; }
    public FOMInteractionInterfacesHeader(FOMParser fom, string interactionInterfacesHeaderFilename, string encodingHeaderFilename)
    {
      FOM = fom;
      InteractionInterfacesHeaderFilename = System.IO.Path.GetFileName(interactionInterfacesHeaderFilename);
      EncodingHeaderFilename = System.IO.Path.GetFileName(encodingHeaderFilename);
    }
  }
  }