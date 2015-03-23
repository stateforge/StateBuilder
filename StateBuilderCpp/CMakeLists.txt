################################################################################
# StateBuilderCpp CMakeLists.txt
################################################################################
#The project name is the same as the generated MS Visual solution file(.sln)
SET(PROJECT_NAME StateBuilderCpp)

PROJECT(${PROJECT_NAME})

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

#Where to find all FindXXX.cmake
SET(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/ ${CMAKE_MODULE_PATH})

SET(STATEBUILDERCPP_COMPILER_HOME ${CMAKE_CURRENT_SOURCE_DIR}/../Compiler/cpp)
SET(RELEASES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../StateForgeWebSite/StateForgeWebSite/Files)

################################################################################
# Include
################################################################################

################################################################################
# Requirements
################################################################################
#Find java, 1.6 is required 
INCLUDE(FindJava)

################################################################################
# Packaging
################################################################################

SET(CPACK_PACKAGE_NAME StateBuilderCpp)
SET(JAR_NAME statebuilder-cpp)

SET(CPACK_PACKAGE_VERSION_MAJOR "3")
SET(CPACK_PACKAGE_VERSION_MINOR "0")
SET(CPACK_PACKAGE_VERSION_PATCH "0")
SET(CPACK_PACKAGE_VERSION "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")

#Setup jar and exe

#Windows
SET(STATEBUILDERCPP_WINSETUP ${CPACK_PACKAGE_NAME}-WinSetup)
SET(STATEBUILDERCPP_WINSETUP_JAR ${STATEBUILDERCPP_WINSETUP}.jar)
SET(STATEBUILDERCPP_WINSETUP_EXE ${STATEBUILDERCPP_WINSETUP}.exe)
SET(STATEBUILDERCPP_WINSETUP_VERSION_EXE ${STATEBUILDERCPP_WINSETUP}-${CPACK_PACKAGE_VERSION_MAJOR}-${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_PACKAGE_VERSION_PATCH}.exe)

#Linux
SET(STATEBUILDERCPP_LINUXSETUP ${CPACK_PACKAGE_NAME}-LinuxSetup)
SET(STATEBUILDERCPP_LINUXSETUP_JAR ${STATEBUILDERCPP_LINUXSETUP}.jar)
SET(STATEBUILDERCPP_LINUXSETUP_VERSION_JAR ${STATEBUILDERCPP_LINUXSETUP}-${CPACK_PACKAGE_VERSION_MAJOR}-${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_PACKAGE_VERSION_PATCH}.jar)

#Mac
SET(STATEBUILDERCPP_MACSETUP ${CPACK_PACKAGE_NAME}-MacSetup)
SET(STATEBUILDERCPP_MACSETUP_JAR ${STATEBUILDERCPP_MACSETUP}.jar)
SET(STATEBUILDERCPP_MACSETUP_VERSION_JAR ${STATEBUILDERCPP_MACSETUP}-${CPACK_PACKAGE_VERSION_MAJOR}-${CPACK_PACKAGE_VERSION_MINOR}-${CPACK_PACKAGE_VERSION_PATCH}.jar)

SET(STATEBUILDERCPP_MACSETUP_APP ${CPACK_PACKAGE_NAME}-MacSetup.app)
SET(STATEBUILDERCPP_MACPKG StateBuilderCpp.pkg)
SET(STATEBUILDERCPP_MACDMG StateBuilderCpp.dmg)

################################################################################
#Setting variable
################################################################################

#The state machine builder jar name with version
SET(STATEBUILDERCPP_JAR_WITH_VERSION "${JAR_NAME}-${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.one-jar.jar")

#The state machine builder jar name 
SET(STATEBUILDERCPP_JAR_NAME "${JAR_NAME}.jar")

#The state machine builder executable name 
SET(STATEBUILDERCPP_EXE "${CPACK_PACKAGE_NAME}.exe")

################################################################################
# Maven
################################################################################

# Find maven, the best building tool.
FIND_PACKAGE(MAVEN REQUIRED)

