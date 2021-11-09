
using System;
using System.Collections.Generic;
using System.Xml;
using System.Linq;
using System.Text;

namespace FOMCodeGen
{
  public class FOMParser
  {
    public class ModelIdentificationStruct
    {
      public string Name { get; set; }
      public string Version { get; set; }
      public string Description { get; set; }
      public string Purpose { get; set; }
      public string Copyright { get; set; }
    }
    public class Attribute
    {
      public Attribute(string name)
      {
        Name = name;
      }
      public string Name { get; set; }
      public DataType DataType { get; set; }
    }

    public class ObjectClass
    {
      public ObjectClass(string name)
      {
        Name = name;
        Attributes = new List<Attribute>();
        ChildClasses = new List<ObjectClass>();
      }
      public string Name { get; set; }
      public string QualifiedName
      {
        get
        {
          if (BaseClass == null)
          {
            return Name;
          }
          else
          {
            return BaseClass.QualifiedName + "." + Name;
          }
        }
      }
      public ObjectClass BaseClass { get; set; }
      public List<ObjectClass> ChildClasses { get; set; }
      public List<Attribute> Attributes { get; set; }
      public List<Attribute> AllAttributes
      {
        get
        {
          List<Attribute> allAttributes = new List<Attribute>();
          if (BaseClass != null)
          {
            foreach (var attribute in BaseClass.AllAttributes)
              allAttributes.Add(attribute);
          }
          foreach (var attribute in Attributes)
            allAttributes.Add(attribute);
          return allAttributes;
        }
      }
      public bool HasValidAttributes
      {
        get
        {
          foreach (var attribute in Attributes)
          {
            if (attribute.DataType != null) return true;
          }
          return false;
        }
      }
    }

