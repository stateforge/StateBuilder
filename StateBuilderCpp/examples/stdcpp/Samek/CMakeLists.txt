
project(Samek)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Verbose make
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROGRAM_NAME Samek)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Automatically and includes, lib, lib dirs and definition 
include(${STATEBUILDERCPP_USE_FILE})

#Set the state machine(s)
set(FSM_FILES Samek.fsmcpp)

statebuildercpp_compile(FSM_CPP ${FSM_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${SMSTDUTILS_INCLUDE_DIR})

add_executable(${PROGRAM_NAME} main.cpp Samek.cpp ${FSM_CPP} ${FSM_FILES})

target_link_libraries(${PROGRAM_NAME} ${STATEBUILDERCPP_LIBRARIES} ${SMSTDUTILS_LIBRARY})

# A warning is an error
if(MSVC) 
  ADD_DEFINITIONS( /W4 ) 
else(MSVC)  
  ADD_DEFINITIONS( -Wall )
endif(MSVC)

add_test(${PROGRAM_NAME} ${PROGRAM_NAME} )