# Build StateBuilderCpp-x.y.z.one-jar.jar and copy and rename it to StateBuilderCpp/bin/StateBuilderCpp-x.y.z.jar
ADD_CUSTOM_TARGET(Maven
                  COMMAND ${MAVEN_EXECUTABLE} -Dmaven.test.skip=true package
                  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/pom.xml
                  WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
  
ADD_CUSTOM_TARGET(CopyJar
	          COMMAND ${CMAKE_COMMAND} -E copy "${STATEBUILDERCPP_COMPILER_HOME}/target/${STATEBUILDERCPP_JAR_WITH_VERSION}" "${CMAKE_CURRENT_SOURCE_DIR}/bin/${STATEBUILDERCPP_JAR_NAME}"
              COMMAND ${CMAKE_COMMAND} -E copy "${STATEBUILDERCPP_COMPILER_HOME}/src/main/resources/StateMachineCpp-v1.xsd" "${CMAKE_CURRENT_SOURCE_DIR}/share/xsd"
              WORKING_DIRECTORY ${STATEBUILDERCPP_COMPILER_HOME})
				  
ADD_DEPENDENCIES(CopyJar Maven) 
################################################################################
#Install
################################################################################

#################################################################################
# Launch4j and Launch4jSetup
#################################################################################

# Find launch4j
FIND_PACKAGE(LAUNCH4J )
IF(LAUNCH4J_FOUND)
  MESSAGE(STATUS "Launch4j has been found")
  #launch4j.xml is used to generated an exe from a jar, as well as other things.
  SET(STATEBUILDERCPP_LAUNCH4J "Launch4j")                  
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/packaging/Launch4j/${STATEBUILDERCPP_LAUNCH4J}.in.xml ${CMAKE_CURRENT_BINARY_DIR}/${STATEBUILDERCPP_LAUNCH4J}.xml @ONLY)
  ADD_CUSTOM_TARGET(Launch4j
                    COMMAND ${LAUNCH4J_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/${STATEBUILDERCPP_LAUNCH4J}.xml
                    DEPENDS packaging/Launch4j/${STATEBUILDERCPP_LAUNCH4J}.in.xml 
                    )
                    
  ADD_DEPENDENCIES(Launch4j CopyJar) 

  #WinSetup.jar => WinSetup.exe
  SET(STATEBUILDERCPP_SETUP_LAUNCH4J "Launch4jSetup")                  
  CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/packaging/Launch4j/${STATEBUILDERCPP_SETUP_LAUNCH4J}.in.xml ${CMAKE_CURRENT_BINARY_DIR}/${STATEBUILDERCPP_SETUP_LAUNCH4J}.xml @ONLY)
  ADD_CUSTOM_TARGET(Launch4jSetupWin
                    ${LAUNCH4J_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/${STATEBUILDERCPP_SETUP_LAUNCH4J}.xml
                    COMMAND ${CMAKE_COMMAND} -E copy ${RELEASES_DIR}/${STATEBUILDERCPP_WINSETUP_EXE} ${RELEASES_DIR}/${STATEBUILDERCPP_WINSETUP_VERSION_EXE}
                    DEPENDS packaging/Launch4j/${STATEBUILDERCPP_SETUP_LAUNCH4J}.in.xml 
                    )
                    
  ADD_DEPENDENCIES(Launch4jSetupWin IzPackWin) 

  
ELSE()
  MESSAGE(STATUS "Launch4j has not been found")
ENDIF()

#################################################################################
# IzPackWin and IzPackLinux
#################################################################################

FIND_PACKAGE(IZPACK)

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/script/preUninstall.in.bat ${CMAKE_CURRENT_SOURCE_DIR}/script/preUninstall.bat @ONLY)
CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/script/postInstall.in.bat ${CMAKE_CURRENT_SOURCE_DIR}/script/postInstall.bat @ONLY)

#Where is StateBuilderCpp
SET(IZPACK_DEPLOYDIR ${CMAKE_CURRENT_SOURCE_DIR})