    public class Parameter
    {
      public Parameter(string name)
      {
        Name = name;
      }
      public string Name { get; set; }
      public DataType DataType { get; set; }
      public string ToEncoderExpression()
      {
        if (DataType is ArrayDataType || DataType is FixedRecordDataType)
        {
          return string.Format("const {0}& {1}Encoder = static_cast<const {0}&>(*{1});", DataType.Encoding, Name);
        }
        else if (DataType is ObjectInstanceHandleType)
        {
          var objectInstanceHandleType = DataType as ObjectInstanceHandleType;
          if (objectInstanceHandleType.ObjectClass != null)
          {
            return string.Format("{0} {1}Encoder(dynamic_cast<IObjectImplBase*>(*{1})->GetObjectInstanceHandle());"
              , DataType.Encoding, Name, objectInstanceHandleType.ObjectClass.Name);
          }
          else
          {
            return string.Format("{0} {1}Encoder(*{1}));", DataType.Encoding, Name);
          }
        }
        else if (DataType is FOMParser.EnumeratedDataType)
        {
          var enumeratedDataType = DataType as FOMParser.EnumeratedDataType;
          return string.Format("{0} {1}Encoder(static_cast<{2}>(*{1}));",
            DataType.Encoding, Name, enumeratedDataType.Representation.CPPType);
        }
        else
        {
          return string.Format("{0} {1}Encoder(*{1});", DataType.Encoding, Name);
        }
      }
    }
    public enum Order { Receive, Timestamp };
    public class InteractionClass
    {
      public InteractionClass(string name)
      {
        Name = name;
        Parameters = new List<Parameter>();
        ChildClasses = new List<InteractionClass>();
        Order = Order.Receive;
      }
      public string Name { get; set; }
      public Order Order { get; set; }
      public string QualifiedName
      {
        get
        {
          if (BaseClass == null)
          {
            return Name;
          }
          else
          {
            return BaseClass.QualifiedName + "." + Name;
          }
        }
      }
      public InteractionClass BaseClass { get; set; }
      public List<InteractionClass> ChildClasses { get; set; }
      public List<Parameter> Parameters { get; set; }
      public List<Parameter> AllParameters
      {
        get
        {
          List<Parameter> allParameters = new List<Parameter>();
          if (BaseClass != null)
          {
            foreach (var parameter in BaseClass.AllParameters)
              allParameters.Add(parameter);
          }
          foreach (var parameter in Parameters)
            allParameters.Add(parameter);
          return allParameters;
        }
      }
      public string CppParameterList
      {
        get
        {
          List<string> parameters = new List<string>();
          foreach (var parameter in AllParameters)
          {
            if (parameter.DataType != null)
            {
              parameters.Add("optional<" + parameter.DataType.ParameterCppType + "> " + parameter.Name);
            }
          }
          return string.Join(", ", parameters);
        }
      }
      public bool HasValidParameters
      {
        get
        {
          foreach (var parameter in Parameters)
          {
            if (parameter.DataType != null) return true;
          }
          return false;
        }
      }
    }
    public class DataType : IComparable<DataType>
    {
      public DataType(string name)
      {
        Name = name;
      }
      public virtual bool Resolve(FOMParser fom)
      {
        return true;
      }
      public virtual bool IsUsing(DataType dataType)
      {
        return false;
      }
      // does the encoding class of this type have 'get()/set()' methods?
      public virtual bool CanTranslateToCpp { get { return true; } }
      // Translate encoder type to native type. Used to pass a interaction parameter into the registered callback.
      public virtual string CppGetter(string var)
      {
        return var + ".get()";
      }
      public virtual string CppSetter(string var, string value)
      {
        return var + ".set(" + value + ")";
      }
      // The name of the data type, as specified in the FOM
      public string Name { get; set; }
      public uint Index { get; set; }
      // The corresponding C++ type (usually the same as the name of the defined type, unless defined otherwise)
      public virtual string CPPType
      {
        get { return Name; }
      }
      // derived const ref type
      public string ConstRefCppType
      {
        get { return "const " + CPPType + "&"; }
      }
      // derived ref type
      public string RefCppType
      {
        get { return CPPType + "&"; }
      }
      // default is by value, may be const ref in compund data types
      public virtual string ParameterCppType
      {
        get { return CPPType; }
      }
      // usually by value
      public virtual string ReturnCppType
      {
        get { return CPPType; }
      }
      public virtual string ModifiableReturnCppType
      {
        get { return ReturnCppType; }
      }
      // The name of the encoding class (either built in or generated by the code generator)
      public virtual string Encoding
      {
        get { return Name + "Encoding"; }
      }
      // Flags whether to generate or not the encoding class
      public bool Generate { get; set; }
      // An optional comment, generated from the 'semantics' tag in the FOM.
      public string Comment
      {
        get
        {
          if (_comment != null && _comment != "")
            return FormatAsComment(_comment);
          else
            return null;
        }
        set { if (value != "NA") _comment = value; }
      }
      public int CompareTo(DataType other)
      {
        // type y depends on/is using type x => x is smaller than y, because x must be defined before y is defined
        if (other.IsUsing(this))
          return -1;
        // type x depends on/is using type y => x is greater than y, because x must be defined after y has been defined
        else if (this.IsUsing(other))
          return 1;
        else
          return Index.CompareTo(other.Index);
      }
      private string _comment;
    }
    // A BasicDataRepresentation is a predefined wrapper for a common C++ common
    // (builtin numeric or string type)
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
      public override string ParameterCppType
      {
        get
        {
          //if (Name == "HLAASCIIstring" || Name == "HLAunicodeString")
          //{
          //  return "const " + CPPType + "&";
          //}
          //else
          //{
            return CPPType;
          //}
        }
      }
      public override string Encoding
      {
        get { return "rti1516ev::" + Name; }
      }
    }
    // a predefined type is one of the types for which a standard encoding already
    // exists in the OpenRTI API
    public class PredefinedType : DataType
    {
      public PredefinedType(string name, string include) : base(name)
      {
        _cppType = "rti1516ev::" + name;
        _include = include;
        _passByReference = false;
        UseCount = 0;
      }
      public PredefinedType(string name, string include, string cppType) : base(name)
      {
        _cppType = cppType;
        _include = include;
        _passByReference = false;
        UseCount = 0;
      }
      // a 'predefined type' represents an encoder defined in some HLA header (such as HLAopaqueData, HLAhandle),
      // *except* basic types, and for which there is no direct C++ equivalent.
      public PredefinedType(string name, string include, string cppType, bool passByReference) : base(name)
      {
        _cppType = cppType;
        _include = include;
        _passByReference = passByReference;
        UseCount = 0;
      }
      private string _cppType;
      public override string CPPType
      {
        get { return _cppType; }
      }
      public override string ParameterCppType
      {
        get
        {
          if (_passByReference)
          {
            return "const " + CPPType + "&";
          }
          else
          {
            return CPPType;
          }
        }
      }
      public override string Encoding
      {
        get { return "rti1516ev::" + Name; }
      }
      public override bool CanTranslateToCpp
      {
        get
        {
          if (Name == "HLAopaqueData")
            return true;
          return false;
        }
      }
      // Translate encoder type to native type. Used to pass a interaction parameter into the registered callback.
      public override string CppGetter(string var)
      {
        if (Name == "HLAopaqueData")
          return base.CppGetter(var);
        return var;
      }
      public virtual string Include
      {
        get { return _include; }
      }
      private string _include;

      public bool PassByReference
      {
        get { return _passByReference; }
      }
      private bool _passByReference;
      public int UseCount { get; set; }
    }
    public class ObjectInstanceHandleType : PredefinedType
    {
      public ObjectInstanceHandleType() : base("HLAobjectInstanceHandle", "#include \"RTI/encoding/HLAhandle.h\"", "rti1516ev::ObjectInstanceHandle")
      {
      }
      public ObjectInstanceHandleType(ObjectClass objectClass)
        : base("HLAobjectInstanceHandle" + "." + objectClass.Name, "", "rti1516ev::ObjectInstanceHandle")
      {
        ObjectClass = objectClass;
        ObjectClassName = objectClass.Name;
      }
      public ObjectInstanceHandleType(string objectClassName)
        : base("HLAobjectInstanceHandle" + "." + objectClassName, "", "rti1516ev::ObjectInstanceHandle")
      {
        ObjectClassName = objectClassName;
      }
      public override bool CanTranslateToCpp
      {
        get
        {
          return true;
        }
      }
      public override string ParameterCppType
      {
        get
        {
          if (ObjectClass != null)
            return "I" + ObjectClass.Name + "*";
          else
            return "rti1516ev::ObjectInstanceHandle";
        }
      }
      public override string ReturnCppType
      {
        get
        {
          if (ObjectClass != null)
            return "I" + ObjectClass.Name + "*";
          else
            return "rti1516ev::ObjectInstanceHandle";
        }
      }
      public override string CppSetter(string var, string value)
      {
        return var + ".set(dynamic_cast<" + ObjectClass.Name + "*>(" + value + ")->GetObjectInstanceHandle())";
      }

