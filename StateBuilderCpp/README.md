Building StateBuilderCpp from source
===================================

## Requirements

**StateBuilderCpp** is written in Java so the following is needed to compile it from source:

* [Maven](https://maven.apache.org/)

* [Eclipse](https://eclipse.org/)

 C++ build tools are also needed to compile the examples:

* [CMake](https://cmake.org/)

* *Visual Studio* or *mingw* on Windows

Optional requirements:

* [Qt](http://www.qt.io/)

* [Launch4j](http://launch4j.sourceforge.net/)

* [IzPack](http://izpack.org/)

## Setup the environment

Clone the source code with git

    $ git clone https://github.com/stateforge/StateBuilder.git

    $ cd StateBuilder/StateBuilderCpp

Import the project with eclipse as a *maven project*

## Build

    $ cd build/setup

Edit *buildSetup.bat* and eventually change the *QTDIR* path

    $ buildSetup.bat

This script calls *cmake* to create and build everything.  

*StateBuilderCpp.exe* should located in the *StateBuilderCpp/bin* directory

```
C:\Users\frederic\StateBuilder\StateBuilderCpp\bin>StateBuilderCpp.exe
StateBuilderCpp 3.0 : All rights reserved, 2011-2015
usage: StateBuilderCpp [option] myStateMachine.fsmcpp
 -d,--directory <arg>      Output directory
 -h,--help                 Show help
 -L,--lock                 Add code for multithreaded state machine
 -p,--prepend-file <arg>   Prepend file, e.g Copyright
 -v,--version              Show version

```

## Change version

The version si set in two different parts:

First edit *StateBuilderCpp/CMakeList.txt* and set the new version of the installers:

```
SET(CPACK_PACKAGE_VERSION_MAJOR "3")
SET(CPACK_PACKAGE_VERSION_MINOR "1")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
```

Now edit *Compiler/cpp/pom.xml* and set the version for the compiler itself:
```
<version>3.1</version>
```


At this point, run the *StateBuilderCpp/buid/setup/buildSetup.bat* script to build the code generator and the installers.
