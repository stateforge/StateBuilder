project(Microwave)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Always verbose make
set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROGRAM_NAME Microwave)

if(APPLE OR WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif()

#Boost
find_package(Boost COMPONENTS date_time filesystem system thread REQUIRED)

link_directories(${Boost_LIBRARY_DIRS})

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Automatically and includes, lib, lib dirs and definition 
include(${STATEBUILDERCPP_USE_FILE})

#Set the state machine(s)
set(FSM_FILES Microwave.fsmcpp)

statebuildercpp_compile(FSM_CPP ${FSM_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${SMBOOSTUTILS_INCLUDE_DIR}
                    ${Boost_INCLUDE_DIRS}
                    ${LOGLITE_INCLUDE_DIR})

add_executable(${PROGRAM_NAME} main.cpp Microwave.cpp ${FSM_CPP} ${FSM_FILES})


target_link_libraries(${PROGRAM_NAME}
  ${STATEBUILDERCPP_LIBRARIES}
  ${Boost_FILESYSTEM_LIBRARY}
  ${SMBOOSTUTILS_LIBRARY} 
)

# Warning on 
if(MSVC) 
  ADD_DEFINITIONS( /W4 ) 
else(MSVC)  
  ADD_DEFINITIONS( -Wall )
endif(MSVC)

add_test(${PROGRAM_NAME} ${PROGRAM_NAME} )