      public override string CPPType
      {
        get
        {
          if (ObjectClass != null)
            return ObjectClass.Name + "*";
          else
            return "rti1516ev::ObjectInstanceHandle";
        }
      }
      public override string Encoding
      {
        get { return "rti1516ev::HLAobjectInstanceHandle"; }
      }
      // Translate encoder type to native type. Used to pass a interaction parameter into the registered callback.
      public override string CppGetter(string var)
      {
        if (ObjectClass != null)
          return "static_cast<" + ObjectClass.Name + "ObjectClass*>(GetObjectClassRegistry()->Get" + ObjectClass.Name + "ObjectClass())->GetObjectInstance(" + var + ".get()).get()";
        else
          // no object class given - just pass in the object instance handle
          return var + ".get()";
      }
      public string ObjectClassName { get; set; }
      public ObjectClass ObjectClass { get; set; }
      public override bool Resolve(FOMParser fom)
      {
        if (ObjectClass == null && ObjectClassName != null)
        {
          ObjectClass = fom.ObjectClasses.Find(x => x.Name == ObjectClassName);
        }
        return ObjectClass != null || ObjectClassName == null;
      }
    }

    // A SimpleDataType is basically a typedef to a BasicDataRepresentation
    public class SimpleDataType : DataType
    {
      public SimpleDataType(string name, BasicDataRepresentation representation) : base(name)
      {
        Representation = representation;
      }
      // The BasicDataRepresentation this name is typedef'd to
      public BasicDataRepresentation Representation { get; set; }
      public override string Encoding
      {
        get { return Representation.Encoding; }
      }
    }

    // A member of a enumeration
    public class Enumerator
    {
      public Enumerator(string name, string value, string comment)
      {
        Name = name;
        Value = value;
        Comment = comment;
      }
      public string Name { get; set; }
      public string Value { get; set; }
      public string Comment
      {
        get
        {
          if (_comment != null && _comment != "")
            return FormatAsComment(_comment);
          else
            return null;
        }
        set { if (value != "NA") _comment = value; }
      }
      private string _comment;
    }
      // An enumeration type
    public class EnumeratedDataType : DataType
    {
      public EnumeratedDataType(string name, BasicDataRepresentation representation) : base(name)
      {
        Representation = representation;
        Enumerators = new Dictionary<string, Enumerator>();
      }
      // The BasicDataRepresentation this type is based on
      public BasicDataRepresentation Representation { get; set; }
      public Dictionary<string, Enumerator> Enumerators { get; set; }
      public override string Encoding
      {
        get { return Representation.Encoding; }
      }
      // Translate encoder type to native type. Used to pass a interaction parameter into the registered callback.
      // enumerated types must be casted from the integral type into the enumerated type.
      public override string CppGetter(string var)
      {
        return "static_cast<" + Name + ">(" + var + ".get())";
      }
      public override string CppSetter(string var, string value)
      {
        return var + ".set(static_cast<" + Representation.CPPType + ">(" + value + "))";
      }
    }
    // An array data type, one of HLAfixedArray or HLAvariableArray.
    public class ArrayDataType : DataType
    {
      public ArrayDataType(string name, DataType dataType) : base(name)
      {
        DataType = dataType;
      }
      public DataType DataType { get; set; }
      public override bool Resolve(FOMParser fom)
      {
        if (DataType is UnresolvedDataType)
        {
          DataType = fom.DataTypes.Find(dataType => dataType.Name == DataType.Name);
        }
        return true;
      }
      public override bool IsUsing(DataType dataType)
      {
        return DataType == dataType;
      }
      public override string CPPType
      {
        get
        {
          if (DataType is FOMParser.BasicDataRepresentation || DataType is FOMParser.SimpleDataType)
            return "std::vector<" + DataType.CPPType + ">";
          else
            return "std::vector<" + DataType.Encoding + ">";
        }
      }
      public override string ParameterCppType
      {
        get { return ConstRefCppType; }
      }
      // usually by value
      public override string ReturnCppType
      {
        get { return ConstRefCppType; }
      }
    }

    public class FixedArrayDataType : ArrayDataType
    {
      public FixedArrayDataType(string name, DataType dataType, string cardinality) : base(name, dataType)
      {
        DataType = dataType;
        Cardinality = cardinality;
      }
      public string Cardinality { get; set; }
    }

    public class VariableArrayDataType : ArrayDataType
    {
      public VariableArrayDataType(string name, DataType dataType) : base(name, dataType)
      {
      }
    }

    public class FixedRecordField
    {
      public FixedRecordField(string name, DataType dataType, uint version)
      {
        Name = name;
        DataType = dataType;
        Version = version;
      }
      public string Name { get; set; }
      public DataType DataType { get; set; }
      public uint Version { get; set; }
      public string Comment { get; set; }
    }
    public class FixedRecordDataType : DataType
    {
      private uint _version;

