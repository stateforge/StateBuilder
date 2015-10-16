project(SILLY)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Verbose make
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROGRAM_NAME Silly)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Automatically add include dirs, lib, lib dirs and definition
include(${STATEBUILDERCPP_USE_FILE})

#Set the state machine(s)
set(FSM_FILES Silly.fsmcpp)

statebuildercpp_compile(FSM_CPP ${FSM_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR}
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${SMSTDUTILS_INCLUDE_DIR}
                    )

add_executable(${PROGRAM_NAME} main.cpp Action.cpp ${FSM_CPP} ${FSM_FILES})

target_link_libraries(${PROGRAM_NAME} ${SMSTDUTILS_LIBRARY})

# A warning is an error
if(MSVC)
  ADD_DEFINITIONS( /W4 )
else(MSVC)
  ADD_DEFINITIONS( -Wall  -Wextra)
endif(MSVC)

# Add a test to use execute with ctest
add_test(Silly ${PROGRAM_NAME} )
