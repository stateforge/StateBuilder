# Locate StateBuilderCpp, the finite state machine builder for C++
# Visit www.stateforge.com fro more information

# The environment variable STATEBUILDERCPP_HOME is used to search for include and executable

# This module defines the following variables that UseStateBuilderCpp.cmake will use.
# STATEBUILDERCPP_INCLUDE_DIRS, the include directories
# STATEBUILDERCPP_LIBRARIES, library dependencies
# STATEBUILDERCPP_LIBRARY_DIRS. where to look for these libraries
# STATEBUILDERCPP_EXECUTABLE, i.e: StateBuilderCpp.exe
# STATEBUILDERCPP_FOUND, If false, don't try to use the state machine builder
# The macro statebuildercpp_compile(FSM_CPP_SRC input.fsm )

IF(STATEBUILDERCPP_USE_BOOST)
  #MESSAGE(STATUS "StateBuilderCpp uses boost")
  FIND_PACKAGE(Boost COMPONENTS date_time system thread)
  SET(STATEBUILDERCPP_INCLUDE_DIRS ${Boost_INCLUDE_DIRS})
  if(APPLE OR WIN32)
    SET(STATEBUILDERCPP_LIBRARIES ${Boost_THREAD_LIBRARY} ${Boost_DATE_TIME_LIBRARY} ${Boost_SYSTEM_LIBRARY})
  ELSE()
    SET(STATEBUILDERCPP_LIBRARIES ${Boost_LIBRARIES})
  ENDIF()
  SET(STATEBUILDERCPP_LIBRARY_DIRS ${Boost_LIBRARY_DIRS})
  SET(STATEBUILDERCPP_DEFINITIONS "-DBOOST")
  SET(STATEBUILDERCPP_COMPONENT_FOUND TRUE)
ENDIF()

#Default is Qt
IF(STATEBUILDERCPP_USE_QT)
  #MESSAGE(STATUS "StateBuilderCpp uses Qt")
  FIND_PACKAGE(Qt4 COMPONENTS Qtcore)
  SET(STATEBUILDERCPP_COMPONENT_FOUND TRUE)
ENDIF()

IF(NOT STATEBUILDERCPP_COMPONENT_FOUND)
  #MESSAGE(STATUS "WARNING: neither STATEBUILDERCPP_USE_BOOST nor STATEBUILDERCPP_USE_QT is set, asynchronous feature cannot work without one of these libraries.")
ENDIF()


FIND_PATH(STATEBUILDERCPP_INCLUDE_DIR fsm/FsmAsync.h
  $ENV{STATEBUILDERCPP_HOME}/src/cpp/include
  $ENV{HOME}/StateBuilderCpp/src/cpp/include
  $ENV{HOME}/AppData/Local/StateForge/StateBuilderCpp/src/cpp/include
  NO_DEFAULT_PATH
)

FIND_PROGRAM(STATEBUILDERCPP_EXECUTABLE
  NAMES 
    StateBuilderCpp.exe
    StateBuilderCpp.sh
  PATHS 
    $ENV{STATEBUILDERCPP_HOME}/bin
    $ENV{HOME}/StateBuilderCpp/bin
    $ENV{HOME}/AppData/Local/StateForge/StateBuilderCpp/bin
  NO_DEFAULT_PATH
)

FIND_PROGRAM(STATEBUILDERCPP_USE_FILE UseStateBuilderCpp.cmake
  $ENV{STATEBUILDERCPP_HOME}/share/cmake
  $ENV{HOME}/StateBuilderCpp/share/cmake
  $ENV{HOME}/AppData/Local/StateForge/StateBuilderCpp/share/cmake
  NO_DEFAULT_PATH
)


IF(NOT EXISTS ${STATEBUILDERCPP_INCLUDE_DIR})
  MESSAGE(STATUS "STATEBUILDERCPP_HOME does not exist, please set this environment variable")
ENDIF()


#Include
#MESSAGE(STATUS "StateBuilderCpp include dir set to ${STATEBUILDERCPP_INCLUDE_DIR}")

