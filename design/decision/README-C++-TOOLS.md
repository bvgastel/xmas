# Useful C++ Software libraries

Purpose of this document is describing a comparison of tools
that can be used for the project.
Looking for multi threading, interprocess communication and serialization.
The current product of this project uses mainly JSON and is standalone. 

  - [POCO C++]
  - [NCBI]
  - [Boost]
  - [Protocol Buffers]
  - [MessagePack]
  - [Avro]
  - [ZeroMQ]


### POCO C++
The POCO C++ libraries focus on internet-centric applications.
Released under the Boost Software License. 
>Conclusion: Not very useful because its focus differs from the needs for this project.

### NCBI
Library supports multi threading, serialization, ipc (sockets & pipes), regex.
>Conclusion: Useful but need to add additional libraries to support JSON.
>Documentation is poor and only a few who referenced to this toolkit, seems
>to be non popular.

### Boost
Boost is a generic library with many features. It supports serialization,
multi threading, able to start external exe's.

...todo
>Conclusion: 

### Protocol Buffers
Google's Protocol Buffers are used for serializing structured data based on RPC.
The structure or message is described by a interface design language (IDL).
Its main goal is communication over wire and store data in a binary form.

>Conclusion:Protocol Buffers is simple and efficient but purely
>used for serializing. The idl *proto files* need to be compiled
>with a specific compiler called *protoc*.

### Avro
Apache Avro is pure serialization based on RPC. Avro is more like a hybrid
it has the same flexibility as Protocol Buffers but the schema is inside
the header and doesn't need a compiler. Schema's are in JSON format.

>Conclusion: Ideal when there's a need for strongly typed formats combined
>with flexibility. Schema's can be made in pure JSON but gives some
>overhead because it is always part of a message it represents.

### MessagePack
MessagePack is like JSON but smaller and faster

>Conclusion: No type checking schemas but a perfect drop-in replacement
>if the API is already using JSON for streaming and storage.

### ZeroMQ
This is a networking library so purely used for communication. In-process,
interprocess, TCP and multicast.

>Conclusion: A pure communication library but easy to use.


## Final Conclusion

Because this project is standalone and the main needs are serialization for
local storage, communication between processes on the same machine
which are related to each other. It already uses JSON for serialization and
communication between processes are C++ objects structures.
Also multi threading, array's , regular expressions are some of the mechanisms
that this project needs. So Boost is the most appropriate choice because it
covers all those needs.
It is always possible to use Avro or MessagePack next to Boost because these
are focused on serialization based on JSON.

##### References:
* [C++ Libraries]
* [Serialization tools]
* [Pipes]

[NCBI]:http://www.ncbi.nlm.nih.gov/IEB/ToolBox/CPP_DOC/
[POCO C++]:http://pocoproject.org/
[Boost]:http://www.boost.org/
[Protocol Buffers]:http://code.google.com/p/protobuf/
[MessagePack]:http://msgpack.org/
[Avro]:http://avro.apache.org/
[ZeroMQ]:http://zeromq.org/

[C++ Libraries]:http://en.cppreference.com/w/cpp/links/libs
[Serialization tools]:https://www.igvita.com/2011/08/01/protocol-buffers-avro-thrift-messagepack/
[Pipes]:http://www.drdobbs.com/windows/using-named-pipes-to-connect-a-gui-to-a/231903148

