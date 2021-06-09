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
    
    #line 1 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "15.0.0.0")]
    public partial class FOMObjectInterfacesHeader : FOMObjectInterfacesHeaderBase
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public virtual string TransformText()
        {
            this.Write("\r\n// Code automatically generated by T4 template from ");
            
            #line 7 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(FOM.Filename));
            
            #line default
            #line hidden
            this.Write(", do not edit\r\n#pragma once\r\n\r\n#include <vector>\r\n#include <functional>\r\n\r\n");
            
            #line 13 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(FOM.PredefinedTypeIncludes));
            
            #line default
            #line hidden
            this.Write("\r\n#include \"");
            
            #line 14 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(EncodingHeaderFilename));
            
            #line default
            #line hidden
            this.Write("\"\r\n\r\n");
            
            #line 16 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"

foreach (var namespacePart in FOM.Namespace)
{

            
            #line default
            #line hidden
            this.Write("namespace ");
            
            #line 20 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(namespacePart));
            
            #line default
            #line hidden
            this.Write(" {\r\n");
            
            #line 21 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 23 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 
foreach (var objectClass in FOM.ObjectClasses)
{

            
            #line default
            #line hidden
            this.Write("class I");
            
            #line 27 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write(";\r\nclass I");
            
            #line 28 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("ObjectClass\r\n{\r\n  public:\r\n    virtual void Publish() = 0;\r\n    virtual void Unpu" +
                    "blish() = 0;\r\n    virtual void Subscribe() = 0;\r\n    virtual void Unsubscribe() " +
                    "= 0;\r\n    virtual I");
            
            #line 35 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("* GetObjectInstance(const std::wstring& instanceName) = 0;\r\n    virtual I");
            
            #line 36 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("* CreateObjectInstance(const std::wstring& instanceName) = 0;\r\n};\r\n\r\nclass I");
            
            #line 39 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            
            #line 39 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"

  if (objectClass.BaseClass != null)
  {

            
            #line default
            #line hidden
            this.Write(" : public I");
            
            #line 42 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.BaseClass.Name));
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 43 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } else {

            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 46 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } // if 
            
            #line default
            #line hidden
            this.Write("{\r\n");
            
            #line 48 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 
  string attributeBitsClass = "uint32_t";
  string attributeBitsPostfix = "";
  if (objectClass.Attributes.Count > 32)
  {
    attributeBitsClass = "uint64_t";
    attributeBitsPostfix = "ull";
  }

            
            #line default
            #line hidden
            this.Write("  public:\r\n");
            
            #line 58 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 if (objectClass.BaseClass == null)  {
            
            #line default
            #line hidden
            this.Write("    using AttributeBits = ");
            
            #line 59 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attributeBitsClass));
            
            #line default
            #line hidden
            this.Write(";\r\n    static const AttributeBits kNone = 0x0;\r\n");
            
            #line 61 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } 
            
            #line default
            #line hidden
            
            #line 62 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 
  ulong bit;
  if (objectClass.BaseClass != null)
    bit = (ulong)(1 << objectClass.BaseClass.AllAttributes.Count);
  else
    bit = 1;
  foreach (var attribute in objectClass.Attributes)
  {
    if (attribute.DataType != null)
    { 
            
            #line default
            #line hidden
            this.Write("    static const AttributeBits k");
            
            #line 72 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attribute.Name));
            
            #line default
            #line hidden
            this.Write("Bit = 0x");
            
            #line 72 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(bit.ToString("X")));
            
            #line default
            #line hidden
            
            #line 72 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attributeBitsPostfix));
            
            #line default
            #line hidden
            this.Write(";\r\n");
            
            #line 73 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"

      bit = bit << 1;
    } // if (attribute.DataType != null)
  } // foreach (var attribute in objectClass.Attributes)

            
            #line default
            #line hidden
            this.Write("\r\n    I");
            
            #line 79 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("() {}\r\n");
            
            #line 80 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 if (objectClass.BaseClass == null)  {
            
            #line default
            #line hidden
            this.Write("    virtual ~I");
            
            #line 81 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("() {}\r\n");
            
            #line 82 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
  } else { 
            
            #line default
            #line hidden
            this.Write("    ~I");
            
            #line 83 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("() {}\r\n");
            
            #line 84 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("    I");
            
            #line 85 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("(const I");
            
            #line 85 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("&) = delete;\r\n    I");
            
            #line 86 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("(I");
            
            #line 86 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("&&) = delete;\r\n    I");
            
            #line 87 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("& operator=(const I");
            
            #line 87 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("&) = delete;\r\n    I");
            
            #line 88 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("& operator=(I");
            
            #line 88 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("&&) = delete;\r\n\r\n");
            
            #line 90 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 if (objectClass.BaseClass == null)  {
            
            #line default
            #line hidden
            this.Write("    virtual std::wstring GetObjectInstanceName() const = 0;\r\n");
            
            #line 92 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } 
            
            #line default
            #line hidden
            
            #line 93 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 
  foreach (var attribute in objectClass.Attributes)
  {
    if (attribute.DataType != null)
    { 
            
            #line default
            #line hidden
            this.Write("    // attribute ");
            
            #line 98 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attribute.Name));
            
            #line default
            #line hidden
            this.Write(" : ");
            
            #line 98 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attribute.DataType.Name));
            
            #line default
            #line hidden
            this.Write("\r\n    virtual ");
            
            #line 99 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attribute.DataType.ReturnCppType));
            
            #line default
            #line hidden
            this.Write(" Get");
            
            #line 99 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attribute.Name));
            
            #line default
            #line hidden
            this.Write("() const = 0;\r\n    virtual void Set");
            
            #line 100 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attribute.Name));
            
            #line default
            #line hidden
            this.Write("(");
            
            #line 100 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attribute.DataType.CPPType));
            
            #line default
            #line hidden
            this.Write(" newValue) = 0;\r\n");
            
            #line 101 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
  } else { 
            
            #line default
            #line hidden
            this.Write("    // attribute ");
            
            #line 102 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(attribute.Name));
            
            #line default
            #line hidden
            this.Write(" : no data type\r\n");
            
            #line 103 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
  
    } // if (attribute.DataType != null)
  } // foreach (var attribute in objectClass.Attributes)

            
            #line default
            #line hidden
            
            #line 107 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 if (objectClass.HasValidAttributes) { 
            
            #line default
            #line hidden
            
            #line 108 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 if (objectClass.BaseClass != null && !objectClass.BaseClass.HasValidAttributes) { 
            
            #line default
            #line hidden
            this.Write(@"    virtual void UpdateAllAttributeValues() = 0;
    virtual void UpdateAllAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual void UpdateModifiedAttributeValues() = 0;
    virtual void UpdateModifiedAttributeValues(const rti1516ev::LogicalTime& time) = 0;
    virtual AttributeBits GetUpdatedAttributes() const = 0;
");
            
            #line 114 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } 
            
            #line default
            #line hidden
            
            #line 115 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 if (objectClass.ChildClasses.Count == 0) { 
            
            #line default
            #line hidden
            this.Write("    using UpdateCallbackType = std::function<void(I");
            
            #line 116 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("*)>;\r\n    virtual uint32_t RegisterUpdateCallback(UpdateCallbackType callback) = " +
                    "0;\r\n    virtual void UnregisterUpdateCallback(uint32_t callbackToken) = 0;\r\n");
            
            #line 119 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } // if (objectClass.ChildClasses.Count == 0) 
            
            #line default
            #line hidden
            
            #line 120 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } // if (objectClass.HasValidAttributes) 
            
            #line default
            #line hidden
            this.Write("};\r\n\r\n");
            
            #line 123 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } // foreach (var objectClass in FOM.ObjectClasses) 
            
            #line default
            #line hidden
            this.Write("\r\nclass IObjectClassRegistry\r\n{\r\n  public:\r\n");
            
            #line 128 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 foreach (var objectClass in FOM.ObjectClasses) { 
            
            #line default
            #line hidden
            this.Write("    virtual I");
            
            #line 129 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("ObjectClass* get");
            
            #line 129 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(objectClass.Name));
            
            #line default
            #line hidden
            this.Write("ObjectClass() const = 0;\r\n");
            
            #line 130 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("  protected:\r\n    virtual ~IObjectClassRegistry() {}\r\n}; // class IObjectClassReg" +
                    "istry\r\n\r\nIObjectClassRegistry* GetClassRegistry();\r\n\r\n");
            
            #line 137 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"

foreach (var namespacePart in FOM.Namespace)
{

            
            #line default
            #line hidden
            this.Write("} // namespace ");
            
            #line 141 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(namespacePart));
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 142 "D:\vfs\OpenRTI-codegen\src\Tools\FOMCodeGen\FOMObjectInterfacesHeader.tt"
  } 
            
            #line default
            #line hidden
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
    public class FOMObjectInterfacesHeaderBase
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
