# OpenRTI

This is a fork of OpenRTI used and enhanced by Vector Informatik GmbH.
For the original README, written by the original author Matthias Froehlich, see 'Original README', below.
All credits for imeplementing and maintaining the original OpenRTI framework go to him.

The compatibility to HLA13 and rti1516, and their respective client libraries, have been removed from this fork.
Instead, a superset of the functionality given by rti1516e has been created, using the namespace rti1516ev (as 'enhanced by vector').

The following features *from the rti1516e HLA standard* have been complemented:
* partial MOM/MIM implementation, with the basic federation/federate objects, and information request/report interactions.
* several additions to the data encoding classes
* support for data type definitions in the FOM
* full thread-safety of the RTI ambassador
* HLA_IMMEDIATE mode (callbacks are delivered automatically by a RTI-owned thread)

The following features *not present in the rti1516e HLA standard* have been added:
* pass a notification handle to the RTI ambassador, to be signalled when callbacks are queued
* unsubscribe from object instance updates
* subscribe to interactions with parameter value filters
* 'self-delivery' of interactions and attribute updates: ability to queue callbacks for interactions and attribute updates sent by the same federate
* ability to set timeouts for connecting and some intialization operations
* 'debugger-friendly' timeout handling: given timeouts will not run down while the application is halted

