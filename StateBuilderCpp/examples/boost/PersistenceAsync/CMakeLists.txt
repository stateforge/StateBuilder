project(PERSISTENCEASYNC)

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Verbose make
#set(CMAKE_VERBOSE_MAKEFILE ON)

set(PROGRAM_NAME PersistenceAsync)

if(APPLE OR WIN32)
  set(Boost_USE_STATIC_LIBS ON)
endif()

#Use Boost
set(STATEBUILDERCPP_USE_BOOST TRUE)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Automatically and includes, lib, lib dirs and definition 
include(${STATEBUILDERCPP_USE_FILE})

#Set the state machine(s)
set(FSM_FILES PersistenceAsync.fsmcpp)

statebuildercpp_compile(FSM_CPP ${FSM_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${SMBOOSTUTILS_INCLUDE_DIR}
                    ${LOGLITE_INCLUDE_DIR}
                   )

add_executable(${PROGRAM_NAME} main.cpp Action.cpp ${FSM_CPP} ${FSM_FILES})
 
link_directories(${Boost_LIBRARY_DIRS})
 
target_link_libraries(${PROGRAM_NAME} 
  ${STATEBUILDERCPP_LIBRARIES}
  ${SMBOOSTUTILS_LIBRARY} 
)

# A warning is an error
if(MSVC) 
  ADD_DEFINITIONS( /W4 ) 
else(MSVC)  
  ADD_DEFINITIONS( -Wall)
endif(MSVC)

# Avoid warning when using boost on Windows
if(WIN32)
  ADD_DEFINITIONS(-D_WIN32_WINNT=0x0501)
ENDif()

#Cmake bug with MinGW and Boost asio
if(MINGW)
  target_link_libraries(${PROGRAM_NAME} ws2_32)
ENDif()


add_test(PersistenceAsync ${PROGRAM_NAME} )