      public FixedRecordDataType(string name, uint version) : base(name)
      {
        Fields = new List<FixedRecordField>();
        Version = version;
        HasChilds = false;
      }
      public override string ParameterCppType
      {
        get { return ConstRefCppType; }
      }
      // usually by value
      public override string ReturnCppType
      {
        get { return ConstRefCppType; }
      }
      public override string ModifiableReturnCppType
      {
        get { return RefCppType; }
      }

      public uint Version
      {
        get
        {
          if (BaseClass != null) {
            var baseVersion = BaseClass.Version;
            if (baseVersion > _version) return baseVersion;
          }
          return _version;
        }
        set {
          if (BaseClass != null && BaseClass.Version > value) {
            Console.WriteLine("Warning: fixed record {} has smaller version than it's base class! Setting to {}", Name, BaseClass.Name, BaseClass.Version);
            _version = BaseClass.Version;
          } else {
            _version = value;
          }
        }
      }
      public List<FixedRecordField> Fields { get; set; }
      public List<FixedRecordField> AllFields
      {
        get
        {
          FixedRecordDataType baseClass = BaseClass;
          // sort the fields ascending, according to field version.
          int numElements = (int)Version + 1;
          List<FixedRecordField>[] sortedFields = new List<FixedRecordField>[numElements];
          for (int i = 0; i < numElements; i++) {
            sortedFields[i] = new List<FixedRecordField>();
          }
          if (baseClass != null) {
            var baseClassFields = baseClass.AllFields;
            foreach (var field in baseClassFields)
              sortedFields[field.Version].Add(field);
          }
          foreach (var field in Fields)
            sortedFields[field.Version].Add(field);

          List<FixedRecordField> allFields = new List<FixedRecordField>();
          foreach (var fieldList in sortedFields) {
            allFields.AddRange(fieldList);
          }
          return allFields;
        }
      }
      public FixedRecordDataType BaseClass { get; set; }
      public bool HasChilds { get; set; }
      public override bool Resolve(FOMParser fom)
      {
        foreach (var field in Fields) {
          if (field.DataType is UnresolvedDataType) {
            field.DataType = fom.DataTypes.Find(dataType => dataType.Name == field.DataType.Name);
          }
        }
        return true;
      }
      public override bool IsUsing(DataType dataType)
      {
        foreach (var field in Fields) {
          if (field.DataType == dataType) {
            return true;
          }
        }
        return false;
      }
      // Translate encoder type to native type. Used to pass a interaction parameter into the registered callback.
      // Fixed records always shall be passed directly (by reference).
      public override string CppGetter(string var)
      {
        return var;
      }
      public override string CppSetter(string var, string value)
      {
        return var + " = " + value;
      }
    }

    public class UnresolvedDataType : DataType
    {
      public UnresolvedDataType(string name) : base(name)
      {
      }
    }
    public List<DataType> DataTypes { get; set; }
    public string Filename { get; set; }
    public string[] Namespace { get; set; }
    public bool UsePrecompiledHeaders { get; set; }
    public ModelIdentificationStruct ModelIdentification { get; set; }
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
    private List<PredefinedType> mPredefinedTypes = new List<PredefinedType>()
    {
      new PredefinedType("HLAopaqueData", "#include \"RTI/encoding/HLAopaqueData.h\"", "std::vector<uint8_t>", true),
      new PredefinedType("HLAhandle", "#include \"RTI/encoding/HLAhandle.h\""),
      new ObjectInstanceHandleType()
    };
    public enum FileContext { kDataTypes, kObjectInterfaces, kObjectHeader, kInteractionInterfaces, kInteractionHeader };
    Dictionary<FileContext, HashSet<DataType>> mPredefinedTypesInContext = new Dictionary<FileContext, HashSet<DataType>>()
    {
      [FileContext.kDataTypes] = new HashSet<DataType>(),
      [FileContext.kObjectInterfaces] = new HashSet<DataType>(),
      [FileContext.kObjectHeader] = new HashSet<DataType>(),
      [FileContext.kInteractionInterfaces] = new HashSet<DataType>(),
      [FileContext.kInteractionHeader] = new HashSet<DataType>(),
    };
    Dictionary<FileContext, HashSet<string>> mPredefinedTypeIncludes = new Dictionary<FileContext, HashSet<string>>()
    {
      [FileContext.kDataTypes] = new HashSet<string>(),
      [FileContext.kObjectInterfaces] = new HashSet<string>(),
      [FileContext.kObjectHeader] = new HashSet<string>(),
      [FileContext.kInteractionInterfaces] = new HashSet<string>(),
      [FileContext.kInteractionHeader] = new HashSet<string>(),
    };

    public string GetPredefinedTypeIncludes(FileContext context)
    {
      List<string> lines = new List<string>();
      foreach (DataType dataType in mPredefinedTypesInContext[context])
      {
        string include = (dataType as PredefinedType).Include;
        if (include.Length > 0)
          lines.Add(include);
      }
      return string.Join("\n", lines);
    }

    private HashSet<string> mDataTypeForwardDeclarations = new HashSet<string>();
    public string DataTypeForwardDeclarations
    {
      get
      {
        string[] lines = new string[mDataTypeForwardDeclarations.Count];
        mDataTypeForwardDeclarations.CopyTo(lines);
        return string.Join("\n", lines);
      }
    }
    private HashSet<string> mEncodingForwardDeclarations = new HashSet<string>();
    public string EncodingForwardDeclarations
    {
      get
      {
        string[] lines = new string[mEncodingForwardDeclarations.Count];
        mEncodingForwardDeclarations.CopyTo(lines);
        return string.Join("\n", lines);
      }
    }

