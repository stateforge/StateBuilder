# - Locate launch4j, a cross-platform Java executable wrapper
# launch4j can be found at http://launch4j.sourceforge.net/
# Written by Frederic Heem, frederic.heem _at_ gmail.com

# This module defines
# LAUNCH4J_EXECUTABLE, where is the executable launch4j
# LAUNCH4J_FOUND, If false, don't try to use launch4j
# The macro LAUNCH4J(launch4jFile) that will invoke launch4j with the launch4j description file 

IF(EXISTS $ENV{LAUNCH4J_HOME})
  FIND_PROGRAM(LAUNCH4J_EXECUTABLE 
    NAMES
      launch4j
    PATHS
      $ENV{LAUNCH4J_HOME}/
    NO_DEFAULT_PATH 
  )
ELSE(EXISTS $ENV{LAUNCH4J_HOME})
  FIND_PROGRAM(LAUNCH4J_EXECUTABLE 
    NAMES 
      launch4j.exe
      launch4j
    PATHS
    "C:/Program Files/Launch4j"
    "C:/Program Files (x86)/Launch4j"
  )
ENDIF(EXISTS $ENV{LAUNCH4J_HOME})

# This macro invokes launch4j
# example: LAUNCH4J(launch4j.xml program.exe program.jar)
MACRO(LAUNCH4J _launch4jFile _program _jar)
    SET(launch4jFile ${CMAKE_CURRENT_SOURCE_DIR}/${_launch4jFile})
    SET(program ${CMAKE_CURRENT_BINARY_DIR}/${_program})
    SET(jar ${CMAKE_CURRENT_SOURCE_DIR}/target/${_jar})
    ADD_CUSTOM_COMMAND(
      OUTPUT ${program}
      COMMAND ${LAUNCH4J_EXECUTABLE}
      ARGS ${launch4jFile}
      DEPENDS ${launch4jFile} ${jar}
    )
ENDMACRO(LAUNCH4J)


IF(LAUNCH4J_EXECUTABLE)
  SET(LAUNCH4J_FOUND "YES")
ENDIF(LAUNCH4J_EXECUTABLE)

MARK_AS_ADVANCED(
  LAUNCH4J_EXECUTABLE
) 

