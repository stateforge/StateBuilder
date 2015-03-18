project(Robot)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 COMPONENTS QtCore QtNetwork REQUIRED)
# set(QT_USE_QTNETWORK TRUE)

include(${QT_USE_FILE})

#Verbose make
#set(CMAKE_VERBOSE_MAKEFILE ON)

#Fix this
# Where to find all FindXXX.cmake
set(CMAKE_MODULE_PATH $ENV{STATEBUILDERCPP_HOME}/share/cmake)

#Use Qt
set(STATEBUILDERCPP_USE_QT TRUE)

#Look for the Finite State Machine Compiler
find_package(STATEBUILDERCPP REQUIRED)

#Set the state machine(s)
set(FSM_FILES Robot.fsmcpp)

# Robot.fsmcpp => RobotFsm.h and RobotFsm.cpp
statebuildercpp_compile(FSM_CPP ${FSM_FILES})

#Add the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR}
                    ${QSLOG_INCLUDE_DIR}
                    ${SMQTUTILS_INCLUDE_DIR}
                    ${QT_INCLUDE_DIRS}
                    ${STATEBUILDERCPP_INCLUDE_DIRS})

set(Robot_SRCS Robot.cpp Arm.cpp Camera.cpp main.cpp ${FSM_CPP} ${FSM_FILES})

qt4_automoc(${Robot_SRCS})

add_executable(Robot ${Robot_SRCS})

target_link_libraries(Robot
                      ${QSLOG_LIBRARY} 
                      ${SMQTUTILS_LIBRARY} 
                      ${QT_LIBRARIES}
                      ${STATEBUILDERCPP_LIBRARIES})

link_directories(${STATEBUILDERCPP_LIBRARY_DIRS})

# Test
add_test(Robot Robot)