    private List<ObjectClass> mObjectClasses = new List<ObjectClass>();
    public List<ObjectClass> ObjectClasses
    {
      get { return mObjectClasses; }
    }
    public List<ObjectClass> ReverseObjectClasses
    {
      get {
        List<ObjectClass> result = new List<ObjectClass>();
        result.AddRange(mObjectClasses);
        result.Reverse();
        return result;
      }
    }


    private List<InteractionClass> mInteractionClasses = new List<InteractionClass>();
    public List<InteractionClass> InteractionClasses
    {
      get { return mInteractionClasses; }
    }
    public bool IsFloatTime { get; set; }
    public string NativeTimeType
    {
      get
      {
        if (IsFloatTime) return "double";
        else return "int64_t";
      }
    }
    public string LogicalTimeType
    {
      get
      {
        if (IsFloatTime) return "rti1516ev::HLAfloat64Time";
        else return "rti1516ev::HLAinteger64Time";
      }
    }
    public string GetLogicalTimeCast(string param)
    {
      if (IsFloatTime)
        return "static_cast<const rti1516ev::HLAfloat64Time&>(" + param + ").getTime()";
      else
        return "static_cast<const rti1516ev::HLAinteger64Time&>(" + param + ").getTime()";
    }
    DataType GetDataType(string name, FileContext context)
    {
      DataType result = DataTypes.Find(dataType => dataType.Name == name);
      if (result != null)
      {
        if (result is PredefinedType)
        {
          if (!result.CanTranslateToCpp)
          {
            mPredefinedTypesInContext[context].Add(result);
          }
          if (context == FileContext.kObjectInterfaces)
          {
            mPredefinedTypesInContext[FileContext.kObjectHeader].Add(result);
          }
        }
        return result;
      }
      else
      {
        mDataTypeForwardDeclarations.Add("class " + name + ";");
        mEncodingForwardDeclarations.Add("class " + name + "Encoding;");
        return new UnresolvedDataType(name);
      }
    }
    bool IsBasicOrPredefinedType(string name)
    {
      DataType result = DataTypes.Find(dataType => dataType.Name == name);
      if (result != null)
      {
        if (result is PredefinedType || result is BasicDataRepresentation)
        {
          return true;
        }
      }
      return false;
    }
    public static string FormatAsComment(string value, string prefix = "")
    {
      var lines = value.Split('\n');
      string result = "";
      bool firstLine = true;
      string spaces = new string(' ', prefix.Length);
      foreach (var line in lines)
      {
        if (firstLine)
          result += "// " + prefix + line;
        else
          result += "\n// " + spaces + line;
        firstLine = false;
      }
      return result;
    }
    public string GeneratorVersion
    {
      get
      {
        System.Reflection.Assembly assembly = System.Reflection.Assembly.GetExecutingAssembly();
        System.Diagnostics.FileVersionInfo fileVersion = System.Diagnostics.FileVersionInfo.GetVersionInfo(assembly.Location);
        return fileVersion.FileVersion;
      }
    }
    // Compare data types according to dependency/usage:
    // If x is using y, y must be defined first.
    // If y is using x, x must be defined first.
    public int CompareDataTypes(DataType x, DataType y)
    {
      // type y depends on/is using type x => x is smaller than y, because x must be defined before y is defined
      if (y.IsUsing(x))
        return -1;
      // type x depends on/is using type y => x is greater than y, because x must be defined after y has been defined
      else if (x.IsUsing(y))
        return 1;
      else
        // if the types are independent, leave order intact.
        return x.Index.CompareTo(y.Index);
    }
    void ParseSimpleDataType(XmlElement simpleDataTypeNode, ref uint index)
    {
      string name = simpleDataTypeNode["name"].FirstChild.InnerText;
      if (IsBasicOrPredefinedType(name))
      {
        System.Console.WriteLine("ignoring predefined type {0}", name);
        return;
      }
      string representation = simpleDataTypeNode["representation"].FirstChild.InnerText;
      var representationType = GetDataType(representation, FileContext.kDataTypes) as BasicDataRepresentation;
      var simpleType = new SimpleDataType(name, representationType);
      simpleType.Index = index++;
      simpleType.Generate = (simpleDataTypeNode["nocode"] == null);
      if (simpleDataTypeNode["semantics"] != null && simpleDataTypeNode["semantics"].FirstChild != null)
        simpleType.Comment = simpleDataTypeNode["semantics"].FirstChild.InnerText;
      DataTypes.Add(simpleType);
    }
    private void ParseEnumeratedDataType(XmlElement enumeratedDataTypeNode, ref uint index)
    {
      string name = enumeratedDataTypeNode["name"].FirstChild.InnerText;
      if (IsBasicOrPredefinedType(name))
      {
        System.Console.WriteLine("ignoring predefined type {0}", name);
        return;
      }
      string representation = enumeratedDataTypeNode["representation"].FirstChild.InnerText;
      var representationType = GetDataType(representation, FileContext.kDataTypes) as BasicDataRepresentation;
      var enumeratedDataType = new EnumeratedDataType(name, representationType);
      foreach (XmlElement enumeratorNode in enumeratedDataTypeNode.SelectNodes("enumerator"))
      {
        string enumeratorName = enumeratorNode["name"].FirstChild.InnerText;
        string enumeratorValue = enumeratorNode["value"].FirstChild.InnerText;
        string enumeratorComment = null;
        if (enumeratorNode["semantics"] != null && enumeratorNode["semantics"].FirstChild != null)
          enumeratorComment = enumeratorNode["semantics"].FirstChild.InnerText;
        enumeratedDataType.Enumerators.Add(enumeratorName, new Enumerator(enumeratorName, enumeratorValue, enumeratorComment));
      }
      enumeratedDataType.Generate = (enumeratedDataTypeNode["nocode"] == null);
      if (enumeratedDataTypeNode["semantics"] != null && enumeratedDataTypeNode["semantics"].FirstChild != null)
        enumeratedDataType.Comment = enumeratedDataTypeNode["semantics"].FirstChild.InnerText;
      DataTypes.Add(enumeratedDataType);
      enumeratedDataType.Index = index++;

    }
    private void ParseArrayDataType(XmlElement arrayDataTypeNode, ref uint index)
    {
      string name = arrayDataTypeNode["name"].FirstChild.InnerText;
      if (IsBasicOrPredefinedType(name))
      {
        System.Console.WriteLine("ignoring predefined type {0}", name);
        return;
      }
      string dataType = arrayDataTypeNode["dataType"].FirstChild.InnerText;
      string cardinality = arrayDataTypeNode["cardinality"].FirstChild.InnerText;
      string encoding = arrayDataTypeNode["encoding"].FirstChild.InnerText;
      string comment = null;
      if (arrayDataTypeNode["semantics"] != null && arrayDataTypeNode["semantics"].FirstChild != null)
        comment = arrayDataTypeNode["semantics"].FirstChild.InnerText;
      bool generate = (arrayDataTypeNode["nocode"] == null);
      if (encoding == "HLAvariableArray")
      {
        if (cardinality != "Dynamic")
        {
          throw new ApplicationException("array with encoding \"HLAvariableArray\" must specify cardinality \"Dynamic\"");
        }
        if (dataType == "HLAunicodeChar")
        {
          var simpleType = new SimpleDataType(name, GetDataType("HLAunicodeString", FileContext.kDataTypes) as BasicDataRepresentation);
          simpleType.Generate = generate;
          DataTypes.Add(simpleType);
          simpleType.Index = index++;
        }
        else
        {
          var arrayDataType = new VariableArrayDataType(name, GetDataType(dataType, FileContext.kDataTypes));
          arrayDataType.Generate = generate;
          arrayDataType.Comment = comment;
          DataTypes.Add(arrayDataType);
          arrayDataType.Index = index++;
        }
      }
      else if (encoding == "HLAfixedArray")
      {
        int result = 0;
        if (!int.TryParse(cardinality, out result))
        {
          throw new ApplicationException("array with encoding \"HLAfixedArray\" must specify integer cardinality");
        }
        var arrayDataType = new FixedArrayDataType(name, GetDataType(dataType, FileContext.kDataTypes), cardinality);
        arrayDataType.Generate = (arrayDataTypeNode["nocode"] == null);
        if (arrayDataTypeNode["semantics"] != null)
          arrayDataType.Comment = arrayDataTypeNode["semantics"].FirstChild.InnerText;
        DataTypes.Add(arrayDataType);
        arrayDataType.Index = index++;
      }
      else
      {
        throw new ApplicationException("encoding must be either \"HLAfixedArray\" or \"HLAvariableArray\"");
      }
    }
    private void ParseFixedRecordDataType(XmlElement fixedRecordDataTypeNode, ref uint index)
    {
      string name = fixedRecordDataTypeNode["name"].FirstChild.InnerText;
      if (IsBasicOrPredefinedType(name))
      {
        return;
      }
      FixedRecordDataType includedFixedRecordType = null;
      if (fixedRecordDataTypeNode["include"] != null)
      {
        var includeNodes = fixedRecordDataTypeNode.GetElementsByTagName("include");
        if (includeNodes.Count > 1)
        {
          throw new ApplicationException("fixed record \"" + name + "\" can only have one 'included' base record");
        }
        includedFixedRecordType = GetDataType(fixedRecordDataTypeNode["include"].FirstChild.InnerText, FileContext.kDataTypes) as FixedRecordDataType;
        includedFixedRecordType.HasChilds = true;
      }

      uint recordVersion = 0;
      if (fixedRecordDataTypeNode["version"] != null)
      {
        string versionString = fixedRecordDataTypeNode["version"].FirstChild.InnerText;
        if (!uint.TryParse(versionString, out recordVersion))
        {
          throw new ApplicationException("record version must be an integer: \"" + versionString + "\"");
        }
      }
      else if (includedFixedRecordType != null)
      {
        recordVersion = includedFixedRecordType.Version;
      }
      var fixedRecordDataType = new FixedRecordDataType(name, recordVersion);
      fixedRecordDataType.BaseClass = includedFixedRecordType;

      foreach (XmlElement fieldNode in fixedRecordDataTypeNode.SelectNodes("field"))
      {
        string fieldName = fieldNode["name"].FirstChild.InnerText;
        string fieldDataType = fieldNode["dataType"].FirstChild.InnerText;
        uint fieldVersion = recordVersion;
        if (fieldNode["version"] != null)
        {
          string versionString = fieldNode["version"].FirstChild.InnerText;
          if (!uint.TryParse(versionString, out fieldVersion))
          {
            throw new ApplicationException("field version must be an integer: \"" + versionString + "\"");
          }
        }
        var field = new FixedRecordField(fieldName, GetDataType(fieldDataType, FileContext.kDataTypes), fieldVersion);
        if (fieldNode["semantics"] != null && fieldNode["semantics"].FirstChild != null)
        {
          field.Comment = fieldNode["semantics"].FirstChild.InnerText;
        }
        fixedRecordDataType.Fields.Add(field);
      }
      FixedRecordField previousField = null;
      foreach (var field in fixedRecordDataType.Fields)
      {
        if (field.Version > fixedRecordDataType.Version)
        {
          throw new ApplicationException(string.Format("fixed record \"{0}\" version {1}, field \"{2}\" version {3}: field version must be lower or equal to record version",
            fixedRecordDataType.Name, fixedRecordDataType.Version, field.Name, field.Version));
        }
        if (previousField != null && previousField.Version > field.Version)
        {
          throw new ApplicationException(string.Format("fixed record \"{0}\", field \"{1}\" version {2}: field version must be greater or equal to previous field \"{3}\" version {4}",
            fixedRecordDataType.Name, field.Name, field.Version, previousField.Name, previousField.Version));
        }
        previousField = field;
      }
      fixedRecordDataType.Generate = (fixedRecordDataTypeNode["nocode"] == null);
      if (fixedRecordDataTypeNode["semantics"] != null)
      {
        fixedRecordDataType.Comment = fixedRecordDataTypeNode["semantics"].FirstChild.InnerText;
      }
      DataTypes.Add(fixedRecordDataType);
      fixedRecordDataType.Index = index++;
    }
    // replace dataType by ObjectInstanceHandleType of named class

