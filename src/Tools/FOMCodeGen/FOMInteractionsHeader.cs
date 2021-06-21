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
    
    #line 1 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
    [global::System.CodeDom.Compiler.GeneratedCodeAttribute("Microsoft.VisualStudio.TextTemplating", "15.0.0.0")]
    public partial class FOMInteractionsHeader : FOMInteractionsHeaderBase
    {
#line hidden
        /// <summary>
        /// Create the template output
        /// </summary>
        public virtual string TransformText()
        {
            this.Write("\r\n// Code automatically generated by T4 template from ");
            
            #line 7 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(System.IO.Path.GetFileName(FOM.Filename)));
            
            #line default
            #line hidden
            this.Write(@", do not edit
#pragma once

#include <vector>
#include ""RTI/Handle.h""
#include ""RTI/RTIambassador.h""
#include ""RTI/encoding/BasicDataElements.h""
#include ""RTI/encoding/HLAfixedRecord.h""
#include ""RTI/encoding/HLAfixedArray.h""
#include ""RTI/encoding/HLAvariableArray.h""
");
            
            #line 17 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(FOM.PredefinedTypeIncludes));
            
            #line default
            #line hidden
            this.Write("\r\n#include \"");
            
            #line 18 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(InteractionInterfacesHeaderFilename));
            
            #line default
            #line hidden
            this.Write("\"\r\n#include \"");
            
            #line 19 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(EncodingHeaderFilename));
            
            #line default
            #line hidden
            this.Write("\"\r\n\r\n");
            
            #line 21 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 foreach (var namespacePart in FOM.Namespace) { 
            
            #line default
            #line hidden
            this.Write("namespace ");
            
            #line 22 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(namespacePart));
            
            #line default
            #line hidden
            this.Write(" {\r\n");
            
            #line 23 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 25 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 foreach (var interactionClass in FOM.InteractionClasses) { 
            
            #line default
            #line hidden
            this.Write("class ");
            
            #line 26 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write(";\r\nclass ");
            
            #line 27 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass : public I");
            
            #line 27 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass\r\n{\r\n  public:\r\n    // I");
            
            #line 30 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass\r\n    ");
            
            #line 31 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass() = default;\r\n    virtual ~");
            
            #line 32 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass() = default;\r\n    void Publish() override;\r\n    void Unpublish()" +
                    " override;\r\n    void Subscribe() override;\r\n    void Unsubscribe() override;\r\n  " +
                    "  void send(");
            
            #line 37 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.CppParameterList));
            
            #line default
            #line hidden
            this.Write(") override;\r\n    void ReceiveInteraction(const rti1516ev::ParameterHandleValueMap" +
                    " & parameters);\r\n    uint32_t RegisterReceiveCallback(ReceiveCallback callback) " +
                    "override;\r\n");
            
            #line 40 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
   if (interactionClass.Order == FOMParser.Order.Timestamp) { 
            
            #line default
            #line hidden
            this.Write("    void sendWithTime(");
            
            #line 41 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.CppParameterList));
            
            #line default
            #line hidden
            
            #line 41 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 if (interactionClass.Parameters.Count > 0) {
            
            #line default
            #line hidden
            this.Write(", ");
            
            #line 41 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
}
            
            #line default
            #line hidden
            
            #line 41 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(FOM.NativeTimeType));
            
            #line default
            #line hidden
            this.Write(@" time) override;
    void ReceiveInteraction(const rti1516ev::ParameterHandleValueMap & parameters, const rti1516ev::LogicalTime& time, rti1516ev::OrderType receivedOrder);
    uint32_t RegisterReceiveCallbackWithTime(ReceiveCallbackWithTime callback) override;
");
            
            #line 44 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
   } // if (interactionClass.Order == FOM.Order.Timestamp) 
            
            #line default
            #line hidden
            this.Write("\r\n    // internal\r\n    rti1516ev::InteractionClassHandle GetInteractionClassHandl" +
                    "e() const { return mInteractionClassHandle; }\r\n");
            
            #line 48 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
   if (interactionClass.BaseClass != null) { 
            
            #line default
            #line hidden
            this.Write("    ");
            
            #line 49 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass(rti1516ev::RTIambassador* rtiAmbassador, ");
            
            #line 49 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.BaseClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass* baseClass);\r\n");
            
            #line 50 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
   } else {
            
            #line default
            #line hidden
            this.Write("    ");
            
            #line 51 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass(rti1516ev::RTIambassador* rtiAmbassador);\r\n");
            
            #line 52 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
   } // if (interactionClass.BaseClass != null) 
            
            #line default
            #line hidden
            
            #line 53 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 
  var baseClass = interactionClass.BaseClass;
  while (baseClass != null)
  {
    foreach (var parameter in baseClass.Parameters)
    {
      if (parameter.DataType != null)
      { 
            
            #line default
            #line hidden
            this.Write("    // parameter ");
            
            #line 61 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write(" : ");
            
            #line 61 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.DataType.Name));
            
            #line default
            #line hidden
            this.Write("\r\n    rti1516ev::ParameterHandle Get");
            
            #line 62 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write("ParameterHandle() const { return mBaseClass->Get");
            
            #line 62 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write("ParameterHandle(); }\r\n");
            
            #line 63 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
    } else { 
            
            #line default
            #line hidden
            this.Write("    // parameter ");
            
            #line 64 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write(" : no data type\r\n");
            
            #line 65 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"

      } // if (parameter.DataType != null)
    } // foreach (var parameter in interactionClass.Parameters)
    baseClass = baseClass.BaseClass;
  } // while (baseClass != null)

            
            #line default
            #line hidden
            
            #line 71 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 
  foreach (var parameter in interactionClass.Parameters)
  {
    if (parameter.DataType != null)
    { 
            
            #line default
            #line hidden
            this.Write("    // parameter ");
            
            #line 76 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write(" : ");
            
            #line 76 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.DataType.Name));
            
            #line default
            #line hidden
            this.Write("\r\n    rti1516ev::ParameterHandle Get");
            
            #line 77 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write("ParameterHandle() const { return m");
            
            #line 77 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write("ParameterHandle; }\r\n");
            
            #line 78 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
  } else { 
            
            #line default
            #line hidden
            this.Write("    // parameter ");
            
            #line 79 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write(" : no data type\r\n");
            
            #line 80 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
  
    } // if (parameter.DataType != null)
  } // foreach (var parameter in interactionClass.Parameters)

            
            #line default
            #line hidden
            this.Write("    rti1516ev::ParameterHandleSet GetAllParameterHandles();\r\n  private:\r\n    rti1" +
                    "516ev::RTIambassador* mRtiAmbassador;\r\n    // interaction class handle\r\n    rti1" +
                    "516ev::InteractionClassHandle mInteractionClassHandle;\r\n");
            
            #line 89 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 if (interactionClass.BaseClass != null) { 
            
            #line default
            #line hidden
            this.Write("    ");
            
            #line 90 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.BaseClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass* mBaseClass;\r\n");
            
            #line 91 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("    bool mPublished = false;\r\n    bool mSubscribed = false;\r\n    // Parameter han" +
                    "dles\r\n");
            
            #line 95 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 
  foreach (var parameter in interactionClass.Parameters)
  {
    if (parameter.DataType != null)
    {

            
            #line default
            #line hidden
            this.Write("    // parameter ");
            
            #line 100 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write(" : ");
            
            #line 100 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.DataType.Name));
            
            #line default
            #line hidden
            this.Write("\r\n    rti1516ev::ParameterHandle m");
            
            #line 101 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write("ParameterHandle;\r\n");
            
            #line 102 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
  } else {

            
            #line default
            #line hidden
            this.Write("    // parameter ");
            
            #line 103 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(parameter.Name));
            
            #line default
            #line hidden
            this.Write(" : no data type\r\n");
            
            #line 104 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
  
    } // if (parameter.DataType != null)
  } // foreach (var parameter in interactionClass.Parameters)

            
            #line default
            #line hidden
            this.Write("    std::map<uint32_t, ReceiveCallback> _receiveCallbacks;\r\n    uint32_t _receive" +
                    "CallbacksNextKey = 0;\r\n");
            
            #line 110 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
   if (interactionClass.Order == FOMParser.Order.Timestamp) { 
            
            #line default
            #line hidden
            this.Write("    std::map<uint32_t, ReceiveCallbackWithTime> _receiveCallbacksWithTime;\r\n    u" +
                    "int32_t _receiveCallbacksWithTimeNextKey = 0;\r\n");
            
            #line 113 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
   } // if (interactionClass.Order == FOM.Order.Timestamp) 
            
            #line default
            #line hidden
            this.Write("};\r\n\r\n\r\n");
            
            #line 117 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 } // foreach (var interactionClass in FOM.InteractionClasses) 
            
            #line default
            #line hidden
            this.Write(@" 

class InteractionClassRegistry : public IInteractionClassRegistry
{
  public:
    InteractionClassRegistry();
    ~InteractionClassRegistry();
    void Initialize(rti1516ev::RTIambassador* rtiAmbassador);
    static InteractionClassRegistry* GetInstance() { return sClassRegistry; }

");
            
            #line 127 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 foreach (var interactionClass in FOM.InteractionClasses) { 
            
            #line default
            #line hidden
            this.Write("    I");
            
            #line 128 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass* Get");
            
            #line 128 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass() const override { return m");
            
            #line 128 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass.get(); }\r\n");
            
            #line 129 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("\r\n    void ReceiveInteraction(rti1516ev::InteractionClassHandle theInteraction, c" +
                    "onst rti1516ev::ParameterHandleValueMap & parameters);\r\n\r\n  private:\r\n    static" +
                    " InteractionClassRegistry* sClassRegistry;\r\n    rti1516ev::RTIambassador* mRtiAm" +
                    "bassador;\r\n");
            
            #line 136 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 foreach (var interactionClass in FOM.InteractionClasses) { 
            
            #line default
            #line hidden
            this.Write("    std::unique_ptr<");
            
            #line 137 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass> m");
            
            #line 137 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(interactionClass.Name));
            
            #line default
            #line hidden
            this.Write("InteractionClass;\r\n");
            
            #line 138 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
 } 
            
            #line default
            #line hidden
            this.Write("}; // class InteractionClassRegistry\r\n\r\n");
            
            #line 141 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"

foreach (var namespacePart in FOM.Namespace)
{

            
            #line default
            #line hidden
            this.Write("} // namespace ");
            
            #line 145 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
            this.Write(this.ToStringHelper.ToStringWithCulture(namespacePart));
            
            #line default
            #line hidden
            this.Write("\r\n");
            
            #line 146 "D:\vfs\CANoe\release\15.2\Projects_Source\OpenRTI-master\src\Tools\FOMCodeGen\FOMInteractionsHeader.tt"
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
    public class FOMInteractionsHeaderBase
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
