project(Microwave02)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Verbose make
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROGRAM_NAME Microwave02)

if(APPLE OR WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif()

#Use Boost
set(STATEBUILDERCPP_USE_BOOST TRUE)

#Arguments
set(STATEBUILDERCPP_ARGS "--lock")

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Automatically and includes, lib, lib dirs and definition 
include(${STATEBUILDERCPP_USE_FILE})

#Set the state machine(s)
set(FSM_FILES Microwave02.fsmcpp)

statebuildercpp_compile(FSM_CPP ${FSM_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${LOGLITE_INCLUDE_DIR}
                    ${SMBOOSTUTILS_INCLUDE_DIR})

add_executable(${PROGRAM_NAME} main.cpp Microwave.cpp ${FSM_CPP} ${FSM_FILES})

link_directories(${Boost_LIBRARY_DIRS})

target_link_libraries(${PROGRAM_NAME}
 ${SMBOOSTUTILS_LIBRARY} 
 ${STATEBUILDERCPP_LIBRARIES}
)

# Warning on 
if(MSVC) 
  ADD_DEFINITIONS( /W4 ) 
else(MSVC)  
  ADD_DEFINITIONS( -Wall )
endif(MSVC)

#Cmake bug with MinGW and Boost asio
if(MINGW)
  target_link_libraries(${PROGRAM_NAME} ws2_32)
ENDif()

add_test(${PROGRAM_NAME} ${PROGRAM_NAME} )


