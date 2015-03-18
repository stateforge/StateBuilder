
find_package(Qt4 COMPONENTS QtCore QtGui QtXml QtTest REQUIRED )
include(${QT_USE_FILE})

set(CMAKE_MODULE_PATH $ENV{STATEBUILDERCPP_HOME}/share/cmake)

#Use Qt
set(STATEBUILDERCPP_USE_QT TRUE)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Set the state machine(s)
set(FSM_FILES ParallelRoot.fsmcpp)

# Ping.fsmcpp => PingFsm.h and PingFsm.cpp
statebuildercpp_compile(FSM_CPP ${FSM_FILES})

include_directories( 
                     ${CMAKE_CURRENT_SOURCE_DIR} 
                     ${CMAKE_CURRENT_BINARY_DIR}
                     ${QSLOG_INCLUDE_DIR}
                     ${SMQTUTILS_INCLUDE_DIR}
					 ${STATEBUILDERCPP_INCLUDE_DIRS}
)

# add the target test: "make test" or ctest
set(CTEST_OUTPUT_ON_FAILURE TRUE)
enable_testing()


set(APPNAME Test)
set(SRCS Test.cpp Action.cpp)
set(HDRS Test.h Action.h)
qt4_wrap_cpp(MOC_SRCS ${HDRS})
add_executable(${APPNAME} ${SRCS} ${QT_MOC_HDRS} ${MOC_SRCS} ${FSM_CPP} ${FSM_FILES})
target_link_libraries(${APPNAME} 
    ${QT_LIBRARIES}
    ${QSLOG_LIBRARY} 
    ${SMQTUTILS_LIBRARY} 
)  
install(TARGETS ${APPNAME} RUNTIME DESTINATION bin)
add_test(${APPNAME} ${APPNAME})

