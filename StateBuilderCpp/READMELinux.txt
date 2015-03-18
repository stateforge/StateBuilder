Welcome the StateBuildCpp, the finite state machine C++ source code generator.
This document is a quick guide to get started on Linux.
For more information or suggestion, please visit www.stateforge.com

* Download:
  # wget http://www.stateforge.com/Files/StateBuilderCpp-LinuxSetup.jar

* Install the requirements:
   
  - Java 1.6 from Sun: build time only.
  - Qt or Boost: needed for asynchronous machine.
  - CMake: build the examples.
   
    Ubuntu 12.04:
      # sudo apt-get install build-essential libqt4-dev libboost-dev libboost-thread-dev libboost-filesystem-dev libboost-date-time-dev cmake openjdk-7-jre-headless

    Ubuntu 10.04:
      # add-apt-repository "deb http://archive.canonical.com/ lucid partner"
      # apt-get update
      # sudo apt-get install build-essential libqt4-dev libboost-dev cmake sun-java6-jre
      # sudo update-java-alternatives -s java-6-sun

    Fedora
      # yum install java gcc-c++ qt-devel boost-devel cmake 

* Install:
  # java -jar StateBuilderCpp-LinuxSetup.jar

* Configure the cmake build system
  # cd $HOME/StateBuilderCpp/build/makefile
  # ./gen-makefile.sh

* Compile the examples:
  # make

* Run the examples:
  # ctest

* Compile and run a single example:
  # cd $HOME/StateBuilderCpp/build/unix/examples/qt/Ping
  # make
  # make test

