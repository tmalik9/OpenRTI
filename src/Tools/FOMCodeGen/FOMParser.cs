
using System;
using System.Collections.Generic;
using System.Xml;

namespace FOMCodeGen
{
  class FOMParser
  {
    public class DataType
    {
      public DataType(string name)
      {
        Name = name;
      }
      public string Name { get; set; }
      public virtual string CPPType
      {
        get { return Name; }
      }
      public virtual string Encoding
      {
        get { return Name + "Encoding"; }
      }
      public bool Generate { get; set; }
    }
    public class BasicDataRepresentation : DataType
    {
      public BasicDataRepresentation(string name, string cppType) : base(name)
      {
        _cppType = cppType;
      }
      private string _cppType;
      public override string CPPType
      {
        get { return _cppType; }
      }
      public override string Encoding
      {
        get { return "rti1516e::" + Name; }
      }
    }
    public class SimpleDataType : DataType
    {
      public SimpleDataType(string name, BasicDataRepresentation representation) : base(name)
      {
        Representation = representation;
      }
      public BasicDataRepresentation Representation { get; set; }
      public override string Encoding
      {
        get { return Representation.Encoding; }
      }
    }
    public class Enumerator
    {
      public Enumerator(string name, string value)
      {
        Name = name;
        Value = value;
      }
      public string Name { get; set; }
      public string Representation { get; set; }
      public string Value { get; set; }
    }
    public class EnumeratedDataType : DataType
    {
      public EnumeratedDataType(string name, BasicDataRepresentation representation) : base(name)
      {
        Representation = representation;
        Enumerators = new Dictionary<string, Enumerator>();
      }
      public BasicDataRepresentation Representation { get; set; }
      public Dictionary<string, Enumerator> Enumerators { get; set; }
      public override string Encoding
      {
        get { return Representation.Encoding; }
      }
    }

    public class ArrayDataType : DataType
    {
      public ArrayDataType(string name, DataType dataType, string cardinality, string encodingBase) : base(name)
      {
        DataType = dataType;
        Cardinality = cardinality;
        EncodingBase = encodingBase;
      }
      public string Cardinality { get; set; }
      public DataType DataType { get; set; }
      // NOTE: the name of the encoder is the generated class name (as in DataType),
      // the 'encoding' xml tag is the name of the base class
      // different meaning: is one of HLAvariableArray, HLAfixedArray
      public string EncodingBase { get; set; }
      public override string CPPType
      {
        get { return "std::vector<" + DataType.CPPType + ">"; }
      }
    }

    public class FixedRecordField
    {
      public FixedRecordField(string name, DataType dataType)
      {
        Name = name;
        DataType = dataType;
      }
      public string Name { get; set; }
      public DataType DataType { get; set; }
      public string Comment { get; set; }
    }
    public class FixedRecordDataType : DataType
    {
      public FixedRecordDataType(string name) : base(name)
      {
        Fields = new Dictionary<string, FixedRecordField>();
      }
      public Dictionary<string, FixedRecordField> Fields { get; set; }
    }