#Windows
#IzPackWinConf.xml => StateBuilderCpp-WinSetup.jar

SET(IZPACK_WIN_CONF "IzPackWinConf")                  

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/packaging/IzPack/${IZPACK_WIN_CONF}.in.xml ${CMAKE_CURRENT_BINARY_DIR}/${IZPACK_WIN_CONF}.xml @ONLY)
ADD_CUSTOM_TARGET(IzPackWin
                  COMMAND ${IZPACK_COMPILE} ${CMAKE_CURRENT_BINARY_DIR}/${IZPACK_WIN_CONF}.xml -b ${IZPACK_DEPLOYDIR} -o ${CMAKE_CURRENT_BINARY_DIR}/${STATEBUILDERCPP_WINSETUP_JAR}
                  DEPENDS packaging/IzPack/${IZPACK_WIN_CONF}.in.xml 
                  )
  
ADD_DEPENDENCIES(IzPackWin Launch4j)

#Linux
#IzPackLinuxConf.xml => StateBuilderCpp-LinuxSetup.jar
SET(IZPACK_LINUX_CONF "IzPackLinuxConf")

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/packaging/IzPack/${IZPACK_LINUX_CONF}.in.xml ${CMAKE_CURRENT_BINARY_DIR}/${IZPACK_LINUX_CONF}.xml @ONLY)
ADD_CUSTOM_TARGET(IzPackUnix
                  ${IZPACK_COMPILE} ${CMAKE_CURRENT_BINARY_DIR}/${IZPACK_LINUX_CONF}.xml -b ${IZPACK_DEPLOYDIR} -o ${RELEASES_DIR}/${STATEBUILDERCPP_LINUXSETUP_JAR}
                  DEPENDS packaging/IzPack/${IZPACK_LINUX_CONF}.in.xml 
                  )

ADD_DEPENDENCIES(IzPackUnix CopyJar)

ADD_CUSTOM_TARGET(IzPackUnixPost
                  COMMAND ${CMAKE_COMMAND} -E copy ${RELEASES_DIR}/${STATEBUILDERCPP_LINUXSETUP_JAR} ${RELEASES_DIR}/${STATEBUILDERCPP_MACSETUP_JAR}
                  COMMAND ${CMAKE_COMMAND} -E copy ${RELEASES_DIR}/${STATEBUILDERCPP_MACSETUP_JAR} ${RELEASES_DIR}/${STATEBUILDERCPP_MACSETUP_VERSION_JAR}
                  COMMAND ${CMAKE_COMMAND} -E copy ${RELEASES_DIR}/${STATEBUILDERCPP_LINUXSETUP_JAR} ${RELEASES_DIR}/${STATEBUILDERCPP_LINUXSETUP_VERSION_JAR}
                  DEPENDS ${RELEASES_DIR}/${STATEBUILDERCPP_LINUXSETUP_JAR} 
                  )
				  
ADD_DEPENDENCIES(IzPackUnixPost IzPackUnix)

################################################################################
# Variable
################################################################################
if(EXISTS $ENV{STATEBUILDERCPP_HOME})
  MESSAGE(STATUS "STATEBUILDERCPP_HOME is already set to $ENV{STATEBUILDERCPP_HOME} but is now overwritten to ${CMAKE_SOURCE_DIR}")
ENDif()

set(ENV{STATEBUILDERCPP_HOME} ${CMAKE_SOURCE_DIR})

#Where to find all FindXXX.cmake
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/packaging/NSIS ${CMAKE_SOURCE_DIR}/share/cmake/ ${CMAKE_MODULE_PATH})

################################################################################
# Configuration options
################################################################################

#Examples
option(STATEBUILDERCPP_EXAMPLE "Build the examples directory." ON)
if(STATEBUILDERCPP_EXAMPLE)
  add_subdirectory(examples)
ELSE()
  #add_subdirectory(examples EXCLUDE_FROM_ALL)
ENDif()

################################################################################
# Test
################################################################################
ENABLE_TESTING()

################################################################################
#Subdirectory
################################################################################
add_subdirectory(src)

