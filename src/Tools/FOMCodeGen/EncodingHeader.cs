﻿// ------------------------------------------------------------------------------
// <auto-generated>
//     This code was generated by a tool.
//     Runtime Version: 15.0.0.0
//  
//     Changes to this file may cause incorrect behavior and will be lost if
//     the code is regenerated.
// </auto-generated>
// ------------------------------------------------------------------------------
namespace FOMCodeGen
{
    using System.Linq;
    using System.Text;
    using System.Collections.Generic;
    using System;
    
    /// <summary>
    /// Class to produce the template output
    /// </summary>
    
    #line 1 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "15.0.0.0")]
    public partial class EncodingHeader : EncodingHeaderBase
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public virtual string TransformText()
        {
            this.Write("\r\n// Code automatically generated from ");
            
            #line 7 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(FOM.Filename));
            
            #line default
            #line hidden
            this.Write(", do not edit\r\n\r\n#pragma once\r\n\r\n#include \"RTI/encoding/BasicDataElements.h\"\r\n#in" +
                    "clude \"RTI/encoding/HLAfixedRecord.h\"\r\n#include \"RTI/encoding/HLAfixedArray.h\"\r\n" +
                    "#include \"RTI/encoding/HLAvariableArray.h\"\r\n\r\nnamespace ");
            
            #line 16 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(FOM.Namespace));
            
            #line default
            #line hidden
            this.Write(" {\r\n\r\n");
            
            #line 18 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

foreach (var dataType in FOM.DataTypes.Values)
{
  if (dataType is FOMParser.FixedRecordDataType && dataType.Generate)
  {
    var fixedRecordDataType = dataType as FOMParser.FixedRecordDataType;

            
            #line default
            #line hidden
            this.Write("class ");
            
            #line 25 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("Encoding : public rti1516e::HLAfixedRecord\r\n{\r\n  public:\r\n    ");
            
            #line 28 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("Encoding()\r\n");
            
            #line 29 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    string sep = ":";
    foreach (var field in fixedRecordDataType.Fields.Values)
    {

            
            #line default
            #line hidden
            this.Write("     ");
            
            #line 34 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(sep));
            
            #line default
            #line hidden
            this.Write(" m");
            
            #line 34 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.Name));
            
            #line default
            #line hidden
            this.Write("(new ");
            
            #line 34 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.DataType.Encoding));
            
            #line default
            #line hidden
            this.Write("())\r\n");
            
            #line 35 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

      sep=",";
    } // foreach fixedRecordDataType.Fields.Values

            
            #line default
            #line hidden
            this.Write("    {\r\n");
            
            #line 40 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    foreach (var field in fixedRecordDataType.Fields.Values)
    {

            
            #line default
            #line hidden
            this.Write("      rti1516e::HLAfixedRecord::appendElementPointer(m");
            
            #line 44 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.Name));
            
            #line default
            #line hidden
            this.Write(");\r\n");
            
            #line 45 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    } // foreach fixedRecordDataType.Fields.Values

            
            #line default
            #line hidden
            this.Write("    }\r\n    ~");
            
            #line 49 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("Encoding()\r\n    {\r\n    }\r\n");
            
            #line 52 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    foreach (var field in fixedRecordDataType.Fields.Values)
    {

            
            #line default
            #line hidden
            this.Write("    void Set");
            
            #line 56 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.Name));
            
            #line default
            #line hidden
            this.Write("(");
            
            #line 56 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.DataType.CPPType));
            
            #line default
            #line hidden
            this.Write(" value)\r\n    {\r\n\t    m");
            
            #line 58 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.Name));
            
            #line default
            #line hidden
            this.Write("->set(value);\r\n    }\r\n    ");
            
            #line 60 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.DataType.CPPType));
            
            #line default
            #line hidden
            this.Write(" Get");
            
            #line 60 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.Name));
            
            #line default
            #line hidden
            this.Write("()\r\n    {\r\n");
            
            #line 62 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

      if (field.DataType is FOMParser.EnumeratedDataType) {

            
            #line default
            #line hidden
            this.Write("\t    return static_cast<");
            
            #line 65 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.DataType.CPPType));
            
            #line default
            #line hidden
            this.Write(">(m");
            
            #line 65 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.Name));
            
            #line default
            #line hidden
            this.Write("->get());\r\n");
            
            #line 66 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

      } else {

            
            #line default
            #line hidden
            this.Write("\t    return m");
            
            #line 69 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.Name));
            
            #line default
            #line hidden
            this.Write("->get();\r\n");
            
            #line 70 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

      } // else (field.DataType is FOMParser.EnumeratedDataType)

            
            #line default
            #line hidden
            this.Write("    }\r\n");
            
            #line 74 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    } // foreach fixedRecordDataType.Fields.Values

            
            #line default
            #line hidden
            this.Write("\r\n  private:\r\n");
            
            #line 79 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    foreach (var field in fixedRecordDataType.Fields.Values)
    {

            
            #line default
            #line hidden
            this.Write("    ");
            
            #line 83 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.DataType.Encoding));
            
            #line default
            #line hidden
            this.Write("* m");
            
            #line 83 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(field.Name));
            
            #line default
            #line hidden
            this.Write(";\r\n");
            
            #line 84 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    } // foreach fixedRecordDataType.Fields.Values

            
            #line default
            #line hidden
            this.Write("}; // class ");
            
            #line 87 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("Encoding\r\n\r\n");
            
            #line 89 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

  } // if (dataType is FOMParser.FixedRecordDataType)
  else if (dataType is FOMParser.EnumeratedDataType && dataType.Generate)
  {
    var enumeratedDataType = dataType as FOMParser.EnumeratedDataType;

            
            #line default
            #line hidden
            this.Write("enum ");
            
            #line 95 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("\r\n{\r\n");
            
            #line 97 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    foreach (var enumerator in enumeratedDataType.Enumerators.Values)
    {

            
            #line default
            #line hidden
            this.Write("      ");
            
            #line 101 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(enumerator.Name));
            
            #line default
            #line hidden
            this.Write(" = ");
            
            #line 101 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(enumerator.Value));
            
            #line default
            #line hidden
            this.Write(",\r\n");
            
            #line 102 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    } // foreach enumeratedDataType.Enumerators.Values

            
            #line default
            #line hidden
            this.Write("}; // enum ");
            
            #line 105 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("\r\n\r\n");
            
            #line 107 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

  } // if (dataType is FOMParser.EnumeratedDataType)
  else if (dataType is FOMParser.ArrayDataType && dataType.Generate)
  {
    var arrayDataType = dataType as FOMParser.ArrayDataType;

            
            #line default
            #line hidden
            this.Write("class ");
            
            #line 113 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("Encoding : public rti1516e::");
            
            #line 113 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.EncodingBase));
            
            #line default
            #line hidden
            this.Write("\r\n{\r\n  public:\r\n    ");
            
            #line 116 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("Encoding()\r\n      : ");
            
            #line 117 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    if (arrayDataType.EncodingBase == "HLAfixedArray") { 
    
            
            #line default
            #line hidden
            this.Write("HLAfixedArray(");
            
            #line 119 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.DataType.Encoding));
            
            #line default
            #line hidden
            this.Write("(), ");
            
            #line 119 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.Cardinality));
            
            #line default
            #line hidden
            this.Write(")");
            
            #line 119 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    } else {
    
            
            #line default
            #line hidden
            this.Write("HLAvariableArray(");
            
            #line 121 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.DataType.Encoding));
            
            #line default
            #line hidden
            this.Write(")");
            
            #line 121 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    } 
            
            #line default
            #line hidden
            this.Write("\r\n      , mData(");
            
            #line 124 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.Cardinality));
            
            #line default
            #line hidden
            this.Write(")\r\n    {\r\n");
            
            #line 126 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
    if (arrayDataType.EncodingBase == "HLAfixedArray") { 
            
            #line default
            #line hidden
            this.Write("      for (int i=0;i<");
            
            #line 127 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.Cardinality));
            
            #line default
            #line hidden
            this.Write(";i++)\r\n      {\r\n        mEncoding[i].setDataPointer(&mData.data()[i]);\r\n        s" +
                    "etElementPointer(i, &mEncoding[i]);\r\n      }\r\n");
            
            #line 132 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("    }\r\n    ");
            
            #line 134 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("Encoding(const ");
            
            #line 134 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.CPPType));
            
            #line default
            #line hidden
            this.Write("& data)\r\n      : ");
            
            #line 135 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    if (arrayDataType.EncodingBase == "HLAfixedArray") { 
    
            
            #line default
            #line hidden
            this.Write("HLAfixedArray(");
            
            #line 137 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.DataType.Encoding));
            
            #line default
            #line hidden
            this.Write("(), ");
            
            #line 137 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.Cardinality));
            
            #line default
            #line hidden
            this.Write(")");
            
            #line 137 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    } else {
    
            
            #line default
            #line hidden
            this.Write("HLAvariableArray(");
            
            #line 139 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.DataType.Encoding));
            
            #line default
            #line hidden
            this.Write(")");
            
            #line 139 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

    } 
            
            #line default
            #line hidden
            this.Write("\r\n      , mData(data)\r\n    {\r\n");
            
            #line 144 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
    if (arrayDataType.EncodingBase == "HLAfixedArray") { 
            
            #line default
            #line hidden
            this.Write("      for (int i=0;i<");
            
            #line 145 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.Cardinality));
            
            #line default
            #line hidden
            this.Write(";i++)\r\n      {\r\n        mEncoding[i].setDataPointer(&mData.data()[i]);\r\n        s" +
                    "etElementPointer(i, &mEncoding[i]);\r\n      }\r\n");
            
            #line 150 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("    }\r\n    void set(const ");
            
            #line 152 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.CPPType));
            
            #line default
            #line hidden
            this.Write("& data) { memcpy(mData.data(), data.data(), mData.size()); }\r\n    const ");
            
            #line 153 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.CPPType));
            
            #line default
            #line hidden
            this.Write("& get() const { return mData; }\r\n  private:\r\n    ");
            
            #line 155 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.CPPType));
            
            #line default
            #line hidden
            this.Write(" mData;\r\n");
            
            #line 156 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
    if (arrayDataType.EncodingBase == "HLAfixedArray") { 
            
            #line default
            #line hidden
            this.Write("    ");
            
            #line 157 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.DataType.Encoding));
            
            #line default
            #line hidden
            this.Write(" mEncoding[");
            
            #line 157 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(arrayDataType.Cardinality));
            
            #line default
            #line hidden
            this.Write("];\r\n");
            
            #line 158 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("}; // class ");
            
            #line 159 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("\r\n\r\n");
            
            #line 161 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

  } // if (dataType is FOMParser.ArrayDataType)
  else // not handled
  {

            
            #line default
            #line hidden
            this.Write("// ");
            
            #line 166 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.GetType()));
            
            #line default
            #line hidden
            this.Write(" ");
            
            #line 166 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(dataType.Name));
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 167 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

  } // else not handled

            
            #line default
            #line hidden
            
            #line 170 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"

} // foreach (var dataType in FOM.DataTypes.Values)

            
            #line default
            #line hidden
            this.Write("} // namespace ");
            
            #line 173 "D:\vfs\CANoe\feature\14.0_DistSim_HLA\Projects_Source\OpenRTI\src\Tools\FOMCodeGen\EncodingHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(FOM.Namespace));
            
            #line default
            #line hidden
            this.Write("\r\n");
            return this.GenerationEnvironment.ToString();
        }
    }
    
    #line default
    #line hidden
    #region Base class
    /// <summary>
    /// Base class for this transformation
    /// </summary>
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "15.0.0.0")]
    public class EncodingHeaderBase
    {
        #region Fields
        private global::System.Text.StringBuilder generationEnvironmentField;
        private global::System.CodeDom.Compiler.CompilerErrorCollection errorsField;
        private global::System.Collections.Generic.List<int> indentLengthsField;
        private string currentIndentField = "";
        private bool endsWithNewline;
        private global::System.Collections.Generic.IDictionary<string, object> sessionField;
        #endregion
        #region Properties
        /// <summary>
        /// The string builder that generation-time code is using to assemble generated output
        /// </summary>
        protected System.Text.StringBuilder GenerationEnvironment
        {
            get
            {
                if ((this.generationEnvironmentField == null))
                {
                    this.generationEnvironmentField = new global::System.Text.StringBuilder();
                }
                return this.generationEnvironmentField;
            }
            set
            {
                this.generationEnvironmentField = value;
            }
        }
        /// <summary>
        /// The error collection for the generation process
        /// </summary>
        public System.CodeDom.Compiler.CompilerErrorCollection Errors
        {
            get
            {
                if ((this.errorsField == null))
                {
                    this.errorsField = new global::System.CodeDom.Compiler.CompilerErrorCollection();
                }
                return this.errorsField;
            }
        }
        /// <summary>
        /// A list of the lengths of each indent that was added with PushIndent
        /// </summary>
        private System.Collections.Generic.List<int> indentLengths
        {
            get
            {
                if ((this.indentLengthsField == null))
                {
                    this.indentLengthsField = new global::System.Collections.Generic.List<int>();
                }
                return this.indentLengthsField;
            }
        }
        /// <summary>
        /// Gets the current indent we use when adding lines to the output
        /// </summary>
        public string CurrentIndent
        {
            get
            {
                return this.currentIndentField;
            }
        }
        /// <summary>
        /// Current transformation session
        /// </summary>
        public virtual global::System.Collections.Generic.IDictionary<string, object> Session
        {
            get
            {
                return this.sessionField;
            }
            set
            {
                this.sessionField = value;
            }
        }
        #endregion
        #region Transform-time helpers
        /// <summary>
        /// Write text directly into the generated output
        /// </summary>
        public void Write(string textToAppend)
        {
            if (string.IsNullOrEmpty(textToAppend))
            {
                return;
            }
            // If we're starting off, or if the previous text ended with a newline,
            // we have to append the current indent first.
            if (((this.GenerationEnvironment.Length == 0) 
                        || this.endsWithNewline))
            {
                this.GenerationEnvironment.Append(this.currentIndentField);
                this.endsWithNewline = false;
            }
            // Check if the current text ends with a newline
            if (textToAppend.EndsWith(global::System.Environment.NewLine, global::System.StringComparison.CurrentCulture))
            {
                this.endsWithNewline = true;
            }
            // This is an optimization. If the current indent is "", then we don't have to do any
            // of the more complex stuff further down.
            if ((this.currentIndentField.Length == 0))
            {
                this.GenerationEnvironment.Append(textToAppend);
                return;
            }
            // Everywhere there is a newline in the text, add an indent after it
            textToAppend = textToAppend.Replace(global::System.Environment.NewLine, (global::System.Environment.NewLine + this.currentIndentField));
            // If the text ends with a newline, then we should strip off the indent added at the very end
            // because the appropriate indent will be added when the next time Write() is called
            if (this.endsWithNewline)
            {
                this.GenerationEnvironment.Append(textToAppend, 0, (textToAppend.Length - this.currentIndentField.Length));
            }
            else
            {
                this.GenerationEnvironment.Append(textToAppend);
            }
        }
        /// <summary>
        /// Write text directly into the generated output
        /// </summary>
        public void WriteLine(string textToAppend)
        {
            this.Write(textToAppend);
            this.GenerationEnvironment.AppendLine();
            this.endsWithNewline = true;
        }
        /// <summary>
        /// Write formatted text directly into the generated output
        /// </summary>
        public void Write(string format, params object[] args)
        {
            this.Write(string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
        }
        /// <summary>
        /// Write formatted text directly into the generated output
        /// </summary>
        public void WriteLine(string format, params object[] args)
        {
            this.WriteLine(string.Format(global::System.Globalization.CultureInfo.CurrentCulture, format, args));
        }
        /// <summary>
        /// Raise an error
        /// </summary>
        public void Error(string message)
        {
            System.CodeDom.Compiler.CompilerError error = new global::System.CodeDom.Compiler.CompilerError();
            error.ErrorText = message;
            this.Errors.Add(error);
        }
        /// <summary>
        /// Raise a warning
        /// </summary>
        public void Warning(string message)
        {
            System.CodeDom.Compiler.CompilerError error = new global::System.CodeDom.Compiler.CompilerError();
            error.ErrorText = message;
            error.IsWarning = true;
            this.Errors.Add(error);
        }
        /// <summary>
        /// Increase the indent
        /// </summary>
        public void PushIndent(string indent)
        {
            if ((indent == null))
            {
                throw new global::System.ArgumentNullException("indent");
            }
            this.currentIndentField = (this.currentIndentField + indent);
            this.indentLengths.Add(indent.Length);
        }
        /// <summary>
        /// Remove the last indent that was added with PushIndent
        /// </summary>
        public string PopIndent()
        {
            string returnValue = "";
            if ((this.indentLengths.Count > 0))
            {
                int indentLength = this.indentLengths[(this.indentLengths.Count - 1)];
                this.indentLengths.RemoveAt((this.indentLengths.Count - 1));
                if ((indentLength > 0))
                {
                    returnValue = this.currentIndentField.Substring((this.currentIndentField.Length - indentLength));
                    this.currentIndentField = this.currentIndentField.Remove((this.currentIndentField.Length - indentLength));
                }
            }
            return returnValue;
        }
        /// <summary>
        /// Remove any indentation
        /// </summary>
        public void ClearIndent()
        {
            this.indentLengths.Clear();
            this.currentIndentField = "";
        }
        #endregion
        #region ToString Helpers
        /// <summary>
        /// Utility class to produce culture-oriented representation of an object as a string.
        /// </summary>
        public class ToStringInstanceHelper
        {
            private System.IFormatProvider formatProviderField  = global::System.Globalization.CultureInfo.InvariantCulture;
            /// <summary>
            /// Gets or sets format provider to be used by ToStringWithCulture method.
            /// </summary>
            public System.IFormatProvider FormatProvider
            {
                get
                {
                    return this.formatProviderField ;
                }
                set
                {
                    if ((value != null))
                    {
                        this.formatProviderField  = value;
                    }
                }
            }
            /// <summary>
            /// This is called from the compile/run appdomain to convert objects within an expression block to a string
            /// </summary>
            public string ToStringWithCulture(object objectToConvert)
            {
                if ((objectToConvert == null))
                {
                    throw new global::System.ArgumentNullException("objectToConvert");
                }
                System.Type t = objectToConvert.GetType();
                System.Reflection.MethodInfo method = t.GetMethod("ToString", new System.Type[] {
                            typeof(System.IFormatProvider)});
                if ((method == null))
                {
                    return objectToConvert.ToString();
                }
                else
                {
                    return ((string)(method.Invoke(objectToConvert, new object[] {
                                this.formatProviderField })));
                }
            }
        }
        private ToStringInstanceHelper toStringHelperField = new ToStringInstanceHelper();
        /// <summary>
        /// Helper to produce culture-oriented representation of an object as a string
        /// </summary>
        public ToStringInstanceHelper ToStringHelper
        {
            get
            {
                return this.toStringHelperField;
            }
        }
        #endregion
    }
    #endregion
}