#Include dirs with dependencies
SET(STATEBUILDERCPP_INCLUDE_DIRS ${STATEBUILDERCPP_INCLUDE_DIRS} ${STATEBUILDERCPP_INCLUDE_DIR})
#MESSAGE(STATUS "StateBuilderCpp include dirs set to ${STATEBUILDERCPP_INCLUDE_DIRS}")

#Libraries
#MESSAGE(STATUS "StateBuilderCpp libraries set to ${STATEBUILDERCPP_LIBRARY}")

#Library dirs
#MESSAGE(STATUS "StateBuilderCpp library dirs set to ${STATEBUILDERCPP_LIBRARY_DIRS}")

SET(STATEBUILDERCPP_DEFINITIONS ${STATEBUILDERCPP_DEFINITIONS} ${STATEBUILDERCPP_DEFINITION})
#MESSAGE(STATUS "StateBuilderCpp definitions are  ${STATEBUILDERCPP_DEFINITIONS}")

#MESSAGE(STATUS "StateBuilderCpp.exe is at ${STATEBUILDERCPP_EXECUTABLE}")


IF(STATEBUILDERCPP_EXECUTABLE)
  SET(STATEBUILDERCPP_FOUND "YES")
ELSE() 
  MESSAGE(FATAL "StateBuilderCpp.exe cannot be found") 
ENDIF()

# This macro invokes the fsm compiler for all files without extension.
# Output cpp files are stored in ${CMAKE_CURRENT_BINARY_DIR}
# example: statebuildercpp_compile(output.cpp input.fsm )
MACRO(statebuildercpp_compile cppFiles)
    #MESSAGE(STATUS "fsm compile macro")
    SET(FSM_CPP)
    FOREACH (it ${ARGN})
      #MESSAGE(STATUS "fsm file: ${it}")
      GET_FILENAME_COMPONENT(file_we ${it} NAME_WE)
      SET(hFile ${CMAKE_CURRENT_BINARY_DIR}/${file_we}Fsm.h)
      #MESSAGE(STATUS "fsm generated h file: ${hFile}")
      SET(cppFile ${CMAKE_CURRENT_BINARY_DIR}/${file_we}Fsm.cpp)
      #MESSAGE(STATUS "fsm generated cpp file: ${cppFile}")
      GET_FILENAME_COMPONENT(fsmFile ${it} ABSOLUTE)
      
      #MESSAGE(STATUS "fsm input: ${fsmFile}")
      SOURCE_GROUP(StateMachine FILES ${fsmFile})
      SET_SOURCE_FILES_PROPERTIES(${hFile} PROPERTIES HEADER_FILE_ONLY FALSE GENERATED TRUE)
      SET_SOURCE_FILES_PROPERTIES(${cppFile} PROPERTIES HEADER_FILE_ONLY FALSE GENERATED TRUE)

      IF(MSVC) 
      else(MSVC)  
        ADD_DEFINITIONS( )
        SET_SOURCE_FILES_PROPERTIES(${cppFile} PROPERTIES COMPILE_FLAGS "-Wno-unused-variable -Wno-unused-parameter")
      endif(MSVC)

      ADD_CUSTOM_COMMAND(
         OUTPUT ${cppFile} ${hFile}
         COMMAND ${STATEBUILDERCPP_EXECUTABLE}
         ARGS ${STATEBUILDERCPP_ARGS} -d ${CMAKE_CURRENT_BINARY_DIR} ${fsmFile}
        DEPENDS ${fsmFile}
        COMMENT "Generating ${cppFile} and  ${hFile} from ${fsmFile}"
      )

      IF(STATEBUILDERCPP_USE_QT)
        QT4_WRAP_CPP(mocFile ${hFile})
      ENDIF()

      SET(${cppFiles} ${${cppFiles}} ${mocFile} ${hFile} ${cppFile})
    ENDFOREACH (it)
ENDMACRO(statebuildercpp_compile)


MARK_AS_ADVANCED(
  STATEBUILDERCPP_INCLUDE_DIR
  STATEBUILDERCPP_LIBRARIES
  STATEBUILDERCPP_LIBRARY_DIRS
  STATEBUILDERCPP_EXECUTABLE
  STATEBUILDERCPP_DEFINITIONS
) 