To ease HLA application programming, a code generator (implemented using .NET/C#) has been added,
which will read a rti1516e FOM and generate C++ code for the following:
* encoder classes for all data types specified in the dataTypes section of the FOM xml.
* C++ interaction classes for all interaction classes specified, with automatic conversions between C++ types and parameters
* C++ object classes and object instance classes for the object classes specified, with automatic conversion between C++ types and attributes,
  automatic attribute update/request/provide and object create/discover/remove mechanisms
Generally, the FOM must be **fully specified**, for the code generator to run correctly.

# Original README:

OpenRTI
-------

An rti library with an rti1516, rti1516e as well as a RTI13 interface.

The key features are:
* Easy to use.
* C++ interface.
* (Java interface. No coding resources allocated yet ... hint)
* Extensible.
* In process RTI.
* Server tree hierarchy.
* Shortest path is in memory without copying data.
* Only depends on a c++11 compiler/stl or newer.


Installation
------------

It's a usual cmake project.

Execute cmake from within a build directory of your choice, give this
cmake the source directory, generate build files for your native build
environment and compile the set of libraries/tests.

Specifically under a UNIX like environment:

cd <builddir>
cmake -DCMAKE_INSTALL_PREFIX=<prefix> <srcdir>
make
make test
make install

Specifically under windows using cl:

cd <builddir>
cmake -DCMAKE_INSTALL_PREFIX=<prefix> <srcdir>
cmake --build . --config Release
ctest --build-config Release
cmake --build . --config Release --target INSTALL

Or depending on your c++ compile environment, you may need to take
different steps. For details refer to www.cmake.org.


Dependencies
------------

The 'dependency only on a c++ compiler' in the key features table
means more specifically that:
The rti libraries and all tool programs like the server process rtinode
that are required to run a rti, the client rti libraries as well as
the complete test suite depends on nothing but a working c++ compiler
and a usually included stl implementation.
This aims to ease the installation and should keep potential collisions
with respect to run time libraries possible required by the calling
application at a bare minimum.
OpenRTI requires at least an c++11 implementation, but can be compiled
also with any c++ compiler and stl library with a newer standard.
NOTE: for this version of OpenRTI c++98 will no longer be sufficient.

OpenRTI can make use of additional components like zlib for on the wire
compression, that is used if it is available. OpenRTI's on the wire
protocol negotiates if this is used or not. So, mixing different builds
one with zlib and one without shall interact seamlessly.
The python binding is obviously only available if python is available.

That said, much more specifically, there is no dependency on boost
as of today and there will not be any in the future. The same goes for
any other dependency library that is not strictly needed.


C++ language standard version
-----------------------------

C++11/14 Should just work.
Starting with cmake 3.1 you can enable use of an other standard version
conveniently in the build system by setting the CMAKE_CXX_STANDARD cmake
variable to an appropriate value.
For example switching to c++11 can be achieved via adding
the -DCMAKE_CXX_STANDARD=11 command line option to the initial
cmake command.
Consult the cmake documentation for more information on setting cmake
variables.


Platforms
---------

Everywhere!
Seriously, if it does not work yet on your platform, please
provide patches to make it work.

Development is done currently on Linux, Win32, MacOS and Solaris.
HP-UX did work at some time in the past but is no longer tested.

Linux is tested on current fedora variants and centos 4, 5 and 6.

Windows is tested so far with 32 and 64 bits on XP with VS 2010/VS2012.

Macos X is fully supported starting with 10.6. Starting with this
version a workaround for a problem originating from the
standardized cyclic link dependency is available. On earlier macos
versions OpenRTI triggers a race in resolving dynamic symbols from
the logical time libraries. This problems shows up in a lot of the
tests for OpenRTI. For single threaded HLA applications or applications
having only a single thread accessing the RTIambassador this problem
should not show up even on macos versions prior to 10.6.

On Solaris, OpenRTI is tested with
CC='cc -mt' CXX='CC -mt' cmake ...

On HP-UX, OpenRTI is tested with
CC='cc -Aa -mt' CXX='aCC -AA -mt' cmake ...


Supported RTI Features
----------------------

In general OpenRTI supports HLA1.3, rti1516, rti1516e interfaces.
The implementation among them is on the same level - according
to the features available on the appropriate level.
The basic transport is currently tcp/ip based so everything
is basically delivered reliable. Object Model Modules are
supported for rti1516e. Update rate reduction is not implemented.

* Federation Management:
  Connect/Disconnect is available.
  Create/Destroy including Modules is available.
  Join/Resign including Modules is available.
  List is available.
  Synchronization Points are available.
  Save/Restore is not available.
  Connection Lost is available (1).
* Declaration Management
  Publish/Subscribe for Object Class Attributes is available.
  Publish/Subscribe for Interaction Classes is available.
  Start/Stop Registration for Object Class is called whenever published (2).
  Turn On/Off Interactions is called whenever published (2).
* Object Management
  Reserve/Release (Multiple) Object Instance Name is available.
  Register/Discover/Delete/Remove Object Instance is available.
  Update/Reflect Attribute Values is avaiable.
  Send/Receive Interaction is available.
  Local Delete Object Instance is missing.
  Attributes In/out Of Scope is missing.
  Request/Provide Attribute value update is available.
  Turn Updates On/Off is missing.
  Request Change/Query/Report Attribute Transportation is missing.
  Request Change/Query/Report Interaction Transportation is missing.
* Ownership Management
  Is missing.
* Time Management
  Is available with the exception of message retraction.
* Data Distribution Management
  Is missing.
* Support Services
  Are available.
* Management Object Model
  Is present with the object model but does not provide any data or methods.

(1) The general behavior of OpenRTI on lost connections does not
    conform to the standard. But the cases where OpenRTI recognizes
    a lost connection in the sense of the standard it is reported by
    the Connection Lost service.
(2) Rather than tracking subsriptions the service is only called
    once the current rti ambassador published itself. This helps
    for applications that rely on the callbacks being called but
    does not help for reducing the load on the application.



Operation
---------

OpenRTI can operate in several ways:

The most important one is probably the client/server scenario:
Have one server process and some federates in several processes on
some machines in a network.

Often it would be also good to exploit some locality in an RTI. To
get this, an OpenRTI server can operate as slave servers under a parent
server. That means that every server in that tree seed all the federations
in that server tree but messages are only routed to servers that need
these messages. So having a sub server on a local subnet, traffic just
interesting for federates connected to that slave server originating from
federates connected to that slave server will never be sent to the
parent server.

But OpenRTI also can work without any external server running. When all
federates are living in one program - may be in different threads, messages
are just exchanged in that program. While this is probably the fastest
operation mode it is also the one that is the easiest to set up.


Connecting to an rti
--------------------

As of this writing, there are three different c++ bindings available
and implemented. All of them have different ways to connect, to an rti
create a federation execution and finally join a federation execution.
With OpenRTI all of them share a common backend implementation
including the means of communication.
Below a general description followed by the way how the possible
connections are encoded into the api calls.


Protocols
---------

To select between the different modes of operation above, OpenRTI
knows some protocols. This is the basic way of the transport used to
communicate.

rti:
 This is the preferred tcp/ip protocol variant for networked
 communication. This is the default with a default hostname of localhost.

thread:
 Just use the in memory communication between threads within a single
 application. Requires no external server setup and is extremely fast.

rtic:
 Like rti above but compresses the data stream if available on both transport
 ends. Compression needs extra cpu cycles and introduces some latency spent in
 compression of the transfered messages. Nevertheless if your available
 bandwidth is limited, this can help getting the data over the line.
 This url is accepted even if compression is not available on one or both
 of the communication ends. In such a case the data is not compressed before
 it gets sent over the wire.

http: (currently just an idea)
 This is a less efficient variant of the rti protocol for networked
 communication. The trick is that his kind of connection uses standard
 http headers and encapsulates the rti messages into that. It should be
 possible to bypass firewalls with that protocol. Not yet sure if this
 idea also helps for proxies. Note that this does not use the CONNECT
 proxy method but really uses native http messages. Which simplifies
 configuration a lot.

pipe:
 Uses the rti binary protocol on a named pipe. Can be used for machine
 local communication in presence of tight packet filters.

rtinode: (experimental, only available in the ambassador)
 A meta protocol to be able to run an rtinode just from inside of any
 ambassador. The protocol uses url query key value pairs to configure
 this rtinode like server. Query keys understood are:
  config=<config file name>
   to provide a configuration file for rtinode
  listen=<listen url>
   to provide a listen url for rtinode
  parent=<parent url>
   to provide a parent url for rtinode
 Note that you may need to escape the queries properly if they contain
 characters that make the url parsing ambiguous else.

 A simple example url setting up an rtinode on localhost on the default
 port reads "rtinode:///FederationExecutionName?listen=rti:///".

The implementation is prepared to some degree to move protocols into a
user provided shared library that could be loaded at runtime.


Federation execution names and URLs
-----------------------------------

For rti1516 it is easy to set the connect parameters in the string list.
But the RTI13 layer does not have this.
So, OpenRTI interprets federation execution names as urls:

 <protocol>://<address>/<path>/<name>

Where the protocol overrides the protocol field from the string list.
The address is the network address of the server. The path contains the
path to the named pipe if protocol is pipe and the name is just the plain
name of the federation execution.

This method does clearly break the rti1516 standard as you can connect to the
same federation execution by different federation execution names depending
on your server setup. But anyway I think that this is a useful idea to address
different servers without relying on environment variables.

So, for the standard conformance question we will need some override somewhere
in the future to disable interpretation of federation execution names as urls.


Connecting to an rti with RTI13
-------------------------------

This oldest binding does not have specific connect calls or arguments
to any factory creating an RTI::RTIambassador.
By that the RTI13 binding accepts the above described urls in any of
calls refering to a federation execution.
For example

  ambassador.joinFederationExecution("rti://myserver:3324/JustMine")

will try to connect to myserver on port 3324 with the rti protocol and
join the Federation 'JustMine' there.


Connecting to an rti with rti1516
---------------------------------

For rti1516 we have that string vector that is given to the ambassador
when it is created. This vector can be used to configure the protocol
that is used to communicate. The strings might contain key value pairs
like <key>=<value>. Supported keys are:

protocol=<p>
  'thread' (default) use the smp, in memory connect.
  'rti' use the tcp/ip binary protocol on some custom port
  'pipe' use the binary protocol on a named pipe

address=<a>
  The interpretation depends on the protocol:
  For 'thread' this is ignored.
  For 'rti' this contains the host:port pair.
  port can be omitted to use the appropriate default port.
  For 'pipe' this should contain the path to the named pipe.

host=<a>
hostname=<a>
  Like address, but only sets the host name or ip address part.

port=<p>
service=<p>
  Like address, but only sets the port number or service name part.

timeout=<t>
  Sets initial packet timeouts for messages that happen synchronous.
  That is mostly for create, destroy, join and resign.

url=<u>
  You can directly use the url scheme as described above and as it
  is used with rti1516e in the arguments to rti1516.


Connecting to an rti with rti1516e
----------------------------------

The most recent rti1516e binding has a connect call which
takes an url as described above to connect to an rti.
The createFederationExecution and joinFederationExecution service
finally takes the federation execution names as mandated by the standard.


Server
------

The standalone server application is called 'rtinode'.
The default operation of rtinode is to listen and respond on any ip adress
at port 14321 for incomming rti connections.
Accepted command line options are:

  -b
     Run in the background.

  -c <configuration file>
     Read the server configuraton from the given configuration file.

  -f <socket file>
     Start up a server only listening on the provided unix socket file
     name. This corresponds to a pipe:///<socket file>/<FederationName>
     url at ambassador connect time.
     The option is only available on unix like operating systems.

  -i <inet address>
     Start up a server only listening on the provided tcp/ip address.
     Valid adresses are address:port pairs like you find in a lot of
     ip based server configurations.
     This corresponds to a rti://address[:port]/<FederationName> url at
     ambassador connect time.
     Provided with an url used for connecting to a server node, the
     protocol/address/service is extracted from that and a listening server
     set up accordingly.

     To determine the tcp/ip address to bind the server, the
     address/service provided is split into a host/service pair
     separated by the last double colon that is found in the address and
     this pair is entered into the getaddrinfo libc call. The first
     working address is used to put up a listening socket for rtinode.
     
     That means that the exact address that is used to listen on depends
     on your systems behavior and name resolution setup. This is expecially
     true for the selection of ipv4/ipv6 and the resolution of names
     provided with the host or service part of the above argument.
     
     Nevertheless, a set of commonly working examples are:
       '*:1234' to listen on any address on port 1234.
       '[::]:14536' to listen on the any ipv6 address on port 14536.
       'localhost' to restrict listening to the ip address that is
          configured for localhost. The result depends on what your
          system thinks 'localhost' should be. Since no port
          is given the default 14321 port is used.

  -p <url>
     Act as a child server by connecting to the parent server at the
     provided url.
      

LogicalTime
-----------

As required by rti1516e, OpenRTI knows about 64 bit integer and
64 bit float times.

The provided logical time implementations should fulfill the table as found
in paragraph 12.3 of the rti1516e standard document:

// let Iz = 0, Ie = epsilon, Iz < I
// any T denotes a value of type time,
// any I denotes a value of type interval:
//
// T' = T + I then T' > T                                    (*)
// T' = T - I then T' < T
// T' = T + Iz then T' == T
// T' = T - Iz then T' == T
// T1 != T2 then |T1 - T2| >= Ie
// T1 == T2 then T1 - T2 == Iz
//
// I' = I + Iz then I' == I
// I' = I - Iz then I' == I
// I1 != I2 then |I1 - I2| >= Ie
// I1 == I2 then I1 - I2 == Iz
// I != Iz then I >= Ie
//
// T > T' then T' < T

For integer valued logical times this properties are trivially valid.
For float valued logical times this is reached by using the smallest
representable double value > 0, which is the denormalized minimum as
epsilon. The '+' operator used with a strictly positive and thus non
zero time interval always results in a logical time that is strictly
greater than the input time to this operation. This is property (*)
in the above table.

While the full set of properties may be needed for time management
to work properly, the property (*) is critical and easily not valid
for a naive implementation of float valued logical time.

The tests/rti1516/time/logical-time test program can be used to test
a logical time implementation for the required behavior.

With included float valued logical time implementations, any attempt
to compare or encode a NaN valued logical time or logical time
interval will result in an invalid time exception. This is to avoid
internal problems with NaN's, specifically the last property in the
table cannot be fulfilled with NaN values but is needed for the use
of logical times as key values in std::map which requires strong
ordering on the key. Else the underlying implementation may behave
undefined and with that OpenRTI may behave undefined also.


Encoding
--------

For all this binary communication, OpenRTI always sends an initial
connect packet and a server always answers with the same type of packet.
This packet starts with a 12 byte fixed header containing the projects
name 'OpenRTI\0' and the 4 byte big endian total packet size. The payload
of that packet is a string key/value map carrying some information about
the client. This includes the client version number, the set of
available encodings and the clients endianess. The string based approach
should be extensible enough to build up a backward compatible
communication setup.
The server then selects the best encoding that matches the client and
may be some server configured policy like compression for example. Then
the server responds with such a string map containing the selected
encoding.
All following messages are exchanged in that selected encoding.
In the best case, this leads to optimal encoding. In the worst case we
have a controlled channel to refuse a connection. The client can then
abort and return with an error message to the user.

The implementation is prepared to some degree to move encodings into a
user provided shared library that could be loaded at runtime.


Locking:
--------

OpenRTI tries to lock object as little as possible.
The implementation assumes that each component lives in one thread. That means,
there is no need for locking for almost all operations OpenRTI's components
will do. The critical part is when messages are sent or received between
threads. This always happens through some kind of a locked message queue. Once
a message is sent to such a message queue, the sender should never touch that
message again. Due to messages being reference counted, the last one switches
the light off...

There are two notable exceptions to this.
1. The thread rti server:
Since this rti server does not have its own thread to live in, its execution is
guarded by a mutex that serializes sending messages to this server.
2. The VariableLengthData:
See next section.


VariableLengthData Lifetime.
----------------------------

The VariableLengthData class is implemented using a reference counted
copy/allocate on write scheme. This means that passing these values around has
just the cost of reference counting. In the best case and with federates living
in the same application, the receiving federate sees exactly the same payload
memory than the sending federate used to write into.
For that copy on write to work no locking is required.


Server state:
-------------

FIXME


Publication and Subscription:
-----------------------------

This is a brief overview how publication and subscription are propagated over
the tree.
Publication is a simple two state value that is tracked for each connect in a
server node. When a server node or ambassador behind a connect changes this
state it sends an appropriate message to the server node. The server node tracks
this publication state for each connect. This server node is responsible to
inform the other connects about its publication state. A server node should
only inform once this state changes for each connect. When the server node
receives a second publication through a second connect, no publication state
should be changed for connects that already know about that.
Publication state is immediately pushed to a new server node once it
is registered.

Subscription is tracked similar to publication. But subscription is only sent to
those connects that publish. That means if we get some delayed publication, all
current subscriptions need to be sent to that server node/ambassador.


Object Instance Management:
---------------------------

Object instance handles need to be globally managed to stay
unique. Also Object instance handles have lifetimes beyond the
timestamped delete instance message sent from the owner. Thus a
federate that has an object handle in use in any sense keeps a
reference of this object instance handle in its parent server
node. Once all federates have dropped their references the handle is
again free and can be reused.

Together with every object instance handle the root server also
allocates a unique object instance name. This holds also for the
object instance name reservations, where the instance handle is
already assigned once the name is reserved.

Every server node tracks the child connects that reference an object
instance handle. Once there are no old connects left that reference
an object instance handle, the server node unreferences this handle at
its parent.

The federate needs to release all the references it owns explicitly
even on destruction. This is to help the server node with the cleanup
since the server nodes only know which connect has a reference to a
handle not which federate. In contrast to that, if a connection dies,
may be due to a crashed federate, the server node will just
unreference the connect handle at all object instance handles it
references. This way we can guarantee a consistent cleanup even on
faulty federates.

