set(TR69COMMON_LIBRARY Tr69Common CACHE INTERNAL "Tr69Common library name")
set(TR69COMMON_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Tr69Common INCLUDE DIRECTORY")

# Where to find all FindXXX.cmake
set(CMAKE_MODULE_PATH $ENV{STATEBUILDERCPP_HOME}/share/cmake)

#Use Qt
set(STATEBUILDERCPP_USE_QT TRUE)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Set the state machine(s)
#set(FSM_FILES Tr69Common.fsmcpp)

#Generate the cpp source code
#statebuildercpp_compile(FSM_CPP ${FSM_FILES})

#Add the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${QSLOG_INCLUDE_DIR}
                    ${SMQTUTILS_INCLUDE_DIR}
                    ${QT_INCLUDE_DIRS}
                    ${QTSOAP_INCLUDE_DIR}
                    ${STATEBUILDERCPP_INCLUDE_DIRS})


set(QT_SRCS
    Tr69Tag.cpp
    Tr69ConnectionBase.cpp
    Tr69ParserBase.cpp
    HttpParser.cpp
)

set(QT_MOC_HDRS
  Tr69ConnectionBase.h
  Tr69ParserBase.h
)

set(HEADERS
  Tr69Tag.h
  HttpParser.h
)

qt4_automoc(${QT_SRCS})
qt4_wrap_cpp(QT_MOC_SRCS ${QT_MOC_HDRS})

#Build the library  
add_library(${TR69COMMON_LIBRARY} ${QT_SRCS} ${QT_MOC_HDRS} ${HEADERS} ${QT_MOC_SRCS} ${FSM_CPP} ${FSM_FILES})

target_link_libraries(${TR69COMMON_LIBRARY}
                      ${QTSOAP_LIBRARY}
                      ${QT_LIBRARIES}
                      ${SMQTUTILS_LIBRARY} 
                      ${STATEBUILDERCPP_LIBRARIES})

install(TARGETS ${TR69COMMON_LIBRARY}
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib
)