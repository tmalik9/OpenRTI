namespace FOMCodeGen
{
  using System.Linq;
  using System.Text;
  using System.Collections.Generic;
  using System;
  public partial class FOMInteractionsHeader : FOMInteractionsHeaderBase
  {
    FOMParser FOM { get; set; }
    string InteractionsHeaderFilename { get; set; }
    string InteractionInterfacesHeaderFilename { get; set; }
    string EncodingHeaderFilename { get; set; }
    public FOMInteractionsHeader(FOMParser fom, string interactionsHeaderFilename, string interactionInterfacesHeaderFilename, string encodingHeaderFilename)
    {
      FOM = fom;
      InteractionsHeaderFilename = System.IO.Path.GetFileName(interactionsHeaderFilename);
      InteractionInterfacesHeaderFilename = System.IO.Path.GetFileName(interactionInterfacesHeaderFilename);
      EncodingHeaderFilename = System.IO.Path.GetFileName(encodingHeaderFilename);
    }
  }
  }