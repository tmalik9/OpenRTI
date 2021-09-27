# FOMCodeGen
## Introduction
To ease HLA application programming, a code generator (implemented using .NET/C#) has been added,
which will read a rti1516e FOM and generate C++ code for the following:
* encoder classes for all data types specified in the dataTypes section of the FOM xml.
* C++ interaction classes for all interaction classes specified, with automatic conversions between C++ types and parameters
* C++ object classes and object instance classes for the object classes specified, with automatic conversion between C++ types and attributes,
  automatic attribute update/request/provide and object create/discover/remove mechanisms
Generally, the FOM must be **fully specified**, for the code generator to run correctly. That is, all interaction parameters, object attributes, record fields etc. must use data types which are actually defined somewhere in the FOM, or be one of the predefined HLA types.

## Command line
The command line is as follows:
FOMCodeGen \<XML-File\> namespace [options]
where the namespace is a dot-separated chain of namespace names, which will surround all generated code. For instance,
if MyFederation.Encodings is specified, all generated code will be surrounded by 
```
namespace MyFederation {
namespace Encodings {
  ... generated code ...
} // namespace Encodings
} // namespace MyFederation
```
Additional parameters include:

\-p: include a line '#include "stdafx.h"' in the generated .cpp files  

\-d: 'dry run', execute everything, but do not write output files. Can be used for FOM consistency checking  

\-o \<dir\>: specify an output directory, other than the current directory  

\-oi \<dir\>: specify an output directory for the generated interface header files, deviating from the output directory.  

\-I \<dir\>: specify a compiler include directory. This option may be specified several times.  
          This is used in conjunction with -o and -oi, to help the code generator produce the appropriate   
		  relative #include paths in the implementation and header files.  
		  The same list of include directories present on the compiler command line should be specified to the code generator,  
		  such that the generated code need not be adapted manually to match the build settings.  
		  
## Output
Each \<FOM\>.xml will produce 9 files:
* \<FOM\>DataTypes.h: Some common classes, and the interface classes for all data type definitions/encodings.
* \<FOM\>Encodings.h: The declarations of the encodings of all data type definitions
* \<FOM\>Encodings.cpp: Their respective implementations

* \<FOM\>InteractionInterfaces.h: Interface classes for all interaction classes contained in the FOM
* \<FOM\>Interactions.h: The implementation classes for the interaction classes
* \<FOM\>Interactions.cpp: The actual implementations of the interaction classes

* \<FOM\>ObjectInterfaces.h: Interface classes for all the object class classes and the object instance classes
* \<FOM\>Objects.h: Their respective implementation classes
* \<FOM\>Objects.cpp: The actual implementations of the object class classes and the object instance classes

The first three files contain definitions from the FOM's dataTypes section, the next 3 files contain the 
implementation of the interaction classes, the last 3 files the object classes.
The '*DataTypes.h/*Interfaces.h' header files declare OpenRTI-independent C++ interfaces for the classes defined in the other files.

## Class registries
Both the interaction classes and the object classes are managed by a respective class registry.
The application must instantiate both class registry, and route most RTI callbacks into the class registries.
Application code can then access the interaction and object classes through the class registries, 
and use these to register callbacks, create objects or send interactions.

## Type Mappings
### Encoders
The main purpose of the generated encoder classes (and their respective interfaces) is, where possible,
to directly map HLA data types to their matching native C++ types, while providing the encode/decode
functions required by the underlying HLA infrastructure.

### Simple and builtin types
|HLA type(s)                    | C++ native type|
|:------------------------------|:---------------|
|HLAboolean                     |bool|
|HLAbyte, HLAoctet              |uint8_t|
|HLAinteger16LE, HLAinteger16BE |int16_t|
|HLAinteger32LE, HLAinteger32BE |int32_t|
|HLAinteger64LE, HLAinteger64BE |int64_t|
|HLAasciiString                 |std::string|
|HLAunicodeString               |std::wstring|
|HLAopaqueData                  |std::vector<uint8_t>|
|HLAhandle                      |rti1516ev::HLAhandle|
|HLAobjectInstanceHandle        |rti1516ev::HLAobjectInstanceHandle|
|HLAobjectInstanceHandle(with appropriate object class attribute)|object instance|

### Enumerations
For enumeration types, the \<FOM\>DataTypes.h will just include the C++ definition of the enumeration type.
The \<FOM\>Encodings.h/.cpp will include an encoding class derived from the basic type encoding 
declared as 'representation' of the enumeration in the FOM. The generated encoding class will accept enumeration
values directly, without casting required (as opposed to the basic type encodings)

### Arrays
While arrays will be mapped to a subclass of rti1516ev::HLAfixedArray or rti1516ev::HLAvariableArray,
the contents can be assigned from and retrieved as std::vector\<native C++ element type\>.

### Fixed Records
Fixed record encoders derive from rti1516ev::HLAfixedRecord. The encoder includes all the declared fields as encoder objects,
which are added 'by reference' to the HLAfixedRecord. Access to the fields is provided through getter/setter methods, 
which provide/accept the mapped native C++ type.

### Interface header file
The \<FOM\>DataTypes.h includes interface classes for all generated array and fixed record encoder types.
Basically, the generated interface header file should not need to include any RTI header files,
such that these interfaces may be used by application code not aware of the underlying runtime infrastructure.
Exceptions from this is, if the FOM includes HLAhandle as field (or otherwise referenced) data type.
In that case, the appropriate OpenRTI header files must be added to \<FOM\>DataTypes.h.
 
The header file \<FOM\>DataTypes.h also carries a definition of a template class 'optional', which is a lighweight version of std::optional.
It is included there because std::optional only exists since C++17, whereas the generated code is meant to be used with C++11 upwards.

Additionally, some exception classes are included, which map to some OpenRTI exceptions (which in turn shall not be directly thrown to the 
application).


### Interaction classes
Interaction classes will be turned into straight forward C++ classes, having methods publish/subscribe, sending and registering callbacks.
All generated send and receive methods will have parameter lists matching the lists of parameters defined in the FOM, with
appropriate C++ parameter types. The send method of the interaction class has the same argument list,
including all parameters of the interaction class as optional arguments.


### Object classes
HLA object classes will turn into ObjectClass classes and ObjectInstance classes.
The ObjectClass is reponsible for managing the respective object instances, and publish/subscribe.
Object instances can be discovered by the ObjectClass, or be created by the application.
All object attributes will be turned into Get/Set attribute methods in the ObjectInstance class.

Additionally a bit field is created, each bit representing one of the attributes. The object instance
manages information about which attributes have been initialized or modified, 
or ever received or updated (by the recent reflectAttributeValues callback).

Object instances can directly be used as interaction parameters, or as attribute values,
if the respective entity is of type HLAobjectInstanceHandle **and** a non-standard tag 'class' is used:

\<attribute\>\<name\>Sender\</name\>\<dataType class="FlexRayController"\>HLAobjectInstanceHandle\</dataType\>\</attribute\>

or

\<parameter\>\<name\>Sender\</name\>\<dataType class="BusController"\>HLAobjectInstanceHandle\</dataType\>\</parameter\>

## Callbacks
### Objects
Object classes can deliver callbacks for object discovery and removal to the application. Additionally, the object class's 
CreateObjectinstance method can supply a callback to the application, when the created object has been successfully registered in the HLA RTI.

Object instances can supply callbacks on attribute update.

### Interactions
Interaction classes can deliver callbacks on reception of interactions. All interaction parameters are mapped to positional,
optional arguments to the callback function. The argument order is the same as the definition order in the FOM (including 
the parameters in the interaction base classes).


