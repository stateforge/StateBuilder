# - Locate maven
# maven can be found at http://maven.apache.org/
# Written by Frederic Heem, frederic.heem _at_ gmail.com

# This module defines
# MAVEN_EXECUTABLE, where is the mvn
# MAVEN_FOUND, If false, don't try to use maven
# The macro MAVEN_PACKAGE(pomFile) that will invoke mvn package

IF(EXISTS $ENV{M2_HOME})
  FIND_PROGRAM(MAVEN_EXECUTABLE 
    NAMES 
      mvn
    PATHS
      $ENV{M2_HOME}/bin
    NO_DEFAULT_PATH 
  )
ELSE(EXISTS $ENV{M2_HOME})
  FIND_PROGRAM(MAVEN_EXECUTABLE 
    NAMES 
      mvn
    PATHS 
  )
ENDIF(EXISTS $ENV{M2_HOME})


# This macro invokes mvn
# example: MAVEN_PACKAGE(jar)
MACRO(MAVEN_PACKAGE _jar)
    SET(jar ${CMAKE_CURRENT_BINARY_DIR}/${_jar})
    ADD_CUSTOM_COMMAND(
      OUTPUT ${jar}
      COMMAND "${MAVEN_EXECUTABLE} package"
      ARGS pom.xml
      DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pom.xml
      WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
ENDMACRO(MAVEN_PACKAGE)


IF(MAVEN_EXECUTABLE)
  SET(MAVEN_FOUND "YES")
ENDIF(MAVEN_EXECUTABLE)

MARK_AS_ADVANCED(
  MAVEN_EXECUTABLE
)


