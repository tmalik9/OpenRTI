namespace FOMCodeGen
{
  using System.Linq;
  using System.Text;
  using System.Collections.Generic;
  using System;
  public partial class EncodingHeader : EncodingHeaderBase
  {
    FOMParser FOM { get; set; }
    public EncodingHeader(string filename)
    {
      FOM = new FOMParser(filename);
      foreach (var entry in FOM.DataTypes)
      {
        System.Console.WriteLine("{0}", entry.Value.Name);
      }
    }
  }
  }