set(TR69CLIENTCORE_LIBRARY Tr69ClientCore CACHE INTERNAL "Tr69ClientCore library name")
set(TR69CLIENTCORE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Tr69ClientCore INCLUDE DIRECTORY")

# Where to find all FindXXX.cmake
set(CMAKE_MODULE_PATH $ENV{STATEBUILDERCPP_HOME}/share/cmake)

#Use Qt
set(STATEBUILDERCPP_USE_QT TRUE)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Set the state machine(s)
set(FSM_FILES Tr69Client.fsmcpp Tr69Connection.fsmcpp Tr69ConnectionRequestServer.fsmcpp Tr69ConnectionRequestResponder.fsmcpp)

#Generate the cpp source code
statebuildercpp_compile(FSM_CPP ${FSM_FILES})

#Add the include directories


include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${QSLOG_INCLUDE_DIR}
                    ${SMQTUTILS_INCLUDE_DIR}
                    ${TR69COMMON_INCLUDE_DIR}
                    ${QT_INCLUDE_DIRS}
                    ${QTSOAP_INCLUDE_DIR}
                    ${QHTTPSERVER_INCLUDE_DIR}
                    ${STATEBUILDERCPP_INCLUDE_DIRS})
                    
set(QT_SRCS
    Tr69Client.cpp
    Tr69Connection.cpp
    Tr69MessageBuilder.cpp
    Tr69Parser.cpp
    Tr69ConnectionRequestServer.cpp
    Tr69ConnectionRequestResponder.cpp
)

set(QT_MOC_HDRS
  Tr69Client.h
  Tr69ClientPrivate.h
  Tr69Connection.h
  Tr69ConnectionPrivate.h
  Tr69MessageBuilder.h
  Tr69Parser.h
  Tr69ConnectionRequestServer.h
  Tr69ConnectionRequestServerPrivate.h
  Tr69ConnectionRequestResponder.h
  Tr69ConnectionRequestResponderPrivate.h
)

qt4_automoc(${QT_SRCS})
qt4_wrap_cpp(QT_MOC_SRCS ${QT_MOC_HDRS})

#Build the library  
add_library(${TR69CLIENTCORE_LIBRARY} 
   ${QT_SRCS} 
   ${QT_MOC_HDRS}
   ${QT_MOC_SRCS} 
   ${FSM_CPP}
   ${FSM_FILES})

target_link_libraries(${TR69CLIENTCORE_LIBRARY}
                      ${QT_LIBRARIES}
                      ${SMQTUTILS_LIBRARY} 
                      ${TR69COMMON_LIBRARY}
                      ${QHTTPSERVER_LIBRARY}
                      ${QTSOAP_LIBRARY}
                      ${STATEBUILDERCPP_LIBRARIES})

install(TARGETS ${TR69CLIENTCORE_LIBRARY}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)