    private DataType GetObjectClassType(DataType dataType, string className)
    {
      if (dataType is ObjectInstanceHandleType)
      {
        ObjectInstanceHandleType objectInstanceHandleType = dataType as ObjectInstanceHandleType;
        if (className != null)
        {
          var attributeTypeObjectClass = mObjectClasses.Find(x => x.Name == className);
          if (attributeTypeObjectClass != null)
          {
            dataType = new ObjectInstanceHandleType(attributeTypeObjectClass);
          }
          else
          {
            dataType = new ObjectInstanceHandleType(className);
          }
        }
        else
        {
          objectInstanceHandleType.UseCount++;
        }
      }
      return dataType;
    }

    private void ParseObjectClasses(XmlElement objectClassNode, ObjectClass baseClass)
    {
      string name = objectClassNode["name"].FirstChild.InnerText;
      ObjectClass objectClass = new ObjectClass(name);
      objectClass.BaseClass = baseClass;
      System.Console.WriteLine("ObjectClass {0}", objectClass.Name);
      mObjectClasses.Add(objectClass);
      if (objectClass.BaseClass != null)
      {
        objectClass.BaseClass.ChildClasses.Add(objectClass);
      }
      var attributeNodes = objectClassNode.SelectNodes("attribute");
      foreach (XmlElement attributeNode in attributeNodes)
      {
        string attributeName = attributeNode["name"].FirstChild.InnerText;
        Attribute attribute = new Attribute(attributeName);
        objectClass.Attributes.Add(attribute);
        XmlElement dataTypeElement = attributeNode["dataType"];
        if (dataTypeElement != null)
        {
          DataType dataType = GetDataType(dataTypeElement.FirstChild.InnerText, FileContext.kObjectInterfaces);
          attribute.DataType = GetObjectClassType(dataType, dataTypeElement.GetAttribute("class"));
        }
      }
      var childClassNodes = objectClassNode.SelectNodes("objectClass");
      foreach (var childClass in childClassNodes)
      {
        ParseObjectClasses(childClass as XmlElement, objectClass);
      }
    }