    public Dictionary<string, DataType> DataTypes { get; set; }
    public string Filename { get; set; }
    public string Namespace { get; set; }
    private Dictionary<string, string> mBasicDataRepresentations = new Dictionary<string, string>()
    {
      { "HLAASCIIchar", "char" },
      { "HLAASCIIstring", "std::string" },
      { "HLAboolean", "bool" },
      { "HLAbyte", "uint8_t" },
      { "HLAfloat32BE", "float" },
      { "HLAfloat32LE", "float" },
      { "HLAfloat64BE", "double" },
      { "HLAfloat64LE", "double" },
      { "HLAinteger16LE", "int16_t" },
      { "HLAinteger16BE", "int16_t" },
      { "HLAinteger32BE", "int32_t" },
      { "HLAinteger32LE", "int32_t" },
      { "HLAinteger64BE", "int64_t" },
      { "HLAinteger64LE", "int64_t" },
      { "HLAoctet", "uint8_t" },
      { "HLAoctetPairBE", "OctetPair" },
      { "HLAoctetPairLE", "OctetPair" },
      { "HLAunicodeChar", "wchar_t" },
      { "HLAunicodeString", "std::wstring" },
    };
    public FOMParser(string filename)
    {
      DataTypes = new Dictionary<string, DataType>();
      foreach (var entry in mBasicDataRepresentations)
      {
        DataTypes.Add(entry.Key, new BasicDataRepresentation(entry.Key, entry.Value));
      }
      Filename = filename;
      Namespace = "N" + System.IO.Path.GetFileNameWithoutExtension(filename) + "Encoding";
      XmlDocument doc = new XmlDocument();
      doc.Load(filename);
      var simpleDataTypeNodes = doc.DocumentElement.SelectNodes("/objectModel/dataTypes/simpleDataTypes/simpleData");
      foreach (XmlElement simpleDataTypeNode in simpleDataTypeNodes)
      {
        string name = simpleDataTypeNode["name"].FirstChild.InnerText;
        string representation = simpleDataTypeNode["representation"].FirstChild.InnerText;
        var simpleType = new SimpleDataType(name, DataTypes[representation] as BasicDataRepresentation);
        DataTypes[name] = simpleType;
        simpleType.Generate = (simpleDataTypeNode["nocode"] == null);
      }

      var enumeratedDataTypeNodes = doc.DocumentElement.SelectNodes("/objectModel/dataTypes/enumeratedDataTypes/enumeratedData");
      foreach (XmlElement enumeratedDataTypeNode in enumeratedDataTypeNodes)
      {
        string name = enumeratedDataTypeNode["name"].FirstChild.InnerText;
        string representation = enumeratedDataTypeNode["representation"].FirstChild.InnerText;
        var enumeratedDataType = new EnumeratedDataType(name, DataTypes[representation] as BasicDataRepresentation);
        DataTypes[name] = enumeratedDataType;
        foreach (XmlElement enumeratorNode in enumeratedDataTypeNode.SelectNodes("enumerator"))
        {
          string enumeratorName = enumeratorNode["name"].FirstChild.InnerText;
          string enumeratorValue = enumeratorNode["value"].FirstChild.InnerText;
          enumeratedDataType.Enumerators.Add(enumeratorName, new Enumerator(enumeratorName, enumeratorValue));
        }
        enumeratedDataType.Generate = (enumeratedDataTypeNode["nocode"] == null);
      }

      var arrayDataTypeNodes = doc.DocumentElement.SelectNodes("/objectModel/dataTypes/arrayDataTypes/arrayData");
      foreach (XmlElement arrayDataTypeNode in arrayDataTypeNodes)
      {
        string name = arrayDataTypeNode["name"].FirstChild.InnerText;
        string dataType = arrayDataTypeNode["dataType"].FirstChild.InnerText;
        string cardinality = arrayDataTypeNode["cardinality"].FirstChild.InnerText;
        string encoding = arrayDataTypeNode["encoding"].FirstChild.InnerText;
        if (encoding == "HLAvariableArray")
        {
          if (cardinality != "Dynamic")
          {
            throw new ApplicationException("array with encoding \"HLAvariableArray\" must specify cardinality \"Dynamic\"");
          }
        }
        else if (encoding == "HLAfixedArray")
        {
          int result = 0;
          if (!int.TryParse(cardinality, out result))
          {
            throw new ApplicationException("array with encoding \"HLAfixedArray\" must specify integer cardinality");
          }
        }
        else
        {
          throw new ApplicationException("encoding must be either \"HLAfixedArray\" or \"HLAvariableArray\"");
        }
        var arrayDataType = new ArrayDataType(name, DataTypes[dataType], cardinality, encoding);
        DataTypes[name] = arrayDataType;
        arrayDataType.Generate = (arrayDataTypeNode["nocode"] == null);
      }

      var fixedRecordDataTypeNodes = doc.DocumentElement.SelectNodes("/objectModel/dataTypes/fixedRecordDataTypes/fixedRecordData");
      foreach (XmlElement fixedRecordDataTypeNode in fixedRecordDataTypeNodes)
      {
        string name = fixedRecordDataTypeNode["name"].FirstChild.InnerText;
        var fixedRecordDataType = new FixedRecordDataType(name);
        DataTypes[name] = fixedRecordDataType;
        if (fixedRecordDataTypeNode["include"] != null)
        {
          var includedFixedRecordType = DataTypes[fixedRecordDataTypeNode["include"].FirstChild.InnerText] as FixedRecordDataType;
          foreach (var field in includedFixedRecordType.Fields)
          {
            fixedRecordDataType.Fields.Add(field.Key, field.Value);
          }
        }
        foreach (XmlElement fieldNode in fixedRecordDataTypeNode.SelectNodes("field"))
        {
          string fieldName = fieldNode["name"].FirstChild.InnerText;
          string fieldDataType = fieldNode["dataType"].FirstChild.InnerText;
          var field = new FixedRecordField(fieldName, DataTypes[fieldDataType]);
          if (fieldNode["semantics"] != null && fieldNode["semantics"].FirstChild != null)
          {
            field.Comment = fieldNode["semantics"].FirstChild.InnerText;
          }
          fixedRecordDataType.Fields.Add(fieldName, field);
        }
        fixedRecordDataType.Generate = (fixedRecordDataTypeNode["nocode"] == null);
      }
    }
  }
}
