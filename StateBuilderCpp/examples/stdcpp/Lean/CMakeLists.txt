project(LEAN)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Verbose make
#set(CMAKE_VERBOSE_MAKEFILE ON)

if(WIN32)
  GET_FILENAME_COMPONENT(STATEBUILDERCPP_HOME
                         "[HKEY_CURRENT_USER\\Software\\StateForge\\StateBuilderCpp;InstallLocation]"
                         ABSOLUTE)
  set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${STATEBUILDERCPP_HOME}/share/cmake )
  MESSAGE(STATUS  ${CMAKE_MODULE_PATH})
elseif(UNIX)
  if(ENV{STATEBUILDERCPP_HOME})
    MESSAGE(STATUS "STATEBUILDERCPP_HOME is set")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{STATEBUILDERCPP_HOME}/share/cmake/)
  else()
    MESSAGE(STATUS "STATEBUILDERCPP_HOME is not set")
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} $ENV{HOME}/StateBuilderCpp/share/cmake/)
  endif()
endif()

set(PROGRAM_NAME Lean)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Automatically add include dirs, lib, lib dirs and definition 
include(${STATEBUILDERCPP_USE_FILE})

#Set the state machine(s)
set(FSM_FILES Lean.fsmcpp LeanObserverInt.fsmcpp LeanObserverNo.fsmcpp)

statebuildercpp_compile(FSM_CPP ${FSM_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${SMSTDUTILS_INCLUDE_DIR}
                    )

add_executable(${PROGRAM_NAME} main.cpp Action.cpp IdObserver.cpp ObserverInt.cpp TestObserverInt.cpp TestObserverNo.cpp ${FSM_CPP} ${FSM_FILES})

target_link_libraries(${PROGRAM_NAME} ${SMSTDUTILS_LIBRARY})

# A warning is an error
if(MSVC) 
  ADD_DEFINITIONS( /W4 ) 
else(MSVC)  
  ADD_DEFINITIONS( -Wall  -Wextra -fno-rtti -fno-exceptions -DSTATEFORGE_NO_PERSISTENCE)
endif(MSVC)

# Add a test to use execute with ctest
add_test(Lean ${PROGRAM_NAME} )
