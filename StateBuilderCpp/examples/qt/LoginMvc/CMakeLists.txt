project(LoginMvc)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 COMPONENTS QtCore QtGui QtNetwork REQUIRED)
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
set(FSM_FILES LoginMvc.fsmcpp)

# LoginMvc.fsm => LoginMvcFsm.h and LoginMvcFsm.cpp
statebuildercpp_compile(FSM_CPP ${FSM_FILES})

#Add the include directories
include_directories(${CMAKE_CURRENT_SOURCE_DIR} 
                    ${CMAKE_CURRENT_BINARY_DIR} 
                    ${QT_INCLUDE_DIRS}
                    ${QSLOG_INCLUDE_DIR}
                    ${SMQTUTILS_INCLUDE_DIR}
                    ${STATEBUILDERCPP_INCLUDE_DIRS})

# Qt designer ui file
set(QT_UIS frame.ui)

# transform ui files into cpp code
qt4_wrap_ui(QT_UIS_H ${QT_UIS})

set(LoginMvc_SRCS  LoginMvc.h LoginMvc.cpp main.cpp ${FSM_CPP} )

#set(MOC_HDRS LoginMvc.h)
#qt4_wrap_cpp(MOC_HDRS ${MOC_HDRS} ${FSM_CPP})

qt4_automoc(${LoginMvc_SRCS})

add_executable(LoginMvc ${LoginMvc_SRCS} ${QT_UIS_H} ${FSM_FILES})

target_link_libraries(LoginMvc 
                      ${QSLOG_LIBRARY}
                      ${SMQTUTILS_LIBRARY}
                      ${QT_LIBRARIES}
                      ${STATEBUILDERCPP_LIBRARIES})

link_directories(${STATEBUILDERCPP_LIBRARY_DIRS})

# Test
#add_test(LoginMvc LoginMvc)


