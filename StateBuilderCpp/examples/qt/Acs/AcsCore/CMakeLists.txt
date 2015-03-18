set(ACSCORE_LIBRARY AcsCore CACHE INTERNAL "AcsCore library name")
set(ACSCORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "AcsCore INCLUDE DIRECTORY")

# Where to find all FindXXX.cmake
set(CMAKE_MODULE_PATH $ENV{STATEBUILDERCPP_HOME}/share/cmake)

#Use Qt
set(STATEBUILDERCPP_USE_QT TRUE)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Set the state machine(s)
set(FSM_FILES Acs.fsmcpp AcsClient.fsmcpp AcsConnection.fsmcpp AcsRequestReboot.fsmcpp AcsRequestDownload.fsmcpp)

#Generate the cpp source code
statebuildercpp_compile(FSM_CPP ${FSM_FILES})

#Add the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${QSLOG_INCLUDE_DIR}
                    ${SMQTUTILS_INCLUDE_DIR}
                    ${TR69COMMON_INCLUDE_DIR}
                    ${QT_INCLUDE_DIRS}
                    ${QHTTPSERVER_INCLUDE_DIR}
                    ${QTSOAP_INCLUDE_DIR}
                    ${STATEBUILDERCPP_INCLUDE_DIRS})

set(QT_SRCS
    AcsClient.cpp
    Acs.cpp
    AcsRequest.cpp
    AcsConnection.cpp
    AcsRequestReboot.cpp
    AcsRequestRebootPrivate.cpp
    AcsRequestDownload.cpp
    AcsRequestSetValue.cpp
    AcsRequestGetValue.cpp
    AcsMessageBuilder.cpp
    AcsParser.cpp
)

set(QT_MOC_HDRS
  AcsClient.h
  AcsClientPrivate.h
  Acs.h
  AcsPrivate.h
  AcsRequest.h
  AcsRequestReboot.h
  AcsRequestRebootPrivate.h
  AcsRequestDownload.h
  AcsRequestSetValue.h
  AcsRequestGetValue.h
  AcsConnection.h
  AcsConnectionPrivate.h  
  AcsMessageBuilder.h
  AcsParser.h
)

qt4_automoc(${QT_SRCS})
qt4_wrap_cpp(QT_MOC_SRCS ${QT_MOC_HDRS})

#Build the library  
add_library(${ACSCORE_LIBRARY} 
   ${QT_SRCS}
   ${QT_MOC_HDRS}
   ${QT_MOC_SRCS}
   ${FSM_CPP}
   ${FSM_FILES})

target_link_libraries(${ACSCORE_LIBRARY} 
                      ${QTSOAP_LIBRARY}
                      ${QHTTPSERVER_LIBRARY}
                      ${QT_LIBRARIES}
                      ${TR69COMMON_LIBRARY}
                      ${SMQTUTILS_LIBRARY} 
                      ${STATEBUILDERCPP_LIBRARIES})

install(TARGETS ${ACSCORE_LIBRARY}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)