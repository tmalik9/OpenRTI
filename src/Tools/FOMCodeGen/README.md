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
FOMCodeGen <XML-File> namespace [options]
where the namespace is a dot-separated chain of namespace names, which will surround all generated code. For instance,
if MyFederation.Encodings is specified, all generated code will be surrounded by 
namespace MyFederation {
namespace Encodings { 
} // namespace Encodings
} // namespace MyFederation

Additional parameters include:
-p: include a line '#include "stdafx.h"' in the generated .cpp files  
-d: 'dry run', execute everything, but do not write output files. Can be used for FOM consistency checking  
-o <dir>: specify an output directory, other than the current directory  
-oi <dir>: specify an output directory for the generated interface header files, deviating from the output directory.  
-I <dir>: specify a compiler include directory. This option may be specified several times.  
          This is used in conjunction with -o and -oi, to help the code generator produce the appropriate   
		  relative #include paths in the implementation and header files.  
		  The same list of include directories present on the compiler command line should be specified to the code generator,  
		  such that the generated code need not be adapted manually to match the build settings.  
		  
## Output
Each <FOM>.xml will produce 9 files:
* <FOM>DataTypes.h: Some common classes, and the interface classes for all data type definitions/encodings.
* <FOM>Encodings.h: The declarations of the encodings of all data type definitions
* <FOM>Encodings.cpp: Their respective implementations
* <FOM>InteractionInterfaces.h: Interface classes for all interaction classes contained in the FOM
* <FOM>Interactions.h: The implementation classes for the interaction classes
* <FOM>Interactions.cpp: The actual implementations of the interaction classes
* <FOM>ObjectInterfaces.h: Interface classes for all the object class classes and the object instance classes
* <FOM>Objects.h: Their respective implementation classes
* <FOM>Objects.cpp: The actual implementations of the object class classes and the object instance classes

The main purpose of the generated encoder classes (and their respective interfaces) is, where possible,
to directly map native C++ types to their respective HLA data types. For instance, a record field defined as HLAinteger32LE will 
be accessible as an int32_t. An enumeration with encoding HLAinteger32LE will be declared in the interface header,
and directly assignable to other entities using that type.
Arrays will be assignable from std::vector<native C++ element type>, and so on.
Basically, the generated interface header files will not need to include any RTI header files,
such that these interfaces may be used by application code not aware of the underlying runtime infrastructure.

Interaction classes will be turned into straight forward C++ classes, having methods publish/subscribe, sending and registering callbacks.
All generated send and receive methods will have parameter lists matching the lists of parameters defined in the FOM, with
appropriate C++ parameter types.

HLA object classes will turn into ObjectClass classes and ObjectInstance classes.
The ObjectClass is reponsible for managing the respective object instances, and publish/subscribe.
Object instances can be discovered by the ObjectClass, or be created by the application.
All object attributes will be turned into Get/Set attribute methods in the ObjectInstance class.

Additionally a bit field is created, each bit representing one of the attributes. The object instance
manages information about which attributes have been initialized or modified, 
or ever received or updated (by the recent reflectAttributeValues callback).

Object instances can directly be used as interaction parameters, or as attribute values,
if the respective entity is of type HLAobjectInstanceHandle **and** a non-standard tag 'class' is used:
<attribute><name>Sender</name><dataType class="FlexRayController">HLAobjectInstanceHandle</dataType></attribute>
or
<parameter><name>Sender</name><dataType class="BusController">HLAobjectInstanceHandle</dataType></parameter>

## Class registries
Both the interaction classes and the object classes are managed by a respective class registry.
The application must instantiate both class registry, and route most RTI callbacks into the class registries.
Application code can then access the interaction and object classes through the class registries, 
and use these to register callbacks, create objects or send interactions.