    private void ParseInteractionClasses(XmlElement interactionClassNode, InteractionClass baseClass)
    {
      string name = interactionClassNode["name"].FirstChild.InnerText;
      InteractionClass interactionClass = new InteractionClass(name);
      interactionClass.BaseClass = baseClass;
      mInteractionClasses.Add(interactionClass);
      System.Console.WriteLine("InteractionClass {0}", interactionClass.Name);
      if (interactionClassNode["order"] != null)
      {
        string order = interactionClassNode["order"].FirstChild.InnerText;
        if (order.Equals("timestamp", StringComparison.OrdinalIgnoreCase))
        {
          interactionClass.Order = Order.Timestamp;
        }
        else if (order.Equals("receive", StringComparison.OrdinalIgnoreCase))
        {
          interactionClass.Order = Order.Receive;
        }
      }

      if (interactionClass.BaseClass != null)
      {
        interactionClass.BaseClass.ChildClasses.Add(interactionClass);
      }
      var parameterNodes = interactionClassNode.SelectNodes("parameter");
      foreach (XmlElement parameterNode in parameterNodes)
      {
        string parameterName = parameterNode["name"].FirstChild.InnerText;
        Parameter parameter = new Parameter(parameterName);
        interactionClass.Parameters.Add(parameter);
        XmlElement dataTypeElement = parameterNode["dataType"];
        if (dataTypeElement != null)
        {
          DataType dataType = GetDataType(dataTypeElement.FirstChild.InnerText, FileContext.kInteractionInterfaces);
          parameter.DataType = GetObjectClassType(dataType, dataTypeElement.GetAttribute("class"));
        }
      }
      var childClassNodes = interactionClassNode.SelectNodes("interactionClass");
      foreach (var childClass in childClassNodes)
      {
        ParseInteractionClasses(childClass as XmlElement, interactionClass);
      }
    }
    public FOMParser(string filename, string enclosingNamespace)
    {
      DataTypes = new List<DataType>();
      IsFloatTime = false;
      uint index = 0;
      foreach (var entry in mBasicDataRepresentations)
      {
        DataTypes.Add(new BasicDataRepresentation(entry.Key, entry.Value));
      }
      foreach (var predefinedType in mPredefinedTypes)
      {
        DataTypes.Add(predefinedType);
      }
      Filename = filename;
      Namespace = enclosingNamespace.Split('.');
      ModelIdentification = new ModelIdentificationStruct();

      XmlDocument doc = new XmlDocument();
      doc.Load(filename);


      var modelIdentificationNode = doc.DocumentElement.SelectSingleNode("/objectModel/modelIdentification");
      if (modelIdentificationNode != null)
      {
        if (modelIdentificationNode["name"] != null)
        {
          ModelIdentification.Name = modelIdentificationNode["name"].FirstChild.InnerText;
        }
        if (modelIdentificationNode["version"] != null)
        {
          ModelIdentification.Version = modelIdentificationNode["version"].FirstChild.InnerText;
        }
        if (modelIdentificationNode["description"] != null)
        {
          ModelIdentification.Description = modelIdentificationNode["description"].FirstChild.InnerText;
        }
        if (modelIdentificationNode["purpose"] != null)
        {
          ModelIdentification.Purpose = modelIdentificationNode["purpose"].FirstChild.InnerText;
        }
        if (modelIdentificationNode["copyright"] != null)
        {
          ModelIdentification.Copyright = modelIdentificationNode["copyright"].FirstChild.InnerText;
        }
      }

      var simpleDataTypeNodes = doc.DocumentElement.SelectNodes("/objectModel/dataTypes/simpleDataTypes/simpleData");
      foreach (XmlElement simpleDataTypeNode in simpleDataTypeNodes)
      {
        ParseSimpleDataType(simpleDataTypeNode, ref index);
      }

      var enumeratedDataTypeNodes = doc.DocumentElement.SelectNodes("/objectModel/dataTypes/enumeratedDataTypes/enumeratedData");
      foreach (XmlElement enumeratedDataTypeNode in enumeratedDataTypeNodes)
      {
        ParseEnumeratedDataType(enumeratedDataTypeNode, ref index);
      }

      var arrayDataTypeNodes = doc.DocumentElement.SelectNodes("/objectModel/dataTypes/arrayDataTypes/arrayData");
      foreach (XmlElement arrayDataTypeNode in arrayDataTypeNodes)
      {
        ParseArrayDataType(arrayDataTypeNode, ref index);
      }

      var fixedRecordDataTypeNodes = doc.DocumentElement.SelectNodes("/objectModel/dataTypes/fixedRecordDataTypes/fixedRecordData");
      foreach (XmlElement fixedRecordDataTypeNode in fixedRecordDataTypeNodes)
      {
        ParseFixedRecordDataType(fixedRecordDataTypeNode, ref index);
      }
      foreach (DataType dataType in DataTypes)
      {
        dataType.Resolve(this);
      }
      List<string> unresolvedDataTypes = new List<string>();
      foreach (DataType dataType in DataTypes)
      {
        if (dataType is UnresolvedDataType)
        {
          unresolvedDataTypes.Add(dataType.Name);
        }
      }
      if (unresolvedDataTypes.Count > 0)
      {
        var types = String.Join("\n", unresolvedDataTypes);
        throw new ApplicationException("some datatypes could not be resolved:\n" + types);
      }
      foreach (DataType dataType in DataTypes)
      {
        System.Console.WriteLine("{0} {1}", DataTypes.IndexOf(dataType), dataType.Name);
      }
      DataTypes.Sort(this.CompareDataTypes);
      foreach (DataType dataType in DataTypes)
      {
        System.Console.WriteLine("{0} {1}", DataTypes.IndexOf(dataType), dataType.Name);
      }

      var objectClassNodes = doc.DocumentElement.SelectNodes("/objectModel/objects/objectClass");
      if (objectClassNodes.Count != 1)
      {
        throw new ApplicationException(string.Format("unexpected FOM format: there should be exactly one objectClass root node, named \"HLAobjectRoot\""));
      }
      var objectRootNode = objectClassNodes.Item(0) as XmlElement;
      ParseObjectClasses(objectRootNode, null);

      var interactionClassNodes = doc.DocumentElement.SelectNodes("/objectModel/interactions/interactionClass");
      if (interactionClassNodes.Count != 1)
      {
        throw new ApplicationException(string.Format("unexpected FOM format: there should be exactly one interactionClass root node, named \"HLAinteractionRoot\""));
      }
      var interactionRootNode = interactionClassNodes.Item(0) as XmlElement;
      ParseInteractionClasses(interactionRootNode, null);
    }

  }
}